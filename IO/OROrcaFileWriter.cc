#include "OROrcaFileWriter.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
#include "ORRunContext.hh"

using namespace std;

OROrcaFileWriter::OROrcaFileWriter(const std::string& label) {
  SetLabel(label);
}

size_t OROrcaFileWriter::Open(ORRunContext* runContext) {
  if(runContext!=NULL) SetRunContext(runContext);

  if(fRunContext==NULL) {
    ORLog(kWarning) << "Open: fRunContext is NULL." << endl;
    return 0;
  }

  string filename = fLabel + ::Form("_run%d", fRunContext->GetRunNumber());
  fOut.open(filename.c_str());

  if(!IsWritable()) {
    ORLog(kError) << "Could not open file " << filename << " to write" << endl;
    return 0;
  }
  return WriteHeader()==0;
}



size_t OROrcaFileWriter::WriteHeader() {
  if(!IsWritable()) {
    ORLog(kWarning) << "WriteHeader: could not write header xml to file." << endl;
    return 0;
  }
  
  const TString& header = fRunContext->GetHeader()->GetRawXML();
  UInt_t word = header.Length()/4 + 2;
  //first word is packet length
  fOut.write((const char*)(&word), 4);
  //second word is length of plist in chars. End is padded with zeros, so do not count these
  word = header.Length();
  while(header[word-1] == 0) word--;
  fOut.write((const char*)(&word), 4);
  fOut.write((const char*)header, header.Length());
  return header.Length() + 8;
}

size_t OROrcaFileWriter::WriteDataRecord(UInt_t* record) {
  if(!IsWritable()) {
    ORLog(kWarning) << "WriteDataRecord: could not write record to file." << endl;
    return 0;
  }    
  
  //unswap records as needed
  if(fRunContext->MustSwap()) ORUtils::Swap(record[0]);
  if(fRunContext->IsRecordSwapped()) {
    fDecoder.Swap(record);
    fRunContext->SetRecordSwapped(false);
  }
  fOut.write((const char*)record, fDecoder.LengthOf(record)*sizeof(UInt_t));
  //re-swap first record if needed
  if(fRunContext->MustSwap()) ORUtils::Swap(record[0]);
  return fDecoder.LengthOf(record)*sizeof(UInt_t);
}
