// ORFlashCamListenerDecoder.hh

#ifndef _ORFlashCamListenerDecoder_hh_
#define _ORFlashCamListenerDecoder_hh_

#include "ORVDataDecoder.hh"

class ORFlashCamListenerDecoder : public ORVDataDecoder {
public:

  ORFlashCamListenerDecoder(){}
  virtual ~ORFlashCamListenerDecoder(){}
  enum EORFlashCamListenerConsts{ kMaxADCChannels = 2400 };
  virtual std::string GetDataObjectPath()
  { return "ORFlashCamListenerModel:FlashCamConfig"; }
  
  /* the architechtural limit for fc250b */
  virtual inline UInt_t GetMaxADCChannels(){ return (UInt_t) kMaxADCChannels; }

  /* id of the flashcam readout object */
  virtual UInt_t GetReadoutID(UInt_t* record);

  /* id of the flashcam listener */
  virtual UInt_t GetListenerID(UInt_t* record);
  
  /* trace event list id */
  virtual Int_t GetEventListID(UInt_t* record);

  /* number of adc channels */
  virtual Int_t GetNADCChannels(UInt_t* record);

  /* number of trigger channels */
  virtual Int_t GetNTriggerChannels(UInt_t* record);
  
  /* number of samples per trace */
  virtual Int_t GetWaveformSamples(UInt_t* record);

  /* number of bits per ADC sample */
  virtual Int_t GetNADCBits(UInt_t* record);

  /* number of samples used by the FPGA integrator */
  virtual Int_t GetIntegratorSamples(UInt_t* record);

  /* precision of the FPGA baseline algorithm (1/LSB) */
  virtual Int_t GetBaselinePrecision(UInt_t* record);

  /* number of global trigger cards */
  virtual Int_t GetNGlobalTriggerCards(UInt_t* record);

  /* number of trigger cards */
  virtual Int_t GetNTriggerCards(UInt_t* record);

  /* number of ADC cards */
  virtual Int_t GetNADCCards(UInt_t* record);

  /* GPS mode (0: not used, 1: sync PPS and 10 MHz) */
  virtual Int_t GetGPSMode(UInt_t* record);

  /* ADC hardware address for a given waveform index in the readout */
  virtual UShort_t GetADCAddress(UInt_t* record, UInt_t index);

  /* ADC input number for a given waveform index in the readout */
  virtual UShort_t GetInputNumber(UInt_t* record, UInt_t index);

  /* print all information from the record */
  virtual void Dump(UInt_t* record);
  
};

#endif
