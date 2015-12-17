#!/bin/sh

RES=30m 
SEARCH=5000k
gmtset GRID_CROSS_SIZE 0 ANOT_FONT_SIZE 10 HEADER_FONT_SIZE 14
for i in *_events.dat; 
do 
BASENAME=`basename $i .dat`

echo $TITLE
echo Gridding $i;
cat $i | awk '{print $1 " " $2 " " log($3)}' - > $BASENAME.log;
#nearneighbor -R-180/180/-90/90 -I$RES -S$SEARCH -G$BASENAME.grd -V $i;
blockmean $BASENAME.log -I$RES -R-180/180/-90/90 |surface -R-180/180/-90/90 -I$RES -G$BASENAME.grd -V
grdinfo -M -C $BASENAME.grd
MIN=`grdinfo -M -C $BASENAME.grd|awk '{ print $6 }' -`

MAX=`grdinfo -M -C $BASENAME.grd|awk '{ print $7 }' -`

makecpt -Cno_green -I -T$MIN/$MAX/1 -Z > hot.cpt

echo Mapping $i
psbasemap -R0/6.5/0/9 -Jx1i -B0 -P -K -U"$BASENAME" > "$BASENAME".ps
pscoast -R-180/180/-90/90 -JH0/5.5i -X0.5i -Y0.5i -O -K -Bg30:."$BASENAME": -Dc -G200 >> $BASENAME.ps
psxy $i -: -R-180/180/-90/90 -JH0/5.5i -O -K -Sc.1 -G0/0/0 >> "$BASENAME".ps
grdimage  -Y5i $BASENAME.grd -Bg30 -R-180/180/-90/90 -JH0/5.5i -Chot.cpt -O  >> "$BASENAME".ps

done

\rm -f .gmtcommands

