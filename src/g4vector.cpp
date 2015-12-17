// Author: John Cairns <john@2ad.com>
// Date:   Jun 22, 2005


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


#include "g4vector.h"

using namespace GZKFast;

G4Vector::G4Vector() : x(0.0) {
    x0=0.0;
}

// construct a 4 vector from 4 values
G4Vector::G4Vector(GFloat X0, GFloat X1, GFloat X2, GFloat X3) {
    setX0(X0);
    setX1(X1);
    setX2(X2);
    setX3(X3);
}

// construct a 4 vector from a scaler and a vector
G4Vector::G4Vector(GFloat X0, const GVector<GFloat> &X) {
    setX0(X0);
    setX(X);
}

// copy constructor
G4Vector::G4Vector(const G4Vector &g4) {
    x0=g4.x0;
    x=g4.x;
}

G4Vector::G4Vector(const GFloat xarry[4]) : x(xarry+1, 3) {
	x0=xarry[0];
}

// set vector values
void G4Vector::setX0(GFloat x0val) { x0=x0val; }
void G4Vector::setX1(GFloat x1) { x.set(0, x1); }
void G4Vector::setX2(GFloat x2) { x.set(1, x2); }
void G4Vector::setX3(GFloat x3) { x.set(2, x3); }

// query vector values
GFloat G4Vector::getX0(void) const { return x0; }
GFloat G4Vector::getX1(void) const { return x.get(0); }
GFloat G4Vector::getX2(void) const { return x.get(1); }
GFloat G4Vector::getX3(void) const { return x.get(2); }


GFloat G4Vector::mag(void) const {
    return x0*x0 - x*x;
}

// assignment
G4Vector &G4Vector::operator=(const G4Vector &g4) {
    x0=g4.x0;
    x=g4.x;
    
    return *this;
}


