#ifndef FUNSTIONS_H
#define FUNSTIONS_H

void my_log(char * format, ...){

	FILE * fd;
	#ifdef LOG_FILE
		fd = fopen(LOG_FILE, "a");
		if (fd == 0) {
			printf("fd is zero \n");
		}
	#else
		fd = (FILE *)1; // stdin
	#endif

	va_list args;
    va_start( args, format );
    fprintf(fd, format, args );
    va_end( args );
	fclose(fd);
}

int is_print(u_char c){
	return (c > 31 && c <= 127);
}



char * get_hex(u_char * data, int size, char delimiter){
	int i;
	char * buff = (char * ) calloc(3, size);
	for (i = 0; i < size; ++i){
		if(i == size - 1)
			sprintf(&buff[3*i], "%02X", data[i]);
		else
			sprintf(&buff[3*i], "%02X%c", data[i], delimiter);
	}
	return buff;
}

void print_hex(u_char * data, int len){
	int i;
	for (i = 0; i < len; ++i)
	{
		printf("%02x ", data[i]);
	}
	printf("\n");
}

char * ip_to_string(u_int ip){
	char * s;
	s = (char *) malloc(20);
    u_char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    sprintf(s, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
    return s;
}


void dump(const unsigned char * p, int len){
	int i, k = 0;
	u_char buff[17];
	memset(buff, 0, 17);
	for (i = 1; i < len + 1; ++i)
	{
		printf("%02x ", p[i-1]);

		buff[(i-1)%16] = (is_print(p[i-1]))? p[i-1] : '.';

		if (i % 8 == 0 && i) {
			printf(" ");
		}


		if (i % 16 == 0 && i){

			printf(" | %s \n", buff);
			memset(buff, 0, 17);
			k = 0;
		}


	}
	printf("\n\n");
}



/*===================================
=            Linked List            =
===================================*/

typedef struct LL {
	void * head;
	u_int number_of_items;

}LL;

typedef struct Item {
	unsigned int index;
	void * data;
	struct Item * next;
}Item;

LL * LL_init(){
	LL * ll = malloc(sizeof(LL));
	ll->head = 0;
	ll->number_of_items = 0;
	return ll;
}

Item * LL_add(LL * ll, void * data){
	Item * item = malloc(sizeof(Item));
	item->data = data;
	item->next = 0;
	ll->number_of_items++;

	if (ll->head == 0){
		ll->head = item;
		item->index = 1;
		return item;
	}

	Item * curr = ll->head;
	int i = 0;
	while(curr->next != 0) {
	    curr = curr->next;
	    i++;
	}
	curr->next = item;



	item->index = ll->number_of_items;
	return item;
}

void LL_print(LL * ll){
	Item * curr = ll->head;
	while(curr) {
	    printf("Item index %i: data_p: %p next_p: %p \n", curr->index, curr->data, curr->next);
	    curr = curr->next;
	}
}

/*-----  End of Linked List  ------*/


#endif
