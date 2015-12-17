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
#include "gintegrate.h"

using namespace GZKFast;

// differential number density of the cmb

class CMBDist : public GFunction {
    GFloat operator()(GFloat E) { 

        return pow(hbar*SpeedOfLight, -3.0) /
            (PI*PI)*E*E/(exp(E/(kBoltzmann*CMBTemp)) - 1.0);
    }
};

// this function integrates the cmb distribution
int main(int argc, char *argv[]) {
    
    try {
        const GFloat a=1.0e-8;
        const GFloat b=1.0e-2;
        std::cout.precision(25);

        CMBDist cmb;
        
        std::cout << "Using Exact Integration:" << std::endl;
        std::cout << "The density of the CMB is " << 
            Zeta_3*2.0*pow(kBoltzmann*CMBTemp/(hbar*SpeedOfLight), 3.0)/(PI*PI)
                  << " cm^-3" << std::endl << std::endl;
        
        std::cout << "Using GSL Integrate:" << std::endl;
        std::cout << "The density of the CMB is " << 
            GIntegrate::integrate(cmb, a, b) << " cm^-3" << 
            std::endl << std::endl;

        std::cout << "Using Vegas method:" << std::endl;
        std::cout << "The density of the CMB is " << 
            GIntegrate::vegasIntegrate(cmb, a, b, 1<<15) << " cm^-3" << 
            std::endl << std::endl;

    } catch (GError g) {
        std::cerr << "GError caught while integrating: " << g.getMessage() 
                  << std::endl;
    }
    
    return EXIT_SUCCESS;
}
