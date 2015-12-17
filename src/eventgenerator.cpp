// Author:  John Cairns <john@2ad.com>
// Date:    July 27, 2005


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


#include "gmath.h"
#include "gloghistogram.h"
#include "eventgenerator.h"

using namespace GZKFast;

EventGenerator::EventGenerator(const char *eventFile, 
                               GFloat expectedE, 
                               Universe *u, 
                               std::list<EvolutionThread*> &threads, 
                               int nBins,
                               GFloat minEnergy,
                               GFloat maxEnergy) : 
    protonHistogram(ProtonNormalization, nBins, minEnergy, maxEnergy, 1.0),
    threadList(threads),
    energyFlux(expectedE),
    eventCount(0),
    time(0.0),
    totalEnergy(0.0),
    universe(u)
{
    std::string histFile(eventFile);
    histFile.append("_hist.dat");
    protonHistFile.open(histFile.c_str(), std::ios::out);

    std::string mapFile(eventFile);
    mapFile.append("_events.dat");
    protonMapFile.open(mapFile.c_str(), std::ios::out);

    protonMapFile.precision(OUTPUT_PRECISION);

#if 0

    // generate a random offset target
    dest.setX(GMath::getSignedRand()*SqrtOneThird);
    dest.setY(GMath::getSignedRand()*SqrtOneThird);
    dest.setZ(GMath::getSignedRand()*SqrtOneThird);

    // compute a random offset to destination
    // within "Earth volume"
            
    // the target can be less than 101% of the target radius
    dest *= universe->getTarget().getRadius()*1.01;
    // add destination
    dest += universe->getTarget().getCenter();
#else
    dest = universe->getTarget().getCenter();
#endif


}

EventGenerator::~EventGenerator() {
    protonHistFile << protonHistogram;
}

void    EventGenerator::add(Source *s) {
    sourceList.push_back(s);
}

int EventGenerator::run(void) {
    // run until asked to stop
    while(getStatus() == GRunThread::RUNNING) {

        std::list<EvolutionThread*>::iterator evolThread = threadList.begin();
        
        for(std::list<Source*>::iterator src=sourceList.begin();
            src != sourceList.end();
            ++src) {

            // create a new particle aimed at target
            Particle *p = (*src)->getEvent(dest);

            GFloat energy = p->getEnergy();

            totalEnergy += energy;


            Position r((*src)->getPosition());
            r -= universe->getPosition();

            GFloat z= Universe::getRedshift(r.mag());


            // log at earth

            energy /= (1.0F + z); 

            protonHistogram.addBin(energy);

            Momentum pvec(p->getMomentum());
            pvec /= pvec.mag();

            GFloat pmag = sqrt(energy*energy-
                               p->getMass()*p->getMass());
            pvec *= pmag;

            protonMapFile.setf(std::ios_base::fixed, 
                               std::ios_base::floatfield);
            protonMapFile 
                << r.getLat() << ", " <<
                r.getLng() << ", ";
            
            protonMapFile.setf(std::ios_base::scientific, 
                               std::ios_base::floatfield);

            protonMapFile << energy << ", " <<
                pvec.getX() << ", " <<
                pvec.getY() << ", " <<
                pvec.getZ() << 
                std::endl;

            if(totalEnergy > energyFlux) {
                time += totalEnergy/energyFlux;
                totalEnergy = 0.F;
            }

            // add event to the first thread that needs work to do
            // step through all sources
            bool added=false;
            while(!added) {
                if(evolThread != threadList.end()) {
                    if((*evolThread)->getEventQueueSize() < 
                       (*evolThread)->getMaxEvents()) {
                        (*evolThread)->addParticle(p);
                        added=true;
                        // update for free
                        if(++evolThread == threadList.end()) {
                            evolThread = threadList.begin();
                        }
                        break;
                    }
                    ++evolThread;
                } else {
                    evolThread = threadList.begin();
                    // give the threads some time to work on existing events
                    GThread::sleep(250);
                }
            }
            
        }

    }
    return 0;
};

GFloat EventGenerator::getTotalEnergy(void) const {
    return totalEnergy;
}
