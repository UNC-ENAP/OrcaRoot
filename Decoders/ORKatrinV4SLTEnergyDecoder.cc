// ORKatrinV4SLTEnergyDecoder.cc

#include "ORKatrinV4SLTEnergyDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"


//**************************************************************************************

ORKatrinV4SLTEnergyDecoder::ORKatrinV4SLTEnergyDecoder() { fDataRecord = NULL; fEventNum = 0; }

bool ORKatrinV4SLTEnergyDecoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;
  fEventNum = (  LengthOf(fDataRecord) - kBufHeadLen) /  kEventLength ;//this sets GetWaveformLen() -tb-//TODO: check it -tb-

  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << std::endl;
  // remarks for Till: LengthOf(...) is from ORVDataDecoder and is the length extracted from data record -tb-
  if(!IsValid() || LengthOf(fDataRecord) != kBufHeadLen + fEventNum * kEventLength) {
    ORLog(kDebug) << "SetDataRecord(): data record is not valid" << std::endl;
    ORLog(kDebug) << "LengthOf(data record) : " << LengthOf(fDataRecord)
      << " kBufHeadLen + fEventNum * kEventLength: " << kBufHeadLen + GetEventNum() * kEventLength << std::endl;
    fDataRecord = NULL;
    fEventNum = 0;
    return false;
  }
  ORLog(kDebug) << "SetDataRecord(): Exiting" << std::endl;
  return true;
}

bool ORKatrinV4SLTEnergyDecoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << std::endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << std::endl; 
    return false;
  } 
  return true;
}

void ORKatrinV4SLTEnergyDecoder::DumpBufferHeader()
{
  if(fDataRecord)
  {
    ORLog(kDebug) << "Dumping Data Buffer Header (Raw Data): " << std::endl;
    ORLog(kDebug) << "**************************************************" << std::endl;
    for(size_t i=2;i<kBufHeadLen; i++)
    {
      ORLog(kDebug) << fDataRecord[i] << std::endl;
    }
    ORLog(kDebug) << "**************************************************" << std::endl;
  }
}


void ORKatrinV4SLTEnergyDecoder::Dump(UInt_t* dataRecord) //debugging 
{
  ORLog(kDebug) << std::endl << std::endl << "ORKatrinV4SLTEnergyDecoder::Dump():" << std::endl ;
  if(!SetDataRecord(dataRecord)) return; 
  
  for (size_t i = 0; i < GetEventNum(); i++) {
  
      ORLog(kDebug) 
        << "  event index : " << i << std::endl
        << "  SLT event functions : " << std::endl
        << "    Crate = " << CrateOf() << "; card = " << CardOf() << std::endl
        << "    The buffer is " << kBufHeadLen << " (32-bit) words long" << std::endl
        << "    The Sec is " << GetSec(i) << std::endl
        << "    The SubSec is " << GetSubSec(i) << std::endl
        << "    The channel is " << GetChannel(i) << std::endl
        << "    EventID: " << GetEventID(i) << std::endl  
        << "    Energy: " << GetEnergy(i) << std::endl  
        << "    PeakADC: " << GetPeakADC(i) << std::endl  
        << "    ValleyADC: " << GetValleyADC(i) << std::endl  
        << "    PeakPos: " << GetPeakPos(i) << std::endl  
        << "    ValleyPos: " << GetValleyPos(i) << std::endl  
        //<< "    EventFlags: " << GetEventFlags() << std::endl        // -tb- 2010-02-16
        //<< "    EventInfo: " << GetEventInfo() << std::endl  // -tb- 2010-02-16
        << " (32-bit) words" << std::endl;
      
  }
}
