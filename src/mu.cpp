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


#include "mu.h"

#include "nu.h"
#include "electron.h"

#include "twobodydecay.h"
#include "threebodydecay.h"
#include "cmb.h"

using namespace GZKFast;


bool Mu::decay(const Universe &universe, 
               Particle::ParticleList &particleList) {

    // does the muon decay?

    GFloat dn = exp(-age/(getGamma()*lifeTime));
    if(GMath::getRand() > dn) {
        
    
        // 100% of the time we get e+ Nu_e Nu_mu
        // ignore other branches

#if 0
        
        // we assume energy is divided equally for all 3
        
        GFloat energy = getEnergy()/3.0F;
        G4Vector p4(energy, Momentum());


        // FIXME FIXME FIXME
        // If this is a mu- then this is actually a positron
        
        Electron* electron = new Electron(p4, r, getSourcePos());
        
        Momentum pe(getMomentum());
        
        
        pe /= pe.mag();   // unit vector

        Momentum pnu(pe);
        
        GFloat pmag = sqrt(energy*energy - 
                           electron->getMass()*electron->getMass());
        
        pe *= pmag;
        
        electron->setMomentum(pe);
        
        particleList.push_back(electron);
        
        Nu *nu1 = new Nu(Nu::ELECTRON, electron->get4Momentum(), r, getSourcePos());
        Nu *nu2 = new Nu(Nu::MUON, electron->get4Momentum(), r, getSourcePos());
        
        pmag = sqrt(energy*energy - nu1->getMass()*nu1->getMass());
        pnu *= pmag;
        
        nu1->setMomentum(pnu);
        particleList.push_back(nu1);
        nu2->setMomentum(pnu);
        particleList.push_back(nu2);
#else
        Electron* electron = new Electron(get4Momentum(), r, getSourcePos());
        Nu *nu1 = new Nu(Nu::ELECTRON, electron->get4Momentum(), r, getSourcePos());
        Nu *nu2 = new Nu(Nu::MUON, electron->get4Momentum(), r, getSourcePos());
        
        ThreeBodyDecay tbd(this, electron, nu1, nu2);
        particleList.push_back(nu1);
        particleList.push_back(nu2);
        particleList.push_back(electron);
#endif
        return true;

    }
    return false;
}
