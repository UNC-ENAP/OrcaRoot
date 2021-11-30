// ORFlashCamListenerDumper.cc

#include "ORFlashCamListenerDumper.hh"

ORFlashCamListenerDumper::ORFlashCamListenerDumper() :
ORDataProcessor(new ORFlashCamListenerDecoder){
  fDecoder = dynamic_cast<ORFlashCamListenerDecoder*>(fDataDecoder);
}

ORFlashCamListenerDumper::~ORFlashCamListenerDumper(){
  delete fDecoder;
}

ORDataProcessor::EReturnCode
ORFlashCamListenerDumper::ProcessMyDataRecord(UInt_t* record){
  fDecoder->Dump(record);
  return kSuccess;
}
