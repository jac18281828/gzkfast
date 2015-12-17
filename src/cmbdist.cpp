//Author(s): John Cairns <john@2ad.com>,
//           Matt Kistler <kistler@mps.ohio-state.edu>
//Date:   6-28-2005


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
#include <fstream>

#include "gmath.h"
#include "cmbdist.h"

#include "protonspectrum.h"

using namespace GZKFast;


CMBDist::CMBDist(GFloat minTemp, GFloat cmbT, GFloat maxTemp) :
	cmbHist(25, 0.*eV, 20*kBoltzmann)
 {
    wLow=minTemp*kBoltzmann/hbar;
    
    wMax=maxTemp*kBoltzmann/hbar;

    cmbTemp=cmbT;

    // compute the integral over this energy range to first order in energy
    normalizer = 1.0/getCMBMeanNumberDensity();

}

CMBDist::CMBDist(const CMBDist &spec) :
    cmbHist(25, 0.*eV, 20*kBoltzmann)
{
    wLow=spec.wLow;
    wMax=spec.wMax;
    cmbTemp=spec.cmbTemp;
    
    normalizer=spec.normalizer;
    
}


CMBDist::~CMBDist() {
    
    // This is Cheap!
    
    // open file for write
#ifdef GZKFAST_DEBUG
    std::ofstream cmbFile("cmbdist.dat", std::ios::out);
    
    // output proton histogram
    cmbFile << cmbHist;
#endif
    
    
}


GFloat  CMBDist::numberDensity(GFloat w) const {
    // from planck distribution
    return 	w*w/((exp(hbar*w/(kBoltzmann*cmbTemp)) - 1.0)*pow(SpeedOfLight, 3.0)*PI*PI);
}

GFloat  CMBDist::getProb(GFloat freq) const {
    if(freq<wMax) {
        
        return numberDensity(freq)*normalizer;
        
    }
    return 0.0;
    
}


GFloat CMBDist::getDistEnergy(void) {
    // Note: this can take an arbitrarily long time to finish

    // get the peak freq
    GFloat peakFreq =  getPeakFreq();
    GFloat peakProb = getProb(peakFreq);
    while(true) {
        // randomly choose a freq
        GFloat freq=wLow+
            GMath::getRand()*(wMax-wLow);
        // calculate the probability that energy
        // would be produced in this distribution
        // You gotta ask yourself one question...
        GFloat probF=getProb(freq);
        // Do I feel lucky?
        GFloat yRand=peakProb*GMath::getRand(); // renormalize to unitary scale
        if(yRand<probF) {
            // Well, do ya, punk?
            cmbHist.addBin((GFloat)hbar*freq);
            return hbar*freq;
        }
        // repeat this process until success
    }
    return 0.0; // unreachable
    
}

GFloat CMBDist::getNumberDensity(GFloat w) {
	return normalizer*numberDensity(w);
}
