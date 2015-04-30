// ORAmptekDP5SpectrumTreeWriter.hh

#ifndef _ORAmptekDP5SpectrumTreeWriter_hh_
#define _ORAmptekDP5SpectrumTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORAmptekDP5SpectrumDecoder.hh"

class ORAmptekDP5SpectrumTreeWriter : public ORVTreeWriter
{
  public:
    ORAmptekDP5SpectrumTreeWriter(std::string treeName = "");
    virtual ~ORAmptekDP5SpectrumTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fSpectrumLength = 0;
        fInfoFlags = 0;
        fDeviceID = 0;
        fSec = 0; 
          //obsolete:
          fSubSec = 0; fTimeStamp = 0; fEventID = 0;fCrate = 0; fCard = 0; fFiber =0;
        fChannel = 0; fTrigChannel = 0; fEnergy = 0; fWaveformLength = 0;
		fChannelMap=0;  //-tb- 2010-02-17
		fEventID=0; fEventFlags=0; fEventInfo=0;  //-tb- 2010-02-17
        fSaveOnlyNonemptyTrees=true; }
    enum EEdelweissSLTWFTreeWriter{
      //kMaxWFLength = ORAmptekDP5SpectrumDecoder::kWaveformLength -tb- this was too small
        kMaxWFLength = ORAmptekDP5SpectrumDecoder::kMaxSpectrumLength * 1, /*64*/ //TODO: remove it -tb-
        kMaxSpectrumLength = ORAmptekDP5SpectrumDecoder::kMaxSpectrumLength * 1 /*64*/ //TODO: test it -tb-
      };
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORAmptekDP5SpectrumDecoder* fEventDecoder;
    UInt_t fSpectrum[kMaxSpectrumLength];
    UInt_t fSpectrumLength;
    UInt_t fInfoFlags;
    UInt_t fDeviceID;
    
    //TODO: there are some vars remaining from EDW SLT object ... -tb-
    UInt_t fSec, fSubSec;
    ULong64_t fTimeStamp;
    UInt_t fChannelMap;
    UShort_t fCrate, fCard, fFiber, fChannel, fTrigChannel;
    UShort_t fWaveform[kMaxWFLength];
    UInt_t fWaveformLength;
    Int_t fEnergy;
    Short_t fTriggerAddr;
    UInt_t fEventID, fEventFlags, fEventInfo;
    //Bool_t saveOnlyNonemptyTrees; //!< flag to skip writing empty trees -tb- 2008-02-19 - MOVED TO BASE CLASS -tb-
};

#endif
