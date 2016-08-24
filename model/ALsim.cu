/*--------------------------------------------------------------------------
   Author: Thomas Nowotny
   Contributed by: Esin Yavuz
  
   Institute: Center for Computational Neuroscience and Robotics
              University of Sussex
              Falmer, Brighton BN1 9QJ, UK 
  
   email to:  T.Nowotny@sussex.ac.uk
  
   initial version: 2011-06-14
  
   This code is based on a previous model of the honeybee antennal lobe,
   used in the article by Nowotny T, Stierle JS, Galizia CG, Szyszka P. 2013
   "Data-driven honeybee antennal lobe model suggests how stimulus-onset
   asynchrony can aid odour segregation." Brain Res.
--------------------------------------------------------------------------*/

#include "AL.h"

#include "gauss.h"
#include "randomGen.h"
#include "randomGen.cc"
#include "standard_deviation.cc"
#include "hr_time.cpp"
randomGen R;
randomGauss RG;

#include "AL.cc"

CStopWatch timer;

int main(int argc, char *argv[])
{
  if (argc != 4) {
    cerr << "usage: ALsim <directory> <infile basename> <CPU= 0, GPU=1>" << endl;
    exit(1);
  }
  
  cerr << "% call was: ";
  for (int i= 0; i < argc; i++) {
    cerr << argv[i] << " ";
  }
  cerr << endl;

  unsigned int which= atoi(argv[3]);

  double tlast= 0.0, tlastwrite= 0.0; 

  stringstream sname;
  char thename[80];

  cerr << "% proto file: ";
  sname.clear();
  sname << argv[1] << "/" << argv[2];
  sname << ".proto" << ends;
  sname >> thename;
  ifstream pris(thename);

  cerr << "% out file: ";
  sname.clear();
  sname << argv[1] << "/" << argv[2];
  sname << ".out.cmp" << ends;
  sname >> thename;
  ofstream os(thename);
  os.precision(10);

  cerr << "% out file st: ";
  sname.clear();
  sname << argv[1] << "/" << argv[2];
  sname << ".out.st" << ends;
  sname >> thename;
	cerr << thename << " is thename" << endl;
  ofstream stos(thename);
  stos.precision(5);

  AL al(which);
	double tme;
  int cntrr= 0;

  cerr << "% odorPath: " << odorPath << endl;
  cerr << "% odorExtension: " << odorExtension << endl;
  cerr << "% LNPNsynFile: " <<  LNPNsynFile << endl;
  R.seedrand((unsigned long) seed, (unsigned long) seed+1, (unsigned long) seed+2);
  RG.seedrand((unsigned long) seed, (unsigned long) seed+1, (unsigned long) seed+2);  
  al.readOdors((int) _nOdor,odorPath, odorExtension);
  al.connect_ORN_PN1();
  al.connect_ORN_PN();
  al.connect_ORN_hLN();
  al.connect_PN_hLN();
  al.connect_PN_LHI();

  al.connect_hLN_PN(LNPNsynFile);
  al.connect_hLN_hLN();
  al.allocate_direct_input();
  al.read_protocol(pris);
  al.randomize_V();
  al.enable();
  if (write_raw || write_all) {
      if (write_raw) al.output_state(os);
      else {
	      al.output_matlab_helper_full(toString(argv[1])+"/"+toString(argv[2]));
	      al.output_full_state(os);
      }
  } 

  if ((int) readState) {
    sname.clear();
    sname << argv[1];
    sname << ".stateIn.bin" << ends;
    sname >> thename;
    ifstream sis(thename, ios::binary);
    assert(sis.good());
    al.read_state(sis);
  }

  al.output_state_st(stos);

  timer.startTimer();
  while (al.continues()) {
    if (write_raw || write_all) {
        if (t-tlastwrite > write_interval) {
            if (write_raw) al.output_state(os);	
	    else al.output_full_state(os);
	    tlastwrite= t;
        }
    }
    al.run();
    if (which == GPU) {
  copySpikeNFromDevice();
	copySpikesFromDevice();
    }


    al.output_state_st(stos);
  }

  if ((int) writeState) {
      sname.clear();
      sname << argv[1];
      sname << ".stateOut.bin" << ends;
      sname >> thename;
      ofstream sis(thename, ios::binary);
      assert(sis.good());
      al.write_state(sis);
  }
  timer.stopTimer();
  stos.close();
  tme= timer.getElapsedTime();
  cudaDeviceReset();
  cerr << "elapsed time: " << tme << endl;
  return 0;
}
  
