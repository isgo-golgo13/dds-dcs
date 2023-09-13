// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler v1.6a_p8 ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from
// be/be_codegen.cpp:381


#include "DCS_DataC.h"
#include "tao/CDR.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
#include "DCS_DataC.inl"
#endif /* !defined INLINE */

// TAO_IDL - Generated from
// be/be_visitor_arg_traits.cpp:73

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


// Arg traits specializations.
namespace TAO
{
}

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from 
// be/be_visitor_sequence/sequence_cs.cpp:65

#if !defined (_DCS_OCTETSEQ_CS_)
#define _DCS_OCTETSEQ_CS_

DCS::OctetSeq::OctetSeq (void)
{}

DCS::OctetSeq::OctetSeq (
    ::CORBA::ULong max
  )
  : TAO::unbounded_value_sequence<
        ::CORBA::Octet
      >
    (max)
{}

DCS::OctetSeq::OctetSeq (
    ::CORBA::ULong max,
    ::CORBA::ULong length,
    ::CORBA::Octet * buffer,
    ::CORBA::Boolean release
  )
  : TAO::unbounded_value_sequence<
        ::CORBA::Octet
      >
    (max, length, buffer, release)
{}

DCS::OctetSeq::OctetSeq (
    const OctetSeq &seq
  )
  : TAO::unbounded_value_sequence<
        ::CORBA::Octet
      >
    (seq)
{}

DCS::OctetSeq::~OctetSeq (void)
{}

#endif /* end #if !defined */

// TAO_IDL - Generated from 
// be/be_visitor_structure/structure_cs.cpp:65



// TAO_IDL - Generated from
// be/be_visitor_sequence/cdr_op_cs.cpp:96
#if !defined _TAO_CDR_OP_DCS_OctetSeq_CPP_
#define _TAO_CDR_OP_DCS_OctetSeq_CPP_

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

::CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const DCS::OctetSeq &_tao_sequence
  )
{
  return TAO::marshal_sequence(strm, _tao_sequence);
}

::CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    DCS::OctetSeq &_tao_sequence
  )
{
  return TAO::demarshal_sequence(strm, _tao_sequence);
}


TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* _TAO_CDR_OP_DCS_OctetSeq_CPP_ */

// TAO_IDL - Generated from
// be/be_visitor_structure/cdr_op_cs.cpp:61


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

::CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const DCS::DCS_DataDiff &_tao_aggregate
  )
{
  return
    (strm << _tao_aggregate.data_file_id) &&
    (strm << _tao_aggregate.data_file.in ()) &&
    (strm << _tao_aggregate.data_change_source.in ()) &&
    (strm << _tao_aggregate.previous_edition) &&
    (strm << _tao_aggregate.new_edition) &&
    (strm << _tao_aggregate.difference);
}

::CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    DCS::DCS_DataDiff &_tao_aggregate
  )
{
  return
    (strm >> _tao_aggregate.data_file_id) &&
    (strm >> _tao_aggregate.data_file.out ()) &&
    (strm >> _tao_aggregate.data_change_source.out ()) &&
    (strm >> _tao_aggregate.previous_edition) &&
    (strm >> _tao_aggregate.new_edition) &&
    (strm >> _tao_aggregate.difference);
}

TAO_END_VERSIONED_NAMESPACE_DECL



