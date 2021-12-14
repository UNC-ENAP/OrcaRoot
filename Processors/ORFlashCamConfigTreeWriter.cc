// ORFlashCamConfigTreeWriter.cc

#include "ORFlashCamConfigTreeWriter.hh"

using namespace std;

ORFlashCamConfigTreeWriter::ORFlashCamConfigTreeWriter(string treeName) :
  ORVTreeWriter(new ORFlashCamConfigDecoder, treeName){
  fDecoder = dynamic_cast<ORFlashCamConfigDecoder*>(fDataDecoder);
  Clear();
  SetDoNotAutoFillTree();
}

ORFlashCamConfigTreeWriter::~ORFlashCamConfigTreeWriter(){
  delete fDecoder;
}

void ORFlashCamConfigTreeWriter::Clear(){
  fReadoutID         = 0;
  fListenerID        = 0;
  fEventListID       = 0;
  fNadcChan          = 0;
  fNtrigChan         = 0;
  fWFsamples         = 0;
  fADCbits           = 0;
  fIntegratorSamples = 0;
  fBaselinePrecision = 0;
  fNglobalTrigCards  = 0;
  fNtrigCards        = 0;
  fNadcCards         = 0;
  fGPSmode           = 0;
  fADCaddress.resize(0);
  fInputNumber.resize(0);
} 

ORDataProcessor::EReturnCode
ORFlashCamConfigTreeWriter::InitializeBranches(){
  fTree->Branch("readoutID",        &fReadoutID,        "readoutID/i");
  fTree->Branch("listenerID",       &fListenerID,       "listenerID/i");
  fTree->Branch("eventListID",      &fEventListID,      "eventListID/I");
  fTree->Branch("nADCchan",         &fNadcChan,         "nADCchan/I");
  fTree->Branch("nTrigChan",        &fNtrigChan,        "nTrigChan/I");
  fTree->Branch("wfSamples",        &fWFsamples,        "wfSamples/I");
  fTree->Branch("adcBits",          &fADCbits,          "adcBits/I");
  fTree->Branch("integratorSamples",&fIntegratorSamples,"integratorSamples/I");
  fTree->Branch("baselinePrecision",&fBaselinePrecision,"baselinePrecision/I");
  fTree->Branch("nGlobalTrigCards", &fNglobalTrigCards, "nGlobalTrigCards/I");
  fTree->Branch("nTrigCards",       &fNtrigCards,       "nTrigCards/I");
  fTree->Branch("nADCcards",        &fNadcCards,        "nADCcards/I");
  fTree->Branch("gpsMode",          &fGPSmode,          "gpsMode/I");
  fTree->Branch("adcAddress",       &fADCaddress);
  fTree->Branch("inputNumber",      &fInputNumber);
  return kSuccess;
}

ORDataProcessor::EReturnCode
ORFlashCamConfigTreeWriter::ProcessMyDataRecord(UInt_t* record){
  fReadoutID         = fDecoder->GetReadoutID(record);
  fListenerID        = fDecoder->GetListenerID(record);
  fEventListID       = fDecoder->GetEventListID(record);
  fNadcChan          = fDecoder->GetNADCChannels(record);
  fNtrigChan         = fDecoder->GetNTriggerChannels(record);
  fWFsamples         = fDecoder->GetWaveformSamples(record);
  fADCbits           = fDecoder->GetNADCBits(record);
  fIntegratorSamples = fDecoder->GetIntegratorSamples(record);
  fBaselinePrecision = fDecoder->GetBaselinePrecision(record);
  fNglobalTrigCards  = fDecoder->GetNGlobalTriggerCards(record);
  fNtrigCards        = fDecoder->GetNTriggerCards(record);
  fNadcCards         = fDecoder->GetNADCCards(record);
  fGPSmode           = fDecoder->GetGPSMode(record);
  fADCaddress.resize(fNadcChan);
  fInputNumber.resize(fNadcChan);
  for(UInt_t i=0; i<(UInt_t)fADCaddress.size(); i++){
    fADCaddress[i]  = fDecoder->GetADCAddress(record, i);
    fInputNumber[i] = fDecoder->GetInputNumber(record, i);
  }
  fTree->Fill();
  return kSuccess;
}
