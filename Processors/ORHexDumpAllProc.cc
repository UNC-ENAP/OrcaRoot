// ORHexDumpAllProc.cc

#include "ORHexDumpAllProc.hh"
#include "ORLogger.hh"
#include "ORRunContext.hh"
#include "ORBasicDataDecoder.hh"
#include <map>
#include <string>

using namespace std;

ORHexDumpAllProc::ORHexDumpAllProc() : ORDataProcessor(NULL)
{ 
  fDataDecoder = new ORBasicDataDecoder;
  fCheckLimits=false;
  fBegin=0;
  fEnd=0;
  fLineLength=0;
}

ORHexDumpAllProc::~ORHexDumpAllProc()
{
  delete fDataDecoder;
}

ORHexDumpAllProc::EReturnCode ORHexDumpAllProc::ProcessDataRecord(UInt_t* record)
{
  if (!fDoProcess || !fDoProcessRun || !fRunContext) return kFailure;
  if (fCheckLimits && ((fRunContext->GetPacketNumber() < fBegin) || (fEnd>=0 && fRunContext->GetPacketNumber() > fEnd))) return kSuccess;
  if (!fPacketList.empty() && !fPacketList.count(fRunContext->GetPacketNumber())) return kSuccess; //if list exists and not in list, return

  if(fRunContext->MustSwap() && !fRunContext->IsRecordSwapped()) {
    /* Swapping the record.  This only must be done once! */
    fDataDecoder->Swap(record);
    fRunContext->SetRecordSwapped();
  }

  if(fIDMap.size() == 0) return kAlarm;
  UInt_t dataID = fDataDecoder->DataIdOf(record);
  string deviceName = fIDMap[dataID];
  if(deviceName == "") deviceName = "UNKNOWN";

  if(!fDeviceList.empty() && !fDeviceList.count(deviceName)) return kSuccess;

  ORLog(kRoutine) << "Packet Number " << fRunContext->GetPacketNumber() 
                  << ": " << deviceName << " (dataID = " << dataID << ")" << endl;
  fDataDecoder->DumpHex(record, fLineLength);

  return kSuccess;
}

ORDataProcessor::EReturnCode ORHexDumpAllProc::StartRun()
{
  fIDMap.clear();
  ORDictionary* dataDescDict = (ORDictionary*) fRunContext->GetHeader()->LookUp("dataDescription");
  if(dataDescDict == NULL) {
    ORLog(kError) << "no dataDescDescription?" << endl;
    return kAlarm;
  }
  ORDictionary::DictMap dataDescMap = dataDescDict->GetDictMap();
  for(ORDictionary::DictMap::iterator i = dataDescMap.begin(); i != dataDescMap.end(); i++) {
    string deviceName = i->first;
    ORDictionary* deviceDict = (ORDictionary*) i->second;
    ORDictionary::DictMap deviceDictMap = deviceDict->GetDictMap();
    for(ORDictionary::DictMap::iterator j = deviceDictMap.begin(); j != deviceDictMap.end(); j++) {
      ORDictValueI* dataIDDVI = (ORDictValueI*) ((ORDictionary*) j->second)->LookUp("dataId");
      if(dataIDDVI == NULL) {
        ORLog(kError) << "no dataId for " << deviceName << ":" << j->first << endl;
        deviceName.clear();
        return kAlarm;
      }
      fIDMap[dataIDDVI->GetI()] = deviceName + ":" + j->first;
    }
  }
  return kSuccess;
}

