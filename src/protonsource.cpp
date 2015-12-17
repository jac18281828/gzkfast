// Author:  John Cairns <john@2ad.com>
// Date:    July 26, 2005


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



#include "protonsource.h"
#include "gmath.h"

using namespace GZKFast;


ProtonSource::ProtonSource(GFloat minE, 
			   GFloat maxE, 
			   GFloat a, 
			   const Position &rVec,
			   const GFloat td) :
    protonSpectrum(minE, maxE, a),
    r(rVec),
    tdPct(td/100.0) {
    
    // normalize tdPct to a percent of the max energy 
    GFloat norm = protonSpectrum.getdNdE(maxE);
    norm /= protonSpectrum.getdNdE(minE);
    
    tdPct *= norm;
    
}

ProtonSource::~ProtonSource() {
}

Particle *ProtonSource::getEvent(const Position &dest) {

    
    // create a new proton of a sampled energy
    GFloat energy = protonSpectrum.getMaxEnergy();

    // every so often, in tdPct, use a deltaSrc instead
    if(GMath::getRand() < tdPct) {
        if(deltaSrcIter == deltaSrc.end()) {
            deltaSrcIter = deltaSrc.begin();
        } else {
            energy = (*deltaSrcIter++).getDistEnergy();
        }
    } else {
        // just return on spectrum
        energy = protonSpectrum.getDistEnergy();
    }
    
    // compute the magnitude of the momentum
    GFloat pProton = sqrt(energy*energy - NucleonMass*NucleonMass);
    
    // create a unit momentum from position to dest
    Momentum pProtonVec(dest);
    
    pProtonVec -= r;
    // make a unit vector
    pProtonVec /= pProtonVec.mag();
    
    // now set the x,y,z values relative to the unit momentum
    pProtonVec *= pProton;

    G4Vector p4(energy, pProtonVec);

    Proton *proton = new Proton(p4, r, r);

    return proton;
}

const Position &ProtonSource::getPosition(void) const {
	return r;
}

void ProtonSource::addDeltaSrc(GFloat energy) {
    deltaSrc.push_back(DeltaSpectrum(energy));
    
    deltaSrcIter=deltaSrc.begin();
}
