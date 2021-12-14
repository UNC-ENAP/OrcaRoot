// ORFLashCamStatusDumper.hh

#include "ORFlashCamStatusDumper.hh"

ORFlashCamStatusDumper::ORFlashCamStatusDumper() :
  ORDataProcessor(new ORFlashCamStatusDecoder){
  fDecoder = dynamic_cast<ORFlashCamStatusDecoder*>(fDataDecoder);
}

ORFlashCamStatusDumper::~ORFlashCamStatusDumper(){
  delete fDecoder;
}

ORDataProcessor::EReturnCode
ORFlashCamStatusDumper::ProcessMyDataRecord(UInt_t* record){
  fDecoder->Dump(record);
  return kSuccess;
}
