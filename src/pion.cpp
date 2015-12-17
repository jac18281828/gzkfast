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


#include "pion.h"

#include "mu.h"
#include "nu.h"

#include "twobodydecay.h"
#include "cmb.h"

using namespace GZKFast;

bool Pion::decay(const Universe &universe, Particle::ParticleList &particleList) {

    GFloat dn = exp(-age/(lifeTime*getGamma()));
    if(GMath::getRand() > dn) {
    
        // 99.988 % pi+ goes to mu+ and neutrino
        // ignore other branches
        
        
        // we assume
        // Enu = m_mu/m_pi Epi
        Mu *mu=new Mu(get4Momentum(), r, getCharge(), getSourcePos());
        Nu *nu=new Nu(Nu::MUON, get4Momentum(), r, getSourcePos());

        TwoBodyDecay tbd(this, mu, nu);
        particleList.push_back(mu);
        particleList.push_back(nu);
		return true;
    }

    return false;
}
