#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include "ORDataProcManager.hh"
#include "ORFileReader.hh"
#include "ORLogger.hh"
#include "ORSocketReader.hh"
#include "ORRunDecoder.hh"
#include "ORHexDumpAllProc.hh"

using namespace std;

static const char Usage[] =
"\n"
"\n"
"Usage: orhexdump [options] [input file(s) / socket host:port]\n"
"\n"
"The one required argument is either the name of a file to be processed or\n"
"a host and port of a socket from which to read data. For a file, you may\n"
"enter a series of files to be processed, or use a wildcard like \"file*.dat\"\n"
"For a socket, the argument should be formatted as host:port.\n"
"\n"
"Available options:\n"
"  -h, --help : print this message and exit\n"
"  -v, --verbosity [verbosity] : set the severity/verbosity for the logger.\n"
"    Choices are: debug, trace, routine, warning, error, and fatal.\n"
"  -b, --begin  [packet #] : dump packets starting with packet #.\n"
"  -e, --end    [packet #] : stop dumping packets after packet #.\n"
"  -p, --packet [packet #] : dump only packet #.\n"
"  -f, --filepackets [file name] : get list of packets to dump from file.\n"
"  -d, --device [device name] : dump packets only for one device.\n"
"  -l, --linelength [words per line] : # of 32 bit words to print in each line.\n"
"\n"
"Example usage:\n"
"orhexdump run194ecpu\n"
"  Hex-dump local file run194ecpu with default verbosity, etc.\n"
"orhexdump --begin 50 --end 60 run194ecpu\n"
"  Hex-dump packets 50 through 60 or local file run194ecpu.\n"
"orhexdump --device ORRunModel:Run --run194ecpu\n"
"  Hex-dump packets 50 through 60 or local file run194ecpu.\n"
"orhexdump run*\n"
"  Same as before, but run over all files beginning with \"run\".\n"
"orhexdump --verbosity debug run194ecpu\n"
"  The same, but with example usage of the verbosity option.\n"
"  Lots of debugging output will appear.\n"
"orhexdump 128.95.100.213:44666\n"
"  Hex dump orca stream on host 128.95.100.213, port 44666 with default verbosity, etc.\n"
"\n"
"\n";


int main(int argc, char** argv)
{
  if(argc == 1) {
    ORLog(kError) << "You must supply some options" << endl << Usage;
    return 1;
  }


  static struct option longOptions[] = {
    {"help", no_argument, 0, 'h'},
    {"verbosity", required_argument, 0, 'v'},
    {"begin", required_argument, 0, 'b'},
    {"end", required_argument, 0, 'e'},
    {"packet", required_argument, 0, 'p'},
    {"filepackets", required_argument, 0, 'f'},
    {"device", required_argument, 0, 'd'},
    {"linelength", required_argument, 0, 'l'}
  };

  string label = "OR";
  ORVReader* reader = NULL;
  Int_t begin = -1;
  Int_t end = -1;
  string filepackets = "";
  string device = "";
  UInt_t linelength = 0;
  while(1) {
    char optId = getopt_long(argc, argv, "hv:b:e:p:f:", longOptions, NULL);
    if(optId == -1) break;
    switch(optId) {
      case('h'): // help
        cout << Usage;
        return 0;
      case('v'): // verbosity
        if(strcmp(optarg, "debug") == 0) ORLogger::SetSeverity(ORLogger::kDebug);
        else if(strcmp(optarg, "trace") == 0) ORLogger::SetSeverity(ORLogger::kTrace);
        else if(strcmp(optarg, "routine") == 0) ORLogger::SetSeverity(ORLogger::kRoutine);
        else if(strcmp(optarg, "warning") == 0) ORLogger::SetSeverity(ORLogger::kWarning);
        else if(strcmp(optarg, "error") == 0) ORLogger::SetSeverity(ORLogger::kError);
        else if(strcmp(optarg, "fatal") == 0) ORLogger::SetSeverity(ORLogger::kFatal);
        else {
          ORLog(kWarning) << "Unknown verbosity setting " << optarg 
                          << "; using kRoutine" << endl;
          ORLogger::SetSeverity(ORLogger::kRoutine);
        }
        break;
      case('b'): // begin
	begin=atoi(optarg);
	break;
      case('e'): // end
        end=atoi(optarg);
	break;
      case('p'): // packet
	if(begin != -1 || end != -1) {
	  ORLog(kError) << "Cannot set packet option if begin or end is already set." << endl;
	  return 0;
	}
	begin=atoi(optarg);
	end=begin;
	break;
      case('f'): // filepackets
	filepackets = optarg;
	break;
      case('d'): // device
        device = optarg;
      case('l'): // linelength
	linelength=atoi(optarg);
	break;
      default: // unrecognized option
        ORLog(kError) << Usage;
        return 1;
    }
  }

  if (argc <= optind) {
    ORLog(kError) << "You must supply a filename or socket host:port" << endl
                  << Usage << endl;
    return 1;
  }

  string readerArg = argv[optind];
  size_t iColon = readerArg.find(":");
  if (iColon == string::npos) {
    reader = new ORFileReader;
    for (int i=optind; i<argc; i++) {
      ((ORFileReader*) reader)->AddFileToProcess(argv[i]);
    }
  } else {
    reader = new ORSocketReader(readerArg.substr(0, iColon).c_str(), 
                                atoi(readerArg.substr(iColon+1).c_str()));
  }

  if (!reader->OKToRead()) {
    ORLog(kError) << "Reader couldn't read" << endl;
    return 1;
  }

  ORLog(kRoutine) << "Setting up data processing manager..." << endl;
  ORDataProcManager dataProcManager(reader);

  ORHexDumpAllProc hexDumper;
  if(begin != -1 || end != -1) hexDumper.SetLimits(begin, end);
  if(filepackets != "") {
    ifstream in(filepackets.c_str());
    hexDumper.SetPacketList(in);
  }
  if(device != "") hexDumper.AddDevice(device);
  if(linelength > 0) hexDumper.SetLineLength(linelength);
  dataProcManager.AddProcessor(&hexDumper);

  ORLog(kRoutine) << "Start processing..." << endl;
  dataProcManager.ProcessDataStream();
  ORLog(kRoutine) << "Finished processing..." << endl;

  delete reader;

  return 0;
}

