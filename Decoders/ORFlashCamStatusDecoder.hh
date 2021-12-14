// ORFlashCamStatusDecoder.hh

#ifndef _ORFlashCamStatusDecoder_hh_
#define _ORFlashCamStatusDecoder_hh_

#include "ORVDataDecoder.hh"

class ORFlashCamStatusDecoder : public ORVDataDecoder {
public:

  ORFlashCamStatusDecoder();
  virtual ~ORFlashCamStatusDecoder(){}
  enum EORFlashCamStatusConsts{ kMaxCards=256, kOtherErrors=5,
    kEnvMonitors=16, kCardTemps=5, kCardVoltages=6, kADCTemps=2, kCTILinks=4 };
  virtual std::string GetDataObjectPath()
  { return "ORFlashCamListenerModel:FlashCamStatus"; }

  /* id of the flashcam readout object */
  virtual UInt_t GetReadoutID(UInt_t* record);

  /* id of the flashcam listener object */
  virtual UInt_t GetListenerID(UInt_t* record);

  /* overall status 0=errors, 1=no errors */
  virtual Int_t GetErrorStatus(UInt_t* record);

  /* status time of fc250 in seconds and microseconds */
  virtual Int_t GetFC250Sec(UInt_t* record);
  virtual Int_t GetFC250uSec(UInt_t* record);

  /* status time of cpu in seconds and microseconds */
  virtual Int_t GetCPUSec(UInt_t* record);
  virtual Int_t GetCPUuSec(UInt_t* record);

  /* status start and stop time in microseconds */
  virtual Int_t GetStartuSec(UInt_t* record);
  virtual Int_t GetStopuSec(UInt_t* record);

  /* total number of cards */
  virtual Int_t GetNCards(UInt_t* record);

  /* fcio id of card */ 
  virtual UInt_t GetCardFCIOID(UInt_t* record, UInt_t index);

  /* status of card */
  virtual UInt_t GetCardStatus(UInt_t* record, UInt_t index);

  /* event counter for card */
  virtual UInt_t GetCardEventNumber(UInt_t* record, UInt_t index);

  /* pps and clock counters */
  virtual UInt_t GetCardPPSCount(UInt_t* record, UInt_t index);
  virtual UInt_t GetCardTickCount(UInt_t* record, UInt_t index);
  virtual UInt_t GetCardMaxTicks(UInt_t* record, UInt_t index);

  /* error counts */
  virtual UInt_t GetCardTotalErrors(UInt_t* record, UInt_t index);
  virtual UInt_t GetCardEnvErrors(UInt_t* record, UInt_t index);
  virtual UInt_t GetCardCTIErrors(UInt_t* record, UInt_t index);
  virtual UInt_t GetCardLinkErrors(UInt_t* record, UInt_t index);
  virtual UInt_t GetCardOtherErrors(UInt_t* record, UInt_t index, UInt_t type);

  /* environmental monitoring data (temps, voltages, currents in mC, mV, mA) */
  virtual UInt_t GetCardTemp(UInt_t* record, UInt_t index, UInt_t ti);
  virtual UInt_t GetCardVoltage(UInt_t* record, UInt_t index, UInt_t vi);
  virtual UInt_t GetCardCurrent(UInt_t* record, UInt_t index);
  virtual UInt_t GetCardHumidity(UInt_t* record, UInt_t index);
  virtual UInt_t GetCardADCTemp(UInt_t* record, UInt_t index, UInt_t ti);

  /* card links */
  virtual UInt_t GetCardCTILink(UInt_t* record, UInt_t index, UInt_t link);
  virtual UInt_t GetCardLinkState(UInt_t* record, UInt_t index, UInt_t card);

  /* print information from the record */
  virtual void Dump(UInt_t* record);

protected:
  virtual size_t CardOffset(UInt_t* record, UInt_t index);
  UInt_t fBaseOffset;
};

#endif
