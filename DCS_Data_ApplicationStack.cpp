#include "DCS_Data_ApplicationStack.h"
#include "DCS_Data_DelegationStack.h"

#include "ace/streams.h"
#include "ace/OS_NS_stdlib.h"

DCS_Data_ApplicationStack::DCS_Data_ApplicationStack(DCS_Data_DelegationStack*  delegation_stack,
                                   const ACE_TString& directory,
                                   const ACE_TString& node)
: delegation_stack_(delegation_stack)
, directory_(directory)
, node_(node)
, data_file_id_(-1)
, current_data_file_edition_(-1)
, data_file_write_count_(0)
{
  if (0 != delegation_stack_)
  {
    delegation_stack_->attach_application(this);
  }
  directory_ += ACE_DIRECTORY_SEPARATOR_STR;
}


DCS_Data_ApplicationStack::~DCS_Data_ApplicationStack()
{
}


void
DCS_Data_ApplicationStack::receive_diff(const DCS::DCS_DataDiff& diff)
{
  ACE_TString diff_name;
  ACE_TString diff_file_name = directory_;

  ACE_DEBUG((LM_DEBUG,
    ACE_TEXT("%s received new diff (%d) -> (%d) for file \"%C\" id %d size %d bytes from %C\n"),
    node_.c_str(),
    diff.previous_edition,
    diff.new_edition,
    diff.data_file.in(),
    diff.data_file_id,
    diff.difference.length(),
    diff.data_change_source.in()
    ));


  // Handle the case where the first file is received
  if (-1 == diff.previous_edition)
  {
    diff_name += ACE_TEXT_CHAR_TO_TCHAR(diff.data_file.in());

    data_file_ = ACE_TEXT_CHAR_TO_TCHAR(diff.data_file.in());
    data_file_id_ = diff.data_file_id;

  }
  else // This is not the first file
  {
    generate_diff_filename(diff_name, diff);
  }

  diff_file_name += diff_name;

  write_difference_file(diff_file_name, diff);

  ACE_DEBUG((LM_INFO,
    ACE_TEXT("Received file \"%s\" from %s\n"),
    diff.data_file.in(),
    diff.data_change_source.in()
    ));
}


bool
DCS_Data_ApplicationStack::generate_diff (long size)
{
  bool generate_result = false;

  // Create the FileDiff and set the values
  DCS::DCS_DataDiff diff;
  diff.data_file = CORBA::string_dup(ACE_TEXT_ALWAYS_CHAR(data_file_.c_str()));
  diff.data_file_id = data_file_id_;
  diff.data_change_source = CORBA::string_dup(ACE_TEXT_ALWAYS_CHAR(node_.c_str()));
  diff.previous_edition = current_data_file_edition_;
  diff.new_edition = current_data_file_edition_ + 1;

  // intialize the data
  diff.difference.length(size);
  for (long cnt = 0; cnt < size; ++cnt)
  {
    diff.difference[cnt] = (CORBA::Octet) (size % 256);
  }

  // write the file
  ACE_TString diffname;
  generate_diff_filename(diffname, diff);
  ACE_TString full_file_name = directory_ + diffname;
  write_difference_file(full_file_name, diff);

  // publish the diff
  if (0 != delegation_stack_)
  {
    generate_result = delegation_stack_->send_diff(diff);
  }

  return generate_result;
}


bool
DCS_Data_ApplicationStack::generate_new_file (const ACE_TString& filename, long size)
{
  bool generate_result = false;
  data_file_ = filename;
  DCS::DCS_DataDiff diff;
  diff.data_file_id = ACE_OS::rand() % 32765;
  diff.data_file = CORBA::string_dup(ACE_TEXT_ALWAYS_CHAR(data_file_.c_str()));
  diff.data_change_source = CORBA::string_dup(ACE_TEXT_ALWAYS_CHAR(node_.c_str()));
  diff.previous_edition = -1;
  diff.new_edition = 0;

  // intialize the data
  diff.difference.length(size);
  for (long cnt = 0; cnt < size; ++cnt)
  {
    diff.difference[cnt] = (CORBA::Octet) 0;
  }

  // save the file information
  data_file_ = ACE_TEXT_CHAR_TO_TCHAR(diff.data_file.in());
  data_file_id_ = diff.data_file_id;

  // write the file
  ACE_TString full_file_name = directory_ + data_file_;
  write_difference_file(full_file_name, diff);

  // publish the diff
  if (0 != delegation_stack_)
  {
    generate_result = delegation_stack_->send_diff(diff);
  }

  return generate_result;
}


void
DCS_Data_ApplicationStack::write_difference_file (const ACE_TString& filename,
                                                  const DCS::DCS_DataDiff& diff)
{

  ofstream output_stream (ACE_TEXT_ALWAYS_CHAR (filename.c_str()), ios::out | ios::trunc | ios::binary);
  // Check that the stream opened correctly
  if (output_stream.fail ())
  {
    ACE_ERROR((LM_ERROR,
      ACE_TEXT("ERROR - Unable to open file %s\n"),
      filename.c_str() ));
  }
  else
  {
    // write the diff to the file
    const char* buffer = (const char*) diff.difference.get_buffer();
    output_stream.write(buffer , diff.difference.length());

    // check that the data was written to the file.
    if (output_stream.bad ())
    {
      ACE_ERROR((LM_ERROR,
        ACE_TEXT("ERROR - Unable to write ouput to the file %s\n"),
        filename.c_str() ));
    }
    else
    {
      // write was successful so set the version and increment write count
      current_data_file_edition_ = diff.new_edition;
      data_file_write_count_++;
    }

    // close the file.
    output_stream.close();
  }

}


long
DCS_Data_ApplicationStack::get_write_count()
{
  return data_file_write_count_;
}


void
DCS_Data_ApplicationStack::generate_diff_filename(ACE_TString& filename,
                                         const DCS::DCS_DataDiff& diff)
{
  ACE_TCHAR outstr[1024];
  ACE_OS::sprintf(outstr,
          ACE_TEXT("diff_%s_%s_%d_%d"),
          ACE_TEXT_CHAR_TO_TCHAR(diff.data_change_source.in()),
          ACE_TEXT_CHAR_TO_TCHAR(diff.data_file.in()),
          diff.previous_edition,
          diff.new_edition);

  filename += outstr;
}

bool
DCS_Data_ApplicationStack::publish_file (const ACE_TString& filename)
{
  bool publish_result = false;
  data_file_ = filename;
  DCS::DCS_DataDiff diff;
  diff.data_file_id = ACE_OS::rand() % 32765;
  diff.data_file = CORBA::string_dup(ACE_TEXT_ALWAYS_CHAR(data_file_.c_str()));
  diff.data_change_source = CORBA::string_dup(ACE_TEXT_ALWAYS_CHAR(node_.c_str()));
  diff.previous_edition = -1;
  diff.new_edition = 0;

  //std::string  full_file_name = directory_ + data_file_;
  // Expect the files to be in the current directory.
  ACE_TString full_file_name = data_file_;

  ACE_stat file_state;
  if (0 != ACE_OS::stat(full_file_name.c_str(), &file_state))
  {
    ACE_ERROR((LM_ERROR, ACE_TEXT("ERROR - Unable to get state of file \"%s\"\n"), full_file_name.c_str()));
  }
  else
  {
    size_t size = file_state.st_size;

    FILE* file_handle = ACE_OS::fopen(full_file_name.c_str(), ACE_TEXT("rb"));

    if ( NULL == file_handle)
    {
      ACE_ERROR((LM_ERROR, ACE_TEXT("ERROR - Unable to open file \"%s\"\n"), full_file_name.c_str()));
    }
    else
    {
      // intialize the data
      diff.difference.length(size);

      size_t read_size = fread(diff.difference.get_buffer(false), 1, size, file_handle);
      if (size != read_size)
      {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR - Read error only read %d out of %d expected bytes from file \"%s\"\n"),
                   read_size, size, full_file_name.c_str() ));
      }
      else
      {
        // save the file information
        data_file_ = ACE_TEXT_CHAR_TO_TCHAR(diff.data_file.in());
        data_file_id_ = diff.data_file_id;

        // publish the diff
        if (0 != delegation_stack_)
        {
          publish_result = delegation_stack_->send_diff(diff);
          if (publish_result)
          {
            ACE_DEBUG((LM_INFO, ACE_TEXT("Published file \"%s\".\n"), diff.data_file.in() ));
          }
        }
      } // end of fread

      ACE_OS::fclose(file_handle);
    } // end of file open
  } // end of file stat
  return publish_result;
}

