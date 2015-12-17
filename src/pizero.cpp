// Author:  kistler@mps.ohio-state.edu
// Date:    Jun 21, 2005


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


#include "pizero.h"

#include "photon.h"

#include "cmb.h"

using namespace GZKFast;

bool PiZero::decay(const Universe &universe, Particle::ParticleList &particleList) {
#if 0
    GFloat dn = exp(-age/lifeTime);
    if(GMath::getRand() > dn) {
#endif

        // 98.798 % piZero goes to 2 photons
        // 1.198 % to e+,e-, and photon (ignore for now)
                

        // no new nu made in this step
        // kind of boring if you like the nu
                
        // will later get distance info from 
        // photon down-scattering

        Momentum p(getMomentum());
        
        p /= p.mag();

        p *= getEnergy()/2.0;
        
        G4Vector p4(getEnergy()/2.0, p);
                
        Photon *photon1=new Photon(p4, getPosition(), getSourcePos());
        Photon *photon2=new Photon(p4, getPosition(), getSourcePos());
                
        particleList.push_back(photon1);
                        
        particleList.push_back(photon2);
        return true;
#if 0
    }
    return false;
#endif
}
