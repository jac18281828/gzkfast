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


#include "photon.h"
#include "electron.h"

#include "gmath.h"
#include "physicalconstants.h"
#include "threebodydecay.h"


using namespace GZKFast;


bool Photon::interact(const Particle *particle,
                      GFloat density,
                      GFloat dx,
                      ParticleList &particleList) {


    // this is \gamma\gamma -> e+ e-
    // pair production on cmb

    // we can interact if particle is a photon
    
    int initSz = particleList.size();
    
    const Photon *photon;
    
    if((photon=dynamic_cast<const Photon*>(particle))!=NULL) {

        GFloat s = 2.0*2.0*getEnergy()*photon->getEnergy();

        GFloat gGammaSigma=getSigma(s);

        if(gGammaSigma > 0.0) { // above eCrit

            GFloat trialProb = GMath::getRand();
            if(trialProb < (1.0 - getProbZero(dx, density, gGammaSigma))) {


                Momentum p(getMomentum());
                
                p /= p.mag();
                
                p *= getEnergy()/2.0;
                
                G4Vector p4(getEnergy()/2.0, p);
                
                Electron *electron1=new Electron(p4, getPosition(), getSourcePos());
                Electron *electron2=new Electron(p4, getPosition(), getSourcePos());
                
                particleList.push_back(electron1);
                
                particleList.push_back(electron2);
            }
        }
    }
    return (particleList.size()>initSz);
}


GFloat Photon::getSigma(GFloat s) {

    // FIXME FIXME FIXME
    // approx cross section is 3/16 thomson

    GFloat Ecrit = ElectronMass*ElectronMass/0.005*eV;

    if(sqrt(s) > Ecrit) {
        return 3.0/16.0*SigThomson;
    }
    return 0.0;
}

