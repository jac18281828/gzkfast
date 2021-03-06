// Author: John Cairns <john@2ad.com>
// Date:   July 26, 2005


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

#ifndef NUDETECTOR_H
#define NUDETECTOR_H 1

#include <fstream>

#include "physicalconstants.h"
#include "detector.h"
#include "particle.h"
#include "sphere.h"
#include "gvector.h"
#include "gloghistogram.h"
#include "gmutex.h"

namespace GZKFast {

// evaluate an interaction with propagating 
// particles
    

    class DLLEXPORT NuDetector : public Detector {
      private:
        std::fstream            nu_muHistFile;
        std::fstream         nu_muMapFile;
        GLogHistogram   nu_muHistogram;
        
        std::fstream            nu_eHistFile;
        std::fstream         nu_eMapFile;
        GLogHistogram   nu_eHistogram;

        std::fstream            nuHistFile;
        std::fstream         nuMapFile;
        GLogHistogram   nuHistogram;
        
        
        std::fstream			nuThetaFile;
        GHistogram		nuThetaHist;

        static GMutex   outputMutex;

        Sphere          detector;

        int             eventCount;


      public:

        //
        NuDetector(const char *nuFile, 
                   const Position &pos, 
                   GFloat rad,
                   int nBins,
                   GFloat minEnergy,
                   GFloat maxEnergy);

        ~NuDetector();

        // given a particle at a certain distance and a given momentum
        // evaluate if this particle would be detected by detector
        
        // return particle if hit
        // return NULL if no hit
        
        const Particle *hit(const Universe *universe, const Particle *particle);


        int     getEventsDetected(void) const;
        
        void onRemove(const Universe *universe, const Particle *particle, bool hit);
    };
};

#endif
