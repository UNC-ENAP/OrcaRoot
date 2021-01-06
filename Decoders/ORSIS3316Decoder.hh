#ifndef _ORSIS3316Decoder_hh_
#define _ORSIS3316Decoder_hh_

#include "ORVDigitizerDecoder.hh"
#include <vector>

/*
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
 ^^^^ ^^^^ ^^^^ ^^----------------------- Data ID (from header)
 -----------------^^ ^^^^ ^^^^ ^^^^ ^^^^- length
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
 --------^-^^^--------------------------- Crate number
 -------------^-^^^^--------------------- Card number
 --------------------^^^^ ^^^^----------- Chan number
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  Num Records in this record
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  Num longs in each record
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  Num of Records that were in the FIFO
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  Num of longs in data header -- can get from the raw data also
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  Spare
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  Spare
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  Spare
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  Spare

 N Data Records follow with format described in manual (NOTE THE FORMAT BITS)

 This is a basic implementation that ignores many features of the data records
*/


class ORSIS3316Decoder : public ORVDigitizerDecoder {
public:
  struct recordptrs {
    UInt_t* start; // start of event record
    UInt_t* header0; // first optional header block
    UInt_t* header1; // second optional header block
    UInt_t* header2; // third optional header block
    UInt_t* header3; // fourth optional header block; contains energy
    UInt_t* WFLen; // len of wf + other info
    UInt_t* AveLen; // len of Ave wf
    UInt_t* WFRecord; // wf samples
    UInt_t* AveRecord; // ave samples
    UInt_t* MAWTest; // MAW test
  };
  
  ORSIS3316Decoder() { fEventRecords.reserve(16); }
  virtual ~ORSIS3316Decoder() {}
  
  /* Basic functions. */
  virtual std::string GetDataObjectPath()
  { return "ORSIS3316Model:Waveform"; }
  
  //! Should return in units of 1 GHz. 
  virtual double GetSamplingFrequency() { return 0.125; }
  virtual UShort_t GetBitResolution() { return 16; }
  virtual bool SetDataRecord(UInt_t* dataRecord);

  /* Event Functions */
  
  //! It is possible to have more than one event in a record. 
  virtual size_t GetNumberOfEvents()
  { return fDataRecord[2]; }
  virtual size_t GetNumberOfEventsInFIFO()
  { return fDataRecord[4]; }
  virtual ULong64_t GetEventTime(size_t i)
  { return fEventRecords[i].start[1] + (fEventRecords[i].start[0] << 16); }
  virtual UInt_t GetEventEnergy(size_t i)
  { return (fEventRecords[i].header3!=0 ? fEventRecords[i].header3[1] : 0); }
  virtual UShort_t GetEventChannel(size_t i)
  { return (fEventRecords[i].start[0] >> 4) & 0xfff; }

  /* Now waveforms */ 
  
  //! This length should be in number of words.
  virtual size_t GetEventWaveformLength(size_t i)
  { return fEventRecords[i].WFLen[0] & 0x2ffffff; }
  //! Gets a point in the waveform.
  virtual UInt_t GetEventWaveformPoint( size_t i, size_t waveformPoint )
  { if(waveformPoint%2==0)
      return fEventRecords[i].WFRecord[waveformPoint/2] & 0xffff;
    else
      return fEventRecords[i].WFRecord[waveformPoint/2] >> 16;
  }
  /* Overload the following function if the data coming from the waveform
   * is actually unsigned.    */
  virtual Bool_t WaveformDataIsSigned() { return true; }
  
protected:
  std::vector<recordptrs> fEventRecords;
  
  const size_t kORCAHeaderLen = 10;
};

#endif
