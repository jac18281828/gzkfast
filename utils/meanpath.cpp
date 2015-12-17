// Author: John Cairns <john@2ad.com>
// Date:   Jun 23, 2006

//     GZKFast - An environment for astrophysical simulation.
//     Copyright (C) 2006 John A Cairns <john@2ad.com>

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
#include <iostream>
#include <fstream>

#include <time.h>

#include "physicalconstants.h"
#include "gmath.h"
#include "gintegrate.h"

using namespace GZKFast;

// differential number density of the cmb
class MeanPath : public GFunction {

    GFloat cmbMin;
    GFloat cmbMax;
    
    GFloat Eproton;
            


    class NDist : public GFunction {
      private:
        GFloat Eproton;
        
        GFloat n_gamma(GFloat E) { 
            
            return pow(hbar*SpeedOfLight, -3.0) /
                (PI*PI)*E*E/(exp(E/(kBoltzmann*CMBTemp)) - 1.0);
        }
        
        GFloat sigmaBW(GFloat s, GFloat sigMax, GFloat M0, GFloat gamma) {
            GFloat m2g2 = M0*gamma;
            m2g2 *= m2g2;
            
            return sigMax*(m2g2)/(pow(s - M0*M0, 2.0) + m2g2);
        }
        
        
        GFloat sigmaPGamma(GFloat s) {
            if(s<1.0) return 1e-20; // zero
            GFloat bwval=sigmaBW(s, .6, 1.232, .120)+
                sigmaBW(s, .08, 1.600, .350)+
                sigmaBW(s, .1, 1.620, .150)+
                sigmaBW(s, .05, 1.700, .300)+
                sigmaBW(s, .08, 1.750, .300);
            if(s<1.8) {
                return bwval;
            } 
            return GMath::max(bwval, .12);
        }
      public:

        NDist(GFloat Ep) {
            Eproton=Ep;
        }

        GFloat operator()(GFloat cmbE) {
            GFloat Eg=2.0*cmbE*Eproton/NucleonMass;
            GFloat invarS = NucleonMass*NucleonMass + 2.0*NucleonMass*Eg;
            invarS/=(GeV*GeV);
            return n_gamma(cmbE)*sigmaPGamma(invarS)*mbarn;
        }
    };

  public:

    MeanPath() {
        cmbMin=1.0e-8;
        cmbMax=1.0e-2;
        Eproton=0.0;
    }
    
    GFloat operator()(GFloat Ep) {
        // simpsons rule
        Eproton=Ep;
        
        // romberbs
        NDist ndist(Ep);
        return 1.0/GIntegrate::integrate(ndist, 
                                         cmbMin,
                                         cmbMax);
    } 
} meanpath;

// this function integrates the cmb distribution
int main(int argc, char *argv[]) {

    
    try {
        std::cout.precision(25);

#if 0

        // plot proton photon cross section

        for(GFloat s=1.0; s<10.0; s+= 10.0/1000.0) {
            std::cout << s << "\t" << sigmaPGamma(s*s) << std::endl;
        }

#endif

#if 1
        // configure integrator for speed
        GFloat Epmin=1e18;
        GFloat Epmax=1e21;
        GFloat step=1000.0;
        for(GFloat Ep=Epmin; Ep<Epmax; Ep+= Epmax/step) {
            std::cout << Ep/GeV << "\t" << (meanpath(Ep)/Mpc) << std::endl;
        }

#endif

    } catch (GError g) {
        std::cerr << "GError caught while integrating: " << g.getMessage() 
                  << std::endl;
    }
    
    return EXIT_SUCCESS;
}
