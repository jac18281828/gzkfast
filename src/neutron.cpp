// Author:  John Cairns <john@2ad.com>
// Date:    May 24, 2005


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


#include "delta.h"
#include "delta1600.h"
#include "neutron.h"
#include "pion.h"
#include "pizero.h"
#include "proton.h"
#include "photon.h"
#include "neutron.h"

#include "cmb.h"
#include "gmath.h"
#include "physicalconstants.h"
#include "threebodydecay.h"

#include "proton.h"
#include "electron.h"
#include "nu.h"

using namespace GZKFast;

bool Neutron::decay(const Universe &universe, 
                    Particle::ParticleList &particleList) {
    
    GFloat dn = exp(-age/(getGamma()*lifeTime));
    if(GMath::getRand()>dn) {
        // Neutron beta decay
        Proton *proton = new Proton(get4Momentum(), r, getSourcePos());
        Electron *electron = new Electron(get4Momentum(), r, getSourcePos());
        Nu *nu = new Nu(Nu::ELECTRON, get4Momentum(), r, getSourcePos());
        
        ThreeBodyDecay tbd(this, proton, nu, electron);
        
        particleList.push_back(proton);
        particleList.push_back(electron);
        particleList.push_back(nu);
        
        return true;
        
    }
    return false;

}

bool Neutron::interact(const Particle *particle,
                       GFloat density,
                       GFloat dx,
                       ParticleList &particleList) {

    //
    // NOTE:  
    //  See Stecker, Phys Rev Lett, V21, #14, p 1016 (1968)
    //
    //

    // now try to scatter nucleons off the cmb
    
    // we can interact if particle is a photon
    
    int initSz = particleList.size();
    
    const Photon *photon;
    
    if((photon=dynamic_cast<const Photon*>(particle))!=NULL) {
        
        // the photon can be heading in any direction
        GFloat photonTheta = 2.0*PI*GMath::getRand();
        GFloat gamma = particle->getGamma();
        Velocity betaVec;
        getBeta(betaVec);
        GFloat beta = betaVec.mag();
        GFloat cmbBoostEnergy = gamma*photon->getEnergy()*(1.0 -
                                                         beta*cos(photonTheta));
        
        // in cm frame
        // s = (Ephoton + Eproton)^2
        
        // best case collision
        GFloat s = getMass()*getMass() + 
            2.0*getMass()*cmbBoostEnergy;
        
        // too slow
        //s = get4Momentum()*photon->get4Momentum();

        GFloat protonPhotonSigma=Proton::getSigma(s);

        // GZK cutoff for a delta resonance
        // do not compute any energy loss lower than the measured
        // cross section for p gamma
        if(protonPhotonSigma > 0.0) {

            // produce a delta with some energy loss.
            Delta *d1232 = new Delta(get4Momentum(), 
                                     s,
                                     getPosition(), 
                                     getCharge(),
                                     getSourcePos());

            if(d1232) {
                
                // colinear production assumed
                GFloat pmag = 
                    sqrt(d1232->getEnergy()*d1232->getEnergy() -
                         d1232->getMass()*d1232->getMass());
                
                Momentum ptot(getMomentum());
                ptot /= ptot.mag(); // unit vector
                ptot *= pmag;
                d1232->setMomentum(ptot);
                
                GFloat sa = 0.5; // spin of proton
                GFloat sb = 1.0; // spin of photon
                GFloat delta1232=d1232->getSigma(s, sa, sb);

                // delta 1232
                GFloat trialProb = GMath::getRand();
                
                if(trialProb < (1.0 - getProbZero(dx, density, delta1232))) {
                    
                    particleList.push_back(d1232);
                } else {
                    delete(d1232);
                

                    // produce a delta with some energy loss.
                    Delta1600 *d1600 = new Delta1600(get4Momentum(), 
                                                     s,
                                                     getPosition(), 
                                                     getCharge(),
                                                     getSourcePos());

                    if(d1600) {
                
                        pmag = sqrt(d1600->getEnergy()*d1600->getEnergy()-
                                    d1600->getMass()*d1600->getMass());
                        
                        ptot = getMomentum();
                        ptot /= ptot.mag(); // unit vector
                        ptot *= pmag;
                        d1600->setMomentum(ptot);
                        
                        
                        GFloat delta1600=d1600->getSigma(s, sa, sb);
                        
                
                        if(trialProb < (1.0 - 
                                        getProbZero(dx,
                                                    density,
                                                    delta1600))) {
                            
                            particleList.push_back(d1600);
                        } else {
                            delete(d1600);
    
                            if (trialProb < 
                                (1.0-getProbZero(dx, density, protonPhotonSigma))) {  
                                
                                // photo pion production directly off the cmb
                                
                                // generate a gaussian-like random 
                                // deviate around .5 with deviation (sqrt 6)/6
                                GFloat eFrac = .5 + GMath::getSignedRand()*.1;
                                
                                // eFrac = .5  (i.e. as in stecker)
                                
                                // important channels
                                
                                // not Proton
                                // gam n -> p pi-
                                
                                
                                // conserve charge!
                                Proton *p = new Proton(get4Momentum(),
                                                       getPosition(),
                                                       getSourcePos());
                                Pion    *pi = new Pion(get4Momentum(), 
                                                       getPosition(), 
                                                       -1.0,
                                                       getSourcePos());
                                
                                // average energy of each particle (see Stecker)
                                GFloat pE = eFrac*getEnergy()*
                                    (1.0 + (p->getMass()*p->getMass()
                                            - pi->getMass()*pi->getMass())/s);
                                
                                p->setEnergy(pE);
                                
                                // at this energy scale we assume the
                                // daugters are formed co-linearly
                                GFloat pPmag = sqrt(pE*pE-p->getMass()*p->getMass());
                                Momentum pP(getMomentum());
                                
                                pP /= pP.mag();
                                
                                pP *= pPmag;
                                
                                p->setMomentum(pP);
                                
                                particleList.push_back(p);
                                
                                GFloat piE = (1.0-eFrac)*getEnergy()*
                                    (1.0 + (pi->getMass()*pi->getMass() -
                                            p->getMass()*p->getMass())/s);
                                
                                pi->setEnergy(piE);
                                GFloat piPmag = sqrt(piE*piE-pi->getMass()*pi->getMass());
                                Momentum piP(getMomentum());
                                
                                piP /= piP.mag();
                                
                                piP *= piPmag;
                                
                                pi->setMomentum(piP);
                                
                                particleList.push_back(pi);
                            }
                        }
                    }
                }
            }
        }
    }
    return (particleList.size()>initSz);
}
