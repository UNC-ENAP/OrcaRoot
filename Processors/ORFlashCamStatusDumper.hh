// ORFlashCamStatusDumper.hh

#ifndef _ORFlashCamStatusDumper_hh_
#define _ORFlashCamStatusDumper_hh_

#include "ORDataProcessor.hh"
#include "ORFlashCamStatusDecoder.hh"

class ORFlashCamStatusDumper : public ORDataProcessor {
public:
  ORFlashCamStatusDumper();
  virtual ~ORFlashCamStatusDumper();

  virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

protected:
  ORFlashCamStatusDecoder* fDecoder;
};

#endif
