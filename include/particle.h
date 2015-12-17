// Author: John Cairns <john@2ad.com>
// Date:   May 12, 2005


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



#ifndef PARTICLE_H
#define PARTICLE_H 1

#include <list>

#include "physicalconstants.h"
#include "universe.h"

#include "gvector.h"
#include "g4vector.h"

namespace GZKFast {

// forward declaration
    class CMB;

    class DLLEXPORT Particle {
      public:
        // common particle functionality 
        // provided for all particles

        // Lorentz gamma factor (boost)
        virtual GFloat	getGamma(void) const;

        // return the beta (velocity/c) vector
        // (calculated from momentum)
        virtual const Velocity &getBeta(Velocity &v) const;

        
        // return the velocity vector
        // (calculated from momentum)
        virtual const Velocity &getVelocity(Velocity &v) const;

        // calculate the Breit-Wigner resonance cross
        // section for p1 p2 -> p3
        static GFloat resonance(GFloat s, 
                                GFloat lambda, 
                                GFloat J,
                                GFloat gamma,
                                GFloat M0,
                                GFloat sa,
                                GFloat sb);

        virtual GFloat getSourceDist(void) const;


        virtual GFloat getProbZero(GFloat dx, GFloat density, GFloat sigma) const;
	

      public:
        
        // public types
                          
        // list of particles
        typedef         std::list<Particle *> ParticleList;

      public:

        // interface required for all particles
        
        // set the energy [eV] of the particle
        virtual void setEnergy(GFloat en) = 0;
        // get the energy of the particle
        virtual GFloat getEnergy(void) const = 0;

        // set the momentum [eV] of the particle
        virtual void setMomentum(const Momentum &pvec) = 0;
        // get the momentum of the particle
        virtual const Momentum &getMomentum(void) const = 0;


        // set the 4 momentum of the particle
        virtual void set4Momentum(const G4Vector &p4) = 0;
        
        // get the 4 momentum of the particle
        virtual const G4Vector &get4Momentum(void) const = 0;

        // set the position vector [cm] of the particle.
        virtual void setPosition(const Position &r) = 0;
        // get the position vector of the particle from Earth
        virtual const Position &getPosition(void) const = 0;

        // Tell the particle to decay, return an array of resulting particles
        // in particleList, return true if the decay took place and false otherwise
        virtual bool decay(const Universe &universe, 
                           ParticleList &particleList) = 0;

        // ask the particle if it will interact with the specified particle
        // test a random variable normalized to the real probability and 
        // if the particle interacts modify the list of products
        virtual bool interact(const Particle *particle, 
                              GFloat density,
                              GFloat dx,
                              ParticleList &particleList) = 0;

        // return the REST mass of the particle
        virtual GFloat getMass(void) const = 0;
        
        // return the charge of this particle
        // +1 - +e
        //  0 - neutral
        // -1 - -e
        virtual GFloat    getCharge(void) const = 0;

        // get life in seconds
        virtual GFloat getAge(void) const = 0;
        
        // set life in seconds
        virtual void   setAge(GFloat t) = 0;

        // get the isospin of the particle
        virtual GFloat getSpin(void) const = 0;

        // get the width of the resonance
        virtual GFloat getWidth(void) const = 0;


        // get the source position where the particle was created
        virtual const Position &getSourcePos(void) const = 0;

    };

};

#endif
