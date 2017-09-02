cat testdata | while read line
do
	echo Input:
	echo $line
	echo Output:
	./a.out $line
	echo
done
