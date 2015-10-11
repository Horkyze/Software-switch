
/*==================================
=        HASH TABLE FOR IP         =
==================================*/

typedef struct Item_h{
	u_long ip;
	//u_char ip[4]; //ip addr has 4 bytes
	int bytes_send;
}Item_h;

typedef struct Hash_table{
	Item_h * table;	// array of Item_h 
	u_int * indexes;  // occupied indexed in table
	u_int current_index_pointer;
	u_int n;
}Hash_table;

int insert(Hash_table * ht, int x);

Hash_table * init_hash_table(int size){

	Hash_table * h = 0;
	h = (Hash_table*) malloc(sizeof(Hash_table)); 

	h->n = size;
	h->table  = 0;
	h->current_index_pointer = 0;

	h->table  	= (Item_h *) malloc(sizeof(Item_h) * size); 
	h->indexes  = (int *)    malloc(sizeof(int) * size); 

	memset(h->table , 0, sizeof(Item_h) * size);

	return h;
}

int hash(u_int a){
	a ^= (a << 13);
	a ^= (a >> 17);
	a ^= (a << 5);
	return a;   
}

// insert into hash table ht value x

int insert_h(Hash_table * ht, u_int x, int bytes)
{

	u_long h = hash(x) % ht->n;
	(ht->table[h]).bytes_send += bytes;
	(ht->table[h]).ip = x;
	ht->indexes[ ht->current_index_pointer++ ] = h;


	return 1;
}


// search in hash table ht for value x
// return 1 if found
int search_h(Hash_table * ht, u_int x)
{
	u_long h = hash(x) % ht->n;

	while((ht->table[h]).ip != x){
		
		if ((ht->table[h]).ip == 0){
			return 0;
		}

		h = (h < ht->n)? h + 1 : 0;
	}

	return ht->table[h].bytes_send;

}



/*-----  End of HASH TABLE  ------*/










/*==================================
=        HASH TABLE                =
==================================*/

typedef struct ITEM_HT{
	LL * ll;
	u_int conversation_id;
}ITEM_HT;

typedef struct HT{
	ITEM_HT * table;	// array of ITEM_HT 
	u_int * indexes;  // occupied indexed in table
	u_int current_index_pointer;
	u_int n;
}HT;



HT * init_ht(int size){

	HT * h = 0;
	h = (HT*) malloc(sizeof(HT)); 

	h->n = size;
	h->table  = 0;
	h->current_index_pointer = 0;

	h->table  	= (ITEM_HT *) malloc(sizeof(ITEM_HT) * size); 
	h->indexes  = (int *)    malloc(sizeof(int) * size); 

	memset(h->table , 0, sizeof(ITEM_HT) * size);
	while(size--){
		(h->table[size]).ll = LL_init();
		(h->table[size]).conversation_id = 0;
	}
	return h;
}



// find conversation id for frame f
// search for linked list in hash table
// add frame to linked list
LL * insert_ht(HT * ht, u_int conversation_id)
{

	u_long h = hash(conversation_id) % ht->n;
	(ht->table[h]).conversation_id = conversation_id;

	ht->indexes[ ht->current_index_pointer++ ] = h;


	return (ht->table[h]).ll;
}


// search in hash table ht for value x
// return 1 if found
// LL * search_ht(HT * ht, u_int x)
// {
// 	u_long h = hash(x) % ht->n;

// 	while((ht->table[h]).ip != x){
		
// 		if ((ht->table[h]).ip == 0){
// 			return 0;
// 		}

// 		h = (h < ht->n)? h + 1 : 0;
// 	}

// 	if (ht->table[h].conversation_id == 0){
// 		return 0;
// 	}

// 	return ht->table[h].ll;

// }



/*-----  End of HASH TABLE  ------*/
