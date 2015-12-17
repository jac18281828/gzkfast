// Author:  John Cairns <john@2ad.com>
// Date:    May 31, 2005


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

#include "protonspectrum.h"

using namespace GZKFast;


ProtonSpectrum::ProtonSpectrum(GFloat startE, GFloat endE, GFloat a) {
    startEnergy=startE;
    endEnergy=endE;
    alpha=a;

  
    normEnergy=getdNdE(startEnergy);
	//std::cout << normEnergy << std::endl;
}

ProtonSpectrum::ProtonSpectrum(const ProtonSpectrum &spec) {
    startEnergy=spec.startEnergy;
    endEnergy=spec.endEnergy;
    alpha=spec.alpha;

    normEnergy=spec.normEnergy;
}

GFloat ProtonSpectrum::getdNdE(GFloat E) {
	return pow(E, alpha);
}

//
// return an E^alpha weighted energy distribution

GFloat ProtonSpectrum::getDistEnergy(void) const {

#if 0
    // flat "anti-gzk" spectrum
    return getMinEnergy()+(getMaxEnergy()-
                           getMinEnergy())*GMath::getRand();
#endif

	// NOTE:   The current 64 bit version of getRand() returns a number
	//         n*5.421e-20, therefore the highest possible energy (n=1)
	//         returned by this implementation is roughly 5e28 eV ... 
	//
	// WARNING:  getRand() is capable of returning 0, which would
	//           make the following return complex infinity about
	//           1 in 1e20 calls.   Too rare to add a test for it.
	//
	//           This will cause the program to crash about 1 in 10^16
	//           runs, or more frequently if you are on Windows, unlucky
	//           or both.
	//
    GFloat energy;
    while((energy=pow(normEnergy*GMath::getRand(), 1.0/alpha)) >
          getMaxEnergy());    
    return energy;
};


// return the least energy of this distribution
GFloat ProtonSpectrum::getMinEnergy(void) const {
	return startEnergy;
}

// return the largest energy of this distribution
GFloat ProtonSpectrum::getMaxEnergy(void) const {
	return endEnergy;
}

