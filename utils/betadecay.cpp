// Author: John Cairns <john@2ad.com>
// Date:   Jun 22, 2005

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


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

#include "physicalconstants.h"
#include "particle.h"
#include "neutron.h"
#include "proton.h"
#include "electron.h"
#include "nu.h"
#include "threebodydecay.h"
#include "ghistogram.h"

using namespace GZKFast;

//
// This program generates a histogram of electron energies
// resulting from beta decay
int main(int argc, char *argv[]) {

    const int nEvents = 5000;
    
    const int nBins   = 50;
    const GFloat minEnergy=0.1*MeV;
    const GFloat maxEnergy=1.0*MeV;

	const char*  electronFileName="../data/beta_electronhist.dat";
	const char*  protonFileName="../data/beta_protonhist.dat";
	const char*  nuFileName="../data/beta_nuhist.dat";

	// initalize the random number generator

	GMath::setSeed(); 


    GHistogram electronEnergyHist(nBins, minEnergy, maxEnergy);
	GHistogram protonEnergyHist(nBins, minEnergy, maxEnergy);
	GHistogram nuEnergyHist(nBins, minEnergy, maxEnergy);
    
    // neutron beta decay
    // a neutron goes to electron and proton
	time_t now = time(NULL);

    for(int i=0; i<nEvents; i++) {
	
        GFloat eOffset = .5*MeV*GMath::getRand();

        G4Vector p4(939.56533*MeV + eOffset, GVector<GFloat>(0.0));

        Neutron  n(p4, GVector<GFloat>(0.0), GVector<GFloat>(0.0));
        Momentum nP;
        nP.setX(GMath::getSignedRand());
        nP.setY(GMath::getSignedRand());
        nP.setZ(GMath::getSignedRand());
        nP /= nP.mag();
        nP *= sqrt(n.getEnergy()*n.getEnergy() - n.getMass()*n.getMass());
		n.setMomentum(nP);

        Proton   p(n.get4Momentum(), GVector<GFloat>(0.0), GVector<GFloat>(0.0));
        Electron e(n.get4Momentum(), GVector<GFloat>(0.0), GVector<GFloat>(0.0));
        Nu       nu(Nu::ELECTRON, n.get4Momentum(), GVector<GFloat>(0.0), GVector<GFloat>(0.0));

        // monte carlo a decay n => p + e + nu
        ThreeBodyDecay tbd(&n, &p, &e, &nu);

        electronEnergyHist.addBin(e.getEnergy());
		protonEnergyHist.addBin(p.getEnergy());
		nuEnergyHist.addBin(nu.getEnergy());

		if((i%100)==0) std::cout << "Event: " << i << ", " << time(NULL)-now << "s\r" << std::flush;
    }

	 // open file for write
     std::ofstream electronHistFile(electronFileName, std::ios::out);
                
     // output electron histogram
     electronHistFile << electronEnergyHist;

	 	 // open file for write
     std::ofstream protonHistFile(protonFileName, std::ios::out);
                
     // output histogram
     protonHistFile << protonEnergyHist;

	 	 // open file for write
     std::ofstream nuHistFile(nuFileName, std::ios::out);
                
     // output histogram
     nuHistFile << nuEnergyHist;
       

    return EXIT_SUCCESS;
}
