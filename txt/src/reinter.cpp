#include <stdio.h>

class StringHelper;


void foo(StringHelper* bar)
{
	const char* s;
	s = reinterpret_cast<char*>(bar);

	printf("%s", s);
}

int main(void)
{
	char *s = "hello world ==\n";
	foo( reinterpret_cast<StringHelper*>(s) );	
	return 0;
}
