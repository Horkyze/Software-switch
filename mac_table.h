
#define TABLE "table.txt"
#define MAC "11:22:33:aa:bb:cc"
#define MAX 1024

void print_mac(){
	printf("-------- MAC TABLE --------\n");
	system("cat table.txt");
	printf("--------    END    --------\n");
}

void clear_mac(){
	system("echo '' > table.txt");
}

char * ex(char * command){
	char * buffer = (char * ) calloc(1, MAX);
	int i = 0;

	FILE * d = popen(command, "r");

	while( i < MAX && fscanf(d, "%c", &buffer[i++]) != EOF) {
		//printf("%s\n", buffer);
	}
	pclose(d);
	return buffer;
}

char * find_port(char * mac){
	char buffer[MAX] = {0};
	strcat(buffer, "cat ");
	strcat(buffer, TABLE);
	strcat(buffer, " | grep -i ");
	strcat(buffer, mac);
	strcat(buffer, " | cut -d '\t' -f2");
	return ex(buffer);
}

// echo "mac" >> table 
void insert(char * mac, char * port){
	char buffer[MAX] = {0};
	strcat(buffer, "echo ");
	strcat(buffer, mac);
	strcat(buffer, "\t");
	strcat(buffer, port);
	strcat(buffer, " >> ");
	strcat(buffer, TABLE);
	ex(buffer);
}



