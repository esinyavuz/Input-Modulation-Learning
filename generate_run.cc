/*--------------------------------------------------------------------------
   Author: Thomas Nowotny
  
   Institute: Center for Computational Neuroscience and Robotics
              University of Sussex
	      Falmer, Brighton BN1 9QJ, UK 
  
   email to:  T.Nowotny@sussex.ac.uk
  
   initial version: 2015-05-27
  
--------------------------------------------------------------------------*/

//--------------------------------------------------------------------------
/*! \file generate_run.cc

\brief This file is part of a tool chain for running the honeybee olfaction model.

*/ 
//--------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <locale>

#ifdef _WIN32
#include <direct.h>
#include <stdlib.h>
#else // UNIX
#include <sys/stat.h> // needed for mkdir
#endif

using namespace std;

//--------------------------------------------------------------------------
/*! \brief template function for string conversion from const char* to C++ string
 */
//--------------------------------------------------------------------------

template<typename T>
std::string toString(T t)
{
  std::stringstream s;
  s << std::showpoint;
  s << t;
  return s.str();
} 

#define tS(X) toString(X) //!< Macro providing the abbreviated syntax tS() instead of toString().


string toUpper(string s)
{
    for (unsigned int i= 0; i < s.length(); i++) {
	s[i]= toupper(s[i]);
    }
    return s;
}

string toLower(string s)
{
    for (unsigned int i= 0; i < s.length(); i++) {
	s[i]= tolower(s[i]);
    }
    return s;
}

//--------------------------------------------------------------------------
/*! \brief Main entry point for generate_run.
 */
//--------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  if (argc != 5)
  {
      cerr << "usage: generate_run <CPU=0, AUTO GPU=1, GPU n= \"n+2\"> <directoru> <basename> <DEBUG 0/1>" << endl;
    exit(1);
  }

  int retval;
  string cmd;
  string gennPath = getenv("GENN_PATH");
  string modelName = "ALmodel";

  int which = atoi(argv[1]);
  string outdir= argv[2];
  string basename= argv[3];
  int dbgMode= atoi(argv[4]);

  // write info in file
  ofstream infoOs("model/settings.h");
  const char *path= getenv ("PWD");
  infoOs << "const char* INPUTFILE =\"" << path << "/" << outdir << "/" << basename << ".in\";" << endl;
  if (which > 1) {
      infoOs << "#define nGPU " << which-2 << endl;
      which= 1;
  }
  infoOs.close();

  // build it
#ifdef _WIN32
  cmd = "cd model && buildmodel.bat " + modelName + " " + toString(dbgMode);
  cmd += " && nmake /nologo /f WINmakefile clean && nmake /nologo /f WINmakefile";
  if (dbgMode == 1) {
    cmd += " DEBUG=1";
  }
#else // UNIX
  cmd = "cd model && buildmodel.sh " + modelName + " " + toString(dbgMode);
  cmd += " && make clean && make";
 if (dbgMode == 1) {
    cmd += " debug";
  }
  else {
    cmd += " release";
  }
#endif
  cerr << cmd << endl;
  retval=system(cmd.c_str());
  if (retval != 0){
    cerr << "ERROR: Following call failed with status " << retval << ":" << endl << cmd << endl;
    cerr << "Exiting..." << endl;
    exit(1);
  }
  // run it!
  cout << "running test..." << endl;
#ifdef _WIN32
  if (dbgMode == 1) {
¯      cmd = "devenv /debugexe model\\ALsim.exe " + outdir + " " + basename + " " + toString(which);
  }
  else {
    cmd = "model\\ALsim.exe " + outdir + " " + basename + " " + toString(which);
  }
#else // UNIX
  if (dbgMode == 1) {
    cmd = "cuda-gdb -tui --args model/ALsim_sim "+ outdir + " " + basename + " " + toString(which);
  }
  else {
    cmd = "model/ALsim "+ outdir + " " + basename + " " + toString(which);
  }
#endif
  retval = system(cmd.c_str());
  if (retval != 0){
    cerr << "ERROR: Following call failed with status " << retval << ":" << endl << cmd << endl;
    cerr << "Exiting..." << endl;
    exit(1);
  }
  return 0;
}
