rm -f index.txt
rm -f b*
split -b 4k update.bin
i=0

for f in x* ; do
mv $f b`printf "%03d" $i`
((i++))
done

for f in b* ; do
crc=`cksum $f | cut -f 1 -d ' '`
echo $f `printf %08x $crc` >> index.txt
done
