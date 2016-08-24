/*--------------------------------------------------------------------------
   Author: Thomas Nowotny
  
   Institute: Institute for Nonlinear Science
              University of California San Diego
              La Jolla, CA 92093-0402
  
   email to:  tnowotny@ucsd.edu
  
   initial version: 2004-01-20
  
--------------------------------------------------------------------------*/

#ifndef AP_H
#define AP_H

#include <string>
#include <sstream>
#include "toString.h"

#define AP_NO 96

enum APTypes {AP_FLOAT, AP_DOUBLE, AP_INT, AP_STRING};

void **AP= NULL;
APTypes *AP_TYPE= NULL;
string *AP_NAME= NULL;
int apn;

template <class T>
void add_array_AP(T *aps, string name, APTypes type, int &APpos, int n)
{
  for (int i= 0; i < n; i++) {
    AP[APpos]= (void *) &aps[i];
    AP_TYPE[APpos]= type;
    AP_NAME[APpos++]= name+toString("[")+toString(i)+toString("]");
  }
}

void setup_AP() {
  cerr << "% entering setup_AP ..." << endl;
  apn= AP_NO;

  if (AP) {
    delete[] AP;
    delete[] AP_TYPE;
    delete[] AP_NAME;
  }
  AP= new void*[apn];
  AP_TYPE= new APTypes[apn];
  AP_NAME= new string[apn];

  int n= 0;
  
  // ORN parameters 
  add_array_AP(myORN_p, toString("myORN_p"), AP_DOUBLE, n, ORN_PNO);
  // ORN initial values
  add_array_AP(myORN_ini, toString("myORN_ini"), AP_DOUBLE, n, ORN_IVARNO);
  // PN parameters
  add_array_AP(myPN_p, toString("myPN_p"), AP_DOUBLE, n, ALN_PNO);
  // PN initial variables
  add_array_AP(myPN_ini, toString("myPN_ini"), AP_DOUBLE, n, ALN_IVARNO); 
  // hLN parameters
  add_array_AP(myhLN_p, toString("myhLN_p"), AP_DOUBLE, n, ALN_PNO);
  // hLN initial variables
  add_array_AP(myhLN_ini, toString("myhLN_ini"), AP_DOUBLE, n, ALN_IVARNO);
  // ORNPN initial variables
  add_array_AP(myORNPN_ini, toString("myORNPN_ini"), AP_DOUBLE, n, SYN1_IVARNO);
  AP[n]= (void *) &myORNPN_gjitter;
  AP_TYPE[n]= AP_DOUBLE;
  AP_NAME[n++]= toString("myORNPN_gjitter");
  // ORNPN_post parameters
  add_array_AP(myORNPN_post_p, toString("myORNPN_post_p"), AP_DOUBLE, n, POSTSYN1_PNO);
  // ORNPN1 parameters
  add_array_AP(myORNPN1_p, toString("myORNPN1_p"), AP_DOUBLE, n, ASYN_PNO);  
   // ORNPN1 initial variables
  add_array_AP(myORNPN1_ini, toString("myORNPN1_ini"), AP_DOUBLE, n, ASYN_IVARNO);
  AP[n]= (void *) &myORNPN1_gjitter;
  AP_TYPE[n]= AP_DOUBLE;
  AP_NAME[n++]= toString("myORNPN1_gjitter");
  // ORNPN1_post parameters
  add_array_AP(myORNPN1_post_p, toString("myORNPN1_post_p"), AP_DOUBLE, n, POSTSYN1_PNO);  
  // ORNhLN initial variables
  add_array_AP(myORNhLN_ini, toString("myORNhLN_ini"), AP_DOUBLE, n, SYN1_IVARNO);
  AP[n]= (void *) &myORNhLN_gjitter;
  AP_TYPE[n]= AP_DOUBLE;
  AP_NAME[n++]= toString("myORNhLN_gjitter");
  // ORNhLN_post parameters
  add_array_AP(myORNhLN_post_p, toString("myORNhLN_post_p"), AP_DOUBLE, n, POSTSYN1_PNO);
  // PNhLN initial variables
  add_array_AP(myPNhLN_ini, toString("myPNhLN_ini"), AP_DOUBLE, n, SYN1_IVARNO);
  // PNhLN_post parameters
  add_array_AP(myPNhLN_post_p, toString("myPNhLN_post_p"), AP_DOUBLE, n, POSTSYN1_PNO);
  // hLNPN initial variables
  add_array_AP(myhLNPN_ini, toString("myhLNPN_ini"), AP_DOUBLE, n, SYN1_IVARNO);
  // hLNPN_post parameters
  add_array_AP(myhLNPN_post_p, toString("myhLNPN_post_p"), AP_DOUBLE, n, POSTSYN1_PNO);
  // PNLHI initial variables
  add_array_AP(myPNLHI_ini, toString("myPNLHI_ini"), AP_DOUBLE, n, SYN1_IVARNO);
  // PNLHI_post parameters
  add_array_AP(myPNLHI_post_p, toString("myPNLHI_post_p"), AP_DOUBLE, n, POSTSYN1_PNO);
// learning/reward stuff
  AP[n]= &base_RORNPN1;
  AP_TYPE[n]= AP_DOUBLE;
  AP_NAME[n++]= toString("base_RORNPN1");
  AP[n]= &RORNPN1_tau;
  AP_TYPE[n]= AP_DOUBLE;
  AP_NAME[n++]= toString("RORNPN1_tau");
  // number of odors 
  AP[n]= &_nOdor;
  AP_TYPE[n]= AP_INT;
  AP_NAME[n++]= toString("_nOdor");
  // other stuff
  AP[n]= &write_interval;
  AP_TYPE[n]= AP_DOUBLE;
  AP_NAME[n++]= toString("write_interval");
  AP[n]= &write_raw;
  AP_TYPE[n]= AP_INT;
  AP_NAME[n++]= toString("write_raw");
  AP[n]= &write_all;
  AP_TYPE[n]= AP_INT;
  AP_NAME[n++]= toString("write_all");
  AP[n]= &readState; 
  AP_TYPE[n]= AP_INT;
  AP_NAME[n++]= toString("readState");
  AP[n]= &writeState;
  AP_TYPE[n]= AP_INT;
  AP_NAME[n++]= toString("writeState");
  AP[n]= &seed;
  AP_TYPE[n]= AP_INT;
  AP_NAME[n++]= toString("seed");
  AP[n]= &odorPath;
  AP_TYPE[n]= AP_STRING;
  AP_NAME[n++]= toString("odorPath");
  AP[n]= &odorExtension;
  AP_TYPE[n]= AP_STRING;
  AP_NAME[n++]= toString("odorExtension");
  AP[n]= &LNPNsynFile;
  AP_TYPE[n]= AP_STRING;
  AP_NAME[n++]= toString("LNPNsynFile");
  
  cerr << "% parameter number " << n << " " << apn << endl;
  assert(n == apn);
  cerr << "% AP set up successfully ..." << endl;
}

void read_AP(istream &is)
{
  string buf;
  while (is.good())
  {
    is >> buf;
    if (is.good()) {
      int pNo= 0;
      while ((pNo < apn) && (buf != AP_NAME[pNo])) pNo++;
      if (pNo >= apn) { cerr << buf << endl;};
      assert(pNo < apn);
      switch (AP_TYPE[pNo]) {
      case AP_FLOAT:
	is >> *((float *)AP[pNo]);
	cerr << "% " << AP_NAME[pNo] << " " << *((float *)AP[pNo]) << endl;
	break;
      case AP_DOUBLE:
	is >> *((double *)AP[pNo]);
	cerr << "% " << AP_NAME[pNo] << " " << *((double *)AP[pNo]) << endl;
	break;
      case AP_INT:
	is >> *((int *)AP[pNo]);
	cerr << "% " << AP_NAME[pNo] << " " << *((int *)AP[pNo]) << endl;
	break;
      case AP_STRING:
	is >> *((string *)AP[pNo]);
	cerr << "% " << AP_NAME[pNo] << " " << *((string *)AP[pNo]) << endl;
	break;
      }
    }
  }
}

void write_AP(ostream &os)
{
  for (int i= 0; i < AP_NO; i++) {
    switch (AP_TYPE[i]) {
    case AP_FLOAT:
      os << "% " << AP_NAME[i] << " " << *((float *)AP[i]) << endl;
      break;
    case AP_DOUBLE:
      os << "% " << AP_NAME[i] << " " << *((double *)AP[i]) << endl;
      break;
    case AP_INT:
      os << "% " << AP_NAME[i] << " " << *((int *)AP[i]) << endl;
      break;
    case AP_STRING:
      os << "% " << AP_NAME[i] << " " << *((string *)AP[i]) << endl;
      break;
    }
  }
}

#endif
