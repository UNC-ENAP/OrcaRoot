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

void ORGretina4ADecoder::SetDecoderDictionary(const ORDecoderDictionary* dict)
{
  ORVDataDecoder::SetDecoderDictionary(dict);
  for(UInt_t iPar = 0; iPar < kNPars; iPar++) fCardPars[iPar].clear();
}

UInt_t ORGretina4ADecoder::GetParameter(EParameters par, UInt_t crate, UInt_t card, UInt_t channel)
{
  if(par >= kNPars) return 0;
  
  UInt_t ccc = (crate << 12) + (card << 4) + channel; // 0xfffff000 + 0xff0 + 0xf
  map<UInt_t, UInt_t>::iterator iter = fCardPars[par].find(ccc);
  
  //return if found
  if(iter != fCardPars[par].end()) return iter->second;
  
  // otherwise we have to look up the info from the header
  // first find a key
  const char* key = "";
  switch(par) {
  case kDSFact : key = "decimationFactor"; break;

  default:
    ORLog(kError) << "Unknown card parameter " << par << endl;
    return (UInt_t) -1;
  }

  // now pull out the value for the key
  UInt_t value = 0;
  const ORVDictValue* val = GetValueFromKey(key, crate, card);
  if(!val)
    value = 0;
  else if(val->IsA(ORVDictValue::kInt))
    value = static_cast<const ORDictValueI*>(val)->GetI();
  else if(val->IsA(ORVDictValue::kArray)) {
    const ORDictValueA* arr = static_cast<const ORDictValueA*>(val);
    if(channel > arr->GetNValues() || !arr->At(channel)->IsA(ORVDictValue::kInt)) value = 0;
    else value = static_cast<const ORDictValueI*>(arr->At(channel))->GetI();
  }
  
  // cache locally and return
  fCardPars[par][ccc] = value;
  return value;
}
