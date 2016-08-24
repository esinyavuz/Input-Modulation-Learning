/*--------------------------------------------------------------------------
   Author: Thomas Nowotny
  
   Institute: Center for Computational Neuroscience and Robotics
              University of Sussex
              Falmer, Brighton BN1 9QJ, UK 
  
   email to:  T.Nowotny@sussex.ac.uk
  
   initial version: 2011-06-14
  
--------------------------------------------------------------------------*/

#ifndef STLVECTORARITHMETIC
#define STLVECTORARITHMETIC

#include <vector>


template <class Type>
vector<Type> operator+(vector<Type> x, vector<Type> y)
{
  assert(x.size() == y.size());
  vector<Type> z= x;
  for (unsigned int i= 0; i < x.size(); i++) {
    z[i]+= y[i];
  }
  return z;
}

template <class Type>
vector<Type> operator+(vector<Type> x, Type y)
{
  vector<Type> z= x;
  for (unsigned int i= 0; i < x.size(); i++) {
    z[i]+= y;
  }
  return z;
}

template <class Type>
vector<Type> operator-(vector<Type> x, vector<Type> y)
{
  assert(x.size() == y.size());
  vector<Type> z= x;
  for (unsigned int i= 0; i < x.size(); i++) {
    z[i]-= y[i];
  }
  return z;
}

template <class Type>
vector<Type> operator-(vector<Type> x, Type y)
{
  vector<Type> z= x;
  for (unsigned int i= 0; i < x.size(); i++) {
    z[i]-= y;
  }
  return z;
}

template <class Type>
Type operator*(vector<Type> x, vector<Type> y)
{
  assert(x.size() == y.size());
  Type m= x[0]*y[0];
  for (unsigned int i= 1; i < x.size(); i++) {
    m+= x[i]*y[i];
  }
  return m;
}

template <class Type>
vector<Type> operator*(Type x, vector<Type> y)
{
  vector<Type> z= y;
  for (unsigned int i= 0; i < y.size(); i++) {
    z[i]*= x;
  }
  return z;
}

template <class Type>
vector<Type> operator/(vector<Type> x, Type y)
{
  assert(y != (Type) 0);
  vector<Type> z= x;
  for (unsigned int i= 0; i < x.size(); i++) {
    z[i]/= y;
  }
  return z;
}

float norm1(vector<float> x) 
{
  assert(x.size() > 0);
  float n= abs(x[0]);
  for (unsigned int i= 1; i < x.size(); i++) {
    n+= abs(x[i]);
  }
  return n;
}
  
double norm1(vector<double> x) 
{
  assert(x.size() > 0);
  double n= abs(x[0]);
  for (unsigned int i= 1; i < x.size(); i++) {
    n+= abs(x[i]);
  }
  return n;
}

float norm2(vector<float> x) 
{
  assert(x.size() > 0);
  float n= x[0]*x[0];
  for (unsigned int i= 1; i < x.size(); i++) {
    n+= x[i]*x[i];
  }
  return sqrt(n);
}
  
double norm2(vector<double> x) 
{
  assert(x.size() > 0);
  double n= x[0]*x[0];
  for (unsigned int i= 1; i < x.size(); i++) {
    n+= x[i]*x[i];
  }
  return sqrt(n);
}
  
template <class Type>
ostream &operator<<(ostream& os, vector<Type> a) 
{
  for (unsigned int i= 0; i < a.size(); i++) {
    os << a[i] << " ";
  }
  os << " ";
  return os;
}

#endif
