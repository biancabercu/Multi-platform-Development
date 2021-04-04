#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
	char h[1][10];
	memcpy(h,"astazi ",10);
	char a[10][10];
	memcpy(a[0], h, 10);
	memcpy(a[1], h, 3);
	printf("%s si %s \n", a[0],a[1]);
}
