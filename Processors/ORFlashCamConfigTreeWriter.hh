// ORFlashCamConfigTreeWriter.hh

#ifndef _ORFlashCamConfigTreeWriter_hh_
#define _ORFlashCamConfigTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORFlashCamConfigDecoder.hh"
#include <string>
#include <vector>

class ORFlashCamConfigTreeWriter : public ORVTreeWriter {
public:
  ORFlashCamConfigTreeWriter(std::string treeName="");
  virtual ~ORFlashCamConfigTreeWriter();

  virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
  virtual inline void Clear();

protected:
  virtual EReturnCode InitializeBranches();
  
  ORFlashCamConfigDecoder* fDecoder;
  UInt_t fReadoutID;
  UInt_t fListenerID;
  Int_t fEventListID;
  Int_t fNadcChan;
  Int_t fNtrigChan;
  Int_t fWFsamples;
  Int_t fADCbits;
  Int_t fIntegratorSamples;
  Int_t fBaselinePrecision;
  Int_t fNglobalTrigCards;
  Int_t fNtrigCards;
  Int_t fNadcCards;
  Int_t fGPSmode;
  std::vector<UShort_t> fADCaddress;
  std::vector<UShort_t> fInputNumber;
};

#endif
