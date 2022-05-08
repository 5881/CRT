#!/bin/zsh
IMG=$1
echo "$1"
STR=$(identify "$IMG"|awk -F' ' '{print $3}')
echo "$STR"
X=$(echo $STR|awk -F'x' '{print $1}')
Y=$(echo $STR|awk -F'x' '{print $2}')
echo "$X $Y"
if [ $((X)) -lt $((Y)) ];
then
echo "X<Y"
convert -normalize -resize x256 -colorspace gray -edge $2 -negate $IMG temp.png
else
echo "X>Y"
convert -normalize -resize 256x -colorspace gray -edge $2 -negate $IMG temp.png
fi
./black "temp.png" "${1:0:(-3)}h"
#rm temp.png
#convert test2.png -colorspace gray -colors 16 -resize x256 -ordered-dither o2x2 t.png
