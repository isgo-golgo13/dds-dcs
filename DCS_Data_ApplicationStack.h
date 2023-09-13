#ifndef DCS_DATA_APPLICATION_STACK_H_
#define DCS_DATA_APPLICATION_STACK_H_

#include "DCS_DataC.h"

#include <ace/String_Base.h>

/// Forward Declarations
class DCS_Data_DelegationStack;


/**
 * @class DCS_Data_ApplicationStack
 *
 * @brief Provides the actual management of the differences.
 *
 */
class DCS_Data_ApplicationStack
{
public:
  /**
   * Constructor
   *
   * @param delegation_stack - pointer to the Delegation Stack (Hides the DDS from the Application Stack)
   * @param directory_name - directory where the files will be stored
   * @param node_name- name of this application
   * @return
   */
  DCS_Data_ApplicationStack(DCS_Data_DelegationStack*  delegation_stack,
                            const ACE_TString& directory,
                            const ACE_TString& node);
  /** Destructor **/
  virtual ~DCS_Data_ApplicationStack();

  /**
   * Receive a published DCS_DataDiff
   *
   * @param diff - the DCS_DataDiff
   */
  void receive_diff (const DCS::DCS_DataDiff& diff);

  /**
   * Create and publish a new DCS_DataDiff for the current file.
   *
   * @param size - size of the diff data.
   *
   * @return  true if the DCS_DataDiff is published
   */
  bool generate_diff (long size);

  /**
   * Create and publish a new file.
   *
   * @param filename - name of the new file
   * @param size - size of the new file
   *
   * @return  true if the new file is created and published
   */
  bool generate_new_file (const ACE_TString& filename, long size);

  /**
   * Open and publish a existing file.
   *
   * @param filename - name of the new file
   *
   * @return  true if the new file is opened and published
   */
  bool publish_file (const ACE_TString& filename);

  /**
   * Get the number of files that have been written.
   *
   * @return number of files written
   */
  long get_write_count();

protected:

  /**
   * Write the data in the DCS_DataDiff.difference to a file
   *
   * @param filename - name of the file to write
   * @param diff - DCS_DataDiff holding the data to write
   */
  void write_difference_file (const ACE_TString& filename,
                              const DCS::DCS_DataDiff& diff);

  /**
   * Generate the filename to store the DCS_DataDiff information in.
   * @param filename - the filename that will be generated
   * @param diff - DCS_DataDiff with the information to be stored
   */
  void generate_diff_filename(ACE_TString& filename,
                              const DCS::DCS_DataDiff& diff);

private:
  /// Delegation Stack (layer) that obscures the sending / receiving
  /// on the DDS system.
  DCS_Data_DelegationStack* delegation_stack_;
  /// Directory to store the files in
  ACE_TString       directory_;
  /// Name of this node
  ACE_TString       node_;

  /// References the IDL DCS_DataDiff struct
  /// Name of the file that the differences are for
  ACE_TString       data_file_;
  /// Id of the file that the differences are for
  ::CORBA::Long     data_file_id_;
  /// The latest difference edition of the file
  ::CORBA::Long     current_data_file_edition_;
  /// The current number of file writes that have occured.
  ::CORBA::Long     data_file_write_count_;

};

#endif  ///->DCS_DATA_APPLICATION_STACK_H
