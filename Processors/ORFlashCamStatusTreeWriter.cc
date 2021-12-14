// ORFlashCamStatusTreeWriter.cc

#include "ORFlashCamStatusTreeWriter.hh"

using namespace std;

ORFlashCamStatusTreeWriter::ORFlashCamStatusTreeWriter(string treeName) :
  ORVTreeWriter(new ORFlashCamStatusDecoder, treeName){
  fDecoder = dynamic_cast<ORFlashCamStatusDecoder*>(fDataDecoder);
  Clear();
  SetDoNotAutoFillTree();
}

ORFlashCamStatusTreeWriter::~ORFlashCamStatusTreeWriter(){
  delete fDecoder;
}

void ORFlashCamStatusTreeWriter::Clear(){
  fReadoutID   = 0;
  fListenerID  = 0;
  fErrorStatus = 0;
  fFC250Sec    = 0;
  fFC250uSec   = 0;
  fCPUSec      = 0;
  fCPUuSec     = 0;
  fStartuSec   = 0;
  fStopuSec    = 0;
  fNCards      = 0;
  fFCIOID.resize(0);
  fCardStatus.resize(0);
  fCardEventNumber.resize(0);
  fCardPPSCount.resize(0);
  fCardTickCount.resize(0);
  fCardMaxTicks.resize(0);
  fTotalErrors.resize(0);
  fCTIErrors.resize(0);
  fLinkErrors.resize(0);
  fCardCurrent.resize(0);
  fCardHumidity.resize(0);
  fOtherErrors.resize(0);
  fCardTemp.resize(0);
  fCardVoltage.resize(0);
  fCardADCTemp.resize(0);
  fCTILink.resize(0);
  fLinkState.resize(0);
}

ORDataProcessor::EReturnCode
ORFlashCamStatusTreeWriter::InitializeBranches(){
  fTree->Branch("readoutID",       &fReadoutID,   "readoutID/i");
  fTree->Branch("listenerID",      &fListenerID,  "listenerID/i");
  fTree->Branch("errorStatus",     &fErrorStatus, "errorStatus/I");
  fTree->Branch("fc250Sec",        &fFC250Sec,    "fc250Sec/I");
  fTree->Branch("fc250uSec",       &fFC250uSec,   "fc250uSec/I");
  fTree->Branch("cpuSec",          &fCPUSec,      "cpuSec/I");
  fTree->Branch("cpuuSec",         &fCPUuSec,     "cpuuSec/I");
  fTree->Branch("startuSec",       &fStartuSec,   "startuSec/I");
  fTree->Branch("stopuSec",        &fStopuSec,    "stopuSec/I");
  fTree->Branch("nCards",          &fNCards,      "nCards/I");
  fTree->Branch("fcioID",          &fFCIOID);
  fTree->Branch("cardStatus",      &fCardStatus);
  fTree->Branch("cardEventNumber", &fCardEventNumber);
  fTree->Branch("cardPPSCount",    &fCardPPSCount);
  fTree->Branch("cardTickCount",   &fCardTickCount);
  fTree->Branch("cardMaxTicks",    &fCardMaxTicks);
  fTree->Branch("totalErrors",     &fTotalErrors);
  fTree->Branch("envErrors",       &fEnvErrors);
  fTree->Branch("ctiErrors",       &fCTIErrors);
  fTree->Branch("linkErrors",      &fLinkErrors);
  fTree->Branch("cardCurrent",     &fCardCurrent);
  fTree->Branch("cardHumidity",    &fCardHumidity);
  fTree->Branch("otherErrors",     &fOtherErrors);
  fTree->Branch("cardTemp",        &fCardTemp);
  fTree->Branch("cardVoltage",     &fCardVoltage);
  fTree->Branch("cardADCTemp",     &fCardADCTemp);
  fTree->Branch("ctiLink",         &fCTILink);
  fTree->Branch("linkState",       &fLinkState);
  return kSuccess;
}

ORDataProcessor::EReturnCode
ORFlashCamStatusTreeWriter::ProcessMyDataRecord(UInt_t* record){
  Clear();
  
  fReadoutID   = fDecoder->GetReadoutID(record);
  fListenerID  = fDecoder->GetListenerID(record);
  fErrorStatus = fDecoder->GetErrorStatus(record);
  fFC250Sec    = fDecoder->GetFC250Sec(record);
  fFC250uSec   = fDecoder->GetFC250uSec(record);
  fCPUSec      = fDecoder->GetCPUSec(record);
  fCPUuSec     = fDecoder->GetCPUuSec(record);
  fStartuSec   = fDecoder->GetStartuSec(record);
  fStopuSec    = fDecoder->GetStopuSec(record);
  fNCards      = fDecoder->GetNCards(record);
  
  for(UInt_t i=0; i<(UInt_t)fNCards; i++){
    fFCIOID.push_back(fDecoder->GetCardFCIOID(record, i));
    fCardStatus.push_back(fDecoder->GetCardStatus(record, i));
    fCardEventNumber.push_back(fDecoder->GetCardEventNumber(record, i));
    fCardPPSCount.push_back(fDecoder->GetCardPPSCount(record, i));
    fCardTickCount.push_back(fDecoder->GetCardTickCount(record, i));
    fCardMaxTicks.push_back(fDecoder->GetCardMaxTicks(record, i));
    fTotalErrors.push_back(fDecoder->GetCardTotalErrors(record, i));
    fEnvErrors.push_back(fDecoder->GetCardEnvErrors(record, i));
    fCTIErrors.push_back(fDecoder->GetCardCTIErrors(record, i));
    fLinkErrors.push_back(fDecoder->GetCardLinkErrors(record, i));
    fCardCurrent.push_back(fDecoder->GetCardCurrent(record, i));
    fCardHumidity.push_back(fDecoder->GetCardHumidity(record, i));
    
    fOtherErrors.push_back(vector<UInt_t>(ORFlashCamStatusDecoder::kOtherErrors));
    for(size_t j=0; j<fOtherErrors.back().size(); j++)
      fOtherErrors.back()[j] = fDecoder->GetCardOtherErrors(record, i, j);
    
    fCardTemp.push_back(vector<UInt_t>(ORFlashCamStatusDecoder::kCardTemps));
    for(size_t j=0; j<fCardTemp.back().size(); j++)
      fCardTemp.back()[j] = fDecoder->GetCardTemp(record, i, j);

    fCardVoltage.push_back(vector<UInt_t>(ORFlashCamStatusDecoder::kCardVoltages));
    for(size_t j=0; j<fCardVoltage.back().size(); j++)
      fCardVoltage.back()[j] = fDecoder->GetCardVoltage(record, i, j);
    
    fCardADCTemp.push_back(vector<UInt_t>(ORFlashCamStatusDecoder::kADCTemps));
    for(size_t j=0; j<fCardADCTemp.back().size(); j++)
      fCardADCTemp.back()[j] = fDecoder->GetCardADCTemp(record, i, j);
    
    fCTILink.push_back(vector<UInt_t>(ORFlashCamStatusDecoder::kCTILinks));
    for(size_t j=0; j<fCTILink.back().size(); j++)
      fCTILink.back()[j] = fDecoder->GetCardCTILink(record, i, j);

    fLinkState.push_back(vector<UInt_t>(fNCards));
    for(size_t j=0; j<fLinkState.back().size(); j++)
      fLinkState.back()[j] = fDecoder->GetCardLinkState(record, i, j);
  }
  fTree->Fill();
  return kSuccess;
}
