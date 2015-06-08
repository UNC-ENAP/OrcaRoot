// ORHexDumpAllProc.hh

#ifndef _ORHexDumpAllProc_hh_
#define _ORHexDumpAllProc_hh_

#include "ORDataProcessor.hh"
#include <set>
#include <istream>

class ORHexDumpAllProc : public ORDataProcessor
{
  public:
    ORHexDumpAllProc();
    virtual ~ORHexDumpAllProc();

    virtual void SetDataId() {}
    EReturnCode ProcessDataRecord(UInt_t* record);
    virtual void SetLimits(Int_t begin, Int_t end=-1) {
      fBegin=begin;
      fEnd=end;
      fCheckLimits=true;
    }
    virtual void SetLineLength(UInt_t linelength) {
      fLineLength = linelength;
    }
    virtual void SetPacketList(const std::set<Int_t>& list) {
      fPacketList = list;
    }
    virtual void SetPacketList(std::istream& list) {
      Int_t pkt;
      while(list >> pkt) fPacketList.insert(pkt);
    }

  private:
    std::map<int, std::string> MakeIDMap();

  protected:
    bool fCheckLimits;
    Int_t fBegin;
    Int_t fEnd; //if fEnd<0, output until end of file
    UInt_t fLineLength;
    std::set<Int_t> fPacketList;
};

#endif
