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

#include "delta.h"

#include "cmb.h"
#include "gmath.h"
#include "physicalconstants.h"
#include "twobodydecay.h"

#include "pion.h"
#include "neutron.h"
#include "pizero.h"

using namespace GZKFast;

Delta::Delta(const G4Vector &p4, 
             const GFloat sChan, 
             const Position &rVec, 
             const GFloat q, 
             const Position &origin) : 
    // we consider two energy ranges
    // "near threshold" and "high"
    // we arbitrarily choose gamma to determine
    // which range this particle falls into
    m0(1.2316F*GeV),
    pu(p4),
    r(rVec),
    src(origin),
    age(0.0),
    charge(q)
{

}


bool Delta::decay(const Universe &universe,  
                  Particle::ParticleList &particleList) {


    // delta usually decays to Nucleon Pion
    // 99 % of the time

    GFloat dn = exp(-age/(getGamma()*getLifetime()));
    if(GMath::getRand()>dn) {

        if(getCharge() > 0.0) { // this is delta+

            // FIXME FIXME FIXME
            // add case for getCharge() == 2 (delta++)


            // there are two possibilites
            // a neutron and a pi+ and a proton and a pi0
    
            // based on isospin calculations (?FIXME?) it is thought
            // that n, pi+ occurs 1/3 of the 
            // time

            // what is the ratio of pi+ vs pi0
            if(GMath::getRand() <= 0.33333) {
                // a neutron and a pi+
                Pion *pi = new Pion(get4Momentum(), r, charge, getSourcePos());
                Neutron *n = new Neutron(get4Momentum(), r, getSourcePos());
            
                TwoBodyDecay tbd(this, pi, n);
            
                particleList.push_back(pi);
                particleList.push_back(n);
            
            } else {
                // a proton and a pi0
                PiZero *pi = new PiZero(get4Momentum(), r, getSourcePos());
                Proton *p = new Proton(get4Momentum(), r, getSourcePos());
            
                TwoBodyDecay tbd(this, pi, p);
                particleList.push_back(pi);
                particleList.push_back(p);
            }

        } else { // this is delta0
            if(GMath::getRand() <= .33333) {
                // a neutron and a pi0
                PiZero *pi = new PiZero(get4Momentum(), r, getSourcePos());
                Neutron *n = new Neutron(get4Momentum(), r, getSourcePos());
            
                TwoBodyDecay tbd(this, pi, n);

                particleList.push_back(pi);
                particleList.push_back(n);

            } else {
                // a proton and a pi-
                Pion *pi = new Pion(get4Momentum(), r, -1.0, getSourcePos());
                Proton *p = new Proton(get4Momentum(), r, getSourcePos());

                TwoBodyDecay tbd(this, pi, p);
                particleList.push_back(pi);
                particleList.push_back(p);

            
            }

        }

        return true;
    }
    return false;
}

GFloat Delta::getSigma(GFloat s,
                       GFloat sa,
                       GFloat sb) {

    if(s > getMass()*getMass()) {
    
        // lambda is approx for CMS
        // (in CM lam = hbar c/ p c, p c = M0/2)
        GFloat lam = 2.0*hbar*SpeedOfLight/getMass();
        
        return Particle::resonance(s, 
                                   lam, 
                                   getSpin(), 
                                   getWidth(), 
                                   getMass(), 
                                   sa, sb);
    }
    return 0.0;
}
