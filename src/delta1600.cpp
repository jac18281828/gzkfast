// Author:  John Cairns <john@2ad.com>
// Date:    May 12, 2005


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


#include <math.h>

#include "neutron.h"
#include "pizero.h"
#include "pion.h"
#include "proton.h"
#include "delta1600.h"
#include "threebodydecay.h"

using namespace GZKFast;

Delta1600::Delta1600(const G4Vector &p4, 
                     GFloat s_Ecm, 
                     const Position &rVec, 
                     GFloat q, 
                     const Position &origin) : 
    // we consider two energy ranges
    // "near threshold" and "high"
    // we arbitrarily choose gamma to determine
    // which range this particle falls into
    Delta(p4, s_Ecm, rVec, q, origin)
{
    m0=1.600*GeV;
}


bool Delta1600::decay(const Universe &universe,  
                      Particle::ParticleList &particleList) {


    GFloat dn = exp(-age/(getGamma()*getLifetime()));
    if(GMath::getRand()>dn) {
        
        // FIXME FIXME FIXME
        // what is the delta1600 decay chain?

        // can any/all of the reactions below actually take place?
        
        if(GMath::getRand() <= 0.75) { 
            
            // what is the ratio of pi+ vs pi0
            if(getCharge() > 0.0) {// this is delta+
                if(GMath::getRand() <= .5) {
                    // Proton Pi0 Pi0
                    Proton *proton = new Proton(get4Momentum(), r, getSourcePos());
                    PiZero *pi1    = new PiZero(get4Momentum(), r, getSourcePos());
                    PiZero *pi2    = new PiZero(get4Momentum(), r, getSourcePos());
                    
                    ThreeBodyDecay threeBodyDecay(this, proton, pi1, pi2);
                    particleList.push_back(proton);
                    particleList.push_back(pi1);
                    particleList.push_back(pi2);
                } else {
                    // Proton Pi+ Pi-
                    Proton *proton = new Proton(get4Momentum(), r, getSourcePos());
                    Pion *pi1    = new Pion(get4Momentum(), r, +1.0, getSourcePos());
                    Pion *pi2    = new Pion(get4Momentum(), r, -1.0, getSourcePos());
                    
                    ThreeBodyDecay threeBodyDecay(this, proton, pi1, pi2);
                    particleList.push_back(proton);
                    particleList.push_back(pi1);
                    particleList.push_back(pi2);
                }
            } else { // delta 0
                // what is the ratio of pi+ vs pi0
                if(GMath::getRand() <= .5) {
                    // Neutron Pi0 Pi0
                    Neutron *neutron = new Neutron(get4Momentum(), r, getSourcePos());
                    PiZero *pi1    = new PiZero(get4Momentum(), r, getSourcePos());
                    PiZero *pi2    = new PiZero(get4Momentum(), r, getSourcePos());
                    
                    ThreeBodyDecay threeBodyDecay(this, neutron, pi1, pi2);
                    particleList.push_back(neutron);
                    particleList.push_back(pi1);
                    particleList.push_back(pi2);
                } else {
                    // Neutron Pi+ Pi-
                    Neutron *neutron = new Neutron(get4Momentum(), r, getSourcePos());
                    Pion *pi1    = new Pion(get4Momentum(), r, +1.0, getSourcePos());
                    Pion *pi2    = new Pion(get4Momentum(), r, -1.0, getSourcePos());
                    
                    ThreeBodyDecay threeBodyDecay(this, neutron, pi1, pi2);
                    particleList.push_back(neutron);
                    particleList.push_back(pi1);
                    particleList.push_back(pi2);
                }
            }
            return true;
        }

        return Delta::decay(universe, particleList);
    }
    return false;
}

