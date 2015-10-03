//OROrcaFileWriter.hh

#ifndef _OROrcaFileWriter_hh_
#define _OROrcaFileWriter_hh_

#include <fstream>
#include <string>
#include "ORBasicDataDecoder.hh"

//! Write data packets to a binary orca file verbatim.

class ORRunContext;

class OROrcaFileWriter
{
  public:
    OROrcaFileWriter(const std::string& label = "");
    virtual ~OROrcaFileWriter() { }

    /*! Open file and write XML header. Return number of bytes 
        written in XML header
    */
    virtual size_t Open(ORRunContext* runContext=NULL);
    //! Save and close file
    virtual void Close() { fOut.close(); }
    virtual bool IsWritable() { return fOut.is_open() && fOut.good() && fRunContext!=NULL; }
  
    //! File name is label_run#
    virtual void SetLabel(const std::string& label) { fLabel = label; }
    virtual void SetRunContext(ORRunContext* runContext) { fRunContext = runContext; }

    //! Return number of bytes written.
    virtual size_t WriteDataRecord(UInt_t* record);
    virtual inline OROrcaFileWriter& operator<<(UInt_t* record)
      { WriteDataRecord(record);
	return *this;
      }

  protected:
    /*! Write header from fRunContext to file. This is automatically
        called when opening file.
    */
    virtual size_t WriteHeader();
  
  protected:
    std::ofstream fOut;
    std::string fLabel;
    ORBasicDataDecoder fDecoder;
    ORRunContext* fRunContext;
};

#endif
