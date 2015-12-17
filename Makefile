#     GZKFast - An environment for astrophysical simulation.
#     Copyright (C) 2005 John A Cairns <john@2ad.com>

#     This program is free software; you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation; either version 2 of the License, or
#     (at your option) any later version.

#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.

#     You should have received a copy of the GNU General Public License
#     along with this program; if not, write to the Free Software
#     Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
#     02110-1301  USA

#     John Cairns
#     2271 Coventry Rd    
#     Upper Arlington, OH, 43221

MAJOR=1
VERSION=${MAJOR}.0.1

CPP=c++
LD=ld
LN=ln
AR=ar

RM=rm -f

OPT= -O2
#OPT= -g -DGZKFAST_DEBUG=1

CPPFLAGS=-DGZKFAST_VERSION=\"${VERSION}\" -Iinclude \
	-Wconversion -Wshadow \
	-Wpointer-arith -Wcast-qual -Wcast-align \
	-Wwrite-strings \
	-fshort-enums -fno-common ${OPT} # -Dinline= \
	#	-ansi -pedantic

LDFLAGS=-lpthread -lgsl -lgslcblas -lm

OBJS=	\
	src/protonspectrum.o \
	src/deltaspectrum.o \
	src/photondetector.o \
	src/twobodydecay.o \
	src/threebodydecay.o \
	src/cmbdist.o \
	src/cmb.o \
	src/pion.o \
	src/pizero.o \
	src/nu.o \
	src/z.o \
	src/nudetector.o \
	src/protondetector.o \
	src/protonsource.o \
	src/mu.o \
	src/gmath.o \
	src/sphere.o \
	src/gerror.o \
	src/gintegrate.o \
	src/gguard.o \
	src/gmutex.o \
	src/bfieldspace.o \
	src/gthread.o \
	src/grunthread.o \
	src/g4vector.o \
	src/grotation.o \
	src/neutron.o \
	src/proton.o \
	src/electron.o \
	src/photon.o \
	src/particle.o \
	src/eventgenerator.o \
	src/evolutionthread.o \
	src/ghistogram.o \
	src/gloghistogram.o \
	src/universe.o \
	src/delta1600.o \
	src/delta.o 

all: libgzkparticle gzkfast betadecay cmbspectrum cmbdensity \
	meanpath

clean:
	${RM} ${OBJS} bin/gzkfast bin/betadecay \
		bin/cmbspectrum bin/cmbdensity \
		bin/protonspec bin/meanpath \
		lib/libgzkparticle.so \
		lib/libgzkparticle.so.${MAJOR} \
		lib/libgzkparticle.so.${VERSION} \
		lib/libgzkparticle.a utils/*.o

allclean: clean
	${RM} data/*.grd data/*.log data/*.dat data/*.ps

libgzkparticle: ${OBJS}
	${CPP} -shared -Wl,-soname,libgzkparticle.so.${MAJOR} -o lib/$@.so.${VERSION} ${OBJS}
	(cd lib && ${LN} -sf $@.so.${VERSION} $@.so.${MAJOR})
	(cd lib && ${LN} -sf $@.so.${VERSION} $@.so)
	${AR} rvu lib/$@.a ${OBJS}
	ranlib lib/$@.a

gzkfast: libgzkparticle utils/gzkfast.o 
	${CPP} ${LDFLAGS} -L./lib -lgzkparticle ${CPPFLAGS} utils/gzkfast.o \
		-o bin/$@ 

deltadecay: libgzkparticle utils/deltadecay.o 
	${CPP} ${LDFLAGS} -L./lib -lgzkparticle ${CPPFLAGS} utils/deltadecay.o \
		-o bin/$@ 

betadecay: libgzkparticle utils/betadecay.o 
	${CPP} ${LDFLAGS} -L./lib -lgzkparticle ${CPPFLAGS} utils/betadecay.o \
		-o bin/$@ 

cmbspectrum: libgzkparticle utils/cmbspectrum.o 
	${CPP} ${LDFLAGS} -L./lib -lgzkparticle ${CPPFLAGS} utils/cmbspectrum.o \
		-o bin/$@ 

cmbdensity: libgzkparticle utils/cmbdensity.o 
	${CPP} ${LDFLAGS} -L./lib -lgzkparticle ${CPPFLAGS} utils/cmbdensity.o \
		-o bin/$@ 

meanpath: libgzkparticle utils/meanpath.o 
	${CPP} ${LDFLAGS} -L./lib -lgzkparticle ${CPPFLAGS} utils/meanpath.o \
		-o bin/$@ 

src/pion.o: src/pion.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/pizero.o: src/pizero.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/nu.o: src/nu.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/z.o: src/z.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/mu.o: src/mu.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/delta1600.o: src/delta1600.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/delta.o: src/delta.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/cmb.o: src/cmb.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/gmath.o: src/gmath.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/g4vector.o: src/g4vector.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/grotation.o: src/grotation.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/neutron.o: src/neutron.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/proton.o: src/proton.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/electron.o: src/electron.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/photon.o: src/photon.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/twobodydecay.o: src/twobodydecay.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/threebodydecay.o: src/threebodydecay.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/protonspectrum.o: src/protonspectrum.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/deltaspectrum.o: src/deltaspectrum.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/ghistogram.o: src/ghistogram.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/gguard.o: src/gguard.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/gmutex.o: src/gmutex.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/gthread.o: src/gthread.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/grunthread.o: src/grunthread.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/gloghistogram.o: src/gloghistogram.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/cmbdist.o: src/cmbdist.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/eventgenerator.o: src/eventgenerator.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/evolutionthread.o: src/evolutionthread.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/bfieldspace.o: src/bfieldspace.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/nudetector.o: src/nudetector.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/protondetector.o: src/protondetector.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/protonsource.o: src/protonsource.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/photondetector.o: src/photondetector.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/gerror.o: src/gerror.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/sphere.o: src/sphere.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/gintegrate.o: src/gintegrate.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/particle.o: src/particle.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

src/universe.o: src/universe.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

utils/gzkfast.o: utils/gzkfast.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

utils/deltadecay.o: utils/deltadecay.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

utils/betadecay.o: utils/betadecay.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

utils/cmbspectrum.o: utils/cmbspectrum.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

utils/cmbdensity.o: utils/cmbdensity.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

utils/meanpath.o: utils/meanpath.cpp
	${CPP} ${CPPFLAGS} -c -o $@ $<

