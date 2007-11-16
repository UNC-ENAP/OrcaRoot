// ORDictionary.cc

#include "ORDictionary.hh"
#include "ORLogger.hh"
#include <sstream> 

using namespace std;

ORDictionary::ORDictionary(const ORDictionary& dict) : ORVDictValue(dict)
{
  SetName(dict.fName);
  /* Now the hard part, copying the dictionary map correctly calling 
     all the copy constructors. */ 
  std::map<const std::string, ORVDictValue*>::const_iterator dictIter;
  for (dictIter = dict.fDictMap.begin();dictIter!=dict.fDictMap.end();dictIter++) {
    EValType type = dictIter->second->GetValueType(); 
    ORVDictValue* insertDictValue = NULL;
    switch (type) {
      case kDict:
        insertDictValue = new ORDictionary((*(ORDictionary*)dictIter->second)); 
        break;
      case kString: 
        insertDictValue = new ORDictValueS((*(ORDictValueS*)dictIter->second)); 
        break;
      case kReal: 
        insertDictValue = new ORDictValueR((*(ORDictValueR*)dictIter->second)); 
        break;
      case kInt: 
        insertDictValue = new ORDictValueI((*(ORDictValueI*)dictIter->second)); 
        break;
      case kBool: 
        insertDictValue = new ORDictValueB((*(ORDictValueB*)dictIter->second)); 
        break;
      case kArray: 
        insertDictValue = new ORDictValueA((*(ORDictValueA*)dictIter->second)); 
        break;
    }
    if (insertDictValue) {
      LoadEntry(dictIter->first, insertDictValue);
    } else {
      ORLog(kError) << "DictValue type not found!" << endl;
    }
  }
}

ORDictionary::~ORDictionary()
{
  map<const std::string, ORVDictValue*>::iterator i;
  for (i = fDictMap.begin(); i != fDictMap.end(); i++) {
    delete i->second;
  }
}

ORVDictValue* ORDictionary::LookUp(string key, char delimiter) 
{
  static std::map<const std::string, ORVDictValue*>::const_iterator dictIter;
  size_t delimPos = key.find(delimiter); 
  if (delimPos == string::npos) {
    dictIter = fDictMap.find(key); 
    if (dictIter==fDictMap.end()) {
      ORLog(kDebug) << "ORDictionary::LookUp(): could not find key " << key
                      << " in dictionary " << fName << endl;
      return NULL;
    }
    return dictIter->second; 
  } else {
    string nextDictName = key.substr(0, delimPos);
    string nextKeyName = key.substr(delimPos+1);
    const ORVDictValue* nextDict = LookUp(nextDictName);
    if (nextDict == NULL) return NULL;
    if (nextDict->GetValueType() != kDict) {
      ORLog(kDebug) << "ORDictionary::LookUp(): key " << nextDictName 
                    << " in dictionary " << fName << " is not a dictionary (ValueType = " 
		    << nextDict->GetValueType() << ")." << endl;
      return NULL;
    }
    return ((ORDictionary*) nextDict)->LookUp(nextKeyName);
  }
}

const ORVDictValue* ORDictionary::LookUp(string key, char delimiter) const 
{
  static std::map<const std::string, ORVDictValue*>::const_iterator dictIter;
  size_t delimPos = key.find(delimiter); 
  if (delimPos == string::npos) {
    dictIter = fDictMap.find(key); 
    if (dictIter==fDictMap.end()) {
      ORLog(kDebug) << "ORDictionary::LookUp(): could not find key " << key
                      << " in dictionary " << fName << endl;
      return NULL;
    }
    return (const ORVDictValue*) dictIter->second; 
  } else {
    string nextDictName = key.substr(0, delimPos);
    string nextKeyName = key.substr(delimPos+1);
    const ORVDictValue* nextDict = LookUp(nextDictName);
    if (nextDict == NULL) return NULL;
    if (nextDict->GetValueType() != kDict) {
      ORLog(kDebug) << "ORDictionary::LookUp(): key " << nextDictName 
                    << " in dictionary " << fName << " is not a dictionary (ValueType = " 
		    << nextDict->GetValueType() << ")." << endl;
      return NULL;
    }
    return ((const ORDictionary*) nextDict)->LookUp(nextKeyName);
  }
}

std::string ORDictValueI::GetStringOfValue() const
{
  std::ostringstream o;
  if (! (o << fI)) return "ERROR";
  return o.str();
}

std::string ORDictValueR::GetStringOfValue() const
{
  std::ostringstream o;
  if (! (o << fR)) return "ERROR";
  return o.str();
}

ORDictValueA::ORDictValueA(const ORDictValueA& dictA) : ORVDictValue(dictA)
{
  SetName(dictA.fName);
  /* Now the hard part, copying the dictionary map correctly calling 
     all the copy constructors. */ 
  for (size_t i = 0; i<dictA.fDictVals.size();i++) { 
    EValType type = dictA.fDictVals[i]->GetValueType(); 
    ORVDictValue* insertDictValue = NULL;
    switch (type) {
      case kDict:
        insertDictValue = new ORDictionary((*(ORDictionary*)dictA.fDictVals[i])); 
        break;
      case kString: 
        insertDictValue = new ORDictValueS((*(ORDictValueS*)dictA.fDictVals[i])); 
        break;
      case kReal: 
        insertDictValue = new ORDictValueR((*(ORDictValueR*)dictA.fDictVals[i])); 
        break;
      case kInt: 
        insertDictValue = new ORDictValueI((*(ORDictValueI*)dictA.fDictVals[i])); 
        break;
      case kBool: 
        insertDictValue = new ORDictValueB((*(ORDictValueB*)dictA.fDictVals[i])); 
        break;
      case kArray: 
        insertDictValue = new ORDictValueA((*(ORDictValueA*)dictA.fDictVals[i])); 
        break;
    }
    if (insertDictValue) {
      LoadValue(insertDictValue);
    } else {
      ORLog(kError) << "DictValue type not found!" << endl;
    }
  }
}

ORDictValueA::~ORDictValueA()
{
  for (unsigned i=0; i < fDictVals.size(); i++) delete fDictVals[i];
}

