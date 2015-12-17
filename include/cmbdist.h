//Author(s): John Cairns
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



#ifndef CMBDIST_H
#define CMBDIST_H 1

#include "physicalconstants.h"
#include "ghistogram.h"
#include "gmath.h"


namespace GZKFast {

    //  return a CMB photon energy given
    //  an input CMB temp using a monte carlo

    class DLLEXPORT CMBDist {
        GHistogram cmbHist;
      protected:
        //lowest frequency of dist
        GFloat        wLow;
        //max energy of dist
        GFloat        wMax;
		// temperature of CMB in Energy units.
        GFloat        cmbTemp;

        // normalization value (from stefan boltzman law)
        GFloat        normalizer;

        // differential number density (planck distribution)
        GFloat        numberDensity(GFloat w) const;
        
        // return probability of photon with 
        // frequency nu
        GFloat        getProb(GFloat freq) const;

      public:
        // construct a CMB distribution at temperature temp.
        CMBDist(GFloat minEnergy, GFloat cmbT, GFloat maxEnergy);

        // copy constructor
        CMBDist(const CMBDist &spec);

        // dtor
        ~CMBDist();

        // return Planck distribution
        GFloat getDistEnergy(void);

        // get the normalized number density by frequency
        GFloat getNumberDensity(GFloat w);


        // return CMB number density
        GFloat getCMBMeanNumberDensity(void) const {
            return 2.0*Zeta_3*pow(kBoltzmann*cmbTemp, 3.0)/
                (pow(hbar*SpeedOfLight, 3.00)*PI*PI);
        }

        // return the peak frequency of the black body radiation
        // this is Weins law
        GFloat getPeakFreq(void) const {
            return cmbTemp*Cairns_max/hbar;
        }

    };

};

#endif

