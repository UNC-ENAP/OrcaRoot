#include "ORFlashCamStatusDecoder.hh"
#include "ORLogger.hh"
#include <sstream>

using namespace std;

ORFlashCamStatusDecoder::ORFlashCamStatusDecoder(){
  fBaseOffset = 14 + kOtherErrors + kEnvMonitors + kCTILinks;
}

UInt_t ORFlashCamStatusDecoder::GetReadoutID(UInt_t* record){
  return (record[1] & 0xffff0000) >> 16;
}

UInt_t ORFlashCamStatusDecoder::GetListenerID(UInt_t* record){
  return record[1] & 0x0000ffff;
}

Int_t ORFlashCamStatusDecoder::GetErrorStatus(UInt_t* record){
  return (Int_t) record[2];
}

Int_t ORFlashCamStatusDecoder::GetFC250Sec(UInt_t* record){
  return (Int_t) record[3];
}

Int_t ORFlashCamStatusDecoder::GetFC250uSec(UInt_t* record){
  return (Int_t) record[4];
}

Int_t ORFlashCamStatusDecoder::GetCPUSec(UInt_t* record){
  return (Int_t) record[5];
}

Int_t ORFlashCamStatusDecoder::GetCPUuSec(UInt_t* record){
  return (Int_t) record[6];
}

Int_t ORFlashCamStatusDecoder::GetStartuSec(UInt_t* record){
  return (Int_t) record[8];
}

Int_t ORFlashCamStatusDecoder::GetStopuSec(UInt_t* record){
  return (Int_t) record[9];
}

Int_t ORFlashCamStatusDecoder::GetNCards(UInt_t* record){
  return (Int_t) record[13];
}

UInt_t ORFlashCamStatusDecoder::GetCardFCIOID(UInt_t* record, UInt_t index){
  return record[CardOffset(record, index)];
}

UInt_t ORFlashCamStatusDecoder::GetCardStatus(UInt_t* record, UInt_t index){
  return record[CardOffset(record, index)+1];
}

UInt_t ORFlashCamStatusDecoder::GetCardEventNumber(UInt_t* record, UInt_t index){
  return record[CardOffset(record, index)+2];
}

UInt_t ORFlashCamStatusDecoder::GetCardPPSCount(UInt_t* record, UInt_t index){
  return record[CardOffset(record, index)+3];
}

UInt_t ORFlashCamStatusDecoder::GetCardTickCount(UInt_t* record, UInt_t index){
  return record[CardOffset(record, index)+4];
}

UInt_t ORFlashCamStatusDecoder::GetCardMaxTicks(UInt_t* record, UInt_t index){
  return record[CardOffset(record, index)+5];
}

UInt_t ORFlashCamStatusDecoder::GetCardTotalErrors(UInt_t* record, UInt_t index){
  return record[CardOffset(record, index)+10];
}

UInt_t ORFlashCamStatusDecoder::GetCardEnvErrors(UInt_t* record, UInt_t index){
  return record[CardOffset(record, index)+11];
}

UInt_t ORFlashCamStatusDecoder::GetCardCTIErrors(UInt_t* record, UInt_t index){
  return record[CardOffset(record, index)+12];
}

UInt_t ORFlashCamStatusDecoder::GetCardLinkErrors(UInt_t* record, UInt_t index){
  return record[CardOffset(record, index)+13];
}

UInt_t ORFlashCamStatusDecoder::GetCardOtherErrors(UInt_t* record,
						   UInt_t index, UInt_t type){
  if(type >= kOtherErrors) return (UInt_t) -1;
  return record[CardOffset(record, index)+14+type];
}

UInt_t ORFlashCamStatusDecoder::GetCardTemp(UInt_t* record,
					    UInt_t index, UInt_t ti){
  if(ti >= kCardTemps) return 0;
  return record[CardOffset(record, index)+14+kOtherErrors+ti];
}

UInt_t ORFlashCamStatusDecoder::GetCardVoltage(UInt_t* record,
					       UInt_t index, UInt_t vi){
  if(vi >= kCardVoltages) return 0;
  return record[CardOffset(record, index)+14+kOtherErrors+kCardTemps+vi];
}

UInt_t ORFlashCamStatusDecoder::GetCardCurrent(UInt_t* record, UInt_t index){
  return record[CardOffset(record, index)+14+
		kOtherErrors+kCardTemps+kCardVoltages];
}

UInt_t ORFlashCamStatusDecoder::GetCardHumidity(UInt_t* record, UInt_t index){
  return record[CardOffset(record, index)+14+
		kOtherErrors+kCardTemps+kCardVoltages+1];
}

UInt_t ORFlashCamStatusDecoder::GetCardADCTemp(UInt_t* record,
					       UInt_t index, UInt_t ti){
  if(ti >= kADCTemps) return 0;
  return record[CardOffset(record, index)+14+
		kOtherErrors+kCardTemps+kCardVoltages+2+ti];
}

UInt_t ORFlashCamStatusDecoder::GetCardCTILink(UInt_t* record,
					       UInt_t index, UInt_t link){
  if(link >= kCTILinks) return (UInt_t) -1;
  return record[CardOffset(record, index)+14+kOtherErrors+kEnvMonitors+link];
}

UInt_t ORFlashCamStatusDecoder::GetCardLinkState(UInt_t* record,
						 UInt_t index, UInt_t card){
  if(card >= (UInt_t) GetNCards(record)){
    ORLog(kRoutine) <<	"card index " << index << " out of range for "
		    <<	"maximum of " << GetNCards(record) << " cards" << endl;
    return 0;
  }
  return record[CardOffset(record, index)+fBaseOffset+card];
}

size_t ORFlashCamStatusDecoder::CardOffset(UInt_t* record, UInt_t index){
  if(index >= (UInt_t) GetNCards(record)){
     ORLog(kRoutine) << "card index " << index << " out of range for "
		     << "maximum of " << GetNCards(record) << " cards" << endl;
     return 0;
  }
  // 2 orca header, 13 fcio header, then card info
  return 15 + index * (fBaseOffset + (UInt_t) GetNCards(record));
}

void ORFlashCamStatusDecoder::Dump(UInt_t* record){

  ORLog(kRoutine)<<"********* Dumping FlashCam Listener Status Data *********"
		 <<endl;
  ORLog(kRoutine) << "Readout ID:   " << GetReadoutID(record)   << endl;
  ORLog(kRoutine) << "Listener ID:  " << GetListenerID(record)  << endl;
  ORLog(kRoutine) << "Error status: " << GetErrorStatus(record) << endl;
  ORLog(kRoutine) << "FC250  sec:   " << GetFC250Sec(record)    << endl;
  ORLog(kRoutine) << "FC250 usec:   " << GetFC250uSec(record)   << endl;
  ORLog(kRoutine) << "CPU    sec:   " << GetCPUSec(record)      << endl;
  ORLog(kRoutine) << "CPU   usec:   " << GetCPUuSec(record)     << endl;
  ORLog(kRoutine) << "Start usec:   " << GetStartuSec(record)   << endl;
  ORLog(kRoutine) << "Stop  usec:   " << GetStopuSec(record)    << endl;
  ORLog(kRoutine) << "N cards:      " << GetNCards(record)      << endl;
  for(UInt_t i=0; i<(UInt_t)GetNCards(record); i++){
    ORLog(kRoutine)<<"FCIO ID "      <<GetCardFCIOID(record, i) <<":"<< endl;
    ORLog(kRoutine)<<"\tStatus:     "<<GetCardStatus(record, i)      << endl;
    ORLog(kRoutine)<<"\tEvent:      "<<GetCardEventNumber(record, i) << endl;
    ORLog(kRoutine)<<"\tPPS:        "<<GetCardPPSCount(record, i)    << endl;
    ORLog(kRoutine)<<"\tTicks:      "<<GetCardTickCount(record, i)   << endl;
    ORLog(kRoutine)<<"\tTot errors: "<<GetCardTotalErrors(record, i) << endl;
    ORLog(kRoutine)<<"\tEnv errors: "<<GetCardEnvErrors(record, i)   << endl;
    ORLog(kRoutine)<<"\tCTI errors: "<<GetCardCTIErrors(record, i)   << endl;
    for(UInt_t j=0; j<kOtherErrors; j++)
      ORLog(kRoutine)<<"\tOther err"<<j<<": "
		     <<GetCardOtherErrors(record, i, j) << endl;
    for(UInt_t j=0; j<kCardTemps; j++)
      ORLog(kRoutine)<<"\tCard temp"<<j<<": "
		     <<GetCardTemp(record, i, j) << endl;
    for(UInt_t j=0; j<kCardVoltages; j++)
      ORLog(kRoutine)<<"\tCard volt"<<j<<": "
		     <<GetCardVoltage(record, i, j) << endl;
    ORLog(kRoutine)<<"\tCard I:     "<<GetCardCurrent(record, i)     << endl;
    ORLog(kRoutine)<<"\tHumidity:   "<<GetCardHumidity(record, i)    << endl;
    for(UInt_t j=0; j<kADCTemps; j++)
      ORLog(kRoutine)<<"\tADC temp"<<j<<":  "
		     <<GetCardADCTemp(record, i, j) << endl;
    for(UInt_t j=0; j<kCTILinks; j++)
      ORLog(kRoutine)<<"\tCTI link"<<j<<":  "
		     <<GetCardCTILink(record, i, j) << endl;
    string links = "\tLink states: ";
    for(UInt_t j=0; j<(UInt_t)GetNCards(record); j++){
      links += to_string(GetCardLinkState(record, i, j));
      if(j < (UInt_t)GetNCards(record)-1) links += ", ";
    }
    ORLog(kRoutine) << links << endl;
  }
  ORLog(kRoutine)<<"*********************************************************"
		 <<endl;
}
