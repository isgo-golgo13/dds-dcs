#include "DCS_Data_DelegationStack.h"

#include "DCS_DataC.h"
#include "DCS_DataTypeSupportC.h"

#include "DCS_Data_Listener.h"


DCS_Data_Listener::DCS_Data_Listener(DCS_Data_DelegationStack* delegation_stack)
: delegation_stack_(delegation_stack)
{
}


DCS_Data_Listener::~DCS_Data_Listener()
{
}


void DCS_Data_Listener::on_data_available (::DDS::DataReader_ptr reader)
    ACE_THROW_SPEC ((::CORBA::SystemException))
{

  try {
    DCS::DCS_DataDiffDataReader_var dcs_data_dr =
      DCS::DCS_DataDiffDataReader::_narrow(reader);
    if (CORBA::is_nil (dcs_data_dr.in ())) {
      ACE_ERROR((LM_ERROR, "ERROR: DCS_Data_Listener::on_data_available() _narrow failed.\n"));
      return;
    }

    DCS::DCS_DataDiff diff;
    DDS::SampleInfo si ;
    DDS::ReturnCode_t status = dcs_data_dr->take_next_sample(diff, si) ;

    if (status == DDS::RETCODE_OK) {

      if (0 != delegation_stack_)
      {
        delegation_stack_->receive_diff(diff);
      }
      else
      {
        ACE_DEBUG((LM_DEBUG,
          "DCS_Data_Listener::on_data_available() no delegation_stack defined\n"));
      }

    } else if (status == DDS::RETCODE_NO_DATA) {
      ACE_ERROR((LM_ERROR,
        "ERROR: DCS_Data_Listener::on_data_available() received DDS::RETCODE_NO_DATA!"));
    } else {
      ACE_ERROR((LM_ERROR,
        "ERROR: DCS_Data_Listener::on_data_available() read Message: Error: %d\n",
        status));
    }
  } catch (CORBA::Exception&) {
    ACE_ERROR((LM_ERROR,
      "ERROR: DCS_Data_Listener::on_data_available() Exception caught in read\n"));
  }

}

void DCS_Data_Listener::on_requested_deadline_missed (
                                   ::DDS::DataReader_ptr,
                                   const ::DDS::RequestedDeadlineMissedStatus &)
    ACE_THROW_SPEC ((::CORBA::SystemException))
{
}


void DCS_Data_Listener::on_requested_incompatible_qos (
                                    ::DDS::DataReader_ptr,
                                    const ::DDS::RequestedIncompatibleQosStatus &)
    ACE_THROW_SPEC ((::CORBA::SystemException))
{
}


void DCS_Data_Listener::on_sample_rejected (
                         ::DDS::DataReader_ptr,
                         const ::DDS::SampleRejectedStatus &
                         )
    ACE_THROW_SPEC ((::CORBA::SystemException))
{
}



void DCS_Data_Listener::on_liveliness_changed (
                            ::DDS::DataReader_ptr,
                            const ::DDS::LivelinessChangedStatus &)
    ACE_THROW_SPEC ((::CORBA::SystemException))
{
}


void DCS_Data_Listener::on_subscription_matched (
                            ::DDS::DataReader_ptr,
                            const ::DDS::SubscriptionMatchedStatus &)
    ACE_THROW_SPEC ((::CORBA::SystemException))
{
}


void DCS_Data_Listener::on_sample_lost (
                     ::DDS::DataReader_ptr,
                     const ::DDS::SampleLostStatus &)
    ACE_THROW_SPEC ((::CORBA::SystemException))
{
}


