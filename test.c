#include <stdio.h>
#include <unistd.h>
#include <time.h>
 
int main()
{
	int i;
	time_t t;
	

	for (i = 5; i; i--) {
				
		t = time(0);
		printf("Time: %s...\n", ctime(&t));
		sleep(1);

	}

 
	return 0;
}