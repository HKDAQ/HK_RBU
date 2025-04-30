#include "DataReceiver.h"

DataReceiverJob_args::DataReceiverJob_args():Thread_args(){}

DataReceiverJob_args::~DataReceiverJob_args(){}

DataReceiver_args::DataReceiver_args():Thread_args(){}

DataReceiver_args::~DataReceiver_args(){}


DataReceiver::DataReceiver():Tool(){}


bool DataReceiver::Initialise(std::string configfile, DataModel &data){

  InitialiseTool(data);
  InitialiseConfiguration(configfile);
  //m_variables.Print();

  LoadConfig();
  CreateThreads();

  ExportConfiguration();
  
  return true;
}


bool DataReceiver::Execute(){

  return true;
}


bool DataReceiver::Finalise(){

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
  
  zmq::poll(&(args->items[0]), 1, 100);
  
  if(args->items[0].revents & ZMQ_POLLIN){
    
    args->messages = new std::vector<zmq::message_t>;
    args->messages->emplace_back();
    args->sock->recv(&args->messages->back());
    while(args->messages->back().more()){
      args->messages->emplace_back();
      args->sock->recv(&args->messages->back());
    }
    
    if(args->messages->size()!=3){
      delete args->messages;
      args->messages = 0;
      //throw an error
      return;
    }
    
    
    zmq::message_t reply(4);
    memcpy(reply.data(), args->messages->at(1).data(), reply.size());
    args->sock->send(args->messages->at(0), ZMQ_SNDMORE); /// need to add checking probablly a poll incase sender dies
    args->sock->send(reply);
    
    
    args->tmp_job= new Job("DataReceive");
    args->tmp_data= new DataReceiverJob_args;
    args->tmp_data->messages=args->messages;
    args->tmp_data->m_data=args->m_data;
    args->tmp_job->data= args->tmp_data;
    
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
    //printf("job submitted %d\n",args->job_queue->size());
  }
  
  
}


void DataReceiver::CreateThreads(){

  m_util=new DAQUtilities(m_data->context);

  for(size_t i = 0; i<2 ; i++){ 
    
    args.emplace_back(new DataReceiver_args());
    
    args.at(i)->m_data = m_data;
    args.at(i)->m_util = m_util;
    
    args.at(i)->sock = new zmq::socket_t(*(m_data->context), ZMQ_ROUTER);
    //socket ops
    args.at(i)->items[0].socket=*args.at(i)->sock;
    args.at(i)->items[0].fd=0;
    args.at(i)->items[0].events=ZMQ_POLLIN;
    args.at(i)->items[0].revents=0;
    args.at(i)->service="ID/OD";
    if(i==1) args.at(i)->service="MPMT";
    args.at(i)->num_connections=0;
    args.at(i)->messages=0;
    args.at(i)->data_port="6666";
    
    args.at(i)->period=boost::posix_time::seconds(200);
    
    args.at(i)->last=boost::posix_time::microsec_clock::universal_time();
    
    m_util->CreateThread("Receiver"+std::to_string(i), &Thread, args.at(i));
    
  }

  m_data->num_threads+=2;
    
  return;
}

void DataReceiver::LoadConfig(){

  if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

  return;
}


bool DataReceiver::ProcessDataIDOD(void*& data){

  DataReceiverJob_args* args = reinterpret_cast<DataReceiverJob_args*>(data);
  
  RAWDAQHeader* daq_header = reinterpret_cast<RAWDAQHeader*>(args->messages->at(1).data());
  uint32_t* words = reinterpret_cast<uint32_t*>(args->messages->at(2).data()); 
  
  std::unordered_map<uint32_t, IDODCollection> collections;
  uint32_t bin=0;
  uint32_t current_bin=0;
  size_t start_pos=0;
  RAWIDODHit* tmp_hit=0;

  size_t current_word =0;
  
  while(current_word < args->messages->at(2).size()/4){
    
    switch (words[current_word] >>30){
    case 0b11 : //hits or ped
      if(((words[current_word+1] >> 25) & 0b1) == 0){ //hit 
	tmp_hit = reinterpret_cast<RAWIDODHit*>(&words[current_word]);
	//process hit
	current_word+=tmp_hit->GetWords();
	break;
      }
      
      //ped
      bin = RAWIDODPed::GetCoarse(&words[current_word]) >> 21;
      if(bin!=current_bin){
	CreateCollection(collections, words, start_pos, current_word, current_bin);
	current_bin=bin;
      }
      current_word+=RAWIDODPed::GetWords();
      break;
      
    case 0b10 : //sync
      bin = RAWIDODSync::GetCounter(&words[current_word]) >> 10;
      if(bin!=current_bin){
	CreateCollection(collections, words, start_pos, current_word, current_bin);
	current_bin=bin;
      }
      current_word+=RAWIDODSync::GetWords();
      break;
      
    case 0b00: //error
      bin = RAWIDODError::GetCoarse(&words[current_word]) >> 10;
      if(bin!=current_bin){
	CreateCollection(collections, words, start_pos, current_word, current_bin);
	current_bin=bin;
      }
      current_word+=RAWIDODError::GetWords();
      break;
      
    case 0b01: //special
      bin = RAWIDODSpecial::GetCoarse(&words[current_word]) >> 10;
      if(bin!=current_bin){
	CreateCollection(collections, words, start_pos, current_word, current_bin);
	current_bin=bin;
      }
      current_word+=RAWIDODSpecial::GetWords();
      break;
    }
    
  }
  

  delete args->messages;
  args->messages=0;
  args->m_data=0;
  delete args;
  args=0;
  data=0;
  
  return true;
}

void DataReceiver::ProcessDataFailIDOD(void*& data){

  if(data!=0){  
    DataReceiverJob_args* args = reinterpret_cast<DataReceiverJob_args*>(data);
    delete args->messages;
    args->messages=0;
    args->m_data=0;
    
    delete args;
    args=0;
    data=0;
  }
  
}

bool DataReceiver::ProcessDataMPMT(void*& data){

  return true;
}

void DataReceiver::ProcessDataFailMPMT(void*& data){

}

void DataReceiver::CreateCollection(std::unordered_map<uint32_t, IDODCollection>& collections, uint32_t*& words, size_t& start_pos, size_t& current_word, uint32_t& current_bin){

  uint32_t header[2];
  collections[current_bin].data.insert(collections[current_bin].data.end(), &header[0], &header[0] + 2);

  collections[current_bin].data.insert(collections[current_bin].data.end(), &words[start_pos], &words[start_pos] + (current_word - start_pos));
  

}
  
