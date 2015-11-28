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

#endif