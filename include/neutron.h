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


#ifndef NEUTRON_H
#define NEUTRON_H 1

#include <math.h>

#include "physicalconstants.h"
#include "proton.h"

namespace GZKFast {
    
    class DLLEXPORT Neutron : public Particle {
      protected:
        const GFloat       lifeTime;
        const GFloat       m0;
        
        G4Vector        pu;
        Position        r,src;

	GFloat		age;

      public:
        Neutron(const G4Vector &p4, 
                const Position &rVec, 
                const Position &origin) : 
            lifeTime(885.7F*s), 
            m0(939.56533F*MeV),
	    pu(p4),
	    r(rVec),
            src(origin),
	    age(0.0)
        {

        }

        // handle neutron decay *more* properly
        bool decay(const Universe &universe, 
                   ParticleList &particleList);

        bool interact(const Particle *particle, 
                      GFloat density,
                      GFloat dx,
                      ParticleList &particleList);

        // set the energy [eV] of the particle
        void setEnergy(GFloat en) {
            pu.setX0(en);
        }
	
        // get the energy of the particle
        GFloat getEnergy(void) const {
            return pu.getX0();
        }

        // set the momentum [eV] of the particle
        void setMomentum(const Momentum &pvec) {
            pu.setX(pvec);
        }

        // get the momentum of the particle
        const Momentum &getMomentum(void) const {
            return pu.getX();
        }

        // set the 4momentum [eV] of the particle
        void set4Momentum(const G4Vector &pvec) {
            pu = pvec;
        }

        // get the momentum of the particle
        const G4Vector &get4Momentum(void) const {
            return pu;
        }

        // set the position vector [cm] of the particle.
        void setPosition(const Position &position) {
            r=position;
        }
        // get the position vector of the particle from Earth
        const Position &getPosition(void) const {
            return r;
        }

        GFloat getMass(void) const {
            return m0;
        }

	GFloat getCharge(void) const {
	    return 0.0;
	}

	// get life in seconds
	GFloat getAge(void) const {
	    return age;
	}
	
	// set life in seconds
	void   setAge(GFloat t) {
	    age=t;
	}
        // get the isospin
        GFloat getSpin(void) const {
            return .5;
        }

        // get the width
        GFloat getWidth(void) const {
            return 0.0;
        }
        
        const Position &getSourcePos(void) const {
            return src;
        }

    };

}

#endif
