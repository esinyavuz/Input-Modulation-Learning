/*--------------------------------------------------------------------------
   Author: Thomas Nowotny
   Contributed by: Esin Yavuz
   
   Institute: Center for Computational Neuroscience and Robotics
              University of Sussex
	      Falmer, Brighton BN1 9QJ, UK 
  
   email to:  T.Nowotny@sussex.ac.uk
  
   initial version: 2015-05-21

   This code is based on a previous model of the honeybee antennal lobe,
   used in the article by Nowotny T, Stierle JS, Galizia CG, Szyszka P. 2013
   "Data-driven honeybee antennal lobe model suggests how stimulus-onset
   asynchrony can aid odour segregation." Brain Res.
--------------------------------------------------------------------------*/

//--------------------------------------------------------------------------
/*! \file ALmodel.cc

\brief This file contains the model definition of a bee olfactory system model
*/
//--------------------------------------------------------------------------

#define DT 0.01  //!< global time step at which the simulation will run
#include "modelSpec.h"
#include "modelSpec.cc"
#include "settings.h"
#include "ALsim.h"

//uncomment the following line to turn on timing measures
//#define TIMING
#define DEBUG   

unsigned int seed= 1234;
int _nGLO= 30;
int _nPN= 5;
int _NPN;
int _nhLN= 1;
int _NhLN;
int _nORN= 15;
int _NORN;
int _nOdor= 17;
int _NLHI= 4; //1-2: input from AL (1:L, 2:NL), 3-4: input from KC (3:L ,4:NL)

#define ORN_PNO 8
double myORN_p[ORN_PNO]= {
  0.1,              // 0 - tspike: spike width
  0.2,              // 1 - trefract: refractory period + spike with
  -60.0,            // 2 - Vrest: resting potential
  50.0,             // 3 - Vspike: potential at top of spike
  0.005,            // 4 - brate: base firing rate [kHz]
  1.4e14,           // 5 - randfac: amplitude of frequency change normalised to random number generator range
  0.004,             // 6 - adrate: rate of adaptation 
  0.002             // 7 - recrate: rate of recovery from adaptation 
};

const char *ORN_p_text[ORN_PNO]= {
  "0 - tspike: spike width",
  "1 - trefract: refractory period + spike with",
  "2 - Vrest: resting potential",
  "3 - Vspike: potential at top of spike",
  "4 - brate: base firing rate [kHz]",
  "5 - randfac: amplitude of frequency change normalised to random number generator range",
  "6 - adrate: rate of adaptation",
  "7 - recrate: rate of recovery from adaptation"
};

#define ORN_IVARNO 11
double myORN_ini[ORN_IVARNO]= {
  -60.0,    // 0 - V: Membrane potential
  0.0,      // 1 - r0: bound odor-receptor fraction for odor 0
  0.0,      // 2 - rs0: "locked-in" odor-receptor fraction for odor 0
  0.0,      // 3 - r1: bound odor-receptor fraction for odor 1
  0.0,      // 4 - rs1: "locked-in" odor-receptor fraction for odor 1
  1.0,      // 5 - ad: adaptation variable (1 = no adaptation currently)
  1.0,      // 6 - rb: fraction of unbound receptor
  0.0,      // 7 - trate: current firing rate
  0.0,      // 8 - refract: whether the neuron is refractory
  0.0,      // 9 - seed: random seed
  0.0      // 10 - kk: array of transition rates of receptor states
};

const char *ORN_ini_text[ORN_IVARNO]= {
  "0 - V: Membrane potential",
  "1 - r0: bound odor-receptor fraction for odor 0",
  "2 - rs0: \"locked-in\" odor-receptor fraction for odor 0",
  "3 - r1: bound odor-receptor fraction for odor 1",
  "4 - rs1: \"locked-in\" odor-receptor fraction for odor 1",
  "5 - ad: adaptation variable (1 = no adaptation currently)",
  "6 - rb: fraction of unbound receptor",
  "7 - trate: current firing rate",
  "8 - refract: whether the neuron is refractory",
  "9 - seed: random seed",
  "10 - kk: array of transition rates of receptor states"
};

#define ALN_PNO 11
double myPN_p[ALN_PNO]= {
  7.15,          // 0 - gNa: Na conductance 
  50.0,          // 1 - ENa: Na equi potential in mV
  1.43,          // 2 - gK: K conductance in
  -95.0,         // 3 - EK: K equi potential in mV
  0.02672,       // 4 - gl: leak conductance in
  -63.563,       // 5 - El: leak equi potential in mV
  0.143,         // 6 - C: membr. capacity density 
  0.0,           // 7 - gM: M conductance
  0.0025,        // 8 - kMalpha: rise rate for M activation
  0.0001,        // 9 - kMbeta: fall rate for M activation
  0.05           // 10 - I0: bias current
};

const char *ALN_p_text[ALN_PNO]= {
  "0 - gNa: Na conductance",
  "1 - ENa: Na equi potential in mV",
  "2 - gK: K conductance",
  "3 - EK: K equi potential in mV",
  "4 - gl: leak conductance",
  "5 - El: leak equi potential",
  "6 - C: membr. capacity density", 
  "7 - gM: M conductance",
  "8 - kMalpha: rise rate for M activation",
  "9 - kMbeta: fall rate for M activation",
  "10 - I0: bias current"
};

#define ALN_IVARNO 5
double myPN_ini[ALN_IVARNO]= {
  -60.5,          // 0 - membrane potential E
  0.01899074535,         // 1 - Na channel activation m
  0.9899576152,          // 2 - not Na channel blocking h
  0.04034804332,         // 3 - K channel activation n
  0.1471045567           // 4 - IM activation r
};

const char *ALN_ini_text[ALN_IVARNO]= {
  "0 - membrane potential E",
  "1 - Na channel activation m",
  "2 - not Na channel blocking h",
  "3 - K channel activation n",
  "4 - M channel activation r"
};

double myhLN_p[ALN_PNO]= {
  7.15,          // 0 - gNa: Na conductance 
  50.0,          // 1 - ENa: Na equi potential in mV
  1.43,          // 2 - gK: K conductance in
  -95.0,         // 3 - EK: K equi potential in mV
  0.02672,       // 4 - gl: leak conductance in
  -63.563,       // 5 - El: leak equi potential in mV
  0.143,         // 6 - C: membr. capacity density 
  0.006,          // 7 - gM: M conductance
  0.025,          // 8 - kMalpha: rise rate for M activation
  0.0001,        // 9 - kMbeta: fall rate for M activation
  -0.03            // 10 - I0: bias current
};

double myhLN_ini[ALN_IVARNO]= {
  -61.43808551,     // 0 - membrane potential E
  0.02987296875,    // 1 - Na channel activation m
  0.9826520875,     // 2 - not Na channel blocking h
  0.06344290756,    // 3 - K channel activation n
  0.2973757385      // 4 - M channel activation r
};

double myLHI_p[ALN_PNO]= {
  7.15,          // 0 - gNa: Na conductance 
  50.0,          // 1 - ENa: Na equi potential in mV
  1.43,          // 2 - gK: K conductance in
  -95.0,         // 3 - EK: K equi potential in mV
  0.02672,       // 4 - gl: leak conductance in
  -63.563,       // 5 - El: leak equi potential in mV
  0.143,         // 6 - C: membr. capacity density 
  0.05,          // 7 - gM: M conductance
  0.02,          // 8 - kMalpha: rise rate for M activation
  0.002,         // 9 - kMbeta: fall rate for M activation
  -0.08            // 10 - I0: bias current
};

double myLHI_ini[ALN_IVARNO]= { 
  -61.43808551,     // 0 - membrane potential E 
  0.02987296875,    // 1 - Na channel activation m
  0.9826520875,     // 2 - not Na channel blocking h
  0.06344290756,    // 3 - K channel activation n
  0.2973757385      // 4 - M channel activation r
};

//--------------------------------------------------------------------------

#define SYN1_IVARNO 1

//---------------------------------------------------------------------------
// ORNPN synapses (not learning atm)
double myORNPN_ini[SYN1_IVARNO]= {
    3.7e-06//0.0035 // 0 - g: conductance
};
double myORNPN_gjitter= 0.1; // in percent of g
double *myORNPN_p= NULL;

#define POSTSYN1_PNO 2
double *myORNPN_post_ini= NULL;
double myORNPN_post_p[POSTSYN1_PNO]= {
    0.0, // 0 - Erev: reversal potential
    0.04 // 1 - beta: decay rate (kHz)
};

//---------------------------------------------------------------------------
// 3 factor learning synapse in ORN-PN connections

#define ASYN_PNO 9
double myORNPN1_p[ASYN_PNO] = {
    7.5e-06,         // 0 - gmax: maximal conductance
    300000,          // 1 - decay timescale conductance
    4.00000e-06,        // 2 - gmid
    3.000000e-06,         // 3 - gslope
    -2.2e-12,         // 4 - pbase: baseline of elgibility p
    1500.0,           // 5 - p_lambda decay time eligibilty trace
    0.8e-14,           // 6 - A = Amplitude of STDP branches
    12,              // 7 - stdp: tau_p
    6              // 8 - stdp: tau_m
};
double myORNPN1_gjitter= 0.1; // in percent of g
double gORNPN1_MIN= 1e-20;

#define ASYN_IVARNO 4
double myORNPN1_ini[ASYN_IVARNO]= {
    -2.2e-12,            // 0 - eligibility trace p
    0.01,           // 1 - graw (not used)
    3.7e-06,        // 2 - g
    0.0             // 3 - lastupdate
};

double *myORNPN1_post_ini= NULL;
double myORNPN1_post_p[2]={
    0.0, // 0 - Erev: reversal potential
    0.01 // 1 - beta: decay rate (kHz)
};

//---------------------------------------------------------------------------
// ORNhLN synapses
double myORNhLN_ini[SYN1_IVARNO]= {
  6.5e-6 // 0 - g: conductance
};
double *myORNhLN_p= NULL;
double myORNhLN_gjitter= 0.1; // in percent of g

double *myORNhLN_post_ini= NULL;
double myORNhLN_post_p[POSTSYN1_PNO]= {
  0.0, // 0 - Erev: reversal potential
  0.01 // 1 - beta: decay rate (kHz)
};

//---------------------------------------------------------------------------
// PNhLN synapses
double myPNhLN_ini[SYN1_IVARNO]= {
  1.5e-05 // 0 - g: conductance
};
double *myPNhLN_p= NULL;
double myPNhLN_gjitter= 0.1; // in percent of g

double *myPNhLN_post_ini= NULL;
double myPNhLN_post_p[POSTSYN1_PNO]= {
  0.0,  // 0 - Erev: reversal potential
  0.02  // 1 - beta: decay rate (kHz)
};

//---------------------------------------------------------------------------
// hLNPN synapses
double myhLNPN_ini[SYN1_IVARNO]= {
  0.0007  // 0 - g: conductance
};
double *myhLNPN_p= NULL;

double *myhLNPN_post_ini= NULL;
double myhLNPN_post_p[POSTSYN1_PNO]= {
  -80.0,  // 0 - Erev: reversal potential
  0.05    // 1 - beta: decay rate (kHz)
};

//---------------------------------------------------------------------------
// hLNhLN synapses
double myhLNhLN_ini[SYN1_IVARNO]= {
  0.0003  // 0 - g: conductance
};
double *myhLNhLN_p= NULL;

double *myhLNhLN_post_ini= NULL;
double myhLNhLN_post_p[POSTSYN1_PNO]= {
  -80.0,  // 0 - Erev: reversal potential
  0.02    // 1 - beta: decay rate (kHz)
};

//---------------------------------------------------------------------------
// PNLHI synapses
double myPNLHI_ini[SYN1_IVARNO]= {
  0.85e-05  // 0 - g: conductance
};
double myPNLHI_gjitter= 0.1;  // as a fraction of g0
double *myPNLHI_p= NULL;

double *myPNLHI_post_ini= NULL;
double myPNLHI_post_p[POSTSYN1_PNO]= {
  0.0,  // 0 - Erev: reversal potential
  0.004    // 1 - beta: decay rate (kHz)
};

//---------------------------------------------------------------------------
//define derived parameters for POSTSYN1
class dpPostsyn1 : public dpclass  
{
public:
  double calculateDerivedParameter(int index, vector<double> pars, double dt = DT){		
    switch (index) {
    case 0:
      return expDecay(pars, dt);
    }
    return -1;
  }
  
  double expDecay(vector<double> pars, double dt) {
    return exp(-pars[1]*dt);
  }
};

double base_RORNPN1= -5;  // baseline for reward (negative == extinction)
double RORNPN1_tau= 50.0;    // time scale of reward change

#include "ALap.h"

//--------------------------------------------------------------------------
/*! \brief This function defines the model 
 */
//--------------------------------------------------------------------------

void modelDefinition(NNmodel &model) 
{	
  // initialize GeNN
  initGeNN();
  setup_AP();
  ifstream is(INPUTFILE);
  if (!is.good()) {
    cerr << "ERROR: input stream cannot be read " << INPUTFILE << endl;
    exit(1);
  }
  read_AP(is);
  write_AP(cerr);
  /******************************************************************/		
  model.setPrecision(GENN_DOUBLE);
  _NORN= _nGLO*_nORN;
  _NPN= _nGLO*_nPN;
  _NhLN= _nGLO*_nhLN;

  // The post-synapses throughout
  postSynModel ps;
  ps.pNames.push_back(tS("Erev"));
  ps.pNames.push_back(tS("beta")); 
  ps.dpNames.push_back(tS("expDecay"));

  ps.postSynDecay=tS("$(inSyn)*=$(expDecay);\n");
  ps.postSyntoCurrent=tS("$(inSyn)*($(Erev)-$(V))");

  ps.dps = new dpPostsyn1;

  unsigned int POSTSYN1= postSynModels.size(); 
  postSynModels.push_back(ps);

  // The normal synapses weightupdate model
  weightUpdateModel w;
  w.varNames.push_back(tS("g"));
  w.varTypes.push_back(tS("scalar"));

  w.simCode = tS("$(addtoinSyn) = $(g);\n\
  $(updatelinsyn);\n\
  ");

  unsigned int SYN1= weightUpdateModels.size();
  weightUpdateModels.push_back(w);

  // learning synapse with 3 factor rule
  weightUpdateModel asynapse;
  // variables
  asynapse.varNames.clear();
  asynapse.varTypes.clear();
  asynapse.varNames.push_back("p");       // 0 - eligibility trace 
  asynapse.varTypes.push_back("scalar"); 
  asynapse.varNames.push_back("graw");    // 1 - raw synaptic comnductance
  asynapse.varTypes.push_back("scalar");
  asynapse.varNames.push_back("g");       // 2 - filtered synaptic cond.
  asynapse.varTypes.push_back("scalar");
  asynapse.varNames.push_back(tS("lastupdate")); // 3 - time of last update
  asynapse.varTypes.push_back(tS("scalar"));
  // parameters
  asynapse.pNames.clear();
  // conductance
  asynapse.pNames.push_back(tS("gmax"));     // 0: maximal conductance
  asynapse.pNames.push_back(tS("g_lambda")); // 1: decay timescale conductance
  asynapse.pNames.push_back(tS("gmid"));     // 2: midpoint of sigmoid filter
  asynapse.pNames.push_back(tS("gslope"));   // 3: inverse slope of sigmoid
  //eligibility trace
  asynapse.pNames.push_back(tS("pbase")); // 4: baseline value for eligibility p
asynapse.pNames.push_back(tS("p_lambda")); // 5: decay timescale eligibility trace
  //stdp curve
  asynapse.pNames.push_back(tS("A"));       // 6: Amplitude of positive STDP branch
  asynapse.pNames.push_back(tS("tau_p"));    // 7: tau_+ of STDP rule
  asynapse.pNames.push_back(tS("tau_m"));    // 8: tau_- of STDP rule
  
  
  asynapse.simCode = tS(" $(addtoinSyn)= $(g);\n\
                            $(updatelinsyn);\n\
                            scalar t_diff= t - $(sT_post); \n		\
                            if (t_diff < 20.0) \n			\
                                $(p)+= $(A);// * exp(-t_diff/$(tau_m));\n \
     ");

  //postsynaptic spikes
  asynapse.simLearnPost = tS(" scalar t_diff = t - $(sT_pre); \n\
                               if (t_diff < 30.0) \n\
                                   $(p) += $(A);// * exp(-t_diff/$(tau_p)); \n\
     ");
  // events (reward received) - expects a extraGlobalSynapseParameter with name R
  asynapse.evntThreshold= tS("1");
  // 
  asynapse.simCodeEvnt= tS("$(p)+= ($(pbase)-$(p))*DT/$(p_lambda); \n\
                            $(graw)+= -$(graw)*DT/$(g_lambda); \n\
                            $(graw)+= $(R)*$(p)*DT;\n\
                            $(g) = $(gmax) * (tanh(($(graw) - $(gmid))/$(gslope)) +1) /2;\n\
     ");
  asynapse.needPreSt= TRUE;
  asynapse.needPostSt= TRUE;
  // extraglobal
  asynapse.extraGlobalSynapseKernelParameters.push_back(tS("R"));
  asynapse.extraGlobalSynapseKernelParameterTypes.push_back(tS("scalar"));
  weightUpdateModels.push_back(asynapse);
  unsigned int ASSOCIATIVE_SYN = weightUpdateModels.size()-1; //this is the synapse index to be used in addSynapsePopulation

// honeybee ORNs
  neuronModel n;
  n.varNames.push_back(tS("V"));
  n.varTypes.push_back(tS("scalar"));
  n.varNames.push_back(tS("r0"));
  n.varTypes.push_back(tS("scalar"));
  n.varNames.push_back(tS("rs0"));
  n.varTypes.push_back(tS("scalar"));
  n.varNames.push_back(tS("r1"));
  n.varTypes.push_back(tS("scalar"));
  n.varNames.push_back(tS("rs1"));
  n.varTypes.push_back(tS("scalar"));
  n.varNames.push_back(tS("ad"));
  n.varTypes.push_back(tS("scalar"));
  n.varNames.push_back(tS("rb"));
  n.varTypes.push_back(tS("scalar"));
  n.varNames.push_back(tS("trate"));
  n.varTypes.push_back(tS("scalar"));
  n.varNames.push_back(tS("refract"));
  n.varTypes.push_back(tS("int"));
  n.varNames.push_back(tS("seed"));
  n.varTypes.push_back(tS("uint64_t"));
  n.varNames.push_back(tS("kk"));
  n.varTypes.push_back(tS("scalar *"));
  n.pNames.push_back(tS("tspike"));
  n.pNames.push_back(tS("trefract"));
  n.pNames.push_back(tS("Vrest"));
  n.pNames.push_back(tS("Vspike"));
  n.pNames.push_back(tS("brate"));
  n.pNames.push_back(tS("randfac"));
  n.pNames.push_back(tS("adrate"));
  n.pNames.push_back(tS("recrate"));
  n.simCode= tS("// do explicit updates for up to two odors \n\
scalar mdt= DT/5.0;\n\
for (int mt=0; mt < 5; mt++) {\n\
// variables of odor 0 \n\
scalar dr= -$(kk)[0]*$(r0)+$(kk)[2]*$(rs0)-$(kk)[3]*$(r0)+$(kk)[1]*$(rb)*pow($(kk)[5],$(kk)[4]); \n\
scalar drs= -$(kk)[2]*$(rs0)+$(kk)[3]*$(r0); \n\
scalar tmp= -$(kk)[1]*$(rb)*pow($(kk)[5],$(kk)[4])+$(kk)[0]*$(r0); \n\
$(r0)+= dr*mdt; \n\
$(rs0)+= drs*mdt; \n\
// variables of odor 1 \n\
dr= -$(kk)[5]*$(r1)+$(kk)[7]*$(rs1)-$(kk)[8]*$(r1)+$(kk)[6]*$(rb)*pow($(kk)[11],$(kk)[9]); \n\
drs= -$(kk)[7]*$(rs1)+$(kk)[8]*$(r1); \n\
tmp+= -$(kk)[6]*$(rb)*pow($(kk)[11],$(kk)[9])+$(kk)[5]*$(r1); \n\
$(r1)+= dr*mdt; \n\
$(rs1)+= drs*mdt; \n\
// common unbound variable and adaptation variable \n\
$(ad)+= ($(recrate)-($(trate)*$(adrate)+$(recrate))*$(ad))*mdt; \n\
$(rb)+= tmp*mdt; \n\
// Spike generation \n\
$(trate)= $(brate)+$(rs0)+$(rs1); \n\
if ($(V) >= $(Vspike)) { \n\
  if (t - $(sT) > $(tspike)) {\n\
    $(V)= $(Vrest); \n\
    $(refract)= 1; \n\
  } \n\
} \n\
else { \n\
  if ($(refract)) { \n\
      if (t - $(sT) > $(trefract)) $(refract)= 0; \n\
  } \n\
  else { \n\
    uint64_t rnd; \n\
    MYRAND($(seed),rnd); \n\
    if (rnd <= $(randfac)*$(trate)*$(ad)) { \n\
      $(V)= $(Vspike); \n\
    } \n\
  } \n\
} \n\
} \n\
");
  n.thresholdConditionCode= tS("($(V) > 0.0)");
// Hack to be able to use extraglobal parameters across both neuron and synapse kernels
  n.extraGlobalNeuronKernelParameters.push_back("R");
  n.extraGlobalNeuronKernelParameterTypes.push_back("scalar");
  unsigned int HONEYORN= nModels.size();
  nModels.push_back(n);

  neuronModel n2; 
  n2.varNames.push_back(tS("V"));
  n2.varTypes.push_back(tS("scalar"));
  n2.varNames.push_back(tS("m"));
  n2.varTypes.push_back(tS("scalar"));
  n2.varNames.push_back(tS("h"));
  n2.varTypes.push_back(tS("scalar"));
  n2.varNames.push_back(tS("n"));
  n2.varTypes.push_back(tS("scalar"));
  n2.varNames.push_back(tS("r"));
  n2.varTypes.push_back(tS("scalar"));
  n2.pNames.push_back(tS("gNa"));
  n2.pNames.push_back(tS("ENa"));
  n2.pNames.push_back(tS("gK"));
  n2.pNames.push_back(tS("EK"));
  n2.pNames.push_back(tS("gl"));
  n2.pNames.push_back(tS("El"));
  n2.pNames.push_back(tS("C"));
  n2.pNames.push_back(tS("gM"));
  n2.pNames.push_back(tS("kMalpha"));
  n2.pNames.push_back(tS("kMbeta"));
  n2.pNames.push_back(tS("I0"));
  n2.simCode= tS("scalar Imem; \n\
scalar mdt= DT/5.0;\n\
for (int mt=0; mt < 5; mt++) {\n\
Imem= -($(m)*$(m)*$(m)*$(h)*$(gNa)*($(V)-$(ENa)) + \n\
$(n)*$(n)*$(n)*$(n)*$(gK)*($(V)-$(EK)) + $(r)*$(gM)*($(V)-$(EK))+ \n\
$(gl)*($(V)-$(El)) - $(I0) - Isyn); \n\
scalar _a= 0.32*(-52.0-$(V)) / (exp((-52.0-$(V))/4.0)-1.0); \n\
scalar _b= 0.28*(25.0+$(V)) / (exp((25.0+$(V))/5.0)-1.0); \n\
$(m)+=(_a*(1.0-$(m)) - _b*$(m))*mdt; \n\
_a= 0.128*exp((-48.0-$(V))/18.0); \n\
_b= 4.0 / (exp((-25.0-$(V))/5.0)+1.0); \n\
$(h)+= (_a*(1.0-$(h)) - _b*$(h))*mdt; \n\
_a= .032*(-50.0-$(V)) / (exp((-50.0-$(V))/5.0)-1.0); \n\
_b= 0.5*exp((-55.0-$(V))/40.0); \n\
$(n)+= (_a*(1.0-$(n)) - _b*$(n))*mdt; \n\
_a= $(kMalpha)/(1.0+exp((20.0-$(V))/5)); \n\
_b= $(kMbeta); \n\
$(r)+= (_a*(1.0-$(r)) - _b*$(r))*mdt; \n\
$(V)+= Imem/$(C)*mdt; \n\
} \n\
");
  n2.thresholdConditionCode= tS("($(V) > 0.0)"); // Wustenberg 2004 : 25 V
  unsigned int HONEYALNEURON= nModels.size();
  nModels.push_back(n2);


  model.setName("ALmodel");
  model.addNeuronPopulation("ORN", _NORN, HONEYORN, myORN_p, myORN_ini);
  model.neuronNeedSt[0]= TRUE;
  model.addNeuronPopulation("PN", _NPN, HONEYALNEURON, myPN_p, myPN_ini);
  model.addNeuronPopulation("hLN", _NhLN, HONEYALNEURON, myhLN_p, myhLN_ini);
  model.addNeuronPopulation("LHI", _NLHI, HONEYALNEURON, myLHI_p, myLHI_ini);

  model.activateDirectInput("LHI", INPRULE);  
  //model.activateDirectInput("KC", INPRULE); // to test only

  model.addSynapsePopulation("ORNPN", SYN1, SPARSE, INDIVIDUALG, NO_DELAY, POSTSYN1, "ORN", "PN", myORNPN_ini, myORNPN_p, myORNPN_post_ini, myORNPN_post_p);
  //model.setMaxConn("ORNPN", _nPN);  

  model.addSynapsePopulation("ORNPN1", ASSOCIATIVE_SYN, SPARSE, INDIVIDUALG, NO_DELAY, POSTSYN1, "ORN", "PN", myORNPN1_ini, myORNPN1_p, myORNPN1_post_ini, myORNPN1_post_p);
  //model.setMaxConn("ORNPN", _nPN);  

  model.addSynapsePopulation("ORNhLN", SYN1, SPARSE, INDIVIDUALG, NO_DELAY, POSTSYN1, "ORN", "hLN", myORNhLN_ini, myORNhLN_p, myORNhLN_post_ini, myORNhLN_post_p);
  //model.setMaxConn("ORNhLN", _nhLN);
  
  model.addSynapsePopulation("PNhLN", SYN1, SPARSE, INDIVIDUALG, NO_DELAY, POSTSYN1, "PN", "hLN", myPNhLN_ini, myPNhLN_p, myPNhLN_post_ini, myPNhLN_post_p);
  //model.setMaxConn("PNhLN", _nhLN);
  
  model.addSynapsePopulation("hLNPN", SYN1, DENSE, INDIVIDUALG, NO_DELAY, POSTSYN1, "hLN", "PN", myhLNPN_ini, myhLNPN_p, myhLNPN_post_ini, myhLNPN_post_p);
 
  model.addSynapsePopulation("hLNhLN", SYN1, SPARSE, INDIVIDUALG, NO_DELAY, POSTSYN1, "hLN", "hLN", myhLNhLN_ini, myhLNhLN_p, myhLNhLN_post_ini, myhLNhLN_post_p);

  model.addSynapsePopulation("PNLHI", SYN1, SPARSE, INDIVIDUALG, NO_DELAY, POSTSYN1, "PN", "LHI", myPNLHI_ini, myPNLHI_p, myPNLHI_post_ini, myPNLHI_post_p);  


    //model.addSynapsePopulation("KCLHI", SYN1, SPARSE, INDIVIDUALG, NO_DELAY, POSTSYN1, "KC", "LHI", myKCLHI_ini, myKCLHI_p, myPNKC_post_ini, myPNKC_post_p); 
#ifdef nGPU 
  cerr << "nGPU: " << nGPU << endl;
  model.setGPUDevice(nGPU);
#endif 
  model.setSeed(seed);
#ifdef TIMING
  model.setTiming(TRUE);
#else
  model.setTiming(FALSE);
#endif // TIMING
  model.finalize();
}
