// Author: John Cairns <john@2ad.com>
// Date:   May 17, 2005

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


#ifndef G4VECTOR_H
#define G4VECTOR_H 1

#include <iostream>


#include "physicalconstants.h"
#include "gmath.h"
#include "gvector.h"

namespace GZKFast {

    // this is a 4 vector as commonly used in Special Relativity
    // and tensor calculus.
    class DLLEXPORT G4Vector {
      private:
        GFloat          x0;
        GVector<GFloat> x;
      public:
        // construct a 4 vector with 4 entries
        G4Vector();
        
        // construct a 4 vector from 4 values
        G4Vector(GFloat x0, GFloat x1, GFloat x2, GFloat x3);

        // construct a 4 vector from a scaler and a vector
        G4Vector(GFloat x0, const GVector<GFloat> &x);

		// construct from a 4 element array
		G4Vector(const GFloat xarry[4]);

        // copy constructor
        G4Vector(const G4Vector &g4);

        GFloat mag(void) const;

        // assignment
        G4Vector &operator=(const G4Vector &g4);

        // set scaler values
        void setX0(GFloat x0val);
        void setX1(GFloat x1);
        void setX2(GFloat x2);
        void setX3(GFloat x3);

        // set vector part
        void setX(const GVector<GFloat> &g3) { x = g3; }

        // query scaler values
        GFloat getX0(void) const;
        GFloat getX1(void) const;
        GFloat getX2(void) const;
        GFloat getX3(void) const;

        // query 3 vector part
        const GVector<GFloat> &getX(void) const { return x; }

        const GFloat operator[](int i) const {
            return get(i);
        };
        
        GFloat get(int i) const {
            if(i==0) return x0;
            return x.get(i-1);
        }
        
        void set(int i, GFloat f) {
            if(i==0) setX0(f);
            x[i-1] = f;
        }


        // 4 dot product
        inline GFloat operator*(const G4Vector &g4) {
            return x0*g4.x0 - x*g4.x;
        };
    
        // add
        G4Vector &operator+=(const G4Vector &r) {
            x0 += r.x0;
            x += r.x;
            
            return *this;
        };

        // subtract
        G4Vector &operator-=(const G4Vector &r) {
            x0 -= r.x0;
            x -= r.x;
            
            return *this;
        };

        // mult scaler
        G4Vector &operator*=(GFloat f) {
            x0 *= f;
            x *= f;
            
            return *this;
        };

        // test this vector approximately equal to another
        bool isApproxEqual(const G4Vector &r, GFloat delta) const {
            return (GMath::isApproxEqual(x0,r.x0,delta) && 
                GMath::isVecEqual(x, r.x,delta));
        }
    };


    // non member functions
    // addition
    inline G4Vector
    operator+(const G4Vector &g1, const G4Vector &g2) {
        G4Vector sum(g1);
        sum += g2;
        return sum;
    }

    //subtraction
    inline G4Vector
    operator-(const G4Vector &g1, const G4Vector &g2) {
        G4Vector diff(g1);
        diff -= g2;
        return diff;
    }

    // multiply by a scaler
    inline G4Vector
    operator*(G4Vector &g1, GFloat f) {
        G4Vector prod(g1);
        prod *= f;
        return prod;
    }

    // dot product
    inline GFloat
    operator*(const G4Vector &g1, const G4Vector &g2) {
		return g1.getX0()*g2.getX0()-g1.getX()*g2.getX();
    }

    // printing
    inline std::ostream
    &operator<<(std::ostream &os, const G4Vector &g4) {
        os << g4.getX0() << " " 
           << g4.getX();
        return os;
    }
};


#endif
