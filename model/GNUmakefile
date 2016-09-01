##--------------------------------------------------------------------------
##   Author: Thomas Nowotny
##  
##   Institute: Center for Computational Neuroscience and Robotics
##              University of Sussex
##            Falmer, Brighton BN1 9QJ, UK 
##  
##   email to:  T.Nowotny@sussex.ac.uk
##  
##   initial version: 2010-02-07
##  
##--------------------------------------------------------------------------

EXECUTABLE	:=ALsim

SOURCES		:=ALsim.cu

INCLUDE_FLAGS        :=-I./include/numlib -I./include/ISAAC_C++ -Xptxas=-v 

NVCCFLAGS := -O3 -use_fast_math --compiler-options "-O3 -ffast-math"
CXXFLAGS	:=-O3 -ffast-math

include	$(GENN_PATH)/userproject/include/makefile_common_gnu.mk
