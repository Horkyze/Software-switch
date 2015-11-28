
#define TABLE "table.txt"
#define MAC "11:22:33:aa:bb:cc"
#define MAX 1024
#define MAC_SIZE 20

LL * mac_table_ll = 0;

typedef struct Record{
	char mac[MAC_SIZE];
	Port * p;
	time_t last_update;
}Record;

Record * create_record_struct(char * mac, Port * p, time_t t){
	Record * r;
	r = (Record *) malloc(sizeof(Record));
	strncpy(r->mac, mac, MAC_SIZE);
	r->p = p;
	r->last_update = t;
	return r;
}

void print_mac(){
	printf("-------- MAC TABLE --------\n");
	if (mac_table_ll == 0){
		printf("--------    END    --------\n");
		return;
	}
	Item * curr = (Item *) mac_table_ll->head;

	while(curr){
		printf("%s %s \t%s\n", ((Record *)curr->data)->mac, ((Record *)curr->data)->p->name, get_time(&((Record *)curr->data)->last_update));
		curr = curr->next;
	}
	printf("--------    END    --------\n");
}

void clear_mac(){
	// for each bla bla..
	free(mac_table_ll);
	mac_table_ll = 0;
}

void mac_delete_old_entries(int older_than){
	if (mac_table_ll == 0){
		return;
	}
	time_t t;
	t = time(0);
	int i = 0;
	Item * curr = (Item *) mac_table_ll->head;
	while(curr){
		if ( t - ((Record *)curr->data)->last_update > older_than ){		
			sprintf(log_b, "AUTO deleted entry from mac table (%s)", ((Record *)curr->data)->mac );
			my_log(log_b);
			LL_delete(mac_table_ll, i);
		}
		curr = curr->next;
		i++;
	}
	return;
}

Record * mac_table_search(char * mac){
	if (mac_table_ll == 0){
		return 0;
	}

	Item * curr = (Item *) mac_table_ll->head;
	while(curr){
		if ( strcasecmp( ((Record *)curr->data)->mac, mac) == 0 ){
			return ((Record *)curr->data);
		}
		curr = curr->next;
	}
	return 0;
}

void mac_table_insert(char * mac, Port * p){
	if (mac_table_ll == 0){
		mac_table_ll = LL_init();
	}
	time_t t;
	Record * r = mac_table_search(mac);

	t = time(NULL);
	if (r){ 
		// if found update time
		r->last_update = t;
	} else {
		// if not found, insert
		LL_add(mac_table_ll, (void *) create_record_struct(mac, p, t));
	}	
}



