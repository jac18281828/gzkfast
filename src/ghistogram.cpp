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


#include <math.h>

#include <iostream>
#include <algorithm>

#include "gmatrix.h"

#include "ghistogram.h"

using namespace GZKFast;

// ctor
GHistogram::GHistogram(int numBins, GFloat distMin, GFloat distMax) {
    nBins=numBins;
    nTot=0;
    minVal=distMin;
    maxVal=distMax;

    binWidth=(maxVal-minVal)/(GFloat)nBins;

    histVals=new int[nBins];
    for(int i=0; i<nBins; i++) {
        histVals[i]=0;
    }
}

// dtor
GHistogram::~GHistogram() {
    if(histVals) delete[] histVals;
}

// return the bin appropriate to value
int GHistogram::getBinNumber(GFloat val) const {
    return (int)floor((val-minVal)/binWidth);
}



// get the number of bins
int GHistogram::getNBins(void) const {
    return nBins;
}

// get the minimum of the distribution
GFloat GHistogram::getDistMin(void) const {
    return minVal;
}

// get the maximum of the distribution
GFloat GHistogram::getDistMax(void) const {
    return maxVal;
}

// get the count on a particular bin
GFloat		GHistogram::getBin(int n) const {
    return (GFloat)histVals[n];
}

// get the count corresponding to a particular energy value
// return the bin for this value
GFloat GHistogram::getBin(GFloat val) const {
    if(inDist(val)) {
        return getBin(getBinNumber(val));
    }
    return 0.0;
}

bool GHistogram::inDist(GFloat val) const {
    if((val > minVal) && (val < maxVal)) {
        return true;
    }
    return false;
}

// add a count to a particular bin by bin index
// return current bin value
GFloat GHistogram::addBin(int n) {
    nTot++;
    return ++histVals[n];
}

// add a count to a particular bin by value
GFloat GHistogram::addBin(GFloat val) {
    if(inDist(val)) {
        return addBin(getBinNumber(val));
    }
    return getBin(val);
}

// get the value represented by the bin
GFloat GHistogram::getBinValue(int n) const {
    return minVal + binWidth*n;
}

int GHistogram::getCount(void) const {
    return nTot;
}


// jac Oct 14 2005
GFloat GHistogram::deltaN(GFloat val, GFloat &err) {

    const int NTAB=10;
    const GFloat CON=1.4, CON2=(CON*CON);    // step size decrease
    const GFloat BIG=std::numeric_limits<GFloat>::max();
    const GFloat SAFE=2.0;   // return when error increases by a factor of SAFE

    int i,j;
        
    GFloat errt, fac, hh, ans;
    GMatrix a(NTAB);

    hh=binWidth*(nBins>>2);   // start with the size of about 1/4 the bins
    a.set(0,0, (getBin(val+hh)-getBin(val-hh))/(2.0*hh));
    err=BIG;
    for(i=1;i<NTAB;i++) {
        // higher order extrapolation and smaller stepsize
        hh /= CON;
        a.set(0,i,(getBin(val+hh)-getBin(val-hh))/(2.0*hh));
        fac=CON2;
        for(j=1;j<=i;j++) { // extrapolate to higher orders
            a.set(j,i, a.get(j-1, i)*fac-a.get(j-1,i-1)/(fac-1.0));
            fac=CON2*fac;
            errt=std::max<GFloat>(fabs(a.get(j,i)-a.get(j-1,i)),fabs(a.get(j,i)-a.get(j-1,i-1)));

            // if the error is reduced accept it
            if (errt <= err) {
                err=errt;
                ans=a.get(j,i);
            }
        }
        if(fabs(a.get(i,i)-a.get(i-1,i-1)) >= SAFE*err) break;
        // when higher order gets work just quit
    }
    return ans;
};

GFloat GHistogram::getBinWidth(void) const {
    return binWidth;
}
