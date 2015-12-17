// Author: John Cairns <john@2ad.com>
// Date:   May 17, 2005


//     GZKFast - An environment for astrophysical simulation.
//     Copyright (C) 2005 John A Cairns <john@2ad.com>

//     This program is free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.

//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.

//     You should have received a copy of the GNU General Public License
//     along with this program; if not, write to the Free Software
//     Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//     02110-1301  USA

//     John Cairns
//     2271 Coventry Rd    
//     Upper Arlington, OH, 43221

#include <limits>

#ifdef WIN32
#include <process.h>
#else
#include <unistd.h>
#endif
#include <time.h>

#include <math.h>

#include <stdlib.h>

#include <gsl/gsl_qrng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_math.h>

#include "gmath.h"

using namespace GZKFast;

// global static

// Fix windows broken compiler 
#ifdef WIN32
// global constants

GULongInt GMath::z1=153587801;
GULongInt GMath::z2=759022222;
GULongInt GMath::z3=12888503317;
GULongInt GMath::z4=1718083407;
GULongInt GMath::z5=123456789;

#else
// global constants

GULongInt GMath::z1=153587801ULL;
GULongInt GMath::z2=759022222ULL;
GULongInt GMath::z3=12888503317ULL;
GULongInt GMath::z4=1718083407ULL;
GULongInt GMath::z5=123456789ULL;

#endif


int  GMath::idum=-123456;

const int GMath::IA=16807;
const int GMath::IM=2147483647;
const int GMath::IQ=127773;
const int GMath::IR=2836;
const int GMath::NDIV=(1+(IM-1)/GMATH_RAND_TABLE_SIZE);
//const GFloat GMath::EPS=3.0e-16F;
const GFloat GMath::EPS=std::numeric_limits<GFloat>::epsilon();
const GFloat GMath::AM=1.0F/IM;
const GFloat GMath::RNMX=(1.0-EPS);

int GMath::iy=0;
int GMath::iv[GMATH_RAND_TABLE_SIZE];

void GMath::setSeed(int seed) {
    
    gsl_qrng *gslqrng = gsl_qrng_alloc (gsl_qrng_sobol, 1);
    double x;

    if(seed>0)
        idum=-seed;
    else
        idum=seed;

    for(int i=0; i<(seed&0xffff); i++) gsl_qrng_get (gslqrng, &x);

    // setup LFSR from system random sequence
    z1=0;
    while(z1<=1) {gsl_qrng_get(gslqrng, &x); z1=(GULongInt)floor(x*IM);}
    z2=0;
    while(z2<=7) {gsl_qrng_get(gslqrng, &x);z2=(GULongInt)floor(x*IM);}
    z3=0;
    while(z3<=15) {gsl_qrng_get(gslqrng, &x);z3=(GULongInt)floor(x*IM);}
    z4=0;
    while(z4<=127) {gsl_qrng_get(gslqrng, &x);z4=(GULongInt)floor(x*IM);}
    gsl_qrng_get(gslqrng, &x);
    z5=(GULongInt)floor(x*IM);
    
    gsl_qrng_free (gslqrng);
    
}

void GMath::setSeed(void) {
    // do something random here?

    time_t now=time(NULL);
    
    // try to remove dependance on current time
#ifdef WIN32
    int  pid=_getpid();
#else
    int  pid=getpid();
#endif
    now ^= pid | pid << 0x7 | pid << 0xf | pid << 0x16;
    
    // propagate significant bits upward
    
    now ^= (now&0xff) << 0xf;
    
    setSeed(now&0xffffffff);
}

GFloat GMath::getSignedRand(void) {
    return (getRand()-.5F)*2.0F;
}


GFloat GMath::getRand(void) {
	return lfsr258();
}
// LFSR Generator of
// Pierre L'Ecuyer, Mathematics of Computation, Vol 68
// Number 225, Jan 1999, p 261-269.

double GMath::lfsr258 (void)
{ /* Generates numbers between 0 and 1. */
    GULongInt b;
    
    // bill gates why do you have to be this way?
#ifdef WIN32
    b = (((z1 << 1) ^ z1) >> 53);
    z1 = (((z1 & 18446744073709551614) << 10) ^ b);
    b = (((z2 << 24) ^ z2) >> 50);
    z2 = (((z2 & 18446744073709551104) << 5) ^ b);
    b = (((z3 << 3) ^ z3) >> 23);
    z3 = (((z3 & 18446744073709547520) << 29) ^ b);
    b = (((z4 << 5) ^ z4) >> 24);
    z4 = (((z4 & 18446744073709420544) << 23) ^ b);
    b = (((z5 << 3) ^ z5) >> 33);
    z5 = (((z5 & 18446744073701163008) << 8) ^ b);
    return ((z1 ^ z2 ^ z3 ^ z4 ^ z5) * 5.4210108624275221e-20);
#else
    b = (((z1 << 1) ^ z1) >> 53);
    z1 = (((z1 & 18446744073709551614ULL) << 10) ^ b);
    b = (((z2 << 24) ^ z2) >> 50);
    z2 = (((z2 & 18446744073709551104ULL) << 5) ^ b);
    b = (((z3 << 3) ^ z3) >> 23);
    z3 = (((z3 & 18446744073709547520ULL) << 29) ^ b);
    b = (((z4 << 5) ^ z4) >> 24);
    z4 = (((z4 & 18446744073709420544ULL) << 23) ^ b);
    b = (((z5 << 3) ^ z5) >> 33);
    z5 = (((z5 & 18446744073701163008ULL) << 8) ^ b);
    return ((z1 ^ z2 ^ z3 ^ z4 ^ z5) * 5.4210108624275221e-20);
#endif
}


bool GMath::isApproxEqual(GFloat a, GFloat b, GFloat delta) {
	// test for zero
#if 0
	if((fabs(a)<delta)&&(fabs(b)<delta)) return true;
	return (fabs(a-b)<=fabs(delta*b));
#else
        if(gsl_fcmp(a, b, delta)==0) {
            return true;
        }
        return false;
#endif
}


bool GMath::isVecEqual(const GVector<GFloat> &a, const GVector<GFloat> &b, GFloat delta) {
#ifdef WIN32
    for(int i=0; i<size(); i++) {
#else
        for(int i=0; i<a.size(); i++) {
#endif
            if(!isApproxEqual(a.get(i), b.get(i), delta))
                return false;
        }
    return true;
};



GVector<GFloat> GMath::getRandVec(GVector<GFloat> &vec) {
    vec.setX(getSignedRand());
    vec.setY(getSignedRand());
    vec.setZ(getSignedRand());

    vec /= vec.mag();
}


GFloat GMath::getGaussianRand(GFloat sigma) {

    // Box Muller Method
    // return a gaussian
    GFloat S=1.0, V1, V2;
    do {
        V1=getSignedRand();           /* V1=[-1,1] */
        V2=getSignedRand();           /* V2=[-1,1] */
        S=V1 * V1 + V2 * V2;
    } while (S >=1.0);
	
    // first gaussian
    return sqrt(-2 * log(S) / S) * V1*sigma;
    // second gaussian available (ignored)
    // Y=sqrt(-2 * log(S) / S) * V2*sigma;

}
