#!/bin/sh

# print out point sources

echo creating map of point sources

gmtset GRID_CROSS_SIZE 0 ANOT_FONT_SIZE 10 HEADER_FONT_SIZE 14
psbasemap -R0/6.5/0/9 -Jx1i -B0 -P -K -U"The Proton and Neutrino sky" > sources.ps
pscoast -R-180/180/-90/90 -JH0/5.5i -X0.5i -Y0.5i -O -K -Bg30:."Proton Sources": -Dc -G200 >> sources.ps
psxy proton_events.dat -: -R-180/180/-90/90 -JH0/5.5i -O -K -Sc.1 -G0/0/0 >> sources.ps
psxy secondaryproton_events.dat -: -R-180/180/-90/90 -JH0/5.5i -O -K -Sc.05 -G255/0/0 >> sources.ps
pscoast -R-180/180/-90/90 -JH0/2.5i -Y5i -O -K -Bg30:."Gammas": -Dc -G200 >> sources.ps
psxy photon_events.dat -: -R-180/180/-90/90 -JH0/2.5i -O -K -Sc.05 -G255/50/0 >> sources.ps
pscoast -R-180/180/-90/90 -JH0/2.5i -X3i -O -K -Bg30:."Neutrinos": -Dc -G200 >> sources.ps
#psxy neutrino_events.dat -: -R-180/180/-90/90 -JH0/6i -O -Sc.05 -G0/0/0 >> cosmicray.ps
psxy neutrino_e_events.dat -: -R-180/180/-90/90 -JH0/2.5i -O -K -Sc.05 -G0/255/0 >> sources.ps
psxy neutrino_mu_events.dat -: -R-180/180/-90/90 -JH0/2.5i -O -Sc.05 -G0/0/255 >> sources.ps


\rm -f .gmtcommands

