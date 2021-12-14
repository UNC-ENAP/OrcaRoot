// ORFlashCamConfigDumper.hh

#ifndef _ORFlashCamConfigDumper_hh_
#define _ORFlashCamConfigDumper_hh_

#include "ORDataProcessor.hh"
#include "ORFlashCamConfigDecoder.hh"

class ORFlashCamConfigDumper : public ORDataProcessor {
public:
  ORFlashCamConfigDumper();
  virtual ~ORFlashCamConfigDumper();

  virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

protected:
  ORFlashCamConfigDecoder* fDecoder;
};

#endif
