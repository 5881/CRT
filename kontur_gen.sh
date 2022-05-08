#!/bin/zsh
IMG=$1
STR=$(identify "$IMG"|awk -F' ' '{print $3}')
X=$(echo $STR|awk -F'x' '{print $1}')
Y=$(echo $STR|awk -F'x' '{print $2}')
if [ $((X)) -lt $((Y)) ]
then
convert -normalize -brightness-contrast -30x30 -colors 16 -colorspace gray  -resize x256 -ordered-dither o4x4 -negate $IMG temp.png
else
convert -normalize -brightness-contrast -30x30 -colors 16 -colorspace gray   -resize 256x -ordered-dither o4x4 -negate $IMG temp.png
fi
./black "temp.png" "${1:0:(-3)}h"
#rm temp.png
#convert test2.png -colorspace gray -colors 16 -resize x256 -ordered-dither o2x2 t.png
