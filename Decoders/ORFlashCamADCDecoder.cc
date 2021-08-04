#include "ORFlashCamADCDecoder.hh"
#include "ORLogger.hh"

using namespace std;

bool ORFlashCamADCDecoder::SetDataRecord(UInt_t* dataRecord){
  if(IsShort(dataRecord)){
    ORLog(kDebug)<<"SetDataRecord(): data record is short form, invalid"<<endl;
    fDataRecord = NULL;
    return false;
  }
  fDataRecord = dataRecord;
  return true;
}

UInt_t ORFlashCamADCDecoder::CrateOf(){
  return (fDataRecord[2] & 0xf8000000) >> 27;
}

UInt_t ORFlashCamADCDecoder::CardOf(){
  return (fDataRecord[2] & 0x07c00000) >> 22;
}

ULong64_t ORFlashCamADCDecoder::GetEventTime(size_t event){
  if(event >= GetNumberOfEvents()) return 0;
  return ((ULong64_t) fDataRecord[14]) * 1e9 + (ULong64_t) fDataRecord[15] * 4;
}

UInt_t ORFlashCamADCDecoder::GetEventEnergy(size_t event){
  if(event >= GetNumberOfEvents()) return 0;
  UInt_t* wfstart = GetWaveformStart(event);
  return ((--wfstart)[0] & 0xffff0000) >> 16;
}

UInt_t ORFlashCamADCDecoder::GetEventBaseline(size_t event){
  if(event >= GetNumberOfEvents()) return 0;
  UInt_t* wfstart = GetWaveformStart(event);
  return (--wfstart)[0] & 0x0000ffff;
}

UShort_t ORFlashCamADCDecoder::GetEventChannel(size_t event){
  if(event >= GetNumberOfEvents()) return 0;
  return UShort_t((CrateOf() << 12) +
		  (CardOf()  <<  4) + ((fDataRecord[2] & 0x00003c00) >> 10));
}

size_t ORFlashCamADCDecoder::GetEventWaveformLength(size_t event){
  if(event >= GetNumberOfEvents()) return 0;
  return (((fDataRecord[1] & 0x003fffc0) >> 6) + 1) / 2;
}

UInt_t* ORFlashCamADCDecoder::GetWaveformStart(size_t event){
  if(event >= GetNumberOfEvents()) return NULL;
  UInt_t offset = (fDataRecord[1] & 0xf0000000) >> 28; // orca  header length
  offset       += (fDataRecord[1] & 0x0fc00000) >> 22; // fc wf header length
  return fDataRecord + offset;
}

UInt_t ORFlashCamADCDecoder::GetEventWaveformPoint(size_t event,
						   size_t waveformPoint){
  if(event >= GetNumberOfEvents()) return 0;
  return GetWaveformStart(event)[waveformPoint];
}
