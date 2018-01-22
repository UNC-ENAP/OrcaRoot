// ORGretina4ADecoder.hh

#ifndef _ORGretina4ADecoder_hh_
#define _ORGretina4ADecoder_hh_

#include "ORVDigitizerDecoder.hh"

/*
  Decoder for Argonne firmware for the Gretina digitizer.
*/


class ORGretina4ADecoder : public ORVDigitizerDecoder {
public:
  enum EParameters {
    kDSFact, // channel decimation factor index
    kNPars
  };
  
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
  virtual ULong64_t GetEventTime(size_t iEvent)
  { return (ULong64_t(fEvPtrs[iEvent][3] & 0xffff) << 32) + fEvPtrs[iEvent][2]; }
  virtual UInt_t GetEventEnergy(size_t iEvent)
  { UInt_t post = GetPostRiseSum(iEvent), pre = GetPreRiseSum(iEvent);
    return (post > pre) ? (post - pre) : (pre - post);
  }
  virtual UShort_t GetEventChannel(size_t iEvent)
  { return fEvPtrs[iEvent][1] & 0xf; }
  
  // WF Information
  virtual inline size_t GetEventWaveformLength(size_t iEvent)
  { return size_t(GetWFLen(iEvent)); }
  virtual inline UInt_t GetEventWaveformPoint(size_t iEvent, size_t iSample)
  { return UInt_t(fWFPtrs[iEvent][iSample] & 0x3fff); }
  virtual inline Short_t GetSignedWaveformSample(size_t iEvent, size_t iSample)
  { return (fWFPtrs[iEvent][iSample] & 0x3fff) - 0x2000; }
  virtual inline bool IsSampleMarked(size_t iEvent, size_t iSample)
  { return (fWFPtrs[iEvent][iSample] & 0x4000)>0; }
  virtual inline bool GetDownSampleFlag(size_t iEvent, size_t iSample)
  { return (fWFPtrs[iEvent][iSample] & 0x8000)>0; }
  
  // event header decoding
  virtual inline UInt_t GetPacketLength(UInt_t* header)
  { return ((header[1] >> 16) & 0x7ff) + 1; }
  virtual inline UInt_t GetHeaderLength(UInt_t* header)
  { return (header[3] >> 26) /2; }
  virtual inline UInt_t GetWFLen(size_t iEvent)
  { return (GetPacketLength(fEvPtrs[iEvent]) - GetHeaderLength(fEvPtrs[iEvent]))*2; }
  virtual inline UInt_t GetEventType(size_t iEvent)
  { return (fEvPtrs[iEvent][3] >> 23) & 0x7; }
  virtual inline UInt_t GetHeaderType(size_t iEvent)
  { return (fEvPtrs[iEvent][3] >> 16) & 0xf; }
  virtual inline UInt_t GetFlags(size_t iEvent)
  { return fEvPtrs[iEvent][4] & 0xffff; }
  
  virtual inline ULong64_t GetPrevTimeStamp(size_t iEvent)
  { return (ULong64_t(fEvPtrs[iEvent][5]) << 16) + (fEvPtrs[iEvent][4] >> 16); }

  virtual inline UInt_t GetSampledBaseline(size_t iEvent)
  { return fEvPtrs[iEvent][6] & 0xffffff; }  
  virtual inline UInt_t GetPreRiseSum(size_t iEvent)
  { return fEvPtrs[iEvent][8] & 0xffffff; }
  virtual inline UInt_t GetPostRiseSum(size_t iEvent)
  { return ((fEvPtrs[iEvent][9] & 0xffff) << 8) + (fEvPtrs[iEvent][8] >> 24); }

  virtual inline UInt_t GetPeakDetectTS(size_t iEvent)
  { return fEvPtrs[iEvent][9] >> 16; }
  virtual inline UInt_t GetTriggerTS(size_t iEvent)
  { return fEvPtrs[iEvent][10] >> 16; }
  
  virtual inline Short_t GetLastPostRiseEnterSample(size_t iEvent)
  { return Short_t(fEvPtrs[iEvent][10] & 0x3fff) - 0x2000; }
  virtual inline Short_t GetLastPostRiseLeaveSample(size_t iEvent)
  { return Short_t((fEvPtrs[iEvent][11] >> 16) & 0x3fff) - 0x2000; }
  virtual inline Short_t GetPostRiseLeaveSample(size_t iEvent)
  { return Short_t(fEvPtrs[iEvent][11] & 0x3fff) - 0x2000; }
  virtual inline Short_t GetPreRiseEnterSample(size_t iEvent)
  { return Short_t((fEvPtrs[iEvent][12] >> 16) & 0x3fff) - 0x2000; }
  virtual inline Short_t GetPreRiseLeaveSample(size_t iEvent)
  { return Short_t(fEvPtrs[iEvent][12] & 0x3fff) - 0x2000; }
  virtual inline Short_t GetBaseSample(size_t iEvent)
  { return Short_t((fEvPtrs[iEvent][13] >> 16) & 0x3fff) - 0x2000; }
  virtual inline Short_t GetPeakSample(size_t iEvent)
  { return Short_t(fEvPtrs[iEvent][13] & 0x3fff) - 0x2000; }
  
  // Functions related to setting / accessing card parameters
  virtual void SetDecoderDictionary(const ORDecoderDictionary* dict);
  virtual UInt_t GetParameter(EParameters par, UInt_t crate, UInt_t card, UInt_t channel);
  virtual UInt_t GetDSFactor(size_t iEvent)
  { return 0x1 << GetParameter(kDSFact, CrateOf(), CardOf(), GetEventChannel(iEvent)); }

protected:
  std::vector<UInt_t*> fEvPtrs;
  std::vector<Short_t*> fWFPtrs;
  std::map<UInt_t, UInt_t> fCardPars[kNPars]; //cache parameters that are searched in the XML header
};

#endif
