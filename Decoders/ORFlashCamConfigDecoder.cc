#include "ORFlashCamConfigDecoder.hh"
#include "ORLogger.hh"
#include <sstream>

using namespace std;

UInt_t ORFlashCamConfigDecoder::GetReadoutID(UInt_t* record){
  return (record[1] & 0xffff0000) >> 16;
}

UInt_t ORFlashCamConfigDecoder::GetListenerID(UInt_t* record){
  return record[1] & 0x0000ffff;
}

Int_t ORFlashCamConfigDecoder::GetEventListID(UInt_t* record){
  return (Int_t) record[2];
}

Int_t ORFlashCamConfigDecoder::GetNADCChannels(UInt_t* record){
  return (Int_t) record[3];
}

Int_t ORFlashCamConfigDecoder::GetNTriggerChannels(UInt_t* record){
  return (Int_t) record[4];
}

Int_t ORFlashCamConfigDecoder::GetWaveformSamples(UInt_t* record){
  return (Int_t) record[5];
}

Int_t ORFlashCamConfigDecoder::GetNADCBits(UInt_t* record){
  return (Int_t) record[6];
}

Int_t ORFlashCamConfigDecoder::GetIntegratorSamples(UInt_t* record){
  return (Int_t) record[7];
}

Int_t ORFlashCamConfigDecoder::GetBaselinePrecision(UInt_t* record){
  return (Int_t) record[8];
}

Int_t ORFlashCamConfigDecoder::GetNGlobalTriggerCards(UInt_t* record){
  return (Int_t) record[9];
}

Int_t ORFlashCamConfigDecoder::GetNTriggerCards(UInt_t* record){
  return (Int_t) record[10];
}

Int_t ORFlashCamConfigDecoder::GetNADCCards(UInt_t* record){
  return (Int_t) record[11];
}

Int_t ORFlashCamConfigDecoder::GetGPSMode(UInt_t* record){
  return (Int_t) record[12];
}

UShort_t ORFlashCamConfigDecoder::GetADCAddress(UInt_t* record,
						  UInt_t  index){
  if(index >= kMaxADCChannels || (index+13) >= LengthOf(record)) return 0;
  return (UShort_t) ((record[index+13] & 0xffff0000) >> 16);
}

UShort_t ORFlashCamConfigDecoder::GetInputNumber(UInt_t* record,
						   UInt_t  index){
  if(index >= kMaxADCChannels || (index+13) >= LengthOf(record)) return 0;
  return (UShort_t) (record[index+13] & 0x0000ffff);
}

void ORFlashCamConfigDecoder::Dump(UInt_t* record){
  ORLog(kRoutine)<<"********* Dumping FlashCam Listener Config Data *********"
		 << endl;
  ORLog(kRoutine)<<"Readout ID:        "<<GetReadoutID(record)         << endl;
  ORLog(kRoutine)<<"Listener ID:       "<<GetListenerID(record)        << endl;
  ORLog(kRoutine)<<"Event list ID:     "<<GetEventListID(record)       << endl;
  ORLog(kRoutine)<<"Number ADC  chan:  "<<GetNADCChannels(record)      << endl;
  ORLog(kRoutine)<<"Number trig chan:  "<<GetNTriggerChannels(record)  << endl;
  ORLog(kRoutine)<<"Waveform samples:  "<<GetWaveformSamples(record)   << endl;
  ORLog(kRoutine)<<"Bits / sample:     "<<GetNADCBits(record)          << endl;
  ORLog(kRoutine)<<"Integrator length: "<<GetIntegratorSamples(record) << endl;
  ORLog(kRoutine)<<"BL precision:      "<<GetBaselinePrecision(record) << endl;
  ORLog(kRoutine)<<"Global trig cards: "<<GetNGlobalTriggerCards(record)<<endl;
  ORLog(kRoutine)<<"Trigger cards:     "<<GetNTriggerCards(record)     << endl;
  ORLog(kRoutine)<<"ADC cards:         "<<GetNADCCards(record)         << endl;
  ORLog(kRoutine)<<"GPS mode:          "<<GetGPSMode(record)           << endl;
  ORLog(kRoutine)<<"Trace map (addr:input):"                           << endl;
  string m = "";
  for(unsigned int i=0; i<min((UInt_t) kMaxADCChannels,
			      LengthOf(record)-12); i++){
    UShort_t addr = GetADCAddress(record, i);
    UShort_t chan = GetInputNumber(record, i);
    if(addr == 0 && chan == 0) continue;
    stringstream sstream;
    sstream << hex << addr;
    m += "0x" + sstream.str() + ":" + to_string(chan) + ",";
  }
  if(m.size() > 0)
    ORLog(kRoutine) << m.substr(0, m.size()-1) << endl;
  ORLog(kRoutine) << "********************************************************"
		  << endl;
}
