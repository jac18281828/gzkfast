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


#include "delta.h"
#include "delta1600.h"
#include "neutron.h"
#include "pion.h"
#include "pizero.h"
#include "proton.h"
#include "photon.h"

#include "cmb.h"
#include "gmath.h"
#include "physicalconstants.h"
#include "twobodydecay.h"

#include "proton.h"
#include "electron.h"
#include "nu.h"

using namespace GZKFast;

// cross section for protons for specified energy bins
//
// this table is extracted from RPP data:
// http://pdg.lbl.gov/2004/hadronic-xsections/hadron.html
// cross section vs CM energy in GeV

// PLAB (GeV/c), SIG(mbarn)
GFloat Proton::protonCrossSection[GZK_NCROSSROWS][2] = { 
    {(0.93827+0.185)*GeV, 0.0*mbarn},  // start delta at 0
//    {(0.93827+0.118)*GeV, 7.88e-2F*mbarn},
    {(0.93827+0.205)*GeV, 0.1189F*mbarn},
    {(0.93827+0.223)*GeV, 0.1682F*mbarn},
    {(0.93827+0.242)*GeV, 0.2024F*mbarn},
    {(0.93827+0.26)*GeV, 0.3234F*mbarn},
//    {(0.93827+0.265)*GeV, 0.4245F*mbarn},
    {(0.93827+0.279)*GeV, 0.3871F*mbarn},
    {(0.93827+0.29)*GeV, 0.487F*mbarn},
    {(0.93827+0.298)*GeV, 0.5042F*mbarn},
    {(0.93827+0.315)*GeV, 0.5269F*mbarn},
    {(0.93827+0.318)*GeV, 0.5326F*mbarn},
    {(0.93827+0.337)*GeV, 0.5423F*mbarn},
    {(0.93827+0.34)*GeV, 0.4778F*mbarn},
    {(0.93827+0.357)*GeV, 0.4808F*mbarn},
    {(0.93827+0.365)*GeV, 0.4066F*mbarn},
    {(0.93827+0.377)*GeV, 0.411F*mbarn},
    {(0.93827+0.39)*GeV, 0.3341F*mbarn},
    {(0.93827+0.397)*GeV, 0.3119F*mbarn},
    {(0.93827+0.415)*GeV, 0.2444F*mbarn},
    {(0.93827+0.418)*GeV, 0.2496F*mbarn},
    {(0.93827+0.438)*GeV, 0.2109F*mbarn},
    {(0.93827+0.44)*GeV, 0.2245F*mbarn},
    {(0.93827+0.459)*GeV, 0.1742F*mbarn},
    {(0.93827+0.465)*GeV, 0.2005F*mbarn},
    {(0.93827+0.481)*GeV, 0.1888F*mbarn},
    {(0.93827+0.49)*GeV, 0.1783F*mbarn},
    {(0.93827+0.502)*GeV, 0.1764F*mbarn},
    {(0.93827+0.515)*GeV, 0.1769F*mbarn},
    {(0.93827+0.524)*GeV, 0.1671F*mbarn},
};


bool Proton::interact(const Particle *particle,
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

        GFloat protonPhotonSigma=getSigma(s);

        // GZK cutoff for a delta resonance
        // do not compute any energy loss lower than the measured
        // cross section for p gamma
        if(protonPhotonSigma > 0.0) {

            Delta *d1232 = new Delta(get4Momentum(), 
                                     s, 
                                     getPosition(), 
                                     getCharge(),
                                     getSourcePos());

            if(d1232) {
            
                // colinear production assumed
                GFloat pmag = sqrt(d1232->getEnergy()*d1232->getEnergy() -
                                   d1232->getMass()*d1232->getMass());
             
                Momentum ptot(getMomentum());
                ptot /= ptot.mag(); // unit vector
                ptot *= pmag;
                d1232->setMomentum(ptot);


                
                GFloat sa = .5; // spin of proton
                GFloat sb = 1.0; // spin of photon
                GFloat delta1232=d1232->getSigma(s, sa, sb);
                
                // delta 1232
                GFloat trialProb = GMath::getRand();

                if(trialProb < (1.0 - getProbZero(dx, density, delta1232))) {
                
                    particleList.push_back(d1232);
                } else {
                    delete(d1232);

                    Delta1600 *d1600 = new Delta1600(get4Momentum(), 
                                                     s, 
                                                     getPosition(), 
                                                     getCharge(),
                                                     getSourcePos());
                    
                    GFloat delta1600=d1600->getSigma(s, sa, sb);
        
                    
                    pmag = sqrt(d1600->getEnergy()*d1600->getEnergy() -
                                d1600->getMass()*d1600->getMass());
             
                    ptot= getMomentum();
                    ptot /= ptot.mag(); // unit vector
                    ptot *= pmag;
                    d1600->setMomentum(ptot);
                    
                    if(trialProb < (1.0 - getProbZero(dx, 
                                                      density, 
                                                      delta1600))) {
     
                         
                        particleList.push_back(d1600);
                    } else {
                        delete(d1600);

                        // we take the difference of the measured value and
                        // delta to be direct production probabilityelse if (trialProb < 
                        if (trialProb < 
                            (1.0-getProbZero(dx, density, protonPhotonSigma))) {  
 
                            // photo pion production directly off the cmb
                
                            // generate a gaussian-like random deviate around .5 with deviation (sqrt 6)/6
                            GFloat eFrac = .5 + GMath::getSignedRand()*.1;
                            
                            // eFrac = .5  (i.e. as in stecker)
                            
                            // important channels
                            // for a proton
                            // gam p -> n pi+
                            
                            Neutron *n = new Neutron(get4Momentum(), getPosition(),getSourcePos());
                            Pion    *pi = new Pion(get4Momentum(), getPosition(), +1.0, getSourcePos());
                            
                            // average energy of each particle (see Stecker)
                            GFloat nE = eFrac*getEnergy()*
                                (1.0 + (n->getMass()*n->getMass()
                                        - pi->getMass()*pi->getMass())/s);
                            
                            n->setEnergy(nE);
                            
                            // at this energy scale we assume the
                            // daugters are formed co-linearly
                            GFloat nPmag = sqrt(nE*nE-n->getMass()*n->getMass());
                            Momentum nP(getMomentum());
                            
                            nP /= nP.mag();
                            
                            nP *= nPmag;
                            
                            n->setMomentum(nP);
                            
                            particleList.push_back(n);
                            
                            GFloat piE = (1.0-eFrac)*getEnergy()*
                                (1.0 + (pi->getMass()*pi->getMass() -
                                        n->getMass()*n->getMass())/s);
                            
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

        GFloat eeSigma = getSigmaBH(s);
        if(eeSigma > 0.0) {
            
            GFloat trialProb = GMath::getRand();

            if(trialProb < (1.0 - getProbZero(dx, density, eeSigma))) {

                // a proton at rest is formed
                // so subtract that rest mass from available
                // energy before using "decay"
                G4Vector p4(get4Momentum());

                p4.setX0(p4.getX0() - getMass());

                Momentum p(p4.getX());

                p /= p.mag();

                p *= sqrt(p4.getX0()*p4.getX0() - getMass()*getMass());

                p4.setX(p);

                Proton decayP(p4, getPosition(), getSourcePos());

                
                // in this case form a pair of electrons

                Electron *e1 = new Electron(get4Momentum(), 
                                            getPosition(), 
                                            getSourcePos());
                
                // FIXME FIXME FIXME 
                // positive electron 
                // charge does not matter?
                Electron *e2 = new Electron(get4Momentum(), 
                                            getPosition(), 
                                            getSourcePos());
                
                TwoBodyDecay tbd(&decayP, e1, e2);
                particleList.push_back(e1);
                particleList.push_back(e2);

                // ignore the resulting proton at rest!

            }

        }
    }
    return (particleList.size()>initSz);
}

// return the cross section for gamma + proton -> delta
GFloat Proton::getSigma(GFloat s) {
    // get the appropriate 
    // energy in center of mass
    // we take s ~ 2 Ep Egam in the relativeistic limit
    // does mp^2 make any difference?

    GFloat sqrtS = sqrt(s);
    
    if(sqrtS < protonCrossSection[0][0]) {
        return 0.0*mbarn;
    }
    
    for(int i=1; i<GZK_NCROSSROWS; i++) {
        if(sqrtS < protonCrossSection[i][0]) {
            // interpolate
            //
            //
            // a = (x - x0)/(x1 - x0)
            // y = y0 + a(y1-y0)
            GFloat alpha = (sqrtS - protonCrossSection[i-1][0])/
                (protonCrossSection[i][0]-protonCrossSection[i-1][0]);
                
            return protonCrossSection[i-1][1] +
                alpha*(protonCrossSection[i][1]-protonCrossSection[i-1][1]);
        }
    }
    // extend to infinity
    return protonCrossSection[GZK_NCROSSROWS-1][1];
}

GFloat Proton::getSigmaBH(GFloat s) {
    // cross section for pair production

    if(s > pow(getMass()+2.0*ElectronMass, 2.0)) {
        return 3.0/16.0*SigThomson;
    }
    return 0.0;
}
