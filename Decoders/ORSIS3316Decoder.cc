#include "ORSIS3316Decoder.hh"
#include "ORLogger.hh"

using namespace std;

bool ORSIS3316Decoder::SetDataRecord(UInt_t* dataRecord) {
  fDataRecord = dataRecord;
  size_t reclen = fDataRecord[3], headlen = fDataRecord[5];
  
  fEventRecords.assign(GetNumberOfEvents(), {0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

  UInt_t* curptr = fDataRecord+kORCAHeaderLen;
  for(auto& event : fEventRecords) {
    event.start = curptr;
    curptr+=2;
    if(event.start[0] & 0x1) {
      event.header0 = curptr;
      curptr+=7;
    } else
      event.header0=0;

    if(event.start[0] & 0x2) {
      event.header1 = curptr;
      curptr+=2;
    } else
      event.header1=0;

    if(event.start[0] & 0x4) {
      event.header2 = curptr;
      curptr+=3;
    } else
      event.header2=0;

    if(event.start[0] & 0x8) {
      event.header3 = curptr;
      curptr+=2;
    } else
      event.header3=0;
    
    event.WFLen = curptr;
    curptr++;
    if(event.WFLen[0] >> 28 == 0xa) {
      event.AveLen = curptr;
      curptr++;
    } else
      event.AveLen=0;

    if(curptr != event.start+headlen) {
      ORLog(kError) << "ORSIS3316Decoder::SetDataRecord: actual header length does not match expected header length" << endl;
      return false;
    }
    
    event.WFRecord = curptr;
    curptr += (event.WFLen[0] & 0x2ffffff);

    if(event.AveLen!=0) {
      event.AveRecord = curptr;
      curptr += (event.AveLen[0] & 0xffff);
    } else
      event.AveRecord=0;

    if(event.WFLen[0]>>27 & 0x1) {
      event.MAWTest = curptr;
      curptr = event.start + reclen;
    } else if(curptr != event.start + reclen) {
      ORLog(kError) << "ORSIS3316Decoder::SetDataRecord: actual wf length does not match expected wf length" << endl;
      return false;
    }
  }
  return true;
}
