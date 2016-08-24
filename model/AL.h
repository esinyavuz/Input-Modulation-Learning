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

#ifndef AL_H
#define AL_H

#include <fstream>
#include <vector>

#include "ALmodel.cc"
#include "ALmodel_CODE/runner.cc"

union value_t {
    double d;
    int i;
};

class proto_item {
public:
    double t;
    string action;
    union value_t value[4];
};

class AL {
 protected:
    NNmodel model;
  vector<vector<vector<double> > > odorPP;
  unsigned int device;
  scalar *theKK, *d_theKK;
  int iT;
  int enabled;
  vector<proto_item> proto;
  double reward;
  double *directinput,*directinput2, *d_directinput;
  int iProto;

 public:
  AL(unsigned int);
  ~AL();
  void enable();
  void readOdors(int, string, string);
  void connect_ORN_PN1();
	void randomize_V();
  void connect_ORN_PN();
  void connect_ORN_hLN();
  void connect_PN_hLN();
  void connect_PN_LHI();
  void connect_hLN_PN(string);
  void connect_hLN_hLN();
  void initialize_ORN_seeds();
  void initialize_input();
  void read_protocol(ifstream &);
  void run();
  void protocol_handler(double);
  void allocate_direct_input();
  void set_directInput(int, double);
  void add_input(unsigned int, double, unsigned int);
  void remove_input(unsigned int);
  void output_state(ostream &);
  void output_full_state(ostream &);
  void output_matlab_helper_full(string);
  void output_LN(ostream &);
  void output_ORN(ostream &);
  void output_state_st(ostream &);
  int continues();
};

#endif

