// ORFlashCamConfigDumper.cc

#include "ORFlashCamConfigDumper.hh"

ORFlashCamConfigDumper::ORFlashCamConfigDumper() :
ORDataProcessor(new ORFlashCamConfigDecoder){
  fDecoder = dynamic_cast<ORFlashCamConfigDecoder*>(fDataDecoder);
}

ORFlashCamConfigDumper::~ORFlashCamConfigDumper(){
  delete fDecoder;
}

ORDataProcessor::EReturnCode
ORFlashCamConfigDumper::ProcessMyDataRecord(UInt_t* record){
  fDecoder->Dump(record);
  return kSuccess;
}
