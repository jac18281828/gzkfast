// Author:  John Cairns <john@2ad.com>
// Date:    Aug 10, 2005


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

#include "physicalconstants.h"
#include "photon.h"
#include "photondetector.h"
#include "gmath.h"
#include "gguard.h"

using namespace GZKFast;

GMutex	PhotonDetector::outputMutex;

PhotonDetector::PhotonDetector(const char *photonFile, 
                               const Position &pos, 
                               GFloat rad,
                               int nBins,
                               GFloat minEnergy,
                               GFloat maxEnergy) :
    detector(pos, rad),
    photonHistogram(ProtonNormalization, nBins, minEnergy, maxEnergy, 1.),
    eventCount(0)
{
    std::string histFileName(photonFile);
    histFileName += "_hist.dat";
    std::string mapFileName(photonFile);
    mapFileName += "_events.dat";

    photonHistFile.open(histFileName.c_str(), std::ios::out);
    photonMapFile.open(mapFileName.c_str(), std::ios::out);

    photonMapFile.precision(OUTPUT_PRECISION);
}

PhotonDetector::~PhotonDetector() {
    photonHistFile << photonHistogram;
}


const Particle *PhotonDetector::hit(const Universe *universe, const Particle *particle) {

    // not as smart as it could be
    // just test inside our bbox

    const Photon *photon=dynamic_cast<const Photon*>(particle);
    // only detect photons
    if(photon) {
	
        Position hitPoint;
        Position x2(particle->getPosition());
        Velocity vdt;
        particle->getVelocity(vdt);

        vdt *= universe->getTimeStep();
        
        x2 += vdt;

        GFloat t=detector.intersection(x2, 
                                       particle->getPosition(), 
                                       hitPoint);
        if((t<=1.0)&&(t >= 0.0)) { // photon will eventually intersect detector at hitPoint
            GGuard output(outputMutex);
			
            eventCount++;

            Position dx(particle->getPosition());
            dx -= hitPoint;
            GFloat z = Universe::getRedshift(dx.mag());
            GFloat energy = photon->getEnergy()/(1.0F+z);

            photonHistogram.addBin(energy);

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

            photonMapFile.setf(std::ios_base::fixed, 
                               std::ios_base::floatfield);

            // output the proton position on the sky and the energy.
            photonMapFile << hitPoint.getLat() << ", " <<
                hitPoint.getLng() << ", ";

            photonMapFile.setf(std::ios_base::scientific, 
                               std::ios_base::floatfield);
            
            photonMapFile << energy << ", " <<
                pvec.getX() << ", " <<
                pvec.getY() << ", " <<
                pvec.getZ() << 
                std::endl;

            return photon;
        }
    }
    return NULL;
}

int	PhotonDetector::getEventsDetected(void) const {
    return eventCount;
}

