// Author:  John Cairns <john@2ad.com>
// Date:    Jun 1, 2005
// Complete rewrite: Sep 6 2005


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


#include <iostream>
#include <math.h>

#include "gmath.h"
#include "twobodydecay.h"
#include "glorentz.h"
#include "g4vector.h"
#include "gmatrix.h"

using namespace GZKFast;

// handle the decay of particles
// in the form C -> A + B

TwoBodyDecay::TwoBodyDecay(Particle *C, Particle *A, Particle *B) {
    c=C;
    a=A;
    b=B;
    
    // exact solution of 2body problem in CM frame
    if(c->getMass() > 0.F) {
        
        // standard 2 body kinematics, see particle physics texts
        // including stanev.
        
        G4Vector pc4(c->getMass(), Momentum()); // in rest frame
        
        // mass of c squared
        GFloat cM2=c->getMass()*c->getMass();
        
        // exact CM soln to particle b momentum
        GFloat pbMag = sqrt((cM2 -
                             pow(a->getMass() + b->getMass(), 2.0))*
                            (cM2 -
                             pow(a->getMass() - b->getMass(), 2.0))) /
            (2.0*c->getMass());
        
        // energy of b in CM
        GFloat bE = sqrt(pbMag*pbMag + b->getMass()*b->getMass());
        
        // if C has energy in CM frame we would do a lorentz transform
        // here with gammaCM
        // ... we assume C has no CM energy
        
        // Monte Carlo a b momentum in 3 space
        Momentum pb;
        
        pb.setX(GMath::getSignedRand());
        pb.setY(GMath::getSignedRand());
        pb.setZ(GMath::getSignedRand());
        
        pb /= pb.mag();  // unit vector
        
        // b points with magnitude pbMag
        pb *= pbMag;
        
        G4Vector pb4(bE, pb);
        
        // conservation of energy and momentum
        G4Vector pa4(pc4 - pb4);
        
        GFloat gamma = c->getGamma();
        
        Velocity beta;
        
        GLorentz Apart(gamma, c->getBeta(beta));
        
        // exact soln in lab
                
        b->set4Momentum(Apart*pb4);
                
        a->set4Momentum(Apart*pa4);
    } 
    
}

 
