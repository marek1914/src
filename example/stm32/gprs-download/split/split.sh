
if [ -n "$1"  ]; then
  slice=$1
else
  slice=4
fi

mkdir -p split
cd split
rm -f *

#mac os
#size=`stat -f "%z" ../update.bin`
size=`stat  -c "%s" ../update.bin`

align1k=$((size+0x7ff&0xfffff800))
echo $size $align1k

cat ../update.bin ../page.bin | dd of=update_align.bin bs=1 count=$align1k

split -b ${slice}k update_align.bin
rm update_align.bin

i=0

for f in x* ; do
mv $f `printf "%02x" $i`
((i++))
done

i=0
for f in * ; do
if ((!(i%16))) ;then echo >> temp.txt; fi
echo -n "`../crc8 $f` " >> temp.txt
((i++))
done

echo

echo count:`printf "%02x" $i` >>tmp.txt
cat temp.txt >> tmp.txt
tmpcrc=`../crc8 tmp.txt`

echo crc:$tmpcrc >>index.txt
cat tmp.txt >>index.txt

rm -f tmp.txt temp.txt

