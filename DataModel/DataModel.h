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
#include <RAWDAQHeader.h>
#include <RAWIDODHit.h>
#include <RAWIDODPed.h>
#include <RAWIDODSync.h>
#include <RAWIDODError.h>
#include <RAWIDODSpecial.h>
#include <DAQHeader.h>
#include <zmq.hpp>
#include <mutex>

using namespace ToolFramework;

/**
 * \class DataModel
 *
 * This class Is a transient data model class for your Tools within the ToolChain. If Tools need to comunicate they pass all data objects through the data model. There fore inter tool data objects should be deffined in this class. 
 *
 *
 * $Author: B.Richards $ 
 * $Date: 2019/05/26 18:34:00 $
 *
 */

class DataModel : public DAQDataModelBase {


 public:
  
  DataModel(); ///< Simple constructor 

  //TTree* GetTTree(std::string name);
  //void AddTTree(std::string name,TTree *tree);
  //void DeleteTTree(std::string name,TTree *tree);
  JobQueue job_queue;

  unsigned int thread_cap;
  uint16_t num_threads;

  Store monitoring_store;
  std::mutex monitoring_store_mtx;
  
 private:
  
  //std::map<std::string,TTree*> m_trees; 
  
  
  
};



#endif
