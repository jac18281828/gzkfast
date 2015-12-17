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

#ifndef GVECTOR_H
#define GVECTOR_H 1

#include <ostream>
#include <stdlib.h>
#include <math.h>

#include "physicalconstants.h"

namespace GZKFast {
    
    // this class template provides support for 3 (or any specified
    // dimension) vector manipulation and storage

    
    template <class FloatType>
    class DLLEXPORT GVector {
        FloatType       valArry[3];
      public:

        // construct a 3 vector
        GVector() {
            for(int i=0; i<size(); i++) {valArry[i] = 0.0;}
        }


        // construct a vector with initialization
        GVector(FloatType f) {
            for(int i=0; i<size(); i++) {valArry[i] = f;}
        };


        // construct this vector from an array
        GVector(const FloatType *arry, int arryLen) {
            int i=0;
            for(;(i<size())&&(i<arryLen); i++) {valArry[i] = arry[i];}
            for(;i<size(); i++) {valArry[i] = 0.0;}
        }

        const int size(void) const { return 3; }

        // return coordinates
        FloatType getX(void) const { return valArry[0]; }
        FloatType getY(void) const { return valArry[1]; }
        FloatType getZ(void) const { return valArry[2]; }
        FloatType get(int i) const { return (*this)[i]; }

        const FloatType &operator[](int i) const { return valArry[i]; }
        FloatType &operator[](int i) { return valArry[i]; }

        
        // set coordinates
        void setX(FloatType x) { valArry[0] = x; }
        void setY(FloatType y) { valArry[1] = y; }
        void setZ(FloatType z) { valArry[2] = z; }

        // set 
        void set(int i, FloatType f) {valArry[i] = f; }

        // set in sperical coordinates
        void setSpherical(FloatType r, FloatType theta, FloatType phi) {
            setX(r*cos(phi)*sin(theta));
            setY(r*sin(phi)*sin(theta));
            setZ(r*cos(theta));
        }

        // get spherical coordinates
        FloatType getR(void) const {
            return mag();
        }

        // get phi
        FloatType getPhi(void) const {
            if(getX() > 0.0) { // 0 to PI
                return atan(getY()/getX()) + PI/2.0;
            } else {   // PI to 2PI
                return atan(getY()/getX()) + 3.0*PI/2.0;
            }
        }
        
        // get theta (0-PI)
        FloatType getTheta(void) const {
            return acos(getZ()/getR());
        }
        
        // get a latitude (theta in degrees)
        FloatType getLat(void) const {
            return (getTheta()-PI/2.0)*180.0/PI;
        }

        // get a longitude (phi in degrees)
        FloatType getLng(void) const {
            return (getPhi()-PI)*180.0/PI;
        }


        // compute the cross product of this vector and
        // another vector
        const GVector &cross(const GVector &g1, const GVector &g2) {
            FloatType X=g1.getY()*g2.getZ() - g1.getZ()*g2.getY();
            FloatType Y=-g1.getX()*g2.getZ() - g1.getZ()*g2.getX();
            FloatType Z=g1.getX()*g2.getY() - g1.getY()*g2.getZ();
            setX(X); setY(Y); setZ(Z);

            return *this;
        }

        // exchange values in i1 and i2
        void exchange(int i1, int i2) {
            FloatType save=get(i1);
            set(i1, get(i2));
            set(i2, save);
        }
        
        // return the magnitude of this vector
        FloatType mag(void) const {
            FloatType dotProduct=0.0;
#ifdef WIN32
            for(int i=0; i<n(); i++) {
#else
                for(int i=0; i<size(); i++) {
#endif
                    dotProduct += get(i)*get(i);
                }
                return sqrt(dotProduct);
            }
        
            // return the distance from another vector
            FloatType getDistance(const GVector<FloatType> &r) const {
                GVector<FloatType> sep(*this - r);
                return sep.mag();
            }

            GVector<FloatType> &operator+=(const GVector &vec) {
                for(int i=0; i<size(); i++) {
                    valArry[i] += vec.valArry[i];
                }
                return *this;
            }

            GVector<FloatType> &operator-=(const GVector &vec) {
                for(int i=0; i<size(); i++) {
                    valArry[i] -= vec[i];
                }
                return *this;
            }


            GVector &operator*=(const FloatType &f) {
                for(int i=0; i<size(); i++) {
                    valArry[i] *= f;
                }
                return *this;
            }

            GVector &operator/=(const FloatType &f) {
                for(int i=0; i<size(); i++) {
                    valArry[i] /= f;
                }
                return *this;
            }
        };

        // multiply each term by a scaler
        template <class FloatType> inline GVector<FloatType>
        operator*(const GVector<FloatType> &g1, const FloatType &g2) {
            GVector<FloatType> fv(g1);
            for(int i=0; i<fv.size(); i++) {
                fv[i] *= g2;
            }
            return fv;
        };
        

        // dot product
        template <class FloatType> inline FloatType
        operator*(const GVector<FloatType> &g1, const GVector<FloatType> &g2) {
            FloatType product=0.0F;
            for(int i=0; i<g1.size(); i++) {
                product += g1[i]*g2[i];
            }
            return product;
        }
        
        // equality
        template <class FloatType> inline bool
        operator==(const GVector<FloatType> &g1, const GVector<FloatType> &g2) {
            for(int i=0; i<g1.size(); i++) {
                if(g1[i] != g2[i]) return false;
            }
            return true;
        }

    
// printing
        template <class FloatType> 
        inline std::ostream
        &operator<<(std::ostream &os, const GVector<FloatType> &g) {
            for(int i=0; i<g.size(); i++) {
                os << g.get(i) << " ";
            }
            os << std::endl;
            return os;
        }

    
// some commonly used vector types
        typedef GVector<GFloat> Vector3;
        typedef GVector<GFloat> Position;
        typedef GVector<GFloat> Momentum;
        typedef GVector<GFloat> Velocity;
        typedef GVector<GFloat> Acceleration;
        typedef GVector<GFloat> BField;
        typedef GVector<GFloat> EField;
    
    };

#endif
