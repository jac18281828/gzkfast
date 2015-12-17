// Author: John Cairns <john@2ad.com>
// Date:   May 24, 2005

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


#ifndef GMATRIX_H
#define GMATRIX_H 1

#include <math.h>
#include <ostream>
#include <iostream>
#include <valarray>

#include "physicalconstants.h"

#include "gmath.h"
#include "gvector.h"
#include "g4vector.h"

namespace GZKFast {
    
    // This is a matrix class.
    //
    // To prevent alot of arbitrary checking, matricies are
    // assumed to be nxn square matricies
    // 

    class DLLEXPORT GMatrix {
      protected:
        int                                     dim;
        std::valarray<GFloat>                   array;
      public:

        // construct an nxn Matrix
        GMatrix(int m) : dim(m), array(0.0, m*m) {
        };

        // construct an nxn Matrix with initial value
        GMatrix(GFloat f, int m) : dim(m), array(f, m*m) {
        };

        // construct from an array
        GMatrix(GFloat *matrix, int m) : dim(m), array(matrix, m*m) {
        }

        // return value in i,j
        inline const GFloat get(int i, int j) const { return array[i+dim*j]; }

        inline GFloat &get(int i, int j) { return array[i+dim*j]; }

        inline GFloat operator()(int i, int j) const { return get(i,j); }

        inline GFloat &operator()(int i, int j) { return get(i,j); }

        // set value in i,j
        inline void set(int i, int j, GFloat f) { array[i+dim*j] = f; }

        // return length of one side
        int dimension(void) const { return dim; }


        void setSize(int m) { 
            dim = m;
            array.resize(m*m);
        }

        // set this matrix to an identity matrix
        void identity(void) {
            for(int i=0; i<dim; i++) {
                for(int j=0; j<dim; j++) {
                    if(i!=j) {
                        set(i,j, 0.0);
                    } else {
                        set(i,j, 1.0);
                    }
                }
            }
        }

        // assignment
        GMatrix &operator=(const GMatrix &g) {
                        
            for(int i=0; i<dim; i++) {
                for(int j=0; j<dim; j++) {
                    set(i,j, g.get(i,j));
                }
            }
            
            return *this;
        }


        // LUP Solve an equation
        // Solve an equation of the form:
        // Ax = b
        //
        // where THIS matrix is taken to be the A matrix
        // 
        // NOTE: LUP decomposition only works for non-singular matricies
        GVector<GFloat> &LUPSolve(GVector<GFloat> &x, 
                                  const GVector<GFloat> &b) {
            int n = dim;
            GMatrix L(n);
            GMatrix U(n);
            std::valarray<GFloat> pi(n);
            std::valarray<GFloat> y(0.0, n);

            int i;

            if(LUPDecompose(L, U, pi)) {
            
                for(i=0;i<n;i++) {
                    GFloat sumLy=0.0;
                    for(int j=0;j<i;j++) {
                        sumLy+=L.get(i,j)*y[j];
                    }
                    y[i]=b.get((int)pi[i])-sumLy;
                }

                for(i=n-1; i>=0; i--) {
                    GFloat sumUx=0.0;
                    for(int j=i+1;j<n;j++){
                        sumUx+=U.get(i,j)*x.get(j);
                    }
                    x.set(i, (y[i]-sumUx)/U.get(i,i));
                }           
            }
            return x;

        }

        // 
        // LUP Decomposition - Solve this matrix as a system of 
        // linear equations.
        //
        // Decompose a singular matrix into 3 matricies of the form
        //   L - a lower triangular matrix
        //   U - an upper triangular matrix
        //   pi - a permutation vector
        //
        //   These matricies can then be easily employed for
        //   linear algebra.
        //
        // where THIS matrix is taken to be the A matrix
        // 
        // NOTE: LUP decomposition only works for non-singular matricies
        bool LUPDecompose(GMatrix &L, // lower triangular
                          GMatrix &U, // upper triangular
                          std::valarray<GFloat> &pi) { // permutation matrix

            // LUP Decomposition
            int n=dim;
            GMatrix A(*this);
            pi.resize(n);
            int i;
            for(i=0; i<n; i++) {
                pi[i]= i;
            }
            for(int k=0; k<n-1; k++) {
                GFloat p=0.0;
                int kp=0;
                for(i=k; i<n; i++) {
                    if(fabs(A.get(i,k)) > p) {
                        p=fabs(A.get(i,k));
                        kp=i;
                    }
                }
                if (p==0.0F) {
                    // this is a singular matrix, can not solve
                    return false;
                }

                GFloat pikp = pi[kp];
                pi[kp] = pi[k];
                pi[k] = pikp;

                for(i=0; i<n; i++) {
                    A.exchange(k,i, kp,i);
                }
                for(i=k+1;i<n;i++) {
                    A.set(i, k, A.get(i,k)/A.get(k,k));
                    for(int j=k+1;j<n;j++) {
                        A.set(i,j, A.get(i,j)-A.get(i,k)*A.get(k,j));
                    }
                }
            }

            L.setSize(A.dimension());

            
            // LUP Solve
            // A is now decomposed
            // L == lower triangular
            for(i=0; i<n; i++) {
                for(int j=i;j<n;j++){
                    if(i!=j){
                        L.set(i,j, 0.0F);
                    } else {
                        L.set(i,j, 1.0F);
                    }
                }
            }

            // U == upper triangular
            U.setSize(A.dimension());
            for(i=0; i<n; i++) {
                for(int j=0;j<i;j++){
                    U.set(i,j, 0.0F);
                }
            }
            return true;

        }

        // exchange the first and second cell
        void exchange(int i1, int j1, int i2, int j2) {
            GFloat save=get(i1, j1);
            set(i1,j1, get(i2,j2));
            set(i2,j2, save);
        }

    };

    // non member functions
    // addition
    inline GMatrix
    &operator+(GMatrix &g1, GMatrix &g2) {
		GMatrix rc(0.0, g1.dimension());
        for(int i=0; i<g1.dimension(); i++) {
            for(int j=0; j<g1.dimension(); j++) {
                rc(i,j) = g1(i, j) + g2(i,j);
            }
        }
        return g1;
    }

    // multiplication by scaler
    inline GMatrix
    operator*(GMatrix &a, GFloat f) {
        GMatrix b(0.0, a.dimension());
        for(int i=0; i<a.dimension(); i++) {
            for(int j=0; j<a.dimension(); j++) {
                b(i, j) = a(i,j)*f;
            }
        }
        return b;
    }

    // multiplication by matrix
    inline GMatrix
    operator*(const GMatrix &a, const GMatrix &b) {
        GMatrix c(0.0,a.dimension());
        for(int i=0; i<a.dimension(); i++) {
            for(int j=0; j<a.dimension(); j++) {
                for(int k=0; k<a.dimension(); k++)
                    c(i, k) += a(i,j)*b(j,k);
            }
        }
        return c;
    };


    // multiplication by a vector
    inline std::valarray<GFloat>
    operator*(const GMatrix &a, const std::valarray<GFloat> &b) {
        std::valarray<GFloat> c(0.0, a.dimension());
        for(int i=0; i<a.dimension(); i++) {
            for(int j=0; j<a.dimension(); j++) {
                c[i] += a(i,j)*b[j];
            }
                
        }
        return c;
    };



    // multiplication by a 3vector
    inline GVector<GFloat>
    operator*(const GMatrix &a, const GVector<GFloat> &b) {
        GVector<GFloat> c(0.0);
        for(int i=0; (i<a.dimension())&&(i<b.size()); i++) {
            for(int j=0; (j<a.dimension())&&(j<b.size()); j++) {
                c[i] += a(i,j)*b[j];
            }
                
        }
        return c;
    };


    // multiplication by a 4vector
    inline G4Vector
    operator*(const GMatrix &a, const G4Vector &b) {
        GFloat vec[4];
        for(int i=0; i<a.dimension(); i++) {
            GFloat sum=0.0;
            for(int j=0; j<a.dimension(); j++) {
                sum += a(i,j)*b[j];
            }
            vec[i]=sum;;
                
        }
        return G4Vector(vec);
    };



    // printing
    inline std::ostream
    &operator<<(std::ostream &os, GMatrix &g) {
        for(int i=0; i<g.dimension(); i++) {
            for(int j=0; j<g.dimension(); j++) {
                os << g.get(i,j) << " ";
            }
            os << std::endl;
        }

        return os;
    }

};




#endif
