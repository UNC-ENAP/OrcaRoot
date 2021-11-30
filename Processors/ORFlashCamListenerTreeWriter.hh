// ORFlashCamListenerTreeWriter.hh

#ifndef _ORFlashCamListenerTreeWriter_hh_
#define _ORFlashCamListenerTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORFlashCamListenerDecoder.hh"
#include <string>
#include <vector>

class ORFlashCamListenerTreeWriter : public ORVTreeWriter
{
public:
  ORFlashCamListenerTreeWriter(std::string treeName="");
  virtual ~ORFlashCamListenerTreeWriter();

  virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
  virtual inline void Clear();

protected:
  virtual EReturnCode InitializeBranches();
  
  ORFlashCamListenerDecoder* fDecoder;
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
