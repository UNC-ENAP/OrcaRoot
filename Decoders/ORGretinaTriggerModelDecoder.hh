#ifndef _ORGretinaTriggerModelDecoder_hh_
#define _ORGretinaTriggerModelDecoder_hh_

#include "ORVDataDecoder.hh"

//------------------------------------------------------------------------------------------------
// Data Format
// xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
// ^^^^ ^^^^ ^^^^ ^^------------------------data id
//                  ^^ ^^^^ ^^^^ ^^^^ ^^^^--length in longs
//
// xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
// ^^^^ ^^^^ ^^^^ ^^^^ ^^^^ ^^^^ ^^  -------spare bits
//                                  ^-------locked
//                                 ^ -------link was lost
//                                    ^^^^--device id
// xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx--Mac Unix time in seconds since Jan 1,1970 (UT)
// xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
// ^^^^ ^^^^ ^^^^ ^^^^ ---------------------spare bits
//                     ^^^^ ^^^^ ^^^^ ^^^^--timeStampA
// xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
// ^^^^ ^^^^ ^^^^ ^^^^ ---------------------timeStampB
//                     ^^^^ ^^^^ ^^^^ ^^^^--timeStampC
//-----------------------------------------------------------------------------------------------

class ORGretinaTriggerModelDecoder : public ORVDataDecoder {
  public:
    ORGretinaTriggerModelDecoder() {}
    virtual ~ORGretinaTriggerModelDecoder() {}

    virtual std::string GetDataObjectPath() { return "GretinaTriggerModel:Master"; }
    virtual std::string GetDictionaryObjectPath() { return "ORGretinaTriggerModel"; }

    virtual UInt_t GetDeviceID(UInt_t* record) { return record[1] & 0xf; }
    virtual Bool_t LinkWasLost(UInt_t* record) { return record[1] & 0x20; }
    virtual Bool_t IsLocked(UInt_t* record) { return record[1] & 0x10; }
    virtual UInt_t GetUnixTime(UInt_t* record) { return record[2]; }
    virtual ULong64_t GetTimeStamp(UInt_t* record);
};

#endif
