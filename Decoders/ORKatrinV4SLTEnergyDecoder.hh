// ORKatrinV4SLTEnergyDecoder.hh

#ifndef _ORKatrinV4SLTEnergyDecoder_hh_
#define _ORKatrinV4SLTEnergyDecoder_hh_

#include "ORVDigitizerDecoder.hh"
#include "ORLogger.hh"  //for debug output in GetEventPointer -tb-

/** Decodes the binary Orca data format and writes it into a ROOT TFile.
 * The binary data format description is in \file ORKatrinV4SLTDecoder.m .
 * In  \file ORKatrinV4SLTModel.m in - (NSDictionary*) dataRecordDescription
 * the entries in the data dictionary define the data key and its according
 * selector of the decoder. In this case it is "KatrinV4SLTEnergy". The decoder of
 * this dictionary is defined as ORKatrinV4SLTDecoderForEnergy.
 * The source code (in \file ORKatrinV4SLTDecoder.m) of this method (ORKatrinV4SLTDecoderForEnergy)
 * holds the description of this format.
 *
 * This format is recognized by the return value of GetDataObjectPath() which is
 * "ORKatrinV4SLTModel:KatrinV4SLTEnergy".
 */ //-tb- 2016-07-08
class ORKatrinV4SLTEnergyDecoder: public ORVDigitizerDecoder
{
public:
    ORKatrinV4SLTEnergyDecoder();
    virtual ~ORKatrinV4SLTEnergyDecoder() {}
    enum EKatrinV4SLTEnergyConsts {kBufHeadLen = 4, 
		kEventLength = 6, //this is no longer valid, use GetWaveformLen() -tb-
		//maybe rename to kWaveformPageLength ... ?
		kNumFLTChannels = 24};
    size_t fEventNum;
    
    virtual std::string GetDataObjectPath() { return "ORKatrinV4SLTModel:KatrinV4SLTEnergy"; }  
    //!< KatrinV4SLTEnergy is the key in ORKatrinV4SLTModel.m in - (NSDictionary*) ORKatrinV4SLTModel::dataRecordDescription -tb- 2016-07-08 
	
    virtual bool SetDataRecord(UInt_t* record);
	
    //Functions that return data from buffer header:
    virtual inline UInt_t GetSec(size_t iEvent);
    virtual inline UInt_t GetSubSec(size_t iEvent);
    virtual inline UInt_t GetEnergy(size_t iEvent);
    virtual inline UShort_t GetCard(size_t iEvent);
    virtual inline UShort_t GetChannel(size_t iEvent);
    virtual inline UShort_t GetMultiplicity(size_t iEvent);
    virtual inline UInt_t GetEventID(size_t iEvent);
    virtual inline UInt_t GetPeakADC(size_t iEvent);
    virtual inline UInt_t GetPeakPos(size_t iEvent);
    virtual inline UInt_t GetValleyADC(size_t iEvent);
    virtual inline UInt_t GetValleyPos(size_t iEvent);
    
    virtual inline UChar_t GetIsBipolarEvent(size_t iEvent);
    
//these are obsolete: -tb-
virtual inline UInt_t GetChannelMap(size_t iEvent);
virtual inline UInt_t GetEventInfo(size_t iEvent);
    virtual inline UInt_t GetEventFlags(size_t event=0);
	
    // Data Blocks Functions
    virtual inline size_t GetEventNum(); 
    virtual inline UInt_t* GetDataPointer();
    //virtual inline UInt_t* GetWaveformDataPointer();
    virtual inline UInt_t* GetEventPointer(size_t iEvent);
	
    /* Satisfying ORVDigitizerDecoder interface. */
    virtual double GetSamplingFrequency() {return .01;}
    virtual UShort_t GetBitResolution() {return 14;} 
    virtual size_t GetNumberOfEvents() {return GetEventNum();}//-tb-
    virtual ULong64_t GetEventTime(size_t /*event*/); 
    virtual UInt_t GetEventEnergy(size_t iEvent) {return GetEnergy(iEvent);}
    virtual UShort_t GetEventChannel(size_t iEvent) {return GetChannel(iEvent);}
    virtual size_t GetEventWaveformLength(size_t /*event*/) 
	{ return 0; }
    virtual UInt_t GetEventWaveformPoint( size_t /*event*/, 
										 size_t /*waveformPoint*/ )
	{ return (UInt_t) 0; }
	
    //Error checking:
    virtual bool IsValid();
    virtual void DumpBufferHeader();
	
    //debugging:
    void Dump(UInt_t* dataRecord);
    
};

//inline functions: ************************************************************************

/*! Returns the number of the waveform in shorts (two shorts are stored in one long int).
 *  Will be set by SetDataRecord(...)
 */ // -tb-
inline size_t ORKatrinV4SLTEnergyDecoder::GetEventNum() 
{
	return fEventNum;
} 

inline UInt_t ORKatrinV4SLTEnergyDecoder::GetSec(size_t iEvent)
{
    UInt_t* fEventRecord = GetEventPointer(iEvent);
	return (fEventRecord[1] & 0x1fffffff) | ((fEventRecord[0]  & 0x7) << 29);
}

inline UInt_t ORKatrinV4SLTEnergyDecoder::GetSubSec(size_t iEvent)
{
    UInt_t* fEventRecord = GetEventPointer(iEvent);
	return (fEventRecord[0]  & 0x0ffffff8) >> 3;
}

inline UInt_t ORKatrinV4SLTEnergyDecoder::GetEnergy(size_t iEvent)
{
    UInt_t* fEventRecord = GetEventPointer(iEvent);
	return fEventRecord[5] & 0xfffff;
}


inline UShort_t ORKatrinV4SLTEnergyDecoder::GetCard(size_t iEvent)
{
    UInt_t* fEventRecord = GetEventPointer(iEvent);
	return (fEventRecord[2]  >> 24) & 0x1f;
}

inline UShort_t ORKatrinV4SLTEnergyDecoder::GetChannel(size_t iEvent)
{
    UInt_t* fEventRecord = GetEventPointer(iEvent);
	return (fEventRecord[2]  >> 19) & 0x1f;
}

inline UShort_t ORKatrinV4SLTEnergyDecoder::GetMultiplicity(size_t iEvent)
{
    UInt_t* fEventRecord = GetEventPointer(iEvent);
	return (fEventRecord[2]  >> 14) & 0x1f;
}

inline UInt_t ORKatrinV4SLTEnergyDecoder::GetEventID(size_t iEvent)
{
    UInt_t* fEventRecord = GetEventPointer(iEvent);
	return fEventRecord[2] & 0x3fff;
}

inline UInt_t ORKatrinV4SLTEnergyDecoder::GetPeakADC(size_t iEvent)
{
    UInt_t* fEventRecord = GetEventPointer(iEvent);
	return (fEventRecord[3]  & 0x7ff);// & 0xfff however for unknown reasons (FPGA code) bit11 is always 1 -tb-
}

inline UInt_t ORKatrinV4SLTEnergyDecoder::GetPeakPos(size_t iEvent)
{
    UInt_t* fEventRecord = GetEventPointer(iEvent);
	return (fEventRecord[3] >> 16) & 0x1ff;
}

inline UInt_t ORKatrinV4SLTEnergyDecoder::GetValleyADC(size_t iEvent)
{
    UInt_t* fEventRecord = GetEventPointer(iEvent);
    Short_t valleyADC = (4096 - (fEventRecord[4]  & 0xfff));
	return (valleyADC);// 
}

inline UInt_t ORKatrinV4SLTEnergyDecoder::GetValleyPos(size_t iEvent)
{
    UInt_t* fEventRecord = GetEventPointer(iEvent);
	return (fEventRecord[4] >> 16) & 0xfff;
}


inline UChar_t ORKatrinV4SLTEnergyDecoder::GetIsBipolarEvent(size_t iEvent)
{
    if(GetPeakADC(iEvent)==0 && GetValleyADC(iEvent)==0 ) return 0;
	else return 0x1;
}





inline UInt_t ORKatrinV4SLTEnergyDecoder::GetChannelMap(size_t /*iEvent*/)
{
	return (0);
}

inline UInt_t ORKatrinV4SLTEnergyDecoder::GetEventInfo(size_t /*iEvent*/) //changed  2011-06-14 -tb-
{
	return ( 0 );
}


inline UInt_t ORKatrinV4SLTEnergyDecoder::GetEventFlags(size_t /*iEvent*/)
{
	return (0);
}

inline UInt_t* ORKatrinV4SLTEnergyDecoder::GetDataPointer()
{
	return (fDataRecord + kBufHeadLen);
}

inline UInt_t* ORKatrinV4SLTEnergyDecoder::GetEventPointer(size_t iEvent)
{
    if(iEvent >= fEventNum){
        ORLog(kDebug) << "ERROR: ORKatrinV4SLTEnergyDecoder::GetEventPointer(): iEvent >= fEventNum... " << std::endl;
        return NULL;
    }
	return (fDataRecord + kBufHeadLen + iEvent * kEventLength);
}







inline ULong64_t ORKatrinV4SLTEnergyDecoder::GetEventTime( size_t iEvent )
{
	return ((ULong64_t)GetSec(iEvent) << 32 ) + (ULong64_t)GetSubSec(iEvent);
}

#endif

