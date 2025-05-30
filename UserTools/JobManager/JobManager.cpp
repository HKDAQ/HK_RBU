#include "JobManager.h"

JobManager::JobManager():Tool(){}


bool JobManager::Initialise(std::string configfile, DataModel &data){
  
  InitialiseTool(data);
  m_configfile = configfile;
  InitialiseConfiguration(configfile);
  //m_variables.Print();
  LoadConfig();
  
  bool self_serving =true;
  m_variables.Get("self_serving", self_serving);
  
  m_data->num_threads=0;
  
  worker_pool_manager= new WorkerPoolManager(m_data->job_queue, &m_thread_cap, &(m_data->thread_cap), &(m_data->num_threads), nullptr, self_serving);
  
  ExportConfiguration();
  
  return true;
}


bool JobManager::Execute(){
  
  /*  if(m_data->change_config){
    InitialiseConfiguration(m_configfile);  
    LoadConfig();
    ExportConfiguration();
  }
  */

  m_data->monitoring_store_mtx.lock();
  m_data->monitoring_store.Set("pool_threads",worker_pool_manager->NumThreads());
  m_data->monitoring_store.Set("queued_jobs",m_data->job_queue.size());
  m_data->monitoring_store_mtx.unlock();
  // printf("jobmanager q:t = %d:%d\n", m_data->job_queue.size(), worker_pool_manager->NumThreads());
  usleep(1000);
    sleep(5);
  worker_pool_manager->PrintStats();
  printf("buffersize %u\n", m_data->aggrigation_buffer.size());
  if(worker_pool_manager->NumThreads()==m_thread_cap)  m_data->services->SendLog("Warning: Worker Pool Threads Maxed" , 0); //make this a warning
  std::cout<<"globalThreads="<<m_data->num_threads<<std::endl;

  std::cout<<std::endl<<"card counters:"<<std::endl;
  for(std::unordered_map<uint16_t, std::atomic<uint64_t> >::iterator it=m_data->card_counters.begin(); it!= m_data->card_counters.end(); it++){
    //    std::cout<<it->first<<":"<<it->second<<std::endl;
  }

  std::cout<<std::endl<<"pmt counters:"<<std::endl;
  for(std::unordered_map<uint16_t, std::atomic<uint64_t> >::iterator it=m_data->pmt_counters.begin(); it!= m_data->pmt_counters.end(); it++){
    //std::cout<<it->first<<":"<<it->second<<std::endl;
  }


  //sleep(1);
  
  return true;
}


bool JobManager::Finalise(){
  
  delete worker_pool_manager;
  worker_pool_manager=0;
  
  return true;
}

void JobManager::LoadConfig(){
  
  if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
  if(!m_variables.Get("thread_cap",m_thread_cap)) m_thread_cap=200;
  if(!m_variables.Get("global_thread_cap",m_data->thread_cap)) m_data->thread_cap=200; 
  //  m_thread_cap=200;
  //m_data->thread_cap=200;
}
