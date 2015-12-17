// Author: John Cairns
// Date:   Jun 22 2005


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



#ifndef THREEBODYDECAY_H
#define THREEBODYDECAY_H 1


#include "physicalconstants.h"
#include "particle.h"


namespace GZKFast {

// montecarlo a solution for a 3 body decay
// 
// A -> B + C + D
    class DLLEXPORT ThreeBodyDecay {
		
      private:
        // how precisely to test solutions
        static GFloat			decayPrecision;

        Particle *bPart;
        Particle *cPart;
        Particle *dPart;

      protected:
	// If energy, and momentum are conserved, and 4 vector squared
	// are invariant then this must be a valid soltuion to
	// the three body decay problem.
	// 
        bool isSolution(Particle *A, Particle *B, Particle *C, Particle *D);

        // generate a random momentum vector with a maximum magnitude
        // for monte carlo
        const Momentum &getRandomMomentum(Momentum &p,
                                          GFloat maxP) const;

        // rotate the vector by thetaCD with an arbitrary orientation
        const GVector<GFloat> &rotate(GVector<GFloat> &vec, 
                                      GFloat theta,
                                      GFloat orientation);

       public:

        // a decay of form A -> B + C + D
        // A is consumed to form B, C, and D.
        
        // This problem is not exactly solvable, so this constructor
        // monte-carlo's a solution.  
        // 
        // NOTE: Since the monte-carlo method is used, running times 
        //       may be arbitrarily large.
        ThreeBodyDecay(Particle *A, Particle *B, Particle *C, Particle *D);

        // After the decay return the particle states
        
        // Particle A is gone
        Particle* getA(void) { return NULL; }

        // return the resulting a and b particles with
        // correct energy and momentum
        Particle *getB(void) { return bPart; }
        
        Particle *getC(void) { return cPart; }

        Particle *getD(void) { return dPart; }

        static void setDecayPrecision(GFloat prec);
    };
};

#endif


