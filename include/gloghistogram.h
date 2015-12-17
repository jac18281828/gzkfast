// Author: John Cairns <john@2ad.com>
// Date:   Jun 20, 2005

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


#ifndef GLOGHISTOGRAM_H
#define GLOGHISTOGRAM_H 1


#include "physicalconstants.h"
#include "ghistogram.h"

namespace GZKFast {

    class DLLEXPORT GLogHistogram : public GHistogram {
      protected:
        // normalization
        GFloat norm;
        // power of output spectrum
        GFloat alpha;
        // test if value is in our distribution
        bool inDist(GFloat val) const;

      public:

        // ctor
        GLogHistogram(GFloat normalization, int numBins, GFloat distMin, GFloat distMax, GFloat a);

        // get the count corresponding to a particular energy value
        // return the bin for this value
        GFloat getBin(int n) const;

        // add a count to a particular bin by value
        GFloat addBin(GFloat val);

		// Return the sum of the bins
		GFloat integrate(void) const;

		// Return the normalization
		GFloat getNormalization(void) const;

		GFloat getBinValue(int n) const {
			return pow(10.0, GHistogram::getBinValue(n));
		}

		GFloat getWeightedAvgBinValue(void) const;

    };

    // printing
    static inline std::ostream &operator<<(std::ostream &os, const GZKFast::GLogHistogram &g) {

        // skip initial zero bins
        int start=0;
        //while(g.getBin(start) == 0.0) {start++;}
        std::streamsize prec = os.precision();
        os.precision(OUTPUT_PRECISION);
        os.setf(std::ios_base::scientific, std::ios_base::floatfield);
        
        for(int i=start; i<g.getNBins(); i++) {
            os << g.getBinValue(i) << ", " << g.getNormalization()*g.getBin(i) << std::endl;
            };
        
        os << std::flush;
        os.precision(prec);

        return os;
    };


};

#endif
