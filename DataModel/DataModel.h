#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <map>
#include <string>
#include <vector>

//#include "TTree.h"

#include "Store.h"
#include "BoostStore.h"
//#include "DAQLogging.h"
#include "DAQUtilities.h"
#include "SlowControlCollection.h"
#include "DAQDataModelBase.h"
#include <RAWIDODHit.h>
#include <RAWIDODPed.h>
#include <RAWIDODSync.h>
#include <RAWIDODError.h>
#include <RAWIDODSpecial.h>
#include <RAWDAQHeader.h>
#include <zmq.hpp>
#include <mutex>
#include <TimeSlice.h>

using namespace ToolFramework;

/**
 * \class DataModel
 *
 * This class Is a transient data model class for your Tools within the ToolChain. If Tools need to comunicate they pass all data objects through the data model. There fore inter tool data objects should be deffined in this class. 
 *
 *
 * $Author: B.Richards $ 
 *
 */

class DataModel : public DAQDataModelBase {


 public:
  
  DataModel(); ///< Simple constructor 

  bool running = false;
  uint64_t current_counter = 0;

  JobQueue job_queue;
  
  uint32_t thread_cap = 20;
  uint32_t num_threads = 0;
  
  Store monitoring_store;
  std::mutex monitoring_store_mtx;
  
  std::map<uint64_t, TimeSlice*> aggrigation_buffer;
  std::mutex aggrigation_buffer_mtx;

  std::map<uint64_t, TimeSlice*> main_buffer;
  std::mutex main_buffer_mtx;

  Pool<TimeSlice> time_slice_pool;  

  std::map<uint32_t, uint16_t> pmt_id_map;

  
 private:
  
  //std::map<std::string,TTree*> m_trees; 
  
  
  
};



#endif
