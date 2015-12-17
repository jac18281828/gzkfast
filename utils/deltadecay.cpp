// Author: John Cairns <john@2ad.com>
// Date:   Sep 6, 2005

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
#include "delta.h"
#include "pion.h"
#include "proton.h"
#include "neutron.h"
#include "twobodydecay.h"
#include "ghistogram.h"

using namespace GZKFast;

//
// This program generates a histogram of electron energies
// resulting from 2 GeV delta decay
int main(int argc, char *argv[]) {

    const int nEvents = 100000;
    
    const int nBins   = 50;
    const GFloat maxEnergy=2.0*GeV;

    const char*  piLabFileName="../data/piElab.dat";
    const char*  piCMFileName="../data/piEcm.dat";

    GFloat sigMax = 1.0e-2 * mbarn;
    
    // initalize the random number generator
    GMath::setSeed(); 

    GHistogram piELabHist(nBins, 0.0, 1.0);
    GHistogram piECMHist(nBins, 0.0, 1.0);
    
    // a delta goes to pion and proton
    time_t now = time(NULL);
    Position r;
    int i=0;
    while(i<nEvents) {
        GFloat pE = GMath::getRand()*maxEnergy;

        Proton p(pE, r);

        // a 2GeV proton strikes a proton essentially at rest
        GFloat s = p.getMass()*p.getMass() + 2.0*p.getMass()*p.getEnergy();
        
        Delta d(pE, s, r, +1.0);


		// energy is conserved
		if(pE > d.getMass()) {

			Momentum dP; // momentum randomly oriented

			dP.setX(GMath::getSignedRand());
			dP.setY(GMath::getSignedRand());
			dP.setZ(GMath::getSignedRand());

			dP /= dP.mag();


			dP *= sqrt(pE*pE - d.getMass()*d.getMass());

			d.setMomentum(dP);



			GFloat M0 = d.getMass();
			GFloat lam = 2.0*hbar*SpeedOfLight/M0;
			GFloat J = d.getSpin();
			GFloat Gamma = d.getWidth();

			GFloat sa = .5; // spin of proton
			GFloat sb = .5; // spin of proton
			GFloat sig1232 = Particle::resonance(s, lam, J, Gamma, M0, sa, sb);
			if(sig1232 > sigMax) sigMax = sig1232; // find the peak

			if(sig1232 > GMath::getRand()*sigMax) {
				i++; // this is an event

				Neutron n(0.0, r);
				Pion    pi(0.0, r, +1); // +

            
				// two body decay of delta to n pi+
				TwoBodyDecay tbd(&d, &n, &pi);

				piELabHist.addBin(pi.getEnergy()/GeV);
				piECMHist.addBin(pi.getEnergy()/d.getGamma()/GeV);
		
				if((i%100)==0) std::cout << "Event: " << i << ", " << time(NULL)-now << "s\r" << std::flush;
			}
		}
	}

    // open file for write
    std::ofstream piLabFile(piLabFileName, std::ios::out);
                
     // output histogram
    piLabFile << piELabHist;

    // open file for write
    std::ofstream piCMFile(piCMFileName, std::ios::out);
                
     // output histogram
    piCMFile << piECMHist;

    return EXIT_SUCCESS;
}
