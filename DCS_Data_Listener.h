#ifndef DCS_DATA_LISTENER_H_
#define DCS_DATA_LISTENER_H_

#include <dds/DdsDcpsSubscriptionC.h>
#include <dds/DCPS/Definitions.h>

class DCS_Data_DelegationStack;

class DCS_Data_Listener
  : public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener>
{

  public:

  DCS_Data_Listener(DCS_Data_DelegationStack* delegation_stack);

  virtual ~DCS_Data_Listener();


  virtual void on_requested_deadline_missed (
    ::DDS::DataReader_ptr reader,
    const ::DDS::RequestedDeadlineMissedStatus & status)
    ACE_THROW_SPEC ((::CORBA::SystemException));


  virtual void on_requested_incompatible_qos (
    ::DDS::DataReader_ptr reader,
    const ::DDS::RequestedIncompatibleQosStatus & status)
    ACE_THROW_SPEC ((::CORBA::SystemException));


  virtual void on_sample_rejected (
    ::DDS::DataReader_ptr reader,
    const ::DDS::SampleRejectedStatus & status
    )
    ACE_THROW_SPEC ((::CORBA::SystemException));


  virtual void on_liveliness_changed (
    ::DDS::DataReader_ptr reader,
    const ::DDS::LivelinessChangedStatus & status)
    ACE_THROW_SPEC ((::CORBA::SystemException));


  virtual void on_data_available (
    ::DDS::DataReader_ptr reader)
    ACE_THROW_SPEC ((::CORBA::SystemException));


  virtual void on_subscription_matched (
    ::DDS::DataReader_ptr reader,
    const ::DDS::SubscriptionMatchedStatus & status)
    ACE_THROW_SPEC ((::CORBA::SystemException));

  virtual void on_sample_lost (
    ::DDS::DataReader_ptr reader,
    const ::DDS::SampleLostStatus & status)
    ACE_THROW_SPEC ((::CORBA::SystemException));

private:

  // Pointer to the change monitor.  This class does not take ownership.
  DCS_Data_DelegationStack* delegation_stack_;

};

#endif  // DCS_DATA_LISTENER_H


