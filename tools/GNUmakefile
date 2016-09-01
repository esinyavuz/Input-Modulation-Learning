#--------------------------------------------------------------------------
# Author: Thomas Nowotny
#
# Institute: Institute for Nonlinear Dynamics
#            University of California San Diego
#            La Jolla, CA 92093-0402
#
# email to:  tnowotny@ucsd.edu
#
# initial version: 2002-04-26
#
#--------------------------------------------------------------------------

C++ = g++
C = gcc
RM= rm -f

FLAGS= -Wall 
all: st2asdf_mult

#-------------------------------------------------------------------------
# tool for automatic queueing 
#-------------------------------------------------------------------------

st2asdf_mult: st2asdf_mult.cc
	$(C++) $(FLAGS) -g -o st2asdf_mult st2asdf_mult.cc

clean:
	$(RM) *.o st2asdf_mult 
