// ORFlashCamStatusTreeWriter.hh

#ifndef _ORFlashCamStatusTreeWriter_h__
#define _ORFlashCamStatusTreeWriter_h__

#include "ORVTreeWriter.hh"
#include "ORFlashCamStatusDecoder.hh"
#include <vector>

class ORFlashCamStatusTreeWriter : public ORVTreeWriter {
public:
  ORFlashCamStatusTreeWriter(std::string treeName="");
  virtual ~ORFlashCamStatusTreeWriter();

  virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
  virtual inline void Clear();

protected:
  virtual EReturnCode InitializeBranches();

  ORFlashCamStatusDecoder* fDecoder;
  UInt_t fReadoutID;
  UInt_t fListenerID;
  Int_t  fErrorStatus;
  Int_t  fFC250Sec;
  Int_t  fFC250uSec;
  Int_t  fCPUSec;
  Int_t  fCPUuSec;
  Int_t  fStartuSec;
  Int_t  fStopuSec;
  Int_t  fNCards;
  std::vector<UInt_t> fFCIOID;
  std::vector<UInt_t> fCardStatus;
  std::vector<UInt_t> fCardEventNumber;
  std::vector<UInt_t> fCardPPSCount;
  std::vector<UInt_t> fCardTickCount;
  std::vector<UInt_t> fCardMaxTicks;
  std::vector<UInt_t> fTotalErrors;
  std::vector<UInt_t> fEnvErrors;
  std::vector<UInt_t> fCTIErrors;
  std::vector<UInt_t> fLinkErrors;
  std::vector<UInt_t> fCardCurrent;
  std::vector<UInt_t> fCardHumidity;
  std::vector<std::vector<UInt_t> > fOtherErrors;
  std::vector<std::vector<UInt_t> > fCardTemp;
  std::vector<std::vector<UInt_t> > fCardVoltage;
  std::vector<std::vector<UInt_t> > fCardADCTemp;
  std::vector<std::vector<UInt_t> > fCTILink;
  std::vector<std::vector<UInt_t> > fLinkState;
};

#endif
