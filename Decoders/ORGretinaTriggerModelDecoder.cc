//ORGretinaTriggerModelDecoder.cc

#include "ORGretinaTriggerModelDecoder.hh"

ULong64_t ORGretinaTriggerModelDecoder::GetTimeStamp(UInt_t* record) {
  ULong64_t ret = record[3];
  ret <<= 32;
  ret += record[4];
  return ret;
}
