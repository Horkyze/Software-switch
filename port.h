#ifndef PORT_H
#define PORT_H

typedef struct Port {
	int id;
	char * name;
	pcap_t * handle;
	pthread_t thread;

	Stats * in;
	Stats * out;

}Port;

Port * create_port_struct(int i){
	Port * p;
	p = (Port *) malloc(sizeof(Port));
	p->id = i;
	p->name = (char *) calloc(1, 100);
	p->handle = 0;
	p->thread = 0;
	p->in = create_stats_struct();
	p->out = create_stats_struct();
	return p;
}

#endif