// Author:  John Cairns <john@2ad.com>
// Date:    Feb 22, 2007


//     GZKFast - An environment for astrophysical simulation.
//     Copyright (C) 2007 John A Cairns <john@2ad.com>

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


#include "electron.h"
#include "photon.h"

#include "gmath.h"
#include "physicalconstants.h"
#include "threebodydecay.h"


using namespace GZKFast;


bool Electron::interact(const Particle *particle,
                        GFloat density,
                        GFloat dx,
                        ParticleList &particleList) {


    // this is e\gamma -> e\gamma
    // inverse compton scattering

    // we can interact if particle is a photon
    
    int initSz = particleList.size();
    
    const Photon *photon;
    
    if((photon=dynamic_cast<const Photon*>(particle))!=NULL) {

        GFloat s = ElectronMass*ElectronMass + 2.0*getEnergy()*photon->getEnergy()*2.0;
        
        GFloat eGammaSigma=getSigma(s);

        if(eGammaSigma > 0.0) { // always true

            GFloat trialProb = GMath::getRand();
            if(trialProb < (1.0 - getProbZero(dx, density, eGammaSigma))) {

                // approximate the inverse compton losses
                GFloat x = s/(ElectronMass*ElectronMass);
                GFloat f = 0.0;

                if(x > 4.0) { // high energy limit
                    f = 1/log(x);
                } else if(x < 1.0/1.33) { // low energy limit
                    f = 1.33*x;
                } else {
                    f = 0.5;  // middle energy
                }

                // Egamma
                GFloat eGamma = f*getEnergy();
                // E electron, is remaining energy
                GFloat eElectron = getEnergy()*(1.0 - f);

                // create a new photon

                Momentum p(getMomentum());
                
                p /= p.mag();

                p *= eGamma;

                G4Vector p4(eGamma, p);

                Photon *photon1 = 
                    new Photon(p4, getPosition(), getSourcePos());

                particleList.push_back(photon1);

                // ignore very low energy electrons,
                // but track higher ones
                if(eElectron > 10.0*ElectronMass) {

                    Momentum pe(getMomentum());

                    pe *= (1.0 - f);

                    G4Vector pe4(eElectron, pe);
                    Electron *electron = new Electron(pe4, 
                                                      getPosition(),
                                                      getSourcePos());

                    particleList.push_back(electron);
                    
                }
                
            }
        }
        
    }
    return (particleList.size()>initSz);
}


GFloat Electron::getSigma(GFloat s) {

    // FIXME FIXME FIXME
    // approx cross section is thomson
    // should be Klien-Nishina eqn....fill in later

    return SigThomson;
}
