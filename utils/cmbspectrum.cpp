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
#include "gmath.h"
#include "cmbdist.h"
#include "protonspectrum.h"
#include "ghistogram.h"

using namespace GZKFast;

//
// This program generates a histogram of electron energies
// resulting from beta decay
int main(int argc, char *argv[]) {

    GMath::setSeed(); 

    const char *cmbDistFile = "../data/cmbdist.dat";

	float **g = new float*[3];


	const int nSamples = 10000;

    const int nBins = 50;
    const GFloat minTemp = 0.0*Kelvin;
    const GFloat maxTemp = 10.0*2.725*Kelvin;

	// FIXME FIXME FIXME
	// Why doesn't this constructor get called?

#if 1
    GHistogram cmbEnergyHist(nBins, minTemp*kBoltzmann, maxTemp*kBoltzmann);

    CMBDist cmb(minTemp, 2.725*Kelvin, maxTemp);
#else
	ProtonSpectrum cmb(1e19, 1e22, -2.0);

	GLogHistogram cmbEnergyHist(1.0, nBins, 1e19, 1e22, 1.0);


#endif

    for(int i=0; i<nSamples; i++) {

	GFloat energy = cmb.getDistEnergy();

        cmbEnergyHist.addBin(energy);
    }

    // open file for write
    std::ofstream cmbHistFile(cmbDistFile, std::ios::out);
                
    // output cmb histogram
    cmbHistFile << cmbEnergyHist;

    return EXIT_SUCCESS;
}
