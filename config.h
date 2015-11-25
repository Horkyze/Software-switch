#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define RULE_ADD "rule add"
#define RULE_DELETE "rule delete"
#define MAC_DELETE "mac delete"
#define RULE_LIST "rule list"
#define QUIT "quit"

/* This is our thread function.  It is like main(), but for a thread*/
void * config(void * arg){

	char c;
	char * cmd;
	printf("Hello from config\n");
	while (1) {

		scanf("%c", &c);
		if (c == 'c') {
			// now user is in config mode
			pause_rendering = 1;

			while(1){
				printf("config> ");
				cmd = get_line();

				if(strcmp(cmd, RULE_ADD) == 0) {
					printf("PORT ACTION DIRECTION SRT DST PROTO\n eg: ALLOW IN 1.1.1.1 any http\n");
					printf(" > ");
					char a[100];
					char d[100];
					char dst[100];
					char src[100];

					int i;
					
					cmd = get_line();

				} else if(strcmp(cmd, RULE_DELETE) == 0){
					int id;
					printf("\tRule id: ");
					scanf("%d", &id);

				} else if(strcmp(cmd, RULE_LIST) == 0){
					printf("Following rules are activated:\n");
					for (size_t i = 0; i < 5; i++) {
						printf("Rule..\n");
					}
				} else if(strcmp(cmd, MAC_DELETE) == 0){
					printf("Deleting entire mac table...\n ");
					clear_mac();
				} else if (strcmp(cmd, QUIT) == 0){
					pause_rendering = 0;
					break;
				} else if(strcmp(cmd, "help") == 0){
					printf("List of commands:\n");
					printf(" %s\n", RULE_ADD);
					printf(" %s\n", RULE_DELETE);
					printf(" %s\n", RULE_LIST);
					printf(" %s\n", QUIT);

				}

			}
		}

	}

}
