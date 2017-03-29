#include <stdio.h>
#include <string.h>

main(void)
{
	char* p;
	
	char str[]="gaojiezhaojiajia";
	p = strtok(str, "ajo");
	
	if (p)
	printf("%s\n", p);
	else
	printf("sorry\n");


	do{
		p = strtok(NULL, "ajo");
		if (p) {
			printf("%s\n", p);
		}
	} while(p);

}
