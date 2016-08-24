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


#include <cassert>

AL::AL(unsigned int which)
{
  device= which;
  modelDefinition(model);
  allocateMem();
  initialize();
  initialize_ORN_seeds();
  iT= 0;
  t= 0.0;
  reward= 0.0;
  RORNPN1= base_RORNPN1;
  RORN= RORNPN1;
  enabled= 0;
}

AL::~AL()
{
#ifdef DEBUG
  cerr << "# entering destructor ..." << endl;
#endif
}

void AL::enable()
{
    copyStateToDevice();
    initializeAllSparseArrays();
    initialize_input();
    enabled= 1;
}

void AL::readOdors(int _nOdorin, string odorPath, string odorExt) 
{
#ifdef DEBUG
  cerr << "# entering readOdors in " << odorPath << endl;
#endif
  vector<double> pp(6);
  vector<vector<double> > gloPP;

  string name;
  _nOdor= _nOdorin;
  for (int i= 0; i < _nOdor; i++) {
    name= odorPath + toString("/odor") + toString(i) + odorExt;
    ifstream is(name.c_str());
    if (!is.good()) {
      cerr << name << " not found ... exiting" << endl;
      exit(1);
    }
    for (int j= 0; j < _nGLO; j++) {
      for (int k= 0; k < 5; k++) {
	is >> pp[k];
      }
      gloPP.push_back(pp);
    }
#ifdef DEBUG
    cerr << "# odor " << i << " starts with" << endl;
    for (int k= 0; k < 5; k++) {
	cerr << gloPP[0][k] << " ";
    }
    cerr << endl;
#endif
    odorPP.push_back(gloPP);
    gloPP.clear();
  }
#ifdef DEBUG
    cerr << "# odors read successfully ... " << endl;
#endif
}

// Each ORN projects to a PN1 of its GLO
void AL::connect_ORN_PN1()
{
#ifdef DEBUG
    cerr << "# entering connect_ORN_PN1 ..." << endl;
#endif
  unsigned int n= 0;
  unsigned int connN= _NORN;
  allocateORNPN1(connN);
  for (int i= 0; i < _nGLO; i++) {
    for (int j= 0; j < _nORN; j++) {
#ifdef DEBUG
	cerr << i*_nORN+j << ": ";
#endif
	CORNPN1.indInG[i*_nORN+j]= n;
#ifdef DEBUG
	cerr << n << " ";
#endif
	CORNPN1.ind[n]= i*_nPN;
	gORNPN1[n]= myORNPN1_ini[2]*(1.0+RG.n()*myORNPN1_gjitter);
	if (gORNPN1[n] < gORNPN1_MIN) gORNPN1[n]= gORNPN1_MIN;
	double tmp = gORNPN1[n] / myORNPN1_p[0]*2.0 ;
	if ((2.0-tmp)<1e-20) tmp=2.0-1e-19; 
	double theGRaw= (0.5 * log( tmp / (2.0-tmp))*myORNPN1_p[3]) + myORNPN1_p[2];	
//#ifdef DEBUG
	cerr << i*_nPN << " ";
	cerr << gORNPN1[n] << " ";
	cerr << theGRaw << " " << " ";
	cerr << myORNPN1_p[0]*(tanh((theGRaw-myORNPN1_p[2])/myORNPN1_p[3] )+1)/2 << endl;
//#endif
	pORNPN1[n]= -2.2e-12; //pbase
	grawORNPN1[n]= theGRaw;
	lastupdateORNPN1[n++]= 0.0;
#ifdef DEBUG
	cerr << endl;
#endif	
    }
  }
  assert(n == connN);
  CORNPN1.indInG[_NORN]= connN;
  createPosttoPreArray(_NORN, _NPN, &CORNPN1);
}

// Each ORN projects to all other PN of its GLO
void AL::connect_ORN_PN()
{
#ifdef DEBUG
    cerr << "# entering connect_ORN_PN ..." << endl;
#endif
  unsigned int n= 0;
  unsigned int connN= _NORN*(_nPN-1);
  allocateORNPN(connN);
  for (int i= 0; i < _nGLO; i++) {
    for (int j= 0; j < _nORN; j++) {
#ifdef DEBUG
	cerr << i*_nORN+j << ": ";
#endif
	CORNPN.indInG[i*_nORN+j]= n;
      for (int k= 1; k < _nPN; k++) {
#ifdef DEBUG
	  cerr << n << " ";
#endif
	CORNPN.ind[n]= i*_nPN+k;
	gORNPN[n++]= myORNPN_ini[0]*(1.0+RG.n()*myORNPN_gjitter);
      }
#ifdef DEBUG
      cerr << endl;
#endif
	  
    }
  }
  assert(n == connN);
  CORNPN.indInG[_NORN]= connN;
}

// each ORN projects to the hLNs in the corresponding GLO
void AL::connect_ORN_hLN() 
{
#ifdef DEBUG
    cerr << "# entering connect_ORN_hLN ..." << endl;
#endif
  unsigned int n= 0;
  unsigned int connN= _NORN*_nhLN;
  allocateORNhLN(connN);
  for (int i= 0; i < _nGLO; i++) {
    for (int j= 0; j < _nORN; j++) {
      CORNhLN.indInG[i*_nORN+j]= n;
      for (int k= 0; k < _nhLN; k++) {
	CORNhLN.ind[n]= i*_nhLN+k;
	gORNhLN[n++]= myORNhLN_ini[0]*(1.0+RG.n()*myORNhLN_gjitter);
      }
    }
  }
  assert(n == connN);
  CORNhLN.indInG[_NORN]= connN;
}

// excite hLNs from PNs
void AL::connect_PN_hLN()
{
#ifdef DEBUG
    cerr << "# entering connect_PN_hLN ..." << endl;
#endif
  unsigned int n= 0;
  unsigned int connN= _NPN*_nhLN;
  allocatePNhLN(connN);
  for (int i= 0; i < _nGLO; i++) {		
    for (int j= 0; j < _nPN; j++) {
      CPNhLN.indInG[i*_nPN+j]= n;
      for (int k= 0; k < _nhLN; k++) {
	CPNhLN.ind[n]= i*_nhLN+k;
	gPNhLN[n++]= myPNhLN_ini[0]*(1.0+RG.n()*myPNhLN_gjitter);
      }
    }
  }
  assert(n == connN);
  CPNhLN.indInG[_NPN]= connN;
}
 
 // excite hLNs from PNs
void AL::connect_PN_LHI()
{
#ifdef DEBUG
    cerr << "# entering connect_PN_LHI ..." << endl;
#endif
  unsigned int n= 0;
  unsigned int connN= _NLHI*_nGLO;
  allocatePNLHI(connN);
  for (int i= 0; i < _nGLO; i++) {		
    for (int j= 0; j < _nPN; j++) {
#ifdef DEBUG
      cerr << "# " << i*_nPN+j << ", " << n << ": ";
#endif
      CPNLHI.indInG[i*_nPN+j]= n;
      if (j < _NLHI) {
#ifdef DEBUG
	cerr << j << " ";
#endif
	  CPNLHI.ind[n]= j;
	  gPNLHI[n++]= myPNLHI_ini[0]*(1.0+RG.n()*myPNLHI_gjitter);
      }
#ifdef DEBUG
    cerr << endl;
#endif
    }
  }
  assert(n == connN);
  CPNLHI.indInG[_NPN]= connN;
}

   
// each hLN inhibits PNs: Read dense matrix from a file
void AL::connect_hLN_PN(string name)
{
#ifdef DEBUG
    cerr << "# entering connect_hLN_PN ..." << endl;
#endif
    scalar gscale;
  ifstream is(name.c_str());
  if (!is.good()) {
    cerr << "# error reading hLN-PN connections from file " << name << endl;
    exit(1);
  }
  for (int i= 0; i < _nGLO; i++) {
    for (int j= 0; j < _nGLO; j++) {
      is >> gscale;
      for (int k= 0; k < _nhLN; k++) {
	for (int l= 0; l < _nPN; l++) {
	  ghLNPN[(i*_nhLN+k)*_NPN+j*_nPN+l]= gscale*myhLNPN_ini[0];
#ifdef DEBUG
	  cerr << (i*_nhLN+k)*_NPN+j*_nPN+l << " ";
#endif
	}
      }
    }
  }
#ifdef DEBUG
  cerr << endl;
  assert(is.good());
#endif
  is.close();
}

// homo LNs can inhibit each other ...
void AL::connect_hLN_hLN()
{
#ifdef DEBUG
    cerr << "# entering connect_hLN_hLN ..." << endl;
#endif
    unsigned int n= 0;
    unsigned int connN= _NhLN*(_NhLN-_nhLN);
    allocatehLNhLN(connN);
    for (int i= 0; i < _nGLO; i++) {
	for (int k= 0; k < _nhLN; k++) {
	    ChLNhLN.indInG[i*_nhLN+k]= n;
#ifdef DEBUG
	    cerr << i*_nhLN+k << ": ";
#endif
	    for (int j= 0; j < _nGLO; j++) {
		if (i != j) { // only inhibit other GLO
		    for (int l= 0; l < _nhLN; l++) { 
#ifdef DEBUG
			cerr << n << " " << j*_nhLN+l << " ";
#endif
			ChLNhLN.ind[n]= j*_nhLN+l;
			ghLNhLN[n++]= myhLNhLN_ini[0];
		    }
		}
	    }
#ifdef DEBUG
	    cerr << endl;
#endif
	}
    }
    assert(n == connN);
    ChLNhLN.indInG[_NhLN]= connN;
}

void AL::initialize_ORN_seeds() {
#ifdef DEBUG
    cerr << "# entering initialize_ORN_seeds ..." << endl;
#endif
  for (int i= 0; i < _NORN; i++) {
    seedORN[i]= (uint64_t) (R.n()*1e8);
  }
  if (device == GPU) {
    unsigned int size= _NORN*sizeof(uint64_t);
    CHECK_CUDA_ERRORS(cudaMemcpy(d_seedORN, seedORN, size, cudaMemcpyHostToDevice));
  }
}

void AL::initialize_input()
{
#ifdef DEBUG
    cerr << "# entering initialize_input ..." << endl;
#endif
  // need to make sure that allocateMem() has already been called before this 
  size_t size= _nGLO*12*sizeof(scalar);
  scalar **tmpKK= new scalar*[_NORN];
  theKK= new scalar[_nGLO*12];
  CHECK_CUDA_ERRORS(cudaMalloc(&d_theKK, size));
  for (int i= 0; i< _nGLO; i++) {
    for (int j= 0; j < _nORN; j++) {
			kkORN[i*_nORN+j]= theKK+(12*i);
      tmpKK[i*_nORN+j]= d_theKK+(12*i);
#ifdef DEBUG 
      cerr << i*_nORN+j << " " ;
#endif
    }
  }
#ifdef DEBUG 
  cerr << endl;
#endif
  size= _NORN*sizeof(scalar *);
  CHECK_CUDA_ERRORS(cudaMemcpy(d_kkORN, tmpKK, size, cudaMemcpyHostToDevice));
#ifdef DEBUG
   // make sure the odors are initially all removed (initialized to 0)
  remove_input(0);
  remove_input(1);
  cerr << "# exiting initialize_input ..." << endl;
#endif
}

void AL::read_protocol(ifstream &is)
{
    proto_item p;
    int found;
    is >> p.t;
    while (is.good()) {
	is >> p.action;
	found= 0;
	if (p.action == "odor") {
	    is >> p.value[0].i;
	    is >> p.value[1].i;
	    is >> p.value[2].d;
	    is >> p.value[3].i;
	    found= 1;
	}
	if (p.action == "reward") {
	    is >> p.value[0].d;
	    found= 1;
	}
	if (p.action == "input") {
	    is >> p.value[0].i;
	    is >> p.value[1].d;
	    found= 1;
	}
	if (!found) {
	    cerr << "unrecognized action" << p.action << " in protocol ..." << endl;
	    exit(1);
	}
	proto.push_back(p);
	is >> p.t;
    }
    iProto= 0;
#ifdef DEBUG
    cerr << "# protocol read with " << proto.size() << " items." << endl;
#endif
}

void AL::add_input(unsigned int od, double c, unsigned int pos) 
{
#ifdef DEBUG
    cerr << "# entering add_input ..." << endl;
    cerr << "% setting input " << od << " at concentration " << c << " in slot " << pos << endl;
#endif
  // note: theKK[*][5] is the concentration ...
  for (int i= 0; i < _nGLO; i++) {
    for (int l= 0; l < 5; l++) {
      theKK[i*12+pos*6+l]= odorPP[od][i][l];
      theKK[i*12+pos*6+5]= pow(10.0,c);
    }
  }
  if (device == GPU) {
    unsigned int size= _nGLO*12*sizeof(scalar);
    CHECK_CUDA_ERRORS(cudaMemcpy(d_theKK, theKK, size, cudaMemcpyHostToDevice));
  }
}

void AL::remove_input(unsigned int pos) 
{
#ifdef DEBUG
  cerr << "% removing input in slot " << pos << endl;
#endif
  for (int i= 0; i < _nGLO; i++) {
      theKK[i*12+pos*6+5]= 0.0;
  }
  if (device == GPU) {
    unsigned int size= _nGLO*12*sizeof(scalar);
    CHECK_CUDA_ERRORS(cudaMemcpy(d_theKK, theKK, size, cudaMemcpyHostToDevice));
  }
}

void AL::allocate_direct_input(){
    directinput = new double[_NLHI];
    for (int i= 0; i < _NLHI; i++) {
	directinput[i]= 0.0;
    }
  /*  directinput2 = new double[_NKC];
    for (int i= 0; i < _NKC; i++) {
	directinput2[i]= -2.5*0.001;//(-17.5+4*i)*0.001;
    }*/
    if (device == GPU) {
        CHECK_CUDA_ERRORS(cudaMalloc((void**) &d_directinput, _NLHI*sizeof(double)));
	CHECK_CUDA_ERRORS(cudaMemcpy(d_directinput, directinput, _NLHI*sizeof(double), cudaMemcpyHostToDevice));
    }
}

void AL::set_directInput(int id, double val)
{
    directinput[id] = (scalar) val;
    if (device == GPU) CHECK_CUDA_ERRORS(cudaMemcpy(d_directinput, directinput, _NLHI*sizeof(double), cudaMemcpyHostToDevice));
}

void AL::protocol_handler(double t)
{
    proto_item p;
    while ((iProto < proto.size()) && (t >= proto[iProto].t)) {
	p= proto[iProto];
	if (p.action == "odor") {
	    if (p.value[3].i == 1) {
		add_input(p.value[1].i, pow(10.0, p.value[2].d), p.value[0].i); 
		cerr << "# t: " << t << " - added input " << p.value[1].i << endl;
	    }
	    else {
		remove_input(p.value[0].i);
		cerr << "# t: " << t << " - removed input from slot " << p.value[0].i << endl;
	    }
	}
	if (p.action == "reward") {
	    reward= p.value[0].d;
	    cerr << "# t: " << t << " - set reward to " << p.value[0].d << endl;
	}
	if (p.action == "input") {
	    set_directInput(p.value[0].i, p.value[1].d);
	    cerr << "# t: " << t << " - set input to neuron " << p.value[0].i << " to " << p.value[1].d << endl;
	}
	iProto++;
    }
}


// the main step function

void AL::run()
{
#ifdef DEBUG
//    cerr << "# entering run() ..." << endl;
#endif
    assert(enabled);
    protocol_handler(t);
    RORNPN1+= (base_RORNPN1+reward-RORNPN1)/RORNPN1_tau*DT;
    RORN= RORNPN1; // make a true copy
    
		/*if (t==100){
	    for (int i= 0; i < _NKC; i++) {
				directinput2[i]= (10.5+i)*0.001;
	    }
		} */

  if (device == GPU) {
      stepTimeGPU(d_directinput,t);
  }
  else {
      stepTimeCPU(directinput,t);
  }
  iT++;
  t= iT*DT;
}


void AL::output_state(ostream &os)
{
}

void AL::output_full_state(ostream &os)
{
    if (device == GPU) {
	copyStateFromDevice();
    }
    os << t << " ";
/*    for (int i= 0; i < _NORN; i++) {
	os << VORN[i] << " ";
	os << r0ORN[i] << " ";
	os << rs0ORN[i] << " ";
	os << r1ORN[i] << " ";
	os << rs1ORN[i] << " ";
	os << adORN[i] << " ";
	os << rbORN[i] << " ";
	os << trateORN[i] << " ";
	os << refractORN[i] << " ";
	os << seedORN[i] << "   ";
    }
    os << "   ";
    for (int i= 0; i < _NPN; i++) {
	os << VPN[i] << " ";
	os << mPN[i] << " ";
	os << hPN[i] << " ";
	os << nPN[i] << " ";
	os << rPN[i] << "   ";
    }
    os << "   ";
    for (int i= 0; i < _NhLN; i++) {
	os << VhLN[i] << " ";
	os << mhLN[i] << " ";
	os << hhLN[i] << " ";
	os << nhLN[i] << " ";
	os << rhLN[i] << "   ";
    }
    os << "   ";
    for (int i= 0; i < _NPN; i++) {
	os << inSynORNPN[i] << "   ";
    }
    os << "   ";
    for (int i= 0; i < _NPN; i++) {
	os << inSynhLNPN[i] << "   ";
    }
    os << "   ";
    for (int i= 0; i < _NhLN; i++) {
	os << inSynORNhLN[i] << "   ";
    }
    os << "   ";
    for (int i= 0; i < _NhLN; i++) {
	os << inSynPNhLN[i] << "   ";
    }
    os << "   ";
    for (int i= 0; i < _NhLN; i++) {
	os << inSynhLNhLN[i] << "   ";
	}*/
    /*for (int i= 0; i < _NLHI; i++) {
	os << VLHI[i] << " ";
	os << mLHI[i] << " ";
	os << hLHI[i] << " ";
	os << nLHI[i] << " ";
	os << rLHI[i] << "   ";
    }*/
    /*  for (int i= 0; i < CORNPN1.connN; i++) {
      os << pORNPN1[i] << " ";
      os << gORNPN1[i] << " ";
      os << lastupdateORNPN1[i] << " ";
      }*/
    /*for (int i= 0; i < _NLHI; i++) {
      os << inSynPNLHI[i] << " ";
    }*/
    os << pORNPN1[0] << " ";
    os << gORNPN1[0] << " ";
    os << RORNPN1 << " ";
    os << pORNPN1[_nORN] << " ";
    os << pORNPN1[_nORN*5] << " ";
    os << pORNPN1[_nORN*14] << " ";
    os << gORNPN1[_nORN] << " ";
    os << gORNPN1[_nORN*5] << " ";
    os << gORNPN1[_nORN*14] << " ";
    os << grawORNPN1[_nORN] << " ";
    os << grawORNPN1[_nORN*5] << " ";
    os << grawORNPN1[_nORN*14] << " ";
    os << VPN[0] << " ";
    os << VPN[1] << " ";
    os << VPN[5] << " ";
    os << VPN[14] << " ";
   	os << pORNPN1[_nORN*2] << " ";
    os << pORNPN1[_nORN*9] << " ";
    os << pORNPN1[_nORN*25] << " ";
    os << gORNPN1[_nORN*2] << " ";
    os << gORNPN1[_nORN*9] << " ";
    os << gORNPN1[_nORN*25] << " ";
    os << grawORNPN1[_nORN*2] << " ";
    os << grawORNPN1[_nORN*9] << " ";
    os << grawORNPN1[_nORN*25] << " ";
    os << VPN[2] << " ";
    os << VPN[9] << " ";
    os << VPN[24] << " ";
    os << endl;
}

void AL::output_matlab_helper_full(string bname)
{
    string name= bname+"loadDataFull.m";
    ofstream os(name.c_str());
    os << "d= load('" << bname << ".out.cmp');" << endl;
    unsigned int istart= 2;
    unsigned int iend=  1+_NORN*(ORN_IVARNO-1); 
    os << "dORN= d(:," << istart << ":" << iend << ");" << endl;
    istart= iend+1;
    iend+= _NPN*ALN_IVARNO;
    os << "dPN= d(:," << istart << ":" << iend << ");" << endl;
    istart= iend+1;
    iend+= _NhLN*ALN_IVARNO;
    os << "dhLN= d(:," << istart << ":" << iend << ");" << endl;
    istart= iend+1;
    iend+= _NPN;
    os << "dinSynORNPN= d(:," << istart << ":" << iend << ");" << endl;
    istart= iend+1;
    iend+= _NPN;
    os << "dinSynhLNPN= d(:," << istart << ":" << iend << ");" << endl;
     istart= iend+1;
    iend+= _NhLN;
    os << "dinSynORNhLN= d(:," << istart << ":" << iend << ");" << endl;
    istart= iend+1;
    iend+= _NhLN;
    os << "dinSynPNhLN= d(:," << istart << ":" << iend << ");" << endl;
    istart= iend+1;
    iend+= _NhLN;
    os << "dinSynhLNhLN= d(:," << istart << ":" << iend << ");" << endl;
    os.close();
}

void AL::output_LN(ostream &os)
{
}

void AL::output_ORN(ostream &os)
{
}

void AL::randomize_V(){
	float jitter =10; 
	for (int k=0;k<_NPN;k++){
  	VPN[k]= myPN_ini[0]-jitter/2+R.n()*jitter;
	}
}
void AL::output_state_st(ostream &osr)
{
    int offset= 0;
		for (int i= 0; i < spikeCount_ORN; i++) {
	osr << t << " " << spike_ORN[i]+offset << endl;
    }
    offset+= _NORN;
    for (int i= 0; i < spikeCount_PN; i++) {
	osr << t << " " << spike_PN[i]+offset << endl;
    }
    offset+= _NPN;
    for (int i= 0; i < spikeCount_hLN; i++) {
	osr << t << " " << spike_hLN[i]+offset << endl;
    }
    offset+= _NhLN;
    for (int i= 0; i < spikeCount_LHI; i++) {
	osr << t << " " << spike_LHI[i]+offset << endl;
    }   
    offset+=_NLHI; 

		//Following is commented out as it is costly. Uncomment if you want ORN spike times.
   /* for (int i= 0; i < spikeCount_ORN; i++) {
	osr << t << " " << spike_ORN[i]+offset << endl;
    }
    offset+= _NORN;*/
}


int AL::continues() 
{
    return (iProto < proto.size());
}
