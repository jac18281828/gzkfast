// Author: John Cairns <john@2ad.com>
// Date:   Aug 15, 2005


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

#include "physicalconstants.h"
#include "particle.h"
#include "gmath.h"

using namespace GZKFast;

GFloat  Particle::getGamma(void) const {
    return getEnergy()/getMass();
};

const Velocity &Particle::getBeta(Velocity &v) const {
    v = getMomentum();
    
    // p = gamma*m*v
    v /= getMass();
    v /= getGamma();

    return v;
};

const Velocity &Particle::getVelocity(Velocity &v) const {
	getBeta(v);

	v *= SpeedOfLight;

	return v;
};

GFloat Particle::resonance(GFloat s, 
                           GFloat lambda, 
                           GFloat J,
                           GFloat gamma,
                           GFloat M0,
                           GFloat sa,
                           GFloat sb) {
	// FIXME FIXME FIXME
	// for delta 1232 sigmax ~ 500 ubarn
	//  /20 is correction to typical sigMax for thi scase
    GFloat sigMax = 4.0*PI*lambda*lambda*(2.0*J + 1.0)/
        ((2*sa + 1.0)*(2.0*sb + 1.0));
	sigMax/=20.0;
    GFloat sig = sigMax*pow(M0*gamma, 2.0)/
        (pow(s-M0*M0, 2.0) + pow(gamma*M0, 2.0));

    return sig;
}

GFloat GZKFast::Particle::getSourceDist(void) const {
    Position p(getPosition());
    p -= getSourcePos();
    return p.mag();
}

GFloat Particle::getProbZero(GFloat dx, GFloat density, GFloat sigma) const {
 

    // mean free path of proton on gamma
    GFloat meanFreePath=1.0/(density*sigma);

    // probability for the proton to interact 0 
    // times in many trials is exp(-L/l)
    return 1.0-dx/meanFreePath;
}
