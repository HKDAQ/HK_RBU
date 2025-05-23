#ifndef JobManager_H
#define JobManager_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "DataModel.h"

/**
 * \class JobManager
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
 *
 * $Author: B.Richards $
 * $Date: 2019/05/28 10:44:00 $
 */

class JobManager: public Tool {


 public:

  JobManager(); ///< Simple constructor
  bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
  bool Execute(); ///< Executre function used to perform Tool perpose. 
  bool Finalise(); ///< Finalise funciton used to clean up resorces.


 private:

  void LoadConfig();
  std::string m_configfile;
  unsigned int m_thread_cap;
  WorkerPoolManager* worker_pool_manager;


};


#endif
