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

#include "nu.h"
#include "nudetector.h"
#include "gmath.h"
#include "gguard.h"

using namespace GZKFast;


GMutex	NuDetector::outputMutex;

NuDetector::NuDetector(const char *nuFile, 
                       const Position &pos, 
                       GFloat rad,
                       int nBins,
                       GFloat minEnergy,
                       GFloat maxEnergy) :
    detector(pos, rad),
    nu_eHistogram(ProtonNormalization, nBins, minEnergy, maxEnergy, 1.),
    nu_muHistogram(ProtonNormalization, nBins, minEnergy, maxEnergy, 1.),
    nuHistogram(ProtonNormalization, nBins, minEnergy, maxEnergy, 1.),
    nuThetaHist(nBins, 0.0, PI),
    eventCount(0)
{
    std::string histFileName(nuFile);
    histFileName += "_e_hist.dat";
    std::string mapFileName(nuFile);
    mapFileName += "_e_events.dat";
    
    nu_eHistFile.open(histFileName.c_str(), std::ios::out);
    nu_eMapFile.open(mapFileName.c_str(), std::ios::out);
    nu_eMapFile.precision(OUTPUT_PRECISION);
    
    histFileName = nuFile;
    histFileName += "_mu_hist.dat";
    mapFileName  = nuFile;
    mapFileName += "_mu_events.dat";
    
    nu_muHistFile.open(histFileName.c_str(), std::ios::out);
    nu_muMapFile.open(mapFileName.c_str(), std::ios::out);
    nu_muMapFile.precision(OUTPUT_PRECISION);
    

    histFileName = nuFile;
    histFileName += "_hist.dat";
    mapFileName  = nuFile;
    mapFileName += "_events.dat";
    nuHistFile.open(histFileName.c_str(), std::ios::out);
    nuMapFile.open(mapFileName.c_str(), std::ios::out);
    nuMapFile.precision(OUTPUT_PRECISION);
    
    std::string nuThetaFileName = nuFile;
    nuThetaFileName += "_dsdw.dat";
    
    nuThetaFile.open(nuThetaFileName.c_str(), std::ios::out);
    nuThetaFile.precision(OUTPUT_PRECISION);
}

NuDetector::~NuDetector() {
    nu_muHistFile << nu_muHistogram;
    nu_eHistFile << nu_eHistogram;
    nuHistFile << nuHistogram;
    
    GFloat theta = nuThetaHist.getDistMin();
    GFloat dTheta = nuThetaHist.getBinWidth();
    GFloat N = nuThetaHist.getCount();
    for(int i=0; i<nuThetaHist.getNBins(); i++) {
        GFloat err;
        nuThetaFile << theta+i*dTheta << ", " << 
            nuThetaHist.deltaN(theta + i*dTheta, err)/(N*sin(theta+i*dTheta)*dTheta) << std::endl;
	}
}


const Particle *NuDetector::hit(const Universe *universe, const Particle *particle) {

    // not as smart as it could be
    // just test inside our bbox
    
    const Nu *nu=dynamic_cast<const Nu*>(particle);
    // only detect neutrinos
    if(nu) {
	
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
        
        if(t >= 0.0) { // neutrino will eventually intersect detector
            // at hitpoint
            GGuard output(outputMutex);
            
            eventCount++;
            
            Position dx(particle->getPosition());
            dx -= hitPoint;
            GFloat z = Universe::getRedshift(dx.mag());
            GFloat energy = nu->getEnergy()/(1.0F+z);
            
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
            theta += GMath::getGaussianRand(10.0)*PI/180.0;
            phi += GMath::getGaussianRand(10.0)*PI/180.0;

            hitPoint.setSpherical(r, theta, phi);
            
            nuMapFile.setf(std::ios_base::fixed, 
                           std::ios_base::floatfield);
            
            // output the proton position on the sky and the energy.
            nuMapFile << hitPoint.getLat() << ", " <<
                hitPoint.getLng() << ", ";
            
            nuMapFile.setf(std::ios_base::scientific, 
                           std::ios_base::floatfield);
            
            nuMapFile << energy << ", " <<
                pvec.getX() << ", " <<
                pvec.getY() << ", " <<
                pvec.getZ() << 
                std::endl;
            
            nuHistogram.addBin(energy);
            
            //std::cout << nuHistogram;
            
            switch(nu->getType()) {
                case Nu::ELECTRON:
                    nu_eMapFile.setf(std::ios_base::fixed, 
                                     std::ios_base::floatfield);
                    
                    nu_eMapFile << hitPoint.getLat() << ", " <<
                        hitPoint.getLng() << ", ";
                    
                    nu_eMapFile.setf(std::ios_base::scientific, 
                                     std::ios_base::floatfield);
                    
                    nu_eMapFile << energy << ", " <<
                        pvec.getX() << ", " <<
                        pvec.getY() << ", " <<
                        pvec.getZ() << 
                        std::endl;
                    
                    nu_eHistogram.addBin(energy);
                    break;
                case Nu::MUON:
                    nu_muMapFile.setf(std::ios_base::fixed, 
                                      std::ios_base::floatfield);
                    
                    nu_muMapFile << hitPoint.getLat() << ", " <<
                        hitPoint.getLng() << ", ";

                    nu_muMapFile.setf(std::ios_base::scientific, 
                                      std::ios_base::floatfield);
                    
                    nu_muMapFile << energy << ", " <<
                        pvec.getX() << ", " <<
                        pvec.getY() << ", " <<
                        pvec.getZ() << 
                        std::endl;
                    
                    
                    nu_muHistogram.addBin(energy);
                    break;
            }


            return nu;
        }
    }
    return NULL;
}

int	NuDetector::getEventsDetected(void) const {
    return eventCount;
}


void NuDetector::onRemove(const Universe *universe, 
                          const Particle *particle, 
                          bool hitDetector) {
    // track angular distribution of scattered neutrinos
    const Nu *nu=dynamic_cast<const Nu*>(particle);
    // only detect neutrinos
    if(nu) {
        Position src = nu->getSourcePos();
		src -= nu->getPosition();
                
		nuThetaHist.addBin(src.getTheta());
	}
}
