// Author: John Cairns
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


#ifndef GMATH_H
#define GMATH_H 1

#include "physicalconstants.h"
#include "gvector.h"
#include "gerror.h"

namespace GZKFast {

   
// global macros since const is broken in windows
#define GMATH_RAND_TABLE_SIZE 32

    class DLLEXPORT GMath {
      private:

        static const int IA;
        static const int IM;
        static const int IQ;
        static const int IR;
        static const int NDIV;
        static const GFloat EPS;
        static const GFloat AM;
        static const GFloat RNMX;

        static  int idum;
        static  int iy;
        static int iv[GMATH_RAND_TABLE_SIZE];

		
        // Linear Feedback Shift Registers
        static GULongInt z1;
        static GULongInt z2;
        static GULongInt z3;
        static GULongInt z4;
        static GULongInt z5;


      protected:

        // LFSR Generator of
        // Pierre L'Ecuyer, Mathematics of Computation, Vol 68
        // Number 225, Jan 1999, p 261-269.
        static inline double lfsr258(void);

      public:
	
        // seed random generator
        static void setSeed(int seed);
        // seed the random generator with 
        // pseudo random system information
        static void setSeed(void);
        // return a random float between 0. and 1.
        static GFloat getRand(void);


        // return a unit magnitude vector with
        // random components

        static GVector<GFloat> getRandVec(GVector<GFloat> & vec);

        // return a random float between -1. and 1.
        static GFloat getSignedRand(void);

        // return a random gaussian with mean 0 and deviation sigma
        static GFloat getGaussianRand(GFloat sigma);

        // test a floating point number to be within 
        // a small range of a specified target
        static bool isApproxEqual(GFloat a, GFloat b, GFloat delta);

        // test a vector approxequal to another
        static bool isVecEqual(const GVector<GFloat> &a, const GVector<GFloat> &b, GFloat delta);


        static inline GFloat min(GFloat a, GFloat b) {
            return (a<b?a:b);
        }

        static inline GFloat max(GFloat a, GFloat b) {
            return (a>b?a:b);
        }

    };
}

#endif
