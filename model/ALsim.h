/*--------------------------------------------------------------------------
   Author: Thomas Nowotny
   Contributed by: Esin Yavuz
  
   Institute: Center for Computational Neuroscience and Robotics
              University of Sussex
              Falmer, Brighton BN1 9QJ, UK 
  
   email to:  T.Nowotny@sussex.ac.uk
  
   initial version: 2011-06-14
  
--------------------------------------------------------------------------*/

#include <cstdlib>
#include <climits>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


double write_interval= 0.5; // time interval when to write values
short write_raw= 0; // whether to write voltages
short write_all= 0; // whether to write all variables (should be used alternatively to (not at the same time as) write_raw)
short readState= 0;  // whether to read a previous dump of the internal state
short writeState= 0; // whether to dump the internal state

string odorPath= "odors";
string odorExtension= ".para";
string LNPNsynFile="model/inhibition.dat";

double t;

#define PI 3.1415927
