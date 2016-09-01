/*--------------------------------------------------------------------------
   Author: Thomas Nowotny
  
   Institute: Institute for Nonlinear Dynamics
              University of California San Diego
              La Jolla, CA 92093-0402
  
   email to:  tnowotny@ucsd.edu
  
   initial version: 2002-01-25
  
--------------------------------------------------------------------------*/


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cmath>
using namespace std;

double tau_SDF;
double norm= 0.0;
double dt_SDF= 0.1;
int SDFSz;
int krnlStpN;
double krnlShft;
double eps_KRNL= 0.01;

#define PI 3.1415927

void kernel_Prep() 
{
  double sum= 0.0;
  double tsm= 0.0;
  double krnl= 0.0;
  double t;

  int i= 0;
  int mxp= (int) (tau_SDF/dt_SDF);
  while ((i < mxp) || (krnl > eps_KRNL)) {
    t= i*dt_SDF;
    krnl= t*exp(-t/tau_SDF);
    sum+= krnl;
    tsm+= t*krnl;
    i++;
  }
  krnlStpN= i;
  norm= sum*dt_SDF; // this is the integral
  // krnlShft= tsm/sum;  // this aims for centering on the center of mass
  krnlShft= tau_SDF; 
   cerr << krnlShft << endl;
}  

  
void blank_SDF(double **sdf, int cols, int sz)
{
  for (int k= 0; k < cols; k++) {
    for (int i= 0; i < sz; i++) {
      sdf[k][i]= 0.0;
    }
  }
}


void add_to_SDF(double *sdf, double tme)
{
  static double s, t, dt;
  static int iTime;
  
  for (int j= 0; j < krnlStpN; j++) {
    t= tme+j*dt_SDF-krnlShft;
    dt= j*dt_SDF;
    s= dt*exp(-dt/tau_SDF);
    iTime= (int) (t/dt_SDF);
    if ((iTime > 0) && (iTime < SDFSz)) {
      sdf[iTime]+= s;
    }
  }    
}

int main(int argc, char *argv[])
{
  // this tool makes an asymmetric SDF as used by Kroczik et al. 2009
  // this is done with a t*exp(-t/tau) kernel shifted by tshift= centre 
  // of mass of the kernel.

  if (argc != 7) {
    cerr << "usage: st2sdf <infile> <tau_SDF> <dt_SDF> <outfile> <tmax> <neuron no>" << endl;
    exit(1);
  }

  cerr << "# call was: ";
  for (int i= 0; i < argc; i++) {
    cerr << argv[i] << " ";
  }
  cerr << endl;

  ifstream is(argv[1]);
  ofstream os(argv[4]);
  tau_SDF= atof(argv[2]);
  dt_SDF= atof(argv[3]);
  double tmax= atof(argv[5]);
  int nNo= atoi(argv[6]);
  
  double **sdf= new double*[nNo];
  double *sum= new double[nNo];
  SDFSz= (int) (tmax/dt_SDF);
  for (int k= 0; k < nNo; k++) {
    sdf[k]= new double[SDFSz];
    sum[k]= 0.0;
    for (int l= 0; l < SDFSz; l++) {
      sdf[k][l]= 0.0;
    }
  }

  kernel_Prep();
  double t;
  int spk;

  os.precision(10);

  is >> t;
  blank_SDF(sdf, nNo, SDFSz);
  while (is.good()) {
    is >> spk;
    add_to_SDF(sdf[spk], t);
    is >> t;
  }
  int i= 0;
  double sdft= i*dt_SDF;
  while (sdft < tmax) {
    os << sdft << " ";
    for (int k= 0; k < nNo; k++) {
      os << sdf[k][i]/norm << " ";
      sum[k]+= sdf[k][i];
    }
    os << endl;
    i++;
    sdft= i*dt_SDF;
  }

  for (int k= 0; k < nNo; k++) {
    cerr << sum[k]/norm*dt_SDF << " ";
  }
  cerr << endl;

  return 0;
}


