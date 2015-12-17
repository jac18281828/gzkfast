// Author:  John Cairns <john@2ad.com>
// Date:    Feb 28, 2007


//     GZKFast - An environment for astrophysical simulation.
//     Copyright (C) 2005-2007 John A Cairns <john@2ad.com>

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
#include "electron.h"
#include "twobodydecay.h"

#include "cmb.h"


using namespace GZKFast;


Z::Z(const G4Vector &p4, 
     const Position &rVec, 
     const Position &origin) : 
    lifeTime(),
    m0(91.1876*GeV), 
    pu(p4),
    r(rVec),
    src(origin),
    age(0.0)
{

}


bool Z::decay(const Universe &universe, Particle::ParticleList &particleList) {
    // decay modes of Z boson

    GFloat dn = exp(-age/(getGamma()*getLifetime()));
    if(GMath::getRand()>dn) { // the Z will decay

        // the decay modes of the Z
        GFloat trialProb = GMath::getRand();

        // 3 percent to e+e-
        if(trialProb < 0.03) {
            // form an electron positron pair

            Electron *e1 = new Electron(get4Momentum(), 
                                        getPosition(), 
                                        getSourcePos());

            // FIXME FIXME FIXME 
            // positive electron 
            // charge does not matter?
            Electron *e2 = new Electron(get4Momentum(), 
                                        getPosition(), 
                                        getSourcePos());

            TwoBodyDecay tbd(this, e1, e2);
            particleList.push_back(e1);
            particleList.push_back(e2);
            return true;
        } else {
            // FIXME FIXME FIXME
            
            // for now we simply decay to invisible states
            // actually decay to hadrons
            return true;
        }
    }
    return false;

}

GFloat Z::getSigma(GFloat s,
                   GFloat sa,
                   GFloat sb) {

    if(s > m0*m0) {
        // lambda is approx for CMS
        // (in CM lam = hbar c/ p c, p c = M0/2)
        GFloat lam = 2.0*hbar*SpeedOfLight/getMass();
        
        return Particle::resonance(s, 
                                   lam, 
                                   getSpin(), 
                                   getWidth(), 
                                   getMass(), 
                                   sa, 
                                   sb);
    }
    return 0.0;
}
