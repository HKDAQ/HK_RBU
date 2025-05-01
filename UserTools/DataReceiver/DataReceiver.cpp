#include "DataReceiver.h"

DataReceiverJob_args::DataReceiverJob_args():Thread_args(){}

DataReceiverJob_args::~DataReceiverJob_args(){

  if(messages!=0 && message_pool!=0) message_pool->Add(messages); //messages vector is added back to the pool instead of deleted as the pool will delete them in distruction. pool will be destroyed in calling class where its on stack

}

DataReceiver_args::DataReceiver_args():Thread_args(){

  reply.rebuild(4);

}

DataReceiver_args::~DataReceiver_args(){

  if(tmp_job!=0) job_pool.Add(tmp_job); //left over jobs are added back to the pool rather than being deleted as pool will delete them when goes out of scopew
  if(tmp_data!=0) delete tmp_data;
  if(messages!=0) message_pool.Add(messages); //messages vector is added back to the pool instead of deleted as the pool will delete them in distruction.

  delete sock;
}


DataReceiver::DataReceiver():Tool(){}


bool DataReceiver::Initialise(std::string configfile, DataModel &data){

  InitialiseTool(data);
  InitialiseConfiguration(configfile);
  //m_variables.Print();

  LoadConfig();  
  CreateThreads(); //createing mpmt and IDOD threads for receiving electroncis data

  ExportConfiguration();
  
  return true;
}


bool DataReceiver::Execute(){

  return true;
}


bool DataReceiver::Finalise(){


  // clearing up and destrying IDOD and mpmt receiver threads
  for(size_t i=0;i<args.size();i++){
    m_util->KillThread(args.at(i));
    delete args.at(i);
    args.at(i) = 0;
  }
  args.clear();
  m_data->num_threads-=2;


  delete m_util;
  m_util=0;

  return true;
}

void DataReceiver::Thread(Thread_args* arg){

  DataReceiver_args* args=reinterpret_cast<DataReceiver_args*>(arg);


  /////////////////////////// checking to see if period has lapsed to search for new connections and then connecting to anything new //////////////////////////////
  
  args->lapse = args->period -( boost::posix_time::microsec_clock::universal_time() - args->last);
  
  if(args->lapse.is_negative()){
    //printf("in lapse \n");
    args->num_connections = args->connections.size();
    if(args->m_util->UpdateConnections(args->service, args->sock, args->connections, args->data_port) > args->num_connections) args->m_data->services->SendLog("Info: New MPMT connected",v_message); //add pmt id
    args->m_data->monitoring_store_mtx.lock();
    args->m_data->monitoring_store.Set("connected_"+args->service, args->num_connections);
    args->m_data->monitoring_store_mtx.unlock();
    args->last= boost::posix_time::microsec_clock::universal_time();
    //printf("conenctions=%d: %u\n",args->connections.size(), args->m_data->unsorted_data.size());
  }

  // TODO BEN: this is still WCTE needs adapting
  
  ////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////// waiting for incomming electronics data and then sending it off for processing ////////////  

  zmq::poll(&(args->items[0]), 1, 100);
  
  if(args->items[0].revents & ZMQ_POLLIN){ //data comming in from electronics


    //receiving data
    args->messages = args->message_pool.GetNew();
    args->messages->emplace_back();
    args->return_check = args->sock->recv(&args->messages->back()); 
    while(args->messages->back().more()){
      args->messages->emplace_back();
      args->return_check = args->return_check && args->sock->recv(&args->messages->back()); 
    }
    
    // check if number of messages is correct. data from electroncis should be threee messages (identity, header, data) if not throw it away
    if(args->messages->size()!=3 || args->return_check){ 
      args->messages->clear();
      args->message_pool.Add(args->messages); // adding message vector back to pool rather than deleting to save instansiations
      args->messages = 0;
      //      delete args->messages;
      //args->messages = 0;

      //TODO BEN: throw an error
      return;
    }
    
    // sending reply to electroncs which is the message number, which is the first word of the data message
    memcpy(args->reply.data(), args->messages->at(1).data(), args->reply.size());
    args->return_check = args->sock->send(args->messages->at(0), ZMQ_SNDMORE);
    args->return_check = args->return_check && args->sock->send(args->reply);

    if(!args->return_check){

      //TODO BEN: thow warning
      //possibly also delete the data and return??? electronics should send it again and then you will ahve a duplicate if not
    }
    


    // create job to process messages and send them to central thread pool    
    args->tmp_job= args->job_pool.GetNew("DataReceive"); //new Job("DataReceive");
    if(args->tmp_job->data==0){
      args->tmp_data = new DataReceiverJob_args;
      args->tmp_job->data = args->tmp_data;
    }
    else{
      args->tmp_data = reinterpret_cast<DataReceiverJob_args*>(args->tmp_data);
    }    
    args->tmp_data->messages = args->messages;
    if(args->tmp_data->m_data==0) args->tmp_data->m_data = args->m_data;
    if(args->tmp_data->message_pool==0) args->tmp_data->message_pool = &args->message_pool;
    if(args->tmp_job->out_pool==0) args->tmp_job->out_pool = &args->job_pool;
    
    if(args->service == "IDOD"){
      args->tmp_job->func=ProcessDataIDOD;
      args->tmp_job->fail_func=ProcessDataFailIDOD;
    }
    else{
      args->tmp_job->func=ProcessDataMPMT;
      args->tmp_job->fail_func=ProcessDataFailMPMT;
    }
    
    args->m_data->job_queue.AddJob(args->tmp_job);
    args->tmp_job=0;
    args->tmp_data=0;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////
  
  
}


void DataReceiver::CreateThreads(){

  m_util=new DAQUtilities(m_data->context); // utilities class to aid with threading

  // creating receivers one for IDOD data one for mpmt data
  for(size_t i = 0; i<2 ; i++){ 
    
    args.emplace_back(new DataReceiver_args());
    
    args.at(i)->m_data = m_data;
    args.at(i)->m_util = m_util;
    
    args.at(i)->sock = new zmq::socket_t(*(m_data->context), ZMQ_ROUTER);
    args.at(i)->sock->setsockopt(ZMQ_RCVHWM, 20000);
    args.at(i)->sock->setsockopt(ZMQ_LINGER, 100);
    args.at(i)->sock->setsockopt(ZMQ_BACKLOG, 5000);
    args.at(i)->sock->setsockopt(ZMQ_RCVTIMEO, 1000);
    args.at(i)->sock->setsockopt(ZMQ_SNDTIMEO, 200);
    args.at(i)->sock->setsockopt(ZMQ_IMMEDIATE, 1);
    args.at(i)->sock->setsockopt(ZMQ_ROUTER_MANDATORY,1);
    args.at(i)->sock->setsockopt(ZMQ_TCP_KEEPALIVE, 1);
    args.at(i)->sock->setsockopt(ZMQ_TCP_KEEPALIVE_IDLE, 5);
    args.at(i)->sock->setsockopt(ZMQ_TCP_KEEPALIVE_CNT, 12);
    args.at(i)->sock->setsockopt(ZMQ_TCP_KEEPALIVE_INTVL, 5);

    args.at(i)->items[0].socket=*args.at(i)->sock;
    args.at(i)->items[0].fd=0;
    args.at(i)->items[0].events=ZMQ_POLLIN;
    args.at(i)->items[0].revents=0;

    args.at(i)->service="ID/OD";
    if(i==1) args.at(i)->service="MPMT";
    args.at(i)->num_connections=0;
    args.at(i)->messages=0;
    args.at(i)->data_port="6666";  //TODO BEN: this is needs tobe a dynamic variable
    
    args.at(i)->period=boost::posix_time::seconds(200);  //TODO BEN: this is needs tobe a dynamic variable
    
    args.at(i)->last=boost::posix_time::microsec_clock::universal_time();
    
    m_util->CreateThread("Receiver"+std::to_string(i), &Thread, args.at(i));
    
  }

  m_data->num_threads+=2;
    
  return;
}

void DataReceiver::LoadConfig(){

  if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

  //TODO BEN: add more dynamic variables. need to think about what todo if not set
    args.at(i)->sock->setsockopt(ZMQ_RCVHWM, 20000);
    args.at(i)->sock->setsockopt(ZMQ_LINGER, 100);
    args.at(i)->sock->setsockopt(ZMQ_BACKLOG, 5000);
    args.at(i)->sock->setsockopt(ZMQ_RCVTIMEO, 1000);
    args.at(i)->sock->setsockopt(ZMQ_SNDTIMEO, 200);
    args.at(i)->sock->setsockopt(ZMQ_IMMEDIATE, 1);
    args.at(i)->sock->setsockopt(ZMQ_ROUTER_MANDATORY,1);
    args.at(i)->sock->setsockopt(ZMQ_TCP_KEEPALIVE, 1);
    args.at(i)->sock->setsockopt(ZMQ_TCP_KEEPALIVE_IDLE, 5);
    args.at(i)->sock->setsockopt(ZMQ_TCP_KEEPALIVE_CNT, 12);
    args.at(i)->sock->setsockopt(ZMQ_TCP_KEEPALIVE_INTVL, 5);

    args.at(i)->items[0].socket=*args.at(i)->sock;
    args.at(i)->items[0].fd=0;
    args.at(i)->items[0].events=ZMQ_POLLIN;
    args.at(i)->items[0].revents=0;

    args.at(i)->service="ID/OD";
    if(i==1) args.at(i)->service="MPMT";
    args.at(i)->num_connections=0;
    args.at(i)->messages=0;
    args.at(i)->data_port="6666";  //TODO BEN: this is needs tobe a dynamic variable
    
    args.at(i)->period=boost::posix_time::seconds(200);  //TODO BEN: this is needs tobe a dynamic variable

  return;
}


bool DataReceiver::ProcessDataIDOD(void*& data){

  DataReceiverJob_args* args = reinterpret_cast<DataReceiverJob_args*>(data);


  /////////// freeing up variables from jobs last use ////////
  args->collections.clear(); 
  args->bin=0;
  args->current_bin=0;
  args->start_pos=0;
  args->tmp_hit=0;
  args->current_word =0;
  //////////////////////////////////////////

  // making special header packet to store cardid and first sync
  args->header[1] = 0b01000000111111110000000000000000;
  args->header[2] = 0;   //TODO BEN: fix these

  
  args->daq_header = reinterpret_cast<RAWDAQHeader*>(args->messages->at(1).data()); //decode DAQheader
  args->words = reinterpret_cast<uint32_t*>(args->messages->at(2).data()); 


  /////////////// looping over data words extracting hits and local collection
  
  while(args->current_word < args->messages->at(2).size()/4){
    
    switch (args->words[args->current_word] >>30){
    case 0b11 : //hits or ped object
      if(((args->words[args->current_word+1] >> 25) & 0b1) == 0){ //hit data object 
	args->tmp_hit = reinterpret_cast<RAWIDODHit*>(&args->words[args->current_word]);
	//TODO BEN: process hit
	args->current_word+=args->tmp_hit->GetWords();
	break;
      }
    
      //ped data object
      args->bin = RAWIDODPed::GetCoarse(&args->words[args->current_word]) >> 21;
      if(args->collections.count(args->bin)==0) args->collections[args->bin].insert(args->collections[args->bin].end(), &args->header[0], &args->header[0] + 2);
      if(args->bin!=args->current_bin){
	args->collections[args->current_bin].insert(args->collections[args->current_bin].end(), &args->words[args->start_pos], &args->words[args->start_pos] + (args->current_word - args->start_pos));
	args->current_bin=args->bin;
	args->start_pos=args->current_word;
      }
      args->current_word+=RAWIDODPed::GetWords();
      break;
    
   
    case 0b10 : //sync data object
      args->bin = RAWIDODSync::GetCounter(&args->words[args->current_word]) >> 10;
      if(args->collections.count(args->bin)==0) args->collections[args->bin].insert(args->collections[args->bin].end(), &args->header[0], &args->header[0] + 2);
      if(args->bin!=args->current_bin){
	args->collections[args->current_bin].insert(args->collections[args->current_bin].end(), &args->words[args->start_pos], &args->words[args->start_pos] + (args->current_word - args->start_pos));
	args->current_bin=args->bin;
	args->start_pos=args->current_word;
      }
      args->current_word+=RAWIDODSync::GetWords();
      break;
      
    case 0b00: //error
      args->bin = RAWIDODError::GetCoarse(&args->words[args->current_word]) >> 10;
      if(args->collections.count(args->bin)==0) args->collections[args->bin].insert(args->collections[args->bin].end(), &args->header[0], &args->header[0] + 2);
      if(args->bin!=args->current_bin){
	args->collections[args->current_bin].insert(args->collections[args->current_bin].end(), &args->words[args->start_pos], &args->words[args->start_pos] + (args->current_word - args->start_pos));
	args->current_bin=args->bin;
	args->start_pos=args->current_word;
      }
      args->current_word+=RAWIDODError::GetWords();
      break;
      
    case 0b01: //special data object
      args->bin = RAWIDODSpecial::GetCoarse(&args->words[args->current_word]) >> 10;
      if(args->collections.count(args->bin)==0) args->collections[args->bin].insert(args->collections[args->bin].end(), &args->header[0], &args->header[0] + 2);
      if(args->bin!=args->current_bin){
	args->collections[args->current_bin].insert(args->collections[args->current_bin].end(), &args->words[args->start_pos], &args->words[args->start_pos] + (args->current_word - args->start_pos));
	args->current_bin=args->bin;
	args->start_pos=args->current_word;
      }
      args->current_word+=RAWIDODSpecial::GetWords();
      break;
    }
    
  }

  // need to add the last words onto collecitons as currently only when bin bnumber changes
  args->current_word++;
  args->collections[args->current_bin].insert(args->collections[args->current_bin].end(), &args->words[args->start_pos], &args->words[args->start_pos] + (args->current_word - args->start_pos));
 

  /// TODO BEN: now uplaod to central collection!!!

  
  //  delete args->messages;
  args->message_pool->Add(args->messages); //returning messafes to pool for reuse
  args->messages=0;
  //  args->m_data=0;
  //delete args;
  //args=0;
  //data=0;
  
  return true;
}

void DataReceiver::ProcessDataFailIDOD(void*& data){

  if(data!=0){  
    DataReceiverJob_args* args = reinterpret_cast<DataReceiverJob_args*>(data);
    //    delete args->messages;
    args->message_pool->Add(args->messages);    //returning messafes to pool for reuse
    args->messages=0;
    //args->m_data=0;
    
    //delete args;
    //args=0;
    //data=0;
  }
  
}

bool DataReceiver::ProcessDataMPMT(void*& data){

  return true;
}

void DataReceiver::ProcessDataFailMPMT(void*& data){

}

