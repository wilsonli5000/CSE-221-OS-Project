/* Author: Jingyuan Li 
  This script is created to test the ram access time. 
*/

#include <stdio.h>
#include <iostream>
#include "cycle.h"
#include <math.h>
#include <stdlib.h> 
using namespace std;

#define ITERATIONS 1000000


void measure_access_time(int arraysize, int arraystride)
{

  //declair the array with desired array size. all values are -1 by default.
  int* arr = new int[arraysize];
  for (int i = 0;  i < arraysize; i++)
    arr[i] = -1;

  int pos = 0;
  //set values to array according to stride. 
  for(int i = arraystride; i < arraysize; i += arraystride)
  {
    arr[pos] = i;
    pos = i;
  }
  arr[pos] = 0;
  arr[arraysize - 1] = 0;

  //calculate the ram access time to read the array into caches and memory
  double sum = 0.0;
  int c = 7; //NOTE: change this back to random number generator when benchmarking
  int access = 0;

  ticks t1 = getticks();        //ticks is defined in cycle.h. start recording cycle.
  for(int i = 0; i < ITERATIONS; i++)
  {
    c = access;
  }
  ticks t2 = getticks();
  double overhead = elapsed(t2,t1);

  ticks t3 = getticks();
  for(int i = 0; i < ITERATIONS; i++)
  {
    access = arr[c];
    c = access;
  }
  ticks t4 = getticks();
  sum += elapsed(t4,t3);

  delete[] arr;
  cout << ((sum-overhead)/clicks) << endl; 
  
}



int main(int argc, const char* argv[])
{

  int minsize = (int) pow(2,10);
  long long int maxsize = (long long int) pow(2,30);
  int stridesize;

  stridesize = (int) pow(2,20); 
  for(long long int memsize = minsize; memsize <= maxsize; memsize = memsize*1.3 )
  {
    int testing_size = memsize / 4;
    measure_access_time(testing_size, stridesize);
  }
  
}