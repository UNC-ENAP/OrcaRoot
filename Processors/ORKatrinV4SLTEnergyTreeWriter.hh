// ORKatrinV4SLTEnergyTreeWriter.hh

#ifndef _ORKatrinV4SLTEnergyTreeWriter_hh_
#define _ORKatrinV4SLTEnergyTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORKatrinV4SLTEnergyDecoder.hh"

class ORKatrinV4SLTEnergyTreeWriter : public ORVTreeWriter
{
  public:
    ORKatrinV4SLTEnergyTreeWriter(std::string treeName = "");
    virtual ~ORKatrinV4SLTEnergyTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fSec = 0; fSubSec = 0; fEventID = 0;fCrate = 0; fCard = 0; 
        fChannel = 0; fEnergy = 0; fWaveformLength = 0;
		fChannelMap=0;  //-tb- 2010-02-17
		fEventID=0; fEventFlags=0; fEventInfo=0;  //-tb- 2010-02-17
        fSaveOnlyNonemptyTrees=true; }
    enum EKatrinV4SLTWFTreeWriter{
      //kMaxWFLength = ORKatrinV4SLTEnergyDecoder::kWaveformLength -tb- this was too small
      };
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORKatrinV4SLTEnergyDecoder* fEventDecoder;
    UInt_t fSec, fSubSec;
    UInt_t fChannelMap;
    UShort_t fCrate, fCard, fChannel, fMultiplicity;
    Short_t fPeakADC, fValleyADC,  fPeakPos, fValleyPos;
    //UShort_t fWaveform[kMaxWFLength];
    UInt_t fWaveformLength;
    UInt_t fEnergy;
    UInt_t fEventID, fEventFlags, fEventInfo;
    Bool_t isFLTthruSLT, isBipolar;
    //Bool_t saveOnlyNonemptyTrees; //!< flag to skip writing empty trees -tb- 2008-02-19 - MOVED TO BASE CLASS -tb-
};

#endif
