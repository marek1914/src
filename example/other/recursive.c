#include <stdio.h>

test(int s)
{
	if(s==0){
		printf("s is %d\n", s);
		return;
	}
	test(s-1);
	printf("s is %d\n", s);
}

main()
{
	test(20);
}
