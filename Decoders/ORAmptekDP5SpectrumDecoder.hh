// ORAmptekDP5SpectrumDecoder.hh

#ifndef _ORAmptekDP5SpectrumDecoder_hh_
#define _ORAmptekDP5SpectrumDecoder_hh_

#include "ORVDigitizerDecoder.hh"

/** Decodes the binary Orca data format and writes it into a ROOT TFile.
 * The binary data format description is in \file ORAmptekDP5Decoder.m .
 * In  \file ORAmptekDP5Model.m in - (NSDictionary*) dataRecordDescription
 * the entries in the data dictionary define the data key and its according
 * selector of the decoder. In this case it is "AmptekDP5Spectrum". The decoder of
 * this dictionary is defined as ORAmptekDP5DecoderForSpectrum.
 * The source code (in \file ORAmptekDP5Decoder.m) of this method (ORAmptekDP5DecoderForSpectrum)
 * holds the description of this format.
 *
 * Note to the name: a AmptekDP5Spectrum is read out by the SLT FIFO -> SLTFLTEvent
 *
 * This format is recognized by the return value of GetDataObjectPath() which is
 * "ORAmptekDP5Model:AmptekDP5Spectrum".
 */ //-tb- 2013-02-6
class ORAmptekDP5SpectrumDecoder: public ORVDigitizerDecoder
{
public:
    ORAmptekDP5SpectrumDecoder();
    virtual ~ORAmptekDP5SpectrumDecoder() {}
    enum EAmptekDP5SpectrumConsts {kBufHeadLen = 8, //header in int32, see ORAmptekDP5Decoder, see Orca code
		//kWaveformLength = 10000, //this is no longer valid/constant, use GetWaveformLen() -tb-
		kMaxSpectrumLength = 10000, //8192 -tb-
		//maybe rename to kWaveformPageLength ... ?
		kNumFLTChannels = 24};
    size_t fSpectrumLength;
    size_t fWaveformLength;
    
    virtual std::string GetDataObjectPath() { return "ORAmptekDP5Model:AmptekDP5Spectrum"; }  
    //!< AmptekDP5Spectrum is the key in ORAmptekDP5Model.m in - (NSDictionary*) ORAmptekDP5Model::dataRecordDescription -tb- 2015-04-12 
	
    virtual bool SetDataRecord(UInt_t* record);
	
    //Functions that return data from buffer header:
    virtual inline UInt_t GetSec();
    virtual inline UInt_t GetSubSec();
    virtual inline UInt_t GetChannelMap();
    virtual inline UShort_t GetFiber();
    virtual inline UShort_t GetChannel();
    virtual inline UShort_t GetTrigChannel();
    virtual inline UInt_t GetEventInfo();
    virtual inline UInt_t GetEnergy();
    virtual inline UInt_t GetEventID();
    virtual inline UInt_t GetEventFlags(size_t event=0);
	
    // Spectrum Functions
    virtual inline size_t GetSpectrumLen(); 
    virtual inline UChar_t* GetSpectrumDataPointer();
    virtual size_t CopySpectrumData(UInt_t* spectrum, size_t len);
    virtual inline UInt_t GetInfoFlags();
    virtual inline UInt_t GetDeviceID();
    virtual inline UInt_t HasStatus();
    virtual inline UInt_t GetAcqTime();
    virtual inline UInt_t GetRealTime();
    
    // Waveform Functions
    virtual inline size_t GetWaveformLen(); 
    virtual inline UInt_t* GetWaveformDataPointer();
    virtual size_t CopyWaveformDataDouble(double* waveform, size_t len);
    virtual size_t CopyWaveformData(UShort_t* waveform, size_t len);
	
    /* Satisfying ORVDigitizerDecoder interface. */
    virtual double GetSamplingFrequency() {return .01;}
    virtual UShort_t GetBitResolution() {return 16;} 
    virtual size_t GetNumberOfEvents() {return 1;}
    virtual ULong64_t GetEventTime(size_t /*event*/); 
    virtual UInt_t GetEventEnergy(size_t /*event*/) {return GetEnergy();}
    virtual UShort_t GetEventChannel(size_t /*event*/) {return GetChannel();}
    virtual size_t GetEventWaveformLength(size_t /*event*/) 
	{ return GetWaveformLen(); }
    virtual UInt_t GetEventWaveformPoint( size_t /*event*/, 
										 size_t waveformPoint )
	{ return (UInt_t) GetWaveformDataPointer()[waveformPoint]; }
	
    //Error checking:
    virtual bool IsValid();
    virtual void DumpBufferHeader();
	
    //debugging:
    void Dump(UInt_t* dataRecord);
    
};

//inline functions: ************************************************************************


inline UChar_t* ORAmptekDP5SpectrumDecoder::GetSpectrumDataPointer()
{
	return (    ((UChar_t*)fDataRecord) + kBufHeadLen*4 + 6);
}

/*! Returns the number of the waveform in shorts (two shorts are stored in one long int / int32).
 *  Will be set by SetDataRecord(...)
 */ // -tb-
inline size_t ORAmptekDP5SpectrumDecoder::GetSpectrumLen() 
{
	return fSpectrumLength;
} 


inline UInt_t ORAmptekDP5SpectrumDecoder::GetInfoFlags() 
{
	return (fDataRecord[5]);
}

inline UInt_t ORAmptekDP5SpectrumDecoder::GetDeviceID() 
{
	return (fDataRecord[1] /* & 0xf */);
}

inline UInt_t ORAmptekDP5SpectrumDecoder::HasStatus() 
{
	return (fDataRecord[5] & 0x1);
}

inline UInt_t ORAmptekDP5SpectrumDecoder::GetAcqTime() 
{
	return (fDataRecord[6]);
}

inline UInt_t ORAmptekDP5SpectrumDecoder::GetRealTime() 
{
	return (fDataRecord[7]);
}



/*! Returns the number of the waveform in shorts (two shorts are stored in one long int / int32).
 *  Will be set by SetDataRecord(...)
 */ // -tb-
inline size_t ORAmptekDP5SpectrumDecoder::GetWaveformLen() 
{
	return fWaveformLength;
} 

inline UInt_t ORAmptekDP5SpectrumDecoder::GetSubSec() 
{
	return (fDataRecord[3]); //usec of gettimeofday -tb-
}

inline UInt_t ORAmptekDP5SpectrumDecoder::GetSec()
{
	return (fDataRecord[2]); //unix time -tb-
}

inline UInt_t ORAmptekDP5SpectrumDecoder::GetChannelMap()
{
	return (fDataRecord[4]);//f2 = channel map (18 bit) -tb-
}

inline UInt_t ORAmptekDP5SpectrumDecoder::GetEnergy()
{
	return (fDataRecord[5] & 0x00FFFFFF);//f3: energy (and U(?)) -tb-
}

inline UInt_t ORAmptekDP5SpectrumDecoder::GetEventInfo() //changed  2011-06-14 -tb-
{
	return ( fDataRecord[6] );//f4: page + triggAddr
}

inline UInt_t ORAmptekDP5SpectrumDecoder::GetEventID()
{
	return ( ( fDataRecord[6] & 0x0000F000 ) >> 12);
}

inline UInt_t ORAmptekDP5SpectrumDecoder::GetEventFlags(size_t)
{
	return (fDataRecord[7]);//flags (WF version etc.)
}


//TODO: fDataRecord[8] = spare


inline UShort_t ORAmptekDP5SpectrumDecoder::GetFiber()
{
	return ( fDataRecord[1] & 0xF000 ) >> 12;
}

inline UShort_t ORAmptekDP5SpectrumDecoder::GetChannel()
{
	return ( fDataRecord[1] & 0x0F00 ) >> 8;
}

inline UShort_t ORAmptekDP5SpectrumDecoder::GetTrigChannel()
{
	return ( fDataRecord[1] & 0xFF00 ) >> 8;
}

inline UInt_t* ORAmptekDP5SpectrumDecoder::GetWaveformDataPointer()
{
	return (fDataRecord + kBufHeadLen);
}

inline ULong64_t ORAmptekDP5SpectrumDecoder::GetEventTime( size_t )
{
	return ((ULong64_t)GetSec() << 32 ) + (ULong64_t)GetSubSec();
}

#endif

