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
					printf("PORT ACTION DIRECTION SRC DST PROTO\n eg: 1 ALLOW IN 1.1.1.1 any 80\n");
					printf(" > ");
					char * p 	= (char *) calloc(20, 1);
					char * a 	= (char *) calloc(20, 1);
					char * d 	= (char *) calloc(20, 1);
					char * dst 	= (char *) calloc(20, 1);
					char * src 	= (char *) calloc(20, 1);
					char * proto = (char *) calloc(20, 1);
					printf("PORT (1 or 2):\t");					
					scanf("%s", p);
					printf("ACTION (allow or deny):\t");
					scanf("%s", a);
					printf("DIRECTION (in or our):\t");
					scanf("%s", d);
					printf("SRC_ADDR:\t");
					scanf("%s", src);
					printf("DST_ADDR:\t");
					scanf("%s", dst);
					printf("PORTOCOL PORT:\t");
					scanf("%s", proto);
					//int i;
					create_rule(p, a, d, src, dst, proto);
					//cmd = get_line();

				} else if(strcmp(cmd, RULE_DELETE) == 0){
					int id;
					printf("Rule id: ");
					scanf("%d", &id);
					LL_delete(rules_ll, id);

				} else if(strcmp(cmd, RULE_LIST) == 0){
					printf("Following rules are activated:\n");
					for (size_t i = 0; i < 5; i++) {
						printf("Rule..\n");
					}
				} else if(strcmp(cmd, MAC_DELETE) == 0){
					printf("Deleting entire mac table...\n ");
					clear_mac();
				} else if (strcmp(cmd, QUIT) == 0 || strcmp(cmd, "q") == 0){
					pause_rendering = 0;
					break;
				} else if(strcmp(cmd, "help") == 0 || strcmp(cmd, "?") == 0){
					printf("List of commands:\n");
					printf(" %s\n", RULE_ADD);
					printf(" %s\n", RULE_DELETE);
					printf(" %s\n", RULE_LIST);
					printf(" %s\n", QUIT);

				}

			}
		} else if (c == 'm'){
			mock_rule();
		}

	}

}
