// Author:  John Cairns <john@2ad.com>
// Date:    Jun 22, 2005

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

#include "physicalconstants.h"
#include "gmath.h"
#include "gvector.h"
#include "g4vector.h"
#include "gmatrix.h"
#include "grotation.h"
#include "glorentz.h"
#include "threebodydecay.h"

using namespace GZKFast;


GFloat ThreeBodyDecay::decayPrecision=PrecisionGoal;

// handle the decay of particles
// in the form A -> B + C + D

ThreeBodyDecay::ThreeBodyDecay(Particle *A, 
                               Particle *B, 
                               Particle *C, 
                               Particle *D) {

    Particle *aPart=A;
    bPart=B;
    cPart=C;
    dPart=D;

    GFloat ma=aPart->getMass();
    GFloat mb=getB()->getMass(), mc=getC()->getMass(), md=getD()->getMass();



    // transform to the rest frame of particle a,
    // so we must compute the relative velocities
    GFloat gamma = aPart->getGamma();

    Velocity beta;
    aPart->getBeta(beta);

    // now set a at rest
    
    G4Vector a4(ma, 0.0, 0.0, 0.0);
    aPart->set4Momentum(a4);
        
    // now do the decay...

    // Monte Carlo a solution to 3 body decay
    // NOTE: this can take an arbitrarily long time to execute

    // by conservation of energy
    // we can limit our guess to this maximum value
    GFloat maxE = aPart->getEnergy()-mb-mc;

    GFloat maxP = sqrt(maxE*maxE-md*md);

    // one particle can only take half the available momentum to conserve total momentum
    maxP /= 2.0;

    // the momentum of each particle
    Momentum pd;
    Momentum pc;
    
    do {

        // Guess momentum values for B, C, and D


        // because we have only 3 constraints we must
        // guess at a minimum the value of pd, and the
        // scattering angle between d and c.
        
        // guess d (blindly)
 
        dPart->setMomentum(getRandomMomentum(pd, maxP));
        dPart->setEnergy(sqrt(getD()->getMomentum()*getD()->getMomentum()+md*md));

        // guess the scattering angle thetaDC blindly
        GFloat thetaCD=GMath::getRand()*PI;
        
        // with this info it is possible to compute |PC|, and PB directly

        // PC is most difficult
        // 
        //
        // From conservation of momentum we write:
        //
        // Pb + Pc + Pd = Pa
        // 
        // Pc + Pd = Pa - Pb
        //
        // (squaring)
        // mc^2 + md^2 + 2 Pc.Pd = ma^2 + mb^2 - 2 ma Eb
        //
        // then conservation of energy
        // Eb = Ea - Ec - Ed
        //
        // also from Einsteins famous equation we have
        // Pc.Pd =  sqrt[Ec^2 - mc^2] * sqrt[Ed^2 - md^2] cos(thetaDC)
        // 
        // finally
        // 
        // mc^2 + md^2 + 2 sqrt(Ec^2 - mc^2) sqrt(Ed^2 - md^2) cos(thetaDC)
        //       == ma^2 + mb^2 - 2 ma (Ea - Ec - Ed)
        // 
        // then painfully solving for Ec we have

        GFloat Ea=aPart->getEnergy();
        GFloat Ed=getD()->getEnergy();
        GFloat Pd = getD()->getMomentum().mag();
        // a new solution
        GFloat Ac = ma*(ma - 2*Ea) + mb*mb - mc*mc - md*md + 2.0*ma*Ed;
        
        // now we solve for
        // 2 Pc Pd Cos[thetaCD] - 2 ma Sqrt[Pc^2 + mc^2] == A
        
        GFloat whichPc=GMath::getRand()<.5?1.0:-1.0;
        GFloat Pc = -Ac*Pd*cos(thetaCD) + 
            whichPc*sqrt(Ac*Ac*ma*ma-4*pow(ma, 4.0)*mc*mc + 4*pow(ma*mc*Pd*cos(thetaCD), 2.0));
        
        Pc /= 2.0*(ma*ma - pow(Pd*cos(thetaCD), 2.0));

        // start with pc oriented toward pd
        pc=pd;

        // now pc is a vector with angle thetaCD to pd, and arbitrary
        // orientation in 2PI...we just choose
        // now pc has angle thetaCD from pd and a random orientation
        pc = rotate(pc, thetaCD, GMath::getRand()*2.0*PI);
        
        // with arbitrary orientation in space

        // construct a unit vector
        pc /= pc.mag();
        // set the magnitude correctly
        pc *= Pc;   // magnitude of Pc;
        cPart->setMomentum(pc);
        GFloat Ec = sqrt(Pc*Pc + mc*mc);
        cPart->setEnergy(Ec);

        // so C and D are completely determined now,
        // just choose B by momentum conservation
        G4Vector pb(aPart->get4Momentum());
        pb -= getC()->get4Momentum();
        pb -= getD()->get4Momentum();

        bPart->set4Momentum(pb);

    } while(!isSolution(aPart, bPart, cPart, dPart)); 
    
    // whoa! we finally finished
    
    // now boost particles back into the lab frame
    
    GLorentz x4Boost(gamma, beta);
#if 0

    std::cout.flush();
    
    std::cout << "p4b = " << b->get4Momentum();
    std::cout << "boost = " << x4Boost;
#endif

    bPart->set4Momentum(x4Boost*getB()->get4Momentum());
    cPart->set4Momentum(x4Boost*getC()->get4Momentum());
    dPart->set4Momentum(x4Boost*getD()->get4Momentum());        
}


// If energy, and momentum are conserved, and 4 vector squared
// are invariant then this must be a valid soltuion to
// the three body decay problem.
// 
bool ThreeBodyDecay::isSolution(Particle *A, 
                                Particle *B, 
                                Particle *C, 
                                Particle *D) {
    
    // B satisfies E=mc^2
    if(!GMath::isApproxEqual(B->getEnergy()*B->getEnergy(), 
                             B->getMomentum()*B->getMomentum()+B->getMass()*B->getMass(), decayPrecision)) 
        return false;

    // 4 momentum squared is invariant (free to choose CM frame)
    G4Vector Ptot(B->get4Momentum()+C->get4Momentum()+D->get4Momentum());

    GFloat maSq=A->getMass()*A->getMass();
    GFloat pSq = Ptot*Ptot;

    if(!GMath::isApproxEqual(maSq,pSq,decayPrecision)) {
        return false;
    }

    // conservation of total energy
    if(!GMath::isApproxEqual(A->getEnergy(),
                             B->getEnergy() + 
                             C->getEnergy() + 
                             D->getEnergy(), decayPrecision)) {
        return false;
    }

    // conservation of total momentum
    Momentum pTot(B->getMomentum());
    pTot +=       C->getMomentum();
    pTot += D->getMomentum();

    if(!GMath::isVecEqual(A->getMomentum(),pTot, decayPrecision)) {
        return false;
    }

    return true;
}


const Momentum &ThreeBodyDecay::getRandomMomentum(Momentum &p, GFloat maxP) const {
        
    GFloat   maxComponentSq=maxP*maxP;

    p.setX(GMath::getSignedRand());
    p.setY(GMath::getSignedRand());
    p.setZ(GMath::getSignedRand());

    p /= p.mag(); // unit vector

    // scale the magnitude from 1 up to max value
    p *= maxP*GMath::getRand();  

    return p;   
}

void ThreeBodyDecay::setDecayPrecision(GFloat prec) {
    decayPrecision=prec;
}


const GVector<GFloat> &ThreeBodyDecay::rotate(GVector<GFloat> &vec,
                                              GFloat theta,
                                              GFloat orientation) {

    // vector now points along some direction in space
    // we don't know how to do transformations on it
    // so we must first transfer to a coordinate system where
    // we know how to work

    // to transform vec we need to know two things,
    // direction cosines and the projection in the yz plane.
    
    // direction cosines first

    GFloat mag=vec.mag();
    
    vec /= mag;  // make a unit vector
    
    // direction cosines are the components of the unit vector
    GFloat a=vec[0];
    GFloat b=vec[1];
    GFloat c=vec[2];

    // d is the projection of vec in the yz plane
    GFloat d=sqrt(b*b + c*c);

    // now we can transform the vector twice to orient it along the z axis

    // first lets go into the yz plane
    GRotation ryz(c/d, b/d, -b/d, c/d, GRotation::X);
    
    // apply rotation
    vec = ryz*vec;  
    
    // now orient vec along z
    GRotation rz(d, a, -a, d, GRotation::Y);

    vec = rz*vec;
    // vec is now a unit vector along the z axis
    // so we can apply the known transformations in order to
    // get the rotation and orientation that we want
    
    
    GRotation Rtheta(theta, GRotation::X); // either x or y rotation is
                                           // fine
    vec = Rtheta*vec;

    // now apply orientation about Z axis
    GRotation Rorient(orientation, GRotation::Z);
    vec = Rorient*vec;

    // now we have vector transformed as we want it all we
    // have to do is restore it.
    
    rz.invert();

    vec = rz*vec;

    ryz.invert();
    vec = ryz*vec;

    vec *= mag;
    return vec;
}

