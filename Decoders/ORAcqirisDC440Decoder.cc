// ORAcqirisDC440Decoder.cc

#include "ORAcqirisDC440Decoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
#include "ORDictionary.hh"
#include <sstream>
using namespace std;


//**************************************************************************************

ORAcqirisDC440Decoder::ORAcqirisDC440Decoder() { fDataRecord = NULL; }

void ORAcqirisDC440Decoder::Swap(UInt_t* dataRecord)
{
  // Given that there is a 16-bit buffer, we have to swap things if there has 
  // been a switch of endian-ness.  This means the data looks like 2143 and it 
  // needs to look like 1234, so we just need to swap the 16-bit words. 
  // The first long word has already been swapped.  There is a header of length
  // kBufHeadLen that needs to be swapped as longs.
  ORLog(kDebug) << "Calling Swap()" << endl; 
  UInt_t lengthOfBuffer = LengthOf(dataRecord);
  if(lengthOfBuffer < 2) return;
  for(size_t i=1;i<kBufHeadLen; i++) {
    ORUtils::Swap(dataRecord[i]);
  }
  UShort_t* theShortDataRecord = (UShort_t*) (dataRecord + kBufHeadLen);
  for(size_t i=0;i<2*(lengthOfBuffer-1);i++) {
    ORUtils::Swap(theShortDataRecord[i]);
  }
}

bool ORAcqirisDC440Decoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;
  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << endl;
  return true;
}

double ORAcqirisDC440Decoder::GetSamplingFrequency()
{
  //FixME this needs to be changed
  return 1.;
}

size_t ORAcqirisDC440Decoder::CopyWaveformData(Short_t* waveform, size_t len)
//copies the waveform data to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << endl;
  }
  else {
    len = GetWaveformLen(); 
  }
  const Short_t* waveformData = GetWaveformDataPointer();
  memcpy(waveform, waveformData, sizeof(waveformData)*len);
  return len;
}

size_t ORAcqirisDC440Decoder::CopyWaveformDataDouble(double* waveform, size_t len)
//copies the waveform data to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << endl;
  }
  else {
    len = GetWaveformLen(); 
  }
  const Short_t* waveformData = GetWaveformDataPointer();
  for(size_t i=0;i<len;i++) 
  {
    waveform[i] = (Double_t) waveformData[i];  
  }
  return len;
}


//Error checking: **********************************************************************


bool ORAcqirisDC440Decoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << endl; 
    return false;
  } 
  return true;
}

void ORAcqirisDC440Decoder::DumpBufferHeader()
{
  if(fDataRecord)
  {
    ORLog(kDebug) << "Dumping Data Buffer Header (Raw Data): " << endl;
    ORLog(kDebug) << "**************************************************" << endl;
    for(size_t i=1;i<kBufHeadLen; i++)
    {
      ORLog(kDebug) << fDataRecord[i] << endl;
    }
    ORLog(kDebug) << "**************************************************" << endl;
  }
}


//debugging: *************************************************************************


void ORAcqirisDC440Decoder::Dump(UInt_t* dataRecord) //debugging 
{
  cout << endl << endl << "ORAcqirisDC440Decoder::Dump():" << endl ;
  if(!SetDataRecord(dataRecord)) return; 
  cout << "  Header functions: " << endl
    << "    Crate = " << CrateOf() << "; card = " << CardOf() << endl
    << "    The buffer is " << kBufHeadLen << " (32-bit) words long" << endl
    << "    The channel is " << GetChannelNum() << endl
    << "    Timestamp: " << GetTimeStamp() << endl
    << "    Index offset: " << GetIndexOffset() << endl
    << "    The waveform data has " << GetWaveformLen() << " (16-bit) words" << endl;
}
