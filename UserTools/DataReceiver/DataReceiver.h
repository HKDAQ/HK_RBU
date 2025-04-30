#ifndef DataReceiver_H
#define DataReceiver_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "DataModel.h"
#include <unordered_map>


/**
 * \struct DataReceiverJob_args
 *
 * This is a struct to place data you want your thread to access or exchange with it. The idea is the datainside is only used by the threa\d and so will be thread safe
 *
 * $Author: B.Richards $
 * $Date: 2019/05/28 10:44:00 $
 */

struct DataReceiverJob_args:Thread_args{

  DataReceiverJob_args();
  ~DataReceiverJob_args();
  DataModel* m_data;
  std::vector<zmq::message_t>* messages;

};

/**
 * \struct DataReceiver_args
 *
 * This is a struct to place data you want your thread to access or exchange with it. The idea is the datainside is only used by the threa\d and so will be thread safe
 *
 * $Author: B.Richards $
 * $Date: 2019/05/28 10:44:00 $
 */

struct DataReceiver_args:Thread_args{

  DataReceiver_args();
  ~DataReceiver_args();
  DataModel* m_data;
  DAQUtilities* m_util;
  zmq::socket_t* sock;
  std::map<std::string,Store*> connections;
  zmq::pollitem_t items[1];
  boost::posix_time::time_duration lapse;
  boost::posix_time::time_duration period;
  boost::posix_time::ptime last;
  std::string service;
  std::vector<zmq::message_t>* messages;
  uint16_t num_connections;
  Job* tmp_job;
  DataReceiverJob_args* tmp_data;
  std::string data_port;

};





/**
 * \class DataReceiver
 *
 * This is a template for a Tool that produces a single thread that can be assigned a function seperate to the main thread. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
*/

class DataReceiver: public Tool {


 public:

  DataReceiver(); ///< Simple constructor
  bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
  bool Execute(); ///< Executre function used to perform Tool perpose. 
  bool Finalise(); ///< Finalise funciton used to clean up resorces.


 private:

  static void Thread(Thread_args* arg); ///< Function to be run by the thread in a loop. Make sure not to block in it
  void CreateThreads();
  void LoadConfig();
  static bool ProcessDataIDOD(void*& data);
  static void ProcessDataFailIDOD(void*& data);
  static bool ProcessDataMPMT(void*& data);
  static void ProcessDataFailMPMT(void*& data);
  static void CreateCollection(std::unordered_map<uint32_t, IDODCollection>& collections,  uint32_t*& words, size_t& start_pos, size_t& current_word, uint32_t& current_bin);
  
  DAQUtilities* m_util;  ///< Pointer to utilities class to help with threading
  std::vector<DataReceiver_args*> args; ///< thread args (also holds pointer to the thread)

};


#endif
