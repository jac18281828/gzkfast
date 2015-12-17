// Author:  John Cairns <john@2ad.com>
// Date:    July 26, 2005


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


#include <stdlib.h>

#include <list>

#include "evolutionthread.h"
#include "photon.h"

#include "electron.h"
#include "gguard.h"
#include "sphere.h"

using namespace GZKFast;


GMutex          EvolutionThread::universeMutex;

EvolutionThread::EvolutionThread(int events, Universe *u) : maxEvents(events),
                                                            nParticles(0),
                                                            universe(u)
                                                            
{
}


void EvolutionThread::stop(void) {

    // wait for the queue to reach zero before stopping
    while(getEventQueueSize() > 0) {
        GThread::sleep(2000);
    }
    // okay we are all finished
    GRunThread::stop();
}

int EvolutionThread::run(void) {
    // continue while status is running
    while((status == GRunThread::RUNNING) ||
          (getEventQueueSize() > 0)) {
        if(getEventQueueSize() == 0) {
            GThread::sleep(100);  // wait for some events to be added
        } else {
       
        
            // for each particle
            // propagate it through all spaces
            // then test it against each detector

            for(std::list<Particle *>::iterator particle=particleList.begin();
                particle != particleList.end();
                ++particle) {

                Particle *p=*particle;
                
                bool production = false;
                // propagate the particle in all known spaces
                for(std::list<Space *>::iterator space=spaceList.begin();
                    space != spaceList.end();
                    ++space) {
                    if((*space)->propagate(p, particleList))
                        production=true;
                    (*space)->redshift(p);
                }

                // if the particle has propagated outside of the 
                // known universe remove it or produced other particles
                if(production || !universe->contains(p->getPosition())) {
                    remove(p,false);
                    break;
                } else {
                    // test to see if the particle will decay
                    if(p->decay(*universe, particleList)) {
                        // particle decayed
                        remove(p,false);
                        break;
                    }

                    // the particle has not decayed we may be able to detect it
                
                    // now that the particle has propagated try
                    // to detect it
                    bool wasHit=false;
                    for(std::list<Detector *>::iterator detector=detectorList.begin();
                        detector != detectorList.end();
                        ++detector) {
                        if((*detector)->hit(universe, p)) {
                            // so long and thanks for all the fish
                            wasHit=true;
                            remove(p,wasHit);
                            break;
                        }
                    }
                    if(wasHit) break;
                    else {
                        // particle was not oriented toward any detector
                        // the particle must be oriented toward the target
                        // volume to continue propagating
                        if(!universe->getTarget().contains(p->getPosition())) {
#if 0
                            Position hitPoint;

                            // will the particle hit in the next step
                            Position x2(p->getPosition());
                            Velocity v;
                            x2 += p->getVelocity(v)*universe->getTimeStep();
                            
                            GFloat t=universe->getTarget().intersection(x2, 
                                                                        p->getPosition(), 
                                                                        hitPoint);
                            if(t<-10.0) {  // particle will "never" intersect target volume
                                remove(p,false);
                                break;
                            }
#endif

                            // particle is arbitrarily far from its origin

                            
                            GFloat dist = p->getSourceDist();
                            if(dist > 10.0*universe->getRadius()) {
                                // stray particle
                                remove(p, false);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return EXIT_SUCCESS;
}

void EvolutionThread::addDetector(Detector *d) {
    GGuard guard(universeMutex);

    detectorList.push_back(d);
}

void EvolutionThread::addSpace(Space *s) {
    GGuard guard(universeMutex);

    spaceList.push_back(s);
}

void EvolutionThread::addParticle(Particle *p) {

    // wait for queue size to decrease
    while(getEventQueueSize() > getMaxEvents()) {
        GThread::sleep(25);
    }
	
    universeMutex.lock();   // must be able to access system

    particleList.push_back(p);
    nParticles++;

    universeMutex.unlock();
}


int EvolutionThread::getMaxEvents(void) const {
    return maxEvents;
}

int EvolutionThread::getEventQueueSize(void) const {
    return nParticles;
}

void EvolutionThread::remove(Particle *particle, bool wasHit) {

    for(std::list<Detector *>::iterator detector=detectorList.begin();
        detector != detectorList.end();
        ++detector) {
        // remove event for detector
        (*detector)->onRemove(universe, particle, wasHit);
    }
    
    GGuard uGuard(universeMutex);
    nParticles--;
    if(particle) delete particle;
    particleList.remove(particle);
}
