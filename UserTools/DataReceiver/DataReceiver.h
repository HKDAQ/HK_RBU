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
 * This is a struct to place data you want your thread to access or exchange with it. The idea is the data inside is only used by the thread and so will be thread safe. this data will be used by the processing job which is run on individual output messages form the elctronics.
 *
 * $Author: B.Richards $
 * 
 */

struct DataReceiverJob_args:Thread_args{

  DataReceiverJob_args();
  ~DataReceiverJob_args();
  DataModel* m_data = 0; ///< pointer to central data model to get agrigated electronics data collections
  std::vector<zmq::message_t>* messages; ///< vector of input messages from electronics to be processed
  Pool<std::vector<zmq::message_t> >* message_pool; ///< message pool to return used message vectors to to save reallocation
  
  RAWDAQHeader* daq_header = 0; ///< pointer to be assigned to daq header message when decoded
  uint32_t* words = 0; ///< pointer to array of words in the electronics messages
  std::unordered_map<uint64_t, std::vector<uint32_t> > collections; ///< temporary storage for ~16ms time slices of data words before being passed to central storage  
  std::unordered_map<uint64_t, std::vector<TPUHit> > tpu_hit_collection;
  uint64_t bin = 0; ///< time bin of data object being decoded
  uint64_t current_bin = 0; ///< time bin of last processed data object
  size_t start_pos = 0; ///< position of the first data object in words that hasnt yet been added to collections
  RAWIDODHit* tmp_hit = 0; ///< pointer for use in decoding hit words
  size_t current_word = 0; ///< current word being processed
  uint32_t header[3] = {0,0,0}; ///< holder for header words
  uint32_t current_time = 0;
  bool bad_sync = false;
  std::queue<TimeSlice*> time_slices;
  std::queue<TimeSlice*> tpu_time_slices;

};

/**
 * \struct DataReceiver_args
 *
 * This is a struct to place data you want your thread to access or exchange with it. The idea is the data inside is only used by the thread and so will be thread safe. This is used for the two threads receiveing IDOD data and MPMT data.
 *
 * $Author: B.Richards $
 */

struct DataReceiver_args:Thread_args{

  DataReceiver_args();
  ~DataReceiver_args();
  DataModel* m_data = 0; ///< pointer to central data model to get agrigated electronics data collections
  DAQUtilities* m_util = 0; ///< pointer to utilities class to help with thread createion and deletion
  zmq::socket_t* sock = 0; ///< socket pointer for receiving data form electronics
  std::map<std::string,Store*> connections; ///< list of connections that have been made
  zmq::pollitem_t items[1]; ///< poll list to avoid blocking
  boost::posix_time::time_duration lapse; ///< duration to tell if need to update connections
  boost::posix_time::time_duration period; ///< period to check for new electroncis baord connections 
  boost::posix_time::ptime last; ///< time of last search for new connections
  std::string service = ""; ///< mane of service to look for when connecting to new services
  std::vector<zmq::message_t>* messages = 0; ///< temporary pointer for received messages
  uint16_t num_connections = 0; ///< current number of connections
  Job* tmp_job = 0; //< temprary pointer for creating jobs
  DataReceiverJob_args* tmp_data= 0 ; ///< temporary point for making job data
  std::string data_port = ""; ///< port to connect to new electronics baords on
  Pool<Job> job_pool; ///< pool to reuse old jobs to save new instansiations
  Pool<std::vector<zmq::message_t> > message_pool; ///< pool to reuse old message vectors to save new instansiations
  bool return_check = true; ///< variable for checking retuns values
  zmq::message_t reply; ///< mesasge to send electronics reply 
};





/**
 * \class DataReceiver
 *
 * This Tool receives data form the ID OD boards in one thread and MPMT baords in another thread. Jobs are then created from this data and passed to the central worker thread pool to process. The processing jobs decode the data and create reduced trigger data pushing thm both back to the central data store
 *
 * $Author: B.Richards $
 */

class DataReceiver: public Tool {


 public:

  DataReceiver(); ///< Simple constructor
  bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
  bool Execute(); ///< Executre function used to perform Tool perpose. 
  bool Finalise(); ///< Finalise funciton used to clean up resorces.


 private:

  static void Thread(Thread_args* arg); ///< Function to be run by the thread in a loop. Make sure not to block in it
  void CreateThreads(); ///< Function to create the IDOD and MPMT data receiver threads
  void LoadConfig(); ///< Function to load dynamic variable configuration form central store
  bool VarifyConfig(); ///< function to varify configruation
  void DefineVariables();
  static bool ProcessDataIDOD(void*& data); ///< Function to decode and proceess IDOD data
  static void ProcessDataFailIDOD(void*& data);  ///< Fucntion to run on filure of ProcessDataIDOD
  static bool ProcessDataMPMT(void*& data);  ///< Function to decode and proceess MPMT data
  static void ProcessDataFailMPMT(void*& data); ///< Fucntion to run on filure of ProcessDataMPMT
  
  DAQUtilities* m_util = 0;  ///< Pointer to utilities class to help with threading
  std::vector<DataReceiver_args*> args; ///< thread args (also holds pointer to the thread)
  
  std::vector<std::string> variable_names;
  int32_t receive_high_watermark = 20000;
  int32_t linger_ms = 100;
  int32_t backlog = 5000;
  int32_t receive_timeout_ms = 1000;
  int32_t send_timeout_ms = 200;
  int32_t immediate = 1;
  int32_t router_mandatory = 1;
  int32_t tcp_keepalive = 1;
  int32_t tcp_keepalive_idle_sec = 5;
  int32_t tcp_keepalive_count = 12;
  int32_t tcp_keepalive_interval_sec = 5;
  std::string idod_service = "ID/OD";
  int32_t idod_data_port = 6666;
  std::string mpmt_service = "MPMT";
  int32_t mpmt_data_port = 6666;
  int32_t search_period_sec = 200;


};


#endif

