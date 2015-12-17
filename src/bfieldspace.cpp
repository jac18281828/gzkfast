// Author: John Cairns <john@2ad.com>
// Date:   Aug 3, 2005


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



#include <stdlib.h>
#include <math.h>

#include "g4vector.h"
#include "gmatrix.h"
#include "physicalconstants.h"
#include "bfieldspace.h"
#include "gmath.h"

using namespace GZKFast;

BFieldSpace::BFieldSpace(Universe *u) : 
    universe(u)
{
}


// propagate a particle through space
bool BFieldSpace::propagate(Particle *particle, Particle::ParticleList &particleList) {

    // compute the hamiltonian for xdot and pdot
    // relativistic corrections

    GFloat   dt = universe->getTimeStep();

    Velocity v;

    // in its frame
    particle->getVelocity(v);

    // for a static uniform magnetic induction bField.
    if(particle->getCharge() != 0.0) {
        // in its frame
        Momentum p(particle->getMomentum());
        // in the particle frame
        p /= particle->getGamma();

        Momentum dp;


        // perturb the bfield slightly
        BField bField(universe->getBField());

        GFloat bMag = bField.mag();

        BField dB;
        dB.setX(GMath::getSignedRand());
        dB.setY(GMath::getSignedRand());
        dB.setZ(GMath::getSignedRand());
        dB /= dB.mag(); // unit vec

        dB *= bMag/10.0;

        bField += dB;
#if 0           // this effect is not measurable

                // particle loses energy via syncrotron radiation [Sangjin Lee]
        
        GFloat dE = -4/3* SigThompson * (bField*bField)/(8*PI)*
            pow(fabs(particle->getCharge())*ElectronCharge*ElectronMass/particle->getMass(), 4.0)*
            pow(particle->getEnergy()/ElectronMass, 2.0)*dt;
        
        particle->setEnergy(particle->getEnergy() + dE);
        
        
        p /= p.mag();
        
        GFloat pmag = sqrt(particle->getEnergy()*particle->getEnergy() - particle->getMass()*particle->getMass());
        
        p*=pmag;
#endif


        // particle is deflected by field
        dp.cross(v, bField);
        dp *= particle->getCharge()*ElectronCharge/SpeedOfLight;
        
        p += dp*dt;
        
        p *= particle->getGamma(); // back to lab frame
        
        particle->setMomentum(p);
        // unchanged by B field alone
        //particle->setEnergy(sqrt(p*p + particle->getMass()*particle->getMass()));
        
    }

    
    Position r(particle->getPosition());
    r += v*dt;
    
    particle->setPosition(r);
    
    particle->setAge(particle->getAge()+dt);    
    return false;
}
        
// the redshift due to propagation
Particle *BFieldSpace::redshift(Particle *particle) {
    Position dx;
    particle->getVelocity(dx);
    dx *= universe->getTimeStep();

    GFloat dz = universe->getRedshift(dx.mag());

    particle->setEnergy(particle->getEnergy()/(1.0+dz));
    
    
    Momentum p(particle->getMomentum());
    
    p /= p.mag();

    GFloat pmag = sqrt(particle->getEnergy()*particle->getEnergy() - particle->getMass()*particle->getMass());

    p*=pmag;
    
    particle->setMomentum(p);

    return particle;
}
