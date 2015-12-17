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


#include <string>

#include "proton.h"
#include "neutron.h"
#include "nu.h"
#include "protondetector.h"
#include "gmath.h"
#include "gguard.h"

using namespace GZKFast;

GMutex   ProtonDetector::outputMutex;

ProtonDetector::ProtonDetector(const char *protonFile,
                               const Position &pos, 
                               GFloat rad,
                               int nBins,
                               GFloat minEnergy,
                               GFloat maxEnergy) :
    detector(pos, rad),
    protonHistogram(ProtonNormalization, nBins, minEnergy, maxEnergy, 1.),
    eventCount(0)
{
    
    std::string histFileName(protonFile);
    histFileName += "_hist.dat";
    std::string mapFileName(protonFile);
    mapFileName += "_events.dat";

    protonHistFile.open(histFileName.c_str(), std::ios::out);
    protonMapFile.open(mapFileName.c_str(), std::ios::out);
    protonMapFile.precision(OUTPUT_PRECISION);
}

ProtonDetector::~ProtonDetector() {
    protonHistFile << protonHistogram;
}


const Particle *ProtonDetector::hit(const Universe *universe, const Particle *particle) {
    
    if(dynamic_cast<const Proton*>(particle) || 
       dynamic_cast<const Neutron*>(particle)) { // only detect protons and neutrons
        Position hitPoint;

        // will the particle hit in the next step
        Position x2(particle->getPosition());
        Velocity vdt;
        particle->getVelocity(vdt);
        vdt *= universe->getTimeStep();
        x2 += vdt;

        GFloat t=detector.intersection(x2, 
                                       particle->getPosition(), 
                                       hitPoint);
        if((t>=0.0) && (t<=1.0)) {  // proton will intersect on next step
            GGuard output(outputMutex);

            eventCount++;
                
            Position dx(particle->getPosition());
            dx -= hitPoint;
            GFloat z = Universe::getRedshift(dx.mag());
            GFloat energy = particle->getEnergy()/(1.0F+z);

            protonHistogram.addBin(energy);

            Momentum pvec(particle->getMomentum());
            pvec /= pvec.mag();
            
            GFloat pmag = sqrt(energy*energy-
                               particle->getMass()*particle->getMass());
            pvec *= pmag;

            // smear hitpoint over a 1 degree field

            GFloat r = hitPoint.getR();
            GFloat theta = hitPoint.getTheta();
            GFloat phi   = hitPoint.getPhi();

            // actual smearing
            theta += GMath::getGaussianRand(1.0)*PI/180.0;
            phi += GMath::getGaussianRand(1.0)*PI/180.0;

            hitPoint.setSpherical(r, theta, phi);

            protonMapFile.setf(std::ios_base::fixed, 
                               std::ios_base::floatfield);

            // output the proton position on the sky and the energy.
            protonMapFile << hitPoint.getLat() << 
                ", " << hitPoint.getLng() << ", ";

            protonMapFile.setf(std::ios_base::scientific, 
                               std::ios_base::floatfield);

            protonMapFile << energy << ", " <<
                pvec.getX() << ", " <<
                pvec.getY() << ", " <<
                pvec.getZ() <<
                std::endl;

            
            return particle;
        }
    }
    return NULL; // not a detection
}


int	ProtonDetector::getEventsDetected(void) const {
	return eventCount;
}
