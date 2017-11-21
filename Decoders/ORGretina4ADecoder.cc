// ORGretina4ADecoder.cc

#include "ORGretina4ADecoder.hh"
#include "ORLogger.hh"

using namespace std;

bool ORGretina4ADecoder::SetDataRecord(UInt_t* dataRecord) 
{
  fEvPtrs.clear();
  fWFPtrs.clear();
  if(IsShort(dataRecord)) {
    ORLog(kDebug) << "SetDataRecord(): data record is short form, invalid" << endl;
    fDataRecord = NULL;
    return false;
  }
  fDataRecord = dataRecord;

  size_t pos = 2;
  while(pos < LengthOf(fDataRecord)) {
    UInt_t* ptr = fDataRecord+pos;
    fEvPtrs.push_back(ptr);
    fWFPtrs.push_back((Short_t*)(ptr + size_t(GetHeaderLength(ptr))));
    pos += GetPacketLength(ptr);
  }

  if(pos != LengthOf(fDataRecord)) {
    ORLog(kDebug) << "SetDataRecord(): total length of packets does not match length of record, invalid" << endl;
    fDataRecord = NULL;
    fEvPtrs.clear();
    fWFPtrs.clear();
    return false;
  }
  
  return true;
}
