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



#include <iostream>

#include "z.h"
#include "nu.h"

#include "cmb.h"


using namespace GZKFast;

bool Nu::decay(const Universe &universe, Particle::ParticleList &particleList) {
    // no neutrino decay
    return false;
}

bool Nu::interact(const Particle *particle, 
                  GFloat density, 
                  GFloat dx,
                  ParticleList &particleList) {


    // handle nu nu -> e+ e-
    int initSz = particleList.size();
    
    const Nu *nu;
    
    if((nu=dynamic_cast<const Nu*>(particle))!=NULL) {

        // massive enough to form a z?
        GFloat s = 2.0*getEnergy()*nu->getEnergy()*2.0;

        Z* Zres = new Z(get4Momentum(), getPosition(), getSourcePos());

        // form a Z at this energy

        GFloat sa = 0.5; // nu 1
        GFloat sb = 0.5; // nu 2

        // crosss section to form this Z
        GFloat nunuZSigma=Zres->getSigma(s, sa, sb);

        if(nunuZSigma > 0.0) {
            
            GFloat trialProb = GMath::getRand();
            if(trialProb < (1.0 - getProbZero(dx, density, nunuZSigma))) {
                // we formed a Z
                particleList.push_back(Zres);
            } else {
                delete(Zres);
            }
        } else {
            delete(Zres);
        }

    }

    return (particleList.size()>initSz);

}

