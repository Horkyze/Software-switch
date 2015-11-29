#ifndef STATS_H
#define STATS_H

typedef struct Stats{
	// layer 2
	u_long l2_total;

	//layer 3
	u_long l3_ip;
	u_long l3_icmp;
	u_long l3_arp;

	//layer 4
	u_long l4_tcp;
	u_long l4_udp;

	//layer 5
	u_long l5_http; // 80
	u_long l5_telnet; //2
	u_long l5_other; //2
	
}Stats; 

Stats * create_stats_struct(){
	Stats * s;
	s = (Stats *) calloc(sizeof(Stats), 1);
	return s;
}

void print_stats_header(){
	printf("\nPort\tTotal\tIP\tICMP\tARP\tTCP\tUDP\tHTTP\tTelnet\tOther\n");
	printf("-----------------------------------------------------------------------------\n");
}

void print_stats(Stats * s, const char * port){
	printf("%s\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t \n",
		port, s->l2_total, s->l3_ip, s->l3_ip, s->l3_icmp, s->l4_tcp, s->l4_udp, s->l5_http, s->l5_telnet, s->l5_other);
}

#endif