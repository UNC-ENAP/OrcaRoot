// ORFlashCamADCDecoder.hh

#ifndef _ORFlashCamADCDecoder_hh_
#define _ORFlashCamADCDecoder_hh_

#include "ORVDigitizerDecoder.hh"

class ORFlashCamADCDecoder : public ORVDigitizerDecoder {
public:
  
  ORFlashCamADCDecoder(){}
  virtual ~ORFlashCamADCDecoder(){}

  /* Basic functions */
  virtual std::string GetDataObjectPath()
  { return "ORFlashCamADCModel:FlashCamADC"; }
  virtual double GetSamplingFrequency(){ return 0.0625; }
  virtual UShort_t GetBitResolution(){ return 16; }
  virtual bool SetDataRecord(UInt_t* dataRecord);
  virtual UInt_t CrateOf();
  virtual UInt_t CardOf();
  
  /* Event level functions */
  virtual size_t GetNumberOfEvents(){ return 1; }
  virtual ULong64_t GetEventTime(size_t event);
  virtual UInt_t GetEventEnergy(size_t event);
  virtual UInt_t GetEventBaseline(size_t event);
  virtual UShort_t GetEventChannel(size_t event);
  virtual UShort_t GetEventChannel(size_t event, std::string decoder);

  /* Waveform level functions */
  virtual size_t GetEventWaveformLength(size_t event);
  virtual UInt_t* GetWaveformStart(size_t event);
  virtual UInt_t GetEventWaveformPoint(size_t event, size_t waveformPoint);
  virtual Bool_t WaveformDataIsSigned(){ return false; };
  
};

#endif
