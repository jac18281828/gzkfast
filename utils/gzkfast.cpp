// Author: John Cairns <john@2ad.com>
// Date:   May 12, 2005

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


// for random
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <time.h>

// c++
#include <iostream>
#include <list>

#include "cmb.h"


#include "universe.h"
#include "protonsource.h"
#include "protondetector.h"
#include "photondetector.h"
#include "nudetector.h"
#include "eventgenerator.h"
#include "evolutionthread.h"
#include "threebodydecay.h"

using namespace GZKFast;
using namespace std;

#if 1// ndef GZKFAST_DEBUG
static const int        MAX_PARTICLES=50;
#else
static const int        MAX_PARTICLES=10;
#endif

static const int        NBINS=50;
static const GFloat     NU_E_MAX=5e21*eV;
static const GFloat     NU_E_MIN=1.0*TeV;
static const GFloat     PH_E_MAX=5e21*eV;
static const GFloat     PH_E_MIN=100.0*GeV;
static const GFloat     PR_E_MIN=5e19*eV;
static const GFloat     PR_E_MAX=5e22*eV;


static void     usage(const char *name);
static void     license(const char *name);

//
// This program generates a proton of a given distance and
// energy, and injects it into a CMB background

int main(int argc, char *argv[]) {

    // initialize random number generator
    GMath::setSeed();

    // tell them how it is
    license(argv[0]);

    // parameters
    
    // number of sources
    int           nSources = 3;

    // number of events to simulate
    int           nStopEvents = 100;

    // number of processing threads
    int           nThreads = 2;

    
    // this is the proton spectrum
    // alpha=-2 corresponds to "E to the minus 2
    GFloat       alpha = -2.7; 

    // the starting energy
    GFloat    startEnergy = PR_E_MIN; // eV           

    // the ending energy
    GFloat    endEnergy   = PR_E_MAX; // eV
    
    // the shortest distance to generate protons
    GFloat    startDistance = 150.0*Mpc;
    
    // the longest distance to generate protons
    GFloat    endDistance   = 200.0*Mpc;

    GFloat    cmbTemp       = 2.725*Kelvin;

    GFloat    distStep      = 250.0*Kpc;

    GFloat    detRad        = 250.0*Kpc;
    
    std::list<GFloat>            deltaEn;
    GFloat                       deltaPct=0.0;

    Position    earthPosition;

    BField      bfield;
    
    GMath::getRandVec(bfield);

    bfield *= 1e-12*Gauss;
    
    const char *protonMapFile="proton";
    const char *secondaryProtonFile = "proton2";
    const char *nuFile="nu";
    const char *photonFile="photon";
    const char *cmbFile="cmb";


    // parse command line
    for(int arg=1; arg<argc; arg++) {
        int s=1;
        switch(argv[arg][s]) {
            case '-':
                s++;
                break;
            case 's':
                if(++arg < argc) {
                    nSources = atoi(argv[arg]);
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'e':
                if(++arg < argc) {
                    nStopEvents = atoi(argv[arg]);
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 't':
                if(++arg < argc) {
                    nThreads = atoi(argv[arg]);
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'a':
                if(++arg < argc) {
                    alpha = atof(argv[arg]);
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'l':
                if(++arg < argc) {
                    startEnergy = atof(argv[arg])*EeV;
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'h':
                if(++arg < argc) {
                    endEnergy = atof(argv[arg])*EeV;
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'n':
                if(++arg < argc) {
                    startDistance = atof(argv[arg])*Mpc;
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'f':
                if(++arg < argc) {
                    endDistance = atof(argv[arg])*Mpc;
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'd':
                if(strcmp(argv[arg++]+s, "delta")==0) {
                    deltaEn.push_back(atof(argv[arg])*EeV);
                } else if (strcmp(argv[arg++]+s, "deltapct")==0) {
                    deltaPct = atof(argv[arg])*EeV;
                } else {
                    // default to dx
                    if(++arg < argc) {
                        distStep = atof(argv[arg])*Kpc;
                    } else {
                        usage(argv[0]);
                        return EXIT_FAILURE;
                    }
                }
                break;
            case 'r':
                if(++arg < argc) {
                    detRad = atof(argv[arg])*Kpc;
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'b':
                if(++arg < argc) {
                    GFloat field=atof(argv[arg]);
                    bfield.setX(GMath::getSignedRand());
                    bfield.setY(GMath::getSignedRand());
                    bfield.setZ(GMath::getSignedRand());
                    bfield /= bfield.mag();
                    bfield *= field;
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'p':
                if(++arg < argc) {
                    protonMapFile = argv[arg];
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case '2':
                if(++arg < argc) {
                    secondaryProtonFile = argv[arg];
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'v':
                if(++arg < argc) {
                    nuFile = argv[arg];
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'g':
                if(++arg < argc) {
                    photonFile = argv[arg];
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'c':
                if(++arg < argc) {
                    cmbFile = argv[arg];
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'q':
                if(++arg < argc) {
                    GFloat quality = atof(argv[arg]);
                    ThreeBodyDecay::setDecayPrecision(quality);
                } else {
                    usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;

            default:
                usage(argv[0]);
                return EXIT_FAILURE;
        }
    }
    
    Universe    universe(bfield, earthPosition, distStep/SpeedOfLight, detRad);
    
    std::list<EvolutionThread*> threadList;
    
    // create a detector for protons
    ProtonDetector *pDetector = new ProtonDetector(secondaryProtonFile, 
                                                   earthPosition, 
                                                   detRad,
                                                   NBINS,
                                                   GMath::min(PR_E_MIN, startEnergy),
                                                   GMath::max(PR_E_MAX, endEnergy));
    
    // create a detector for neutrinos
    NuDetector *nuDetector = new NuDetector(nuFile,
                                            earthPosition, 
                                            detRad,
                                            NBINS,
                                            GMath::min(NU_E_MIN, startEnergy),
                                            GMath::max(NU_E_MAX, endEnergy));
    
    PhotonDetector *gDetector = new PhotonDetector(photonFile, 
                                                   earthPosition, 
                                                   detRad,
                                                   NBINS,
                                                   PH_E_MIN,
                                                   PH_E_MAX);
    
    CMB         cmb(cmbFile, cmbTemp, &universe, NBINS);

    // create a new simulation environment
    for(int i=0; i<nThreads; i++) {
        EvolutionThread *thread = new EvolutionThread(MAX_PARTICLES, 
                                                      &universe);
        
        thread->addDetector(pDetector);
        thread->addDetector(nuDetector);
        thread->addDetector(gDetector);
        
        thread->addSpace(&cmb);

        // FIXME FIXME FIXME
        // add a neutrino background space
        // for z burst

        // thread->addSpace(nuB);
        
        thread->start();
        threadList.push_back(thread);
    }

    
    // create a new event generator
    EventGenerator      eventGenerator(protonMapFile,
                                       Waxman_P0,
                                       &universe,
                                       threadList,
                                       NBINS,
                                       startEnergy,
                                       endEnergy);

    // deal with particles before generating new ones
    eventGenerator.setPriority(GThread::LOWEST);
    
    std::list<ProtonSource*> protonSrcList;

    GFloat deltaD=endDistance-startDistance;
    // insert nSources sources
    for(int j=0; j<nSources; j++) {
        // generate a random radius in the expected range
        GFloat r = deltaD*GMath::getRand() + startDistance;
        
        // generate a random orientation on the sky
        GFloat theta = PI*GMath::getRand()+PI/2.0;
        // generate a random angle
        GFloat phi   = 2.0*PI*GMath::getRand()+PI/2.0;
        
        Position srcPosition;
        srcPosition.setSpherical(r, theta, phi);
        
        srcPosition += earthPosition;
        
        ProtonSource *pSrc=new ProtonSource(startEnergy, 
                                            endEnergy, 
                                            alpha, 
                                            srcPosition,
                                            deltaPct);
        
        for(std::list<GFloat>::iterator en=deltaEn.begin();
            en!=deltaEn.end();
            ++en) {
            pSrc->addDeltaSrc(*en);
        }
        protonSrcList.push_back(pSrc);
        
        eventGenerator.add(pSrc);
        
        universe.add(pSrc);
    }
    
    // start event generator
    eventGenerator.start();
    
    // wait for nEvents neutrino events
    while(nuDetector->getEventsDetected() < nStopEvents) {
        GThread::sleep(3000);
        
        std::cout << "Queue: ";
        int i=0;
        for(std::list<EvolutionThread*>::iterator thread=threadList.begin();
            thread!=threadList.end();
            i++, ++thread) {
                    
            std::cout << " " << i << ": " << (*thread)->getEventQueueSize() << ", ";
        }
        std::cout << "Detected: " << nuDetector->getEventsDetected() <<
            "/" << nStopEvents << "    \r";
        
        std::cout.flush();
    }
    
    // stop generating events
    eventGenerator.stop();
    
    std::cout << std::endl << "Please wait for all particles to finish propagating..." << std::endl;
    // stop processing events
    for(std::list<EvolutionThread*>::iterator thread=threadList.begin();
        thread!=threadList.end();
        ++thread) {
        (*thread)->stop();
    }
    bool anyThread=true;
    while(anyThread) {
        anyThread=false;
                
        std::cout << "Queue: ";
        int i=0;
        for(std::list<EvolutionThread*>::iterator thread=threadList.begin();
            thread!=threadList.end();
            i++, ++thread) {
            int queueSize = (*thread)->getEventQueueSize();

            if((queueSize > 0) && 
               ((*thread)->getStatus() == GRunThread::RUNNING)) 
            {
                
                anyThread = true;
            }
            std::cout << " \t" << i << ": \t" << (*thread)->getEventQueueSize() << ", ";
        }
        std::cout << "Detected: \t" << nuDetector->getEventsDetected() <<
            "/" << nStopEvents << "\r";
        
        std::cout.flush();
        GThread::sleep(2000);
    }
    
    std::cout << "Writing logs and exiting ... " << std::endl;
    eventGenerator.waitForExit();
    
    for(std::list<EvolutionThread*>::iterator thread1=threadList.begin();
        thread1!=threadList.end();
        ++thread1) {
        
        (*thread1)->waitForExit();
        delete(*thread1);
    }

    
    // cleanup
    for(std::list<ProtonSource *>::iterator source=protonSrcList.begin();
        source != protonSrcList.end();
        ++source) {
        if(*source) delete *source;
    }
    
    if(nuDetector) delete nuDetector;
    if(pDetector) delete pDetector;
    if(gDetector) delete gDetector;
    
    return EXIT_SUCCESS;
    
}


void usage(const char *name) {

    std::cerr << name << ": -[ arguments ] ..." << std::endl;
    std::cerr << "Simulate a flux of ultra high energy neutrinos from cosmic ray sources." << std::endl;
    std::cerr << std::endl;
    std::cerr << "\t" << "-sources #   - The number of sources." << std::endl;
    std::cerr << "\t" << "-events #    - The number of events to simulate." << std::endl;
    std::cerr << "\t" << "-threads #   - The number of processor threads." << std::endl;
    std::cerr << "\t" << "-alpha #     - Simulate E^alpha spectrum." << std::endl;
    std::cerr << "\t" << "-low #       - Least energy [EeV]." << std::endl;
    std::cerr << "\t" << "-hi #        - Highest energy [EeV]." << std::endl;
    std::cerr << "\t" << "-delta #     - Insert a delta fn source in [EeV]." << std::endl;
    std::cerr << "\t" << "-deltapct #  - Percentage flux from delta fn sources." << std::endl;
    std::cerr << "\t" << "-near #      - Least source distance [Mpc]." << std::endl;
    std::cerr << "\t" << "-far #       - Highest source distance [Mpc]." << std::endl;
    std::cerr << "\t" << "-dx #        - distance step [Kpc]." << std::endl;
    std::cerr << "\t" << "-rad #       - detector radius [Kpc]." << std::endl;
    std::cerr << "\t" << "-bfield #    - B field strength [Gauss]." << std::endl;
    std::cerr << "\t" << "-quality #   - The precision of Monte Carlo convergence." << std::endl;
	std::cerr << "\t" << "-proton file - Name of file for input protons." << std::endl;
        std::cerr << "\t" << "-2nd    file - Name of file for secondary protons." << std::endl;
        std::cerr << "\t" << "-v      file - Name of file for neutrinos" << std::endl;
        std::cerr << "\t" << "-cmb    file - Name of file for cmb distributions" << std::endl;
        std::cerr << "\t" << "-gamma  file - Name of file for photons." << std::endl;

}

void license(const char *name) {
    std::cerr << name << " version " << GZKFAST_VERSION << " Copyright 2005-2007 John A Cairns <john@2ad.com>" << std::endl;
    std::cerr << name << " comes with ABSOLUTELY NO WARRANTY;" << std::endl;
    std::cerr << "This is free software, and you are welcome to redistribute it" << std::endl;
    std::cerr << "under certain conditions." << std::endl;
}
