This repository includes the model used in "Input-modulation as an alternative to conventional learning strategies" by Esin Yavuz and Thomas Nowotny, accepted for the ICANN 2016 conference.

It consists of a model of plasticity in the honeybee olfactory system, modulated by appetitive conditioning. 

Neuron and non-plastic synapse models are based on [Nowotny et al. 2013][@Nowotnyetal2013], where more information can be found. Mechanisms for input-modulation by reinforcement learning are explained in [Yavuz et al. 2016][@Yavuzetal2016].


Prerequisites:
- CUDA Toolkit
- GeNN v2.1.1 (available here: https://github.com/genn-team/genn/tree/2.1.1)


#Neuron Parameters

##ORN Parameters 

| Parameter   |  Value  |
|-----|:------:| 
| t_spike (spike width) | 0.1 ms|
| t_refract (refractory period + spike with) | 0.2 ms | 
| V_rest (resting potential) |  -60.0 mV | 
| V_spike (potential at top of spike) |  50.0 mV |
| b_rate (base firing rate) | 3 Hz |
| l_max (maximum rate allowed) | 50.0 Hz |
| \alpha (rate of adaptation) | 4 Hz |
| \beta (rate of recovery from adaptation) | 2 Hz |

##PN Parameters

| Parameter   |  Value  |
|-----|:------:|
| g_Na (Na conductance) | 7.15 \mu S | 
| E_Na (Na equipotential) | 50.0 mV |
| g_K (K^+ conductance) | 1.43 \mu S |
| E_K (K equipotential) | -95.0 mV |
| g_l (leak conductance) | 26.72 nS |
| E_l (leak equipotential) |  -63.563 mV |
| C (membrane capacitance density) | 143 pF | 
| I_0 (bias current) | 0.0 A | 

##LN Parameters

| Parameter   |  Value  |
|-----|:------:|
| g_Na (Na conductance) | 7.15 \mu S | 
| E_Na (Na equipotential) | 50.0 mV |
| g_K (K^+ conductance) | 1.43 \mu S |
| E_K (K equipotential) | -95.0 mV |
| g_l (leak conductance) | 26.72 nS |
| E_l (leak equipotential) |  -63.563 mV |
| C (membrane capacitance density) | 143 pF | 
| g_M (M conductance) | 40 nS | 
| kM_\alpha (rise rate for M activation) | 25 Hz |
| kM_\beta (fall rate for M activation) | 1.0 Hz |
| I_0 (bias current) | 0.0 A | 

##LHI Parameters

| Parameter   |  Value  |
|-----|:------:|
| g_Na (Na conductance) | 7.15 \mu S | 
| E_Na (Na equipotential) | 50.0 mV |
| g_K (K^+ conductance) | 1.43 \mu S |
| E_K (K equipotential) | -95.0 mV |
| g_l (leak conductance) | 26.72 nS |
| E_l (leak equipotential) |  -63.563 |
| C (membrane capacitance density) | 143 pF | 
| g_M (M conductance) | 60 nS | 
| kM_\alpha (rise rate for M activation) | 80 Hz |
| kM_\beta (fall rate for M activation) | 1.0 Hz |
| I_0 (bias current) | -80.0 pA | 


#Synapse Paremeters

##ORN-PN synapses (non-plastic)
|       Parameter   |  Value  | 
| ------------- |:-------------:|
| g (conductance)    |  0.021 nS |
| \sigma_g (jitter for initialising g) | 0.05 |
| E_rev (reversal potential) | 0.0 mV |
| \beta (postsynaptic decay rate) | 20 Hz |

##ORN-PN synapses (plastic)
|       Parameter   |  Value  | 
| ------------- |:-------------:|
| g_ini (initial value for conductance)    |  0.021 nS |
| E_rev (reversal potential) | 0.0 mV |
| \beta (postsynaptic decay rate) | 20 Hz |
| g_max (maximal conductance) | 0.045 nS |
| \tau_{decay} (decay time for recovery) | 300 s |
| \g_mid (medium value for conductance)  |  0.021 pS |
| \g_slope (slope of the STDP curve)  |  0.021 nS |
| p_0 (baseline for eligibility curve) | -5e-12 |
| \tau_p (time constant of eligibility decay) | 1.5 s |
| A (cste to be added to p for STDP)| 6e-14 | 
| \tau_{STDP} (time constant of STDP decay) | 12 ms |

##ORN-LN synapses (non-plastic)
|       Parameter   |  Value  | 
| ------------- |:-------------:|
| g (conductance)    |  0.012 nS |
| \sigma_g (jitter for initialising g) | 0.1 |
| E_rev (reversal potential) | 0.0 mV |
| \beta (postsynaptic decay rate) | 20 Hz |
 
##PN-LN synapses (non-plastic)
|       Parameter   |  Value  | 
| ------------- |:-------------:|
| g (conductance)    |  0.3 nS |
| \sigma_g (jitter for initialising g) | 0.1 |
| E_rev (reversal potential) | 0.0 mV |
| \beta (postsynaptic decay rate) | 20 Hz |
 
##LN-PN synapses (non-plastic)
|       Parameter   |  Value  | 
| ------------- |:-------------:|
| g (conductance)    |  1 nS |
| E_rev (reversal potential) | -80.0 mV |
| \beta (postsynaptic decay rate) | 10 Hz |

##LN-LN synapses (non-plastic)
|       Parameter   |  Value  | 
| ------------- |:-------------:|
| g (conductance)    |  1.2 nS |
| E_rev (reversal potential) | -80.0 mV |
| \beta (postsynaptic decay rate) | 10 Hz |
  
##PN-LHI synapses (non-plastic)
|       Parameter   |  Value  | 
| ------------- |:-------------:|
| g (conductance)    |  0.017 nS |
| \sigma_g (jitter for initialising g) | 0.05 |
| E_rev (reversal potential) | 0.0 mV |
| \beta (postsynaptic decay rate) | 10 Hz |
   
#Reward parameters
|       Parameter   |  Value  | 
| ------------- |:-------------:|
| R_{0} (baseline reward -- or -1 x expectance ) | -8.0 |
| R_max (value of given reward) | 40.0 |
| \tau_{R} (Time constant of reward rise/decay) | 50 ms |

REFERENCES

[@Nowotnyetal2013]: http://dx.doi.org/10.1016/j.brainres.2013.05.038
"Nowotny, T., Stierle, J.S., Galizia, C.G. and Szyszka, P., 2013.
Data-driven honeybee antennal lobe model suggests how stimulus-onset asynchrony 
can aid odour segregation. Brain research, 1536, pp.119-134."

[@Yavuzetal2016]: http://sro.sussex.ac.uk/61572 "Yavuz, E. and Nowotny, T., 2016.
Input-modulation as an alternative to conventional learning strategies. LNCS, in press."                                   
