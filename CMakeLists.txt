cmake_minimum_required(VERSION 3.9)

project(gzkfast)

set(CMAKE_VERBOSE_MAKEFILE on)
SET(CMAKE_CXX_FLAGS_RELEASE -O3)
SET(CMAKE_CXX_FLAGS_DEBUG "-g -DGZKFAST_DEBUG=1")
SET(CMAKE_CXX_FLAGS -DGZKFAST_VERSION=\\"1.0.1\\")

add_compile_options(-std=c++0x -Wint-to-pointer-cast -Wreturn-type)

SET(HEADER include/bfieldspace.h include/cmbdist.h include/cmb.h include/delta1600.h include/delta.h include/deltaspectrum.h include/detector.h include/electron.h include/eventgenerator.h include/evolutionthread.h include/g4vector.h include/gerror.h include/gfunction.h include/gguard.h include/ghistogram.h include/gintegrate.h include/gloghistogram.h include/glorentz.h include/gmath.h include/gmatrix.h include/gmutex.h include/grotation.h include/grunthread.h include/gthread.h include/gvector.h include/mu.h include/neutron.h include/nudetector.h include/nu.h include/particle.h include/photondetector.h include/photon.h include/physicalconstants.h include/pion.h include/pizero.h include/protondetector.h include/proton.h include/protonsource.h include/protonspectrum.h include/source.h include/space.h include/sphere.h include/threebodydecay.h include/twobodydecay.h include/universe.h include/z.h)

SET(SRC src/bfieldspace.cpp src/cmb.cpp src/cmbdist.cpp src/delta1600.cpp src/delta.cpp src/deltaspectrum.cpp src/electron.cpp src/eventgenerator.cpp src/evolutionthread.cpp src/g4vector.cpp src/gerror.cpp src/gguard.cpp src/ghistogram.cpp src/gintegrate.cpp src/gloghistogram.cpp src/gmath.cpp src/gmutex.cpp src/grotation.cpp src/grunthread.cpp src/gthread.cpp src/mu.cpp src/neutron.cpp src/nu.cpp src/nudetector.cpp src/particle.cpp src/photon.cpp src/photondetector.cpp src/pion.cpp src/pizero.cpp src/proton.cpp src/protondetector.cpp src/protonsource.cpp src/protonspectrum.cpp src/sphere.cpp src/threebodydecay.cpp src/twobodydecay.cpp src/universe.cpp src/z.cpp)

include_directories(gzkparticle include)

add_library(gzkparticle SHARED ${HEADER} ${SRC})

add_executable(gzkfast utils/gzkfast.cpp)

target_link_libraries(gzkfast gzkparticle gsl gslcblas m pthread)

add_executable(betadecay utils/betadecay.cpp)

target_link_libraries(betadecay gzkparticle gsl gslcblas m pthread)

add_executable(cmbspectrum utils/cmbspectrum.cpp)

target_link_libraries(cmbspectrum gzkparticle gsl gslcblas m pthread)

add_executable(cmbdensity utils/cmbdensity.cpp)

target_link_libraries(cmbdensity gzkparticle gsl gslcblas m pthread)

add_executable(meanpath utils/meanpath.cpp)

target_link_libraries(meanpath gzkparticle gsl gslcblas m pthread)


