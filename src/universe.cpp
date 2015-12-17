// Author:  John Cairns <john@2ad.com>
// Date:    Aug 3, 2005


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


#include "physicalconstants.h"
#include "universe.h"

using namespace GZKFast;

// a universe centered at p, with radius r, and B field field.
Universe::Universe(const BField &field, 
                   const Position &p, 
                   const GFloat timeStep, 
                   const GFloat targetRad) :
    bfield(field),
    sphere(p,0.0),
    target(p, targetRad),
    dt(timeStep)
{
}

// return the hubble constant of this system
GFloat Universe::getHubble(void) {
    return H0;
}

// return the z associated with a particular distance from
// a coordinate
GFloat Universe::getRedshift(GFloat r) {
    return H0/SpeedOfLight * r;
}

// return the z associated with a particular vector relative
// to the center of the universe
GFloat Universe::getRedshift(const Position &p) const {
    Position offset(p);
    offset -= sphere.getCenter();
    return getRedshift(offset.mag());
}

// return the distance associated with a particular value of z.
GFloat Universe::getDistance(GFloat z) {
    return SpeedOfLight*(z)/H0;
}

// return the intergalactic magnetic field
const BField &Universe::getBField(void) const {
    return bfield;
}

// return the position of the center of this universe
const Position &Universe::getPosition(void) const {
    return sphere.getCenter();
}

// return the radius of this universe
GFloat Universe::getRadius(void) const { 
    return sphere.getRadius();
}

// return true if p is inside this universe, and false otherwise
bool Universe::contains(const Position &p) const {
	return sphere.contains(p);
}

GFloat Universe::getVolume(void) const {
	GFloat radius=sphere.getRadius();
    return 4.0F/3.0F * PI * radius*radius*radius;
}

void Universe::add(const Source *s) {
    Position offset(s->getPosition());

    offset -= sphere.getCenter();

    if(offset.mag() > sphere.getRadius()) {
        sphere.setRadius(offset.mag());
    }
}


GFloat Universe::getTimeStep(void) const {
    return dt;
}

const Sphere &Universe::getTarget(void) const {
	return target;
}
