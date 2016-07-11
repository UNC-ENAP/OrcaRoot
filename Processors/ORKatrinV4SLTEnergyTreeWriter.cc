// ORKatrinV4SLTEnergyTreeWriter.cc

#include "ORKatrinV4SLTEnergyTreeWriter.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

using namespace std;

ORKatrinV4SLTEnergyTreeWriter::ORKatrinV4SLTEnergyTreeWriter(string treeName) :
ORVTreeWriter(new ORKatrinV4SLTEnergyDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORKatrinV4SLTEnergyDecoder*>(fDataDecoder);
  Clear();// ??? why is this needed, what is it doing ??? -tb-
  SetDoNotAutoFillTree();//we have multiple events in one data record -> split into one lbranch leaf per event -> call tree->Fill() local -tb-

}

ORKatrinV4SLTEnergyTreeWriter::~ORKatrinV4SLTEnergyTreeWriter()
{
  delete fEventDecoder;
}

/** Create the ROOT branches.
  * 
  */ //-tb- 2008-02-12
ORDataProcessor::EReturnCode ORKatrinV4SLTEnergyTreeWriter::InitializeBranches()
{
  fTree->Branch("eventSec", &fSec, "eventSec/i");
  fTree->Branch("eventSubSec", &fSubSec, "eventSubSec/i");
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("multiplicity", &fMultiplicity, "multiplicity/s");
  fTree->Branch("channelMap", &fChannelMap, "channelMap/i");
  fTree->Branch("eventID", &fEventID, "eventID/i");
  fTree->Branch("energy_adc", &fEnergy, "energy_adc/i");
  fTree->Branch("PeakADC", &fPeakADC, "PeakADC/S");
  fTree->Branch("ValleyADC", &fValleyADC, "ValleyADC/S");
  fTree->Branch("PeakPos", &fPeakPos, "PeakPos/S");
  fTree->Branch("ValleyPos", &fValleyPos, "ValleyPos/S");
  fTree->Branch("isFLTthruSLT", &isFLTthruSLT, "isFLTthruSLT/O");

  fTree->Branch("eventFlags", &fEventFlags, "eventFlags/i");
  fTree->Branch("eventInfo", &fEventInfo, "eventInfo/i");
  //fTree->Branch("waveform", fWaveform, "waveform[wfLength]/s");
  return kSuccess;
}

/**  This sets the data/variables for the ROOT branches.
  *  TTree::Fill() is called in ORBasicTreeWriter::ProcessMyDataRecord(UInt_t* record) (?)
  *  This class is a ORDataProcessor and ORVTreeWriter, NOT a ORBasicTreeWriter!
  *  (See comments in ORDataProcessor.hh and ORVTreeWriter.hh)
  */ //-tb- 2008-02-12
ORDataProcessor::EReturnCode ORKatrinV4SLTEnergyTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
  if(!fEventDecoder->SetDataRecord(record)) return kFailure;
      // check severity to improve speed:
      
  for (size_t i = 0; i < fEventDecoder->GetEventNum(); i++) {
    
      fCrate = fEventDecoder->CrateOf();
      fCard = fEventDecoder->GetCard(i);
      fChannel = fEventDecoder->GetChannel(i);
      fMultiplicity = fEventDecoder->GetMultiplicity(i);
      fChannelMap = fEventDecoder->GetChannelMap(i);
      fSec = fEventDecoder->GetSec(i);
      fSubSec = fEventDecoder->GetSubSec(i);
      fEventID = fEventDecoder->GetEventID(i);
      fEnergy = fEventDecoder->GetEnergy(i);
      
      fPeakADC = fEventDecoder->GetPeakADC(i); 
      fValleyADC = fEventDecoder->GetValleyADC(i);   
      fPeakPos = fEventDecoder->GetPeakPos(i);  
      fValleyPos = fEventDecoder->GetValleyPos(i); 
      
      isFLTthruSLT = true;
      
      fEventFlags = fEventDecoder->GetEventFlags(i);
      fEventInfo = fEventDecoder->GetEventInfo(i);
   
      if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
      { 
          ORLog(kDebug) << "ProcessMyDataRecord(): "
              << "num-crate-card-channel-sec-subsec-energy_adc-PeakADC-ValleyADC-PeakPos-ValleyPos-chmap-eventID-eventFlags-eventInfo = "
	          << i << "-" << fCrate << "-"  << fCard << "-" << fChannel << "-" 
	          << fSec << "-" << fSubSec << "-" << fEnergy 
              << "-" << fPeakADC << "-" <<  fValleyADC
              << "-" << fPeakPos << "-" <<  fValleyPos
	          << "-" << fChannelMap 
              << "-" << fEventID << "-" <<  fEventFlags  //-tb- 2010-02-16
              << "-" << fEventInfo  //-tb- 2016-07-16
              << endl;
      }
  

      fTree->Fill();
  }

  return kSuccess;
}


