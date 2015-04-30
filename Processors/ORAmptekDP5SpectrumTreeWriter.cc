// ORAmptekDP5SpectrumTreeWriter.cc

#include "ORAmptekDP5SpectrumTreeWriter.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

using namespace std;

ORAmptekDP5SpectrumTreeWriter::ORAmptekDP5SpectrumTreeWriter(string treeName) :
ORVTreeWriter(new ORAmptekDP5SpectrumDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORAmptekDP5SpectrumDecoder*>(fDataDecoder);
  Clear();
  fWaveform[0] = 0;  // this is on the stack, not the heap.
}

ORAmptekDP5SpectrumTreeWriter::~ORAmptekDP5SpectrumTreeWriter()
{
  delete fEventDecoder;
}

/** Create the ROOT branches.
  * 
  */ //-tb- 2008-02-12
ORDataProcessor::EReturnCode ORAmptekDP5SpectrumTreeWriter::InitializeBranches()
{
    
    fTree->Branch("deviceID", &fDeviceID, "deviceID/i");
    fTree->Branch("infoFlags", &fInfoFlags, "infoFlags/i");
    fTree->Branch("eventSec", &fSec, "sec/i");
    fTree->Branch("acqTime", &fAcqTime , "acqTime/i");
    fTree->Branch("realTime", &fRealTime, "realTime/i");
    
    fTree->Branch("spectrumLength", &fSpectrumLength, "spectrumLength/i");
    fTree->Branch("spectrum", fSpectrum, "waveform[spectrumLength]/i");
    
    
  //fTree->Branch("wfLength", &fWaveformLength, "wfLength/i");
  //fTree->Branch("eventSubSec", &fSubSec, "eventSubSec/i");//i=unsigned (capital i=I signed int)
  //fTree->Branch("timeStamp", &fTimeStamp, "timeStamp/l");//l=unsigned long = ULong64_t
  //fTree->Branch("waveform", fWaveform, "waveform[wfLength]/s");//s=unsigned short

  return kSuccess;
}

/**  This sets the data/variables for the ROOT branches.
  *  TTree::Fill() is called in ORBasicTreeWriter::ProcessMyDataRecord(UInt_t* record) (?)
  *  This class is a ORDataProcessor and ORVTreeWriter, NOT a ORBasicTreeWriter!
  *  (See comments in ORDataProcessor.hh and ORVTreeWriter.hh)
  */ //-tb- 2008-02-12
ORDataProcessor::EReturnCode ORAmptekDP5SpectrumTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
  if(!fEventDecoder->SetDataRecord(record)) return kFailure;
      // check severity to improve speed:
    
    fInfoFlags = 0;
    fDeviceID = 0;
    
    fSpectrumLength  = fEventDecoder->GetSpectrumLen();
    fInfoFlags = fEventDecoder->GetInfoFlags();
    fDeviceID = fEventDecoder->GetSec();
    fSec = fEventDecoder->GetSec();
    fAcqTime     =  fEventDecoder->GetAcqTime();
    fRealTime    =  fEventDecoder->GetRealTime();

    
    
    
    if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
    { 
        ORLog(kDebug) << "ProcessMyDataRecord(): "
        << "deviceID-sec-infoFlags-spectrumLen = "
        << fDeviceID  << "-"
        << fSec << "-" << fInfoFlags << "-" 
        << "-" << fSpectrumLength  //-tb- 2015-04-16
        << endl;
    }
    if(fSpectrumLength > kMaxSpectrumLength) {
        ORLog(kError) << "Spectrum length (" << fSpectrumLength 
        << ") exceeds kMaxSpectrumLength (" << kMaxSpectrumLength << ")" << endl;
        return kFailure;
    }
    
    fEventDecoder->CopySpectrumData( fSpectrum, kMaxSpectrumLength );
    
    return kSuccess;
    
#if 0    
  fCrate = fEventDecoder->CrateOf();
  fCard = fEventDecoder->CardOf();
  fFiber = fEventDecoder->GetFiber();
  //fChannel = fEventDecoder->GetChannel();
  fChannel = fEventDecoder->GetTrigChannel();
  fChannelMap = fEventDecoder->GetChannelMap();
  fSec = fEventDecoder->GetSec();
  fSubSec = fEventDecoder->GetSubSec();
  fTimeStamp = (  ((ULong64_t)(fSec & 0x0000ffff)) << 32) | fSubSec; 
  fEventID = fEventDecoder->GetEventID();
  fEnergy = fEventDecoder->GetEnergy();
  fTriggerAddr = fEventDecoder->GetEventInfo() & 0x0fff;
  fWaveformLength = fEventDecoder->GetWaveformLen();
  fEventFlags = fEventDecoder->GetEventFlags();
  fEventInfo = fEventDecoder->GetEventInfo();
   
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
  { 
    ORLog(kDebug) << "ProcessMyDataRecord(): "
      << "crate-card-channel-sec-subsec-energy_adc-chmap-eventID-eventFlags-eventInfo-wfLen = "
	  << fCrate << "-"  << fCard << "-" << fFiber << "-"  << fChannel << "-" 
	  << fSec << "-" << fSubSec << "-" << fEnergy 
	  << "-" << fChannelMap 
      << "-" << fEventID << "-" <<  fEventFlags  //-tb- 2010-02-16
      << "-" << fEventInfo  //-tb- 2010-02-16
      << "-" << fWaveformLength  //-tb- 2013-04-16
      << endl;
  }
  if(fWaveformLength > kMaxWFLength) {
    ORLog(kError) << "Waveform length (" << fWaveformLength 
      << ") exceeds kMaxWFLength (" << kMaxWFLength << ")" << endl;
    return kFailure;
  }
  
  fEventDecoder->CopyWaveformData( fWaveform, kMaxWFLength );

  return kSuccess;
#endif
}


