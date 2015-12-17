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



#include <stdlib.h>
#include <math.h>

#include <exception>
#include <iostream>

#include "g4vector.h"

#include "gmatrix.h"

#include "physicalconstants.h"
#include "cmb.h"
#include "gmath.h"
#include "proton.h"
#include "photon.h"
#include "neutron.h"
#include "delta.h"
#include "pion.h"
#include "twobodydecay.h"
#include "delta1600.h"
#include "gloghistogram.h"
#include "nu.h"


using namespace GZKFast;


CMB::CMB(const char *logFile, GFloat temp, Universe *u, int nBins) : 
    BFieldSpace(u),
    cmbDist(0.0*Kelvin, temp, 10.0*temp*Kelvin),       // average energy of
    // FIXME FIXME FIXME
    // nus are fermions not bosons
    nuDist(0.0*Kelvin, 
           1.79*Kelvin, 
           10.0*temp*Kelvin),       // average energy of
                                                       // photons
    cmbHistogram(nBins, 0.0*Kelvin*kBoltzmann, 10.0*temp*Kelvin*kBoltzmann)
{
    std::string cmbName(logFile);

    cmbName += "_photonhist.dat";

    cmbHistFile.open(cmbName.c_str(), std::ios::out);
}

CMB::~CMB() {
    cmbHistFile << cmbHistogram;
}

// 
// Does the proton interact with the CMB?
bool CMB::propagate(Particle *particle, Particle::ParticleList &particleList) {
    
    // are particles produced
    bool production=false;
    
    // first propagate through B Field
    if(BFieldSpace::propagate(particle, particleList)) {
        production=true;
    }

    // generate a random cmb photon
    GFloat cmbPhotonEnergy=cmbDist.getDistEnergy();

    GFloat density = cmbDist.getCMBMeanNumberDensity();


    GFloat z = universe->getRedshift(particle->getPosition());

    density *= pow(1.0+z, 3.0);
    
    cmbHistogram.addBin(cmbPhotonEnergy);

    // construct a new photon with this energy,
    // and random orientation
    Momentum photonp;

    GMath::getRandVec(photonp);

    photonp *= cmbPhotonEnergy;

    G4Vector photonp4(cmbPhotonEnergy, photonp);

    Photon photon(photonp4, particle->getPosition(), particle->getSourcePos());

    Velocity pV;
    
    if(particle->interact(&photon, 
                          density, 
                          particle->getVelocity(pV).mag()*universe->getTimeStep(),
                          particleList)) {
        production = true;
    } else {

        // generate a random relic neutrino
        GFloat nuEnergy=nuDist.getDistEnergy();

        density = nuDist.getCMBMeanNumberDensity();

        z = universe->getRedshift(particle->getPosition());

        density *= pow(1.0+z, 3.0);
    
        // construct a new neutrino with this energy,
        // and random orientation
        Momentum nup;
        GMath::getRandVec(nup);

        // assume massless neutrino
        nup *= nuEnergy;

        G4Vector nup4(nuEnergy, nup);

        Nu nu(Nu::ELECTRON,
              nup4, 
              particle->getPosition(), 
              particle->getSourcePos());

        if(particle->interact(&nu, 
                              density, 
                              particle->getVelocity(pV).mag()*universe->getTimeStep(),
                              particleList)) {
            production = true;
            
        }
    }
    
    return production;
};
