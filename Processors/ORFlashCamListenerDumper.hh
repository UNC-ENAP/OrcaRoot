// ORFlashCamListenerDumper.hh

#ifndef _ORFlashCamListenerDumper_hh_
#define _ORFlashCamListenerDumper_hh_

#include "ORDataProcessor.hh"
#include "ORFlashCamListenerDecoder.hh"

class ORFlashCamListenerDumper : public ORDataProcessor {
public:
  ORFlashCamListenerDumper();
  virtual ~ORFlashCamListenerDumper();

  virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

protected:
  ORFlashCamListenerDecoder* fDecoder;
};

#endif
