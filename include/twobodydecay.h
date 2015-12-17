// Author: John Cairns
// Date:   Jun 1, 2005

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


#ifndef TWOBODYDECAY_H
#define TWOBODYDECAY_H 1

#include <stdlib.h>

#include "physicalconstants.h"
#include "particle.h"


namespace GZKFast {

// kinematics of the 2 body decay
    class DLLEXPORT TwoBodyDecay {
      private:
	Particle *c;
	Particle *a;
	Particle *b;
      public:
	// a decay of form C -> A + B
	// i.e., c is consumed to form a and b.
	TwoBodyDecay(Particle *C, Particle *A, Particle *B);
	
	// After the decay

	// C is gone
	Particle* getC(void) { return NULL; }
	
	// A and B result
	Particle* getA(void) { return a; }
	Particle* getB(void) { return b; }
	

    };

};

#endif
