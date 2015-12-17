// Author: John Cairns <john@2ad.com>
// Date:   May 12, 2005

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


#ifndef CMB_H
#define CMB_H 1

#include <fstream>

#include "physicalconstants.h"
#include "bfieldspace.h"
#include "ghistogram.h"

#include "universe.h"

#include "cmbdist.h"
#include "gmutex.h"

namespace GZKFast {

    class DLLEXPORT CMB : public BFieldSpace {
      private:

        CMBDist		 cmbDist;

        CMBDist		 nuDist;

        // return the probability for zero interactions to take place
        GFloat getProbZero(Particle *particle, GFloat sigma);

        std::fstream         cmbHistFile;

        GHistogram           cmbHistogram;
      public:
    
        // ctor
        // argument is the average temp of CMB gammas.
        CMB(const char *logFile,
            GFloat temp, 
            Universe *u,
            int nBins);

        // dtor
        ~CMB();

        // propagate a particle through the cmb
        bool propagate(Particle *particle, Particle::ParticleList &particleList);
	
        // get the cross section for a proton on the cmb
        GFloat getSigma(GFloat s);

    };
};

#endif
