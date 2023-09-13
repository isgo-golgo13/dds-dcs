#ifndef DCS_DATA_DELEGATION_STACK_H_
#define DCS_DATA_DELEGATION_STACK_H_

#include "DCS_DataC.h"
#include "dds/DdsDcpsInfoC.h"
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/PublisherImpl.h>
#include <dds/DCPS/SubscriberImpl.h>
#include <dds/DCPS/transport/framework/TheTransportFactory.h>
#include <dds/DCPS/transport/simpleTCP/SimpleTcpConfiguration.h>

#include "DCS_DataTypeSupportImpl.h"


// Forward Declarations
class DCS_Data_ApplicationStack;


/** Variables that will be consistent across all the nodes in the same
    DDS Domain. **/

/// Ids of the transports DDS will use.
const OpenDDS::DCPS::TransportIdType TCP_IMPL_PUB_ID = 10;
const OpenDDS::DCPS::TransportIdType TCP_IMPL_SUB_ID = 11;
/// Id of the DDS domain that all the transactions will take place in.
/// This id needs to be in the domain_ids file that is given to the DCPSInfoRepo application
const int DOMAINID = 411;



/**
 * @class DCS_Data_DelegationStack
 *
 * @brief Mediates the DDS and Application Level.
 *
 */
class DCS_Data_DelegationStack
{
public:
  /** Constructor **/
  DCS_Data_DelegationStack();
  /** Destructor **/
  virtual ~DCS_Data_DelegationStack();

  /**
   * Initialize the DDS system.
   *
   * @param argc    - number of command line arguments
   * @param *argv[] - command line arguments
   *
   * @return true if the DDS system was intialized correctly,
   *         false otherwise.
   */
  bool init_DDS(int& argc, ACE_TCHAR *argv[]);
  /**
   * Shutdown the DDS System
   */
  void shutdown_DDS();

  /**
   * Attach application level code to the abstraction layer.
   *
   * @param app
   */
  void attach_application(DCS_Data_ApplicationStack* application_stack);

  /**
   * Handle the reception of a FileDiff object.
   * This method is called by the DDS system code.
   * This method will try to call the Application Level code.
   *
   * @param diff - FileDiff recevied by the DDS subscriber.
   */
  void receive_diff(const DCS::DCS_DataDiff& diff);
  /**
   * Handle the sending of a FileDiff object.
   * This method is called by the Application Level code.
   * This method will try to call the DDS system code.
   *
   * @param diff - FileDiff to be sent by the DDS publisher
   *
   * @return
   */
  bool send_diff(const DCS::DCS_DataDiff& diff);


private:
  /** References to DDS System Code **/

  /// The publishers DDS transport
  OpenDDS::DCPS::TransportImpl_rch        pub_tcp_impl_;
  /// The subscribers DDS transport
  OpenDDS::DCPS::TransportImpl_rch        sub_tcp_impl_;

  /// Factory for creating Domain Participants
  ::DDS::DomainParticipantFactory_var dpf_;
  /// The Domain Participant reference
  ::DDS::DomainParticipant_var        dp_;
  /// The Topic that will be published and subscribed to
  ::DDS::Topic_var                    topic_;

  /// The publisher reference
  ::DDS::Publisher_var                        pub_;
  /// The Data Writer reference (does the actual publishing)
  ::DDS::DataWriter_var                       dw_;
  /// Implementation of the Data writer (needed for sending the diffs)
  DCS::DCS_DataDiffDataWriter_var  data_diff_writer_;
  /// Instance handle for publishing (one per key published)
  ::DDS::InstanceHandle_t                     handle_;

  /// The subscriber reference
  ::DDS::Subscriber_var         sub_;
  /// Listener for alerting that data has been published
  ::DDS::DataReaderListener_var listener_;
  /// Data Reader for receiving the published data
  ::DDS::DataReader_var         dr_;


  /// The reference to the Application Level code
  DCS_Data_ApplicationStack* application_stack_;
};

#endif
