// ORGretina4ADecoder.hh

#ifndef _ORGretina4ADecoder_hh_
#define _ORGretina4ADecoder_hh_

#include "ORVDigitizerDecoder.hh"

/*
  Decoder for Argonne firmware for the Gretina digitizer.
*/


class ORGretina4ADecoder : public ORVDigitizerDecoder {
public:
  ORGretina4ADecoder() {}
  virtual ~ORGretina4ADecoder() {}

  virtual inline std::string GetDataObjectPath() { return "ORGretina4A:Gretina4A"; }  
  virtual inline std::string GetDictionaryObjectPath() { return "ORGretina4AModel"; }  

  // digitizer information required by ORVDigitizerDecoder
  virtual bool SetDataRecord(UInt_t* dataRecord);
  virtual inline double GetSamplingFrequency()
  { return 0.1; } // in GHz. 
  virtual inline UShort_t GetBitResolution()
  { return 14; }
  virtual inline UInt_t* GetEventPtr(size_t iEvent)
  { return fEvPtrs[iEvent]; }
  
  // Other available digitizer information
  virtual inline UShort_t GetBoardSerialNumber() 
  { return (fDataRecord[1] & 0xffff); }
  virtual inline Bool_t IsFifoAlmostFull()
  { return fDataRecord[1] & 0x80000000; }
  virtual inline Bool_t IsFifoHalfFull()
  { return fDataRecord[1] & 0x40000000; }

  // Header information required by ORVDigitizerDecoder
  virtual inline size_t GetNumberOfEvents()
  { return fEvPtrs.size(); }
  virtual inline ULong64_t GetEventTime(size_t iEvent)
  { return GetTimeStamp(fEvPtrs[iEvent]); }
  virtual inline UInt_t GetEventEnergy(size_t iEvent)
  { return GetEnergy(fEvPtrs[iEvent]); }
  virtual inline UShort_t GetEventChannel(size_t iEvent)
  { return GetChannel(fEvPtrs[iEvent]); }
  virtual inline UInt_t GetEventFlags(size_t iEvent)
  { return GetFlags(fEvPtrs[iEvent]); }

  // WF Information
  virtual inline size_t GetEventWaveformLength(size_t iEvent)
  { return GetWFLen(fEvPtrs[iEvent]); }
  virtual inline UInt_t GetEventWaveformPoint(size_t iEvent, size_t iSample)
  { return UInt_t(fWFPtrs[iEvent][iSample] & 0x3fff); }
  virtual inline Short_t GetSignedWaveformSample(size_t iEvent, size_t iSample)
  { return (fWFPtrs[iEvent][iSample] & 0x3fff) - 0x2000; }
  virtual inline bool IsSampleMarked(size_t iEvent, size_t iSample)
  { return (fWFPtrs[iEvent][iSample] & 0x4000)>0; }
  virtual inline bool GetDownSampleFlag(size_t iEvent, size_t iSample)
  { return (fWFPtrs[iEvent][iSample] & 0x8000)>0; }

protected:
  
  // event header decoding
  virtual inline UInt_t GetPacketLength(UInt_t* header)
  { return ((header[1] >> 16) & 0x7ff) + 1; }
  virtual inline UShort_t GetChannel(UInt_t* header)
  { return header[1] & 0xf; }
  virtual inline UInt_t GetHeaderLength(UInt_t* header)
  { return (header[3] >> 26) /2; }
  virtual inline UInt_t GetWFLen(UInt_t* header)
  { return (GetPacketLength(header) - GetHeaderLength(header))*2; }
  virtual inline UInt_t GetEventType(UInt_t* header)
  { return (header[3] >> 23) & 0x7; }
  virtual inline UInt_t GetHeaderType(UInt_t* header)
  { return (header[3] >> 16) & 0xf; }
  virtual inline UInt_t GetFlags(UInt_t* header)
  { return header[4] & 0xffff; }
  
  virtual inline ULong64_t GetTimeStamp(UInt_t* header)
  { return (ULong64_t(header[3] & 0xffff) << 32) + header[2]; }
  virtual inline ULong64_t GetPrevTimeStamp(UInt_t* header)
  { return (ULong64_t(header[5]) << 16) + (header[4] >> 16); }

  virtual inline UInt_t GetSampledBaseline(UInt_t* header)
  { return header[6] & 0xffffff; }  
  virtual inline UInt_t GetPreRiseSum(UInt_t* header)
  { return header[8] & 0xffffff; }
  virtual inline UInt_t GetPostRiseSum(UInt_t* header)
  { return ((header[9] & 0xffff) << 8) + (header[8] >> 24); }
  virtual inline UInt_t GetEnergy(UInt_t* header)
  { return GetPostRiseSum(header) - GetPreRiseSum(header); }

  virtual inline UInt_t GetPeakDetectTS(UInt_t* header)
  { return header[9] >> 16; }
  virtual inline UInt_t GetTriggerTS(UInt_t* header)
  { return header[10] >> 16; }
  
  virtual inline Short_t GetLastPostRiseEnterSample(UInt_t* header)
  { return Short_t(header[10] & 0x3fff) - 0x2000; }
  virtual inline Short_t GetLastPostRiseLeaveSample(UInt_t* header)
  { return Short_t((header[11] >> 16) & 0x3fff) - 0x2000; }
  virtual inline Short_t GetPostRiseLeaveSample(UInt_t* header)
  { return Short_t(header[11] & 0x3fff) - 0x2000; }
  virtual inline Short_t GetPreRiseEnterSample(UInt_t* header)
  { return Short_t((header[12] >> 16) & 0x3fff) - 0x2000; }
  virtual inline Short_t GetPreRiseLeaveSample(UInt_t* header)
  { return Short_t(header[12] & 0x3fff) - 0x2000; }
  virtual inline Short_t GetBaseSample(UInt_t* header)
  { return Short_t((header[13] >> 16) & 0x3fff) - 0x2000; }
  virtual inline Short_t GetPeakSample(UInt_t* header)
  { return Short_t(header[13] & 0x3fff) - 0x2000; }
  
protected:
  std::vector<UInt_t*> fEvPtrs;
  std::vector<Short_t*> fWFPtrs;
};

#endif
