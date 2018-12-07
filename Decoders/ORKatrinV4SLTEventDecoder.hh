// ORKatrinV4SLTEventDecoder.hh

#ifndef _ORKatrinV4SLTEventDecoder_hh_
#define _ORKatrinV4SLTEventDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

#define kUnknownType               0
#define kSecondsCounterType        1
#define kVetoCounterType           2
#define kDeadCounterType           3
#define kRunCounterType            4
#define kLostFltEventCounterType   5
#define kLostSltEventCounterType   6
#define kLostFltEventTrCounterType 7
#define kSyncMessageType           8


/** Decodes the binary Orca data format and writes it into a ROOT TFile.
  * The binary data format description is in \file ORKatrinV4SLTDecoder.m .
  * In  \file ORKatrinV4SLTModel.m in in - (NSDictionary*) dataRecordDescription
  * the entries in the data dictionary define the data key and its according
  * selector of the decoder. In this case it is "KatrinV4SLT". The decoder of
  * this dictionary is defined as ORKatrinV4SLTDecoderForEvent.
  * The source code (in \file ORKatrinV4SLTDecoder.m) of this class (ORKatrinV4SLTDecoderForEvent)
  * holds the description of this format.
  *
  * This is a part of ORKatrinV4SLTDefs.h which defines the type and subtype of SLTv4 counter events:
  
      \verbatim
		Time Stamp Record Types used with the
		Slt counters (see shipSltTimestamp).
  
  
		#define kUnknownType		0
		#define kSecondsCounterType	1
		#define kVetoCounterType	2
		#define kDeadCounterType	3
		#define kRunCounterType		4
        #define kLostFltEventCounterType 5
        #define kLostSltEventCounterType 6
        #define kLostFltEventTrCounterType 7
        #define kSyncMessageType 8
 
		Time Stamp Record Types used with the
		Slt time stamps (see shipSltTimestamp). Used with the Counter Type above ...
	
		#define kStartRunType		1
		#define kStopRunType		2
		#define kStartSubRunType	3
		#define kStopSubRunType		4

       \endverbatim

  *
  * This format is recognized by the return value of GetDataObjectPath() which is
  * "ORKatrinV4SLTModel:KatrinV4SLTEvent".
  */
class ORKatrinV4SLTEventDecoder : public ORVBasicTreeDecoder
{
  public:
    ORKatrinV4SLTEventDecoder() {}
    virtual ~ORKatrinV4SLTEventDecoder() {}

    enum EKatrinV4FLTEnergyConsts {kNumFLTChannels = 24};
    virtual inline UShort_t CounterTypeOf(UInt_t* record)  { return ( record[1] & 0xF0 ) >> 4; }
    virtual inline UShort_t CounterSubTypeOf(UInt_t* record) { return ( record[1] & 0xF ) ; }
	  
    virtual inline UInt_t EventCounterOf(UInt_t* record) { return record[2]; }
    virtual inline UInt_t TimestampHighOf(UInt_t* record) { return record[3]; }
    virtual inline UInt_t TimestampLowOf(UInt_t* record) { return record[4]; }

    // for basic trees
    virtual size_t GetNPars() { return 7; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* /*record*/) { return 1; }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

    virtual std::string GetDataObjectPath() { return "ORKatrinV4SLTModel:KatrinV4SLTEvent"; }
    //!< KatrinV4SLTEvent is the key in ORKatrinV4SLTModel.m in - (NSDictionary*) dataRecordDescription -tb- 2008-02-12

};


#endif
