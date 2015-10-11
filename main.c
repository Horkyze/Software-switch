/**
A simple packet analyzer utility using pcap library.
Created as a school assigment for Computer Networks curse.
Matej Bellus

**/

#define LOG_FILE "log.txt"

#pragma pack(1)

#include <pcap.h>
#include <arpa/inet.h>
#include <getopt.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"

// custom includes
#include "functions.h"
#include "eth_parser.h"
//#include "parser.h"

typedef struc Rule {
	int action; // ALLOW or DENY
	pcap_t * port;
	int direction; // IN or OUT
	int src_addr_type; // MAC or IP
	int dst_addr_type; // MAC or IP
	void * src_addr; 
	void * dst_addr;
	int protocol; // http, icmp, ...
}Rule;

void print_usage(){
	printf("\nSoftware switch implementation by Matej Bellus (c) \n");
	printf("Usage: s_switch [-l] [-1 <interface_1> -2 <interface_2>] \n\n");
	printf("   -l 			List available interfaces\n");
	printf("   -1 			First interface\n");
	printf("   -2 			Second inteface\n");
	printf("\nEXAMPLE 		sudo ./bin/s_switch -1 eth0 -2 wlan0\n");
}


void list_interfaces(){
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if_t * alldevs;
	pcap_if_t * d;

	pcap_findalldevs(&alldevs, errbuf);
	for(d=alldevs; d; d = d->next){
		printf("%s\n", d->name);
  	}
}

int main(int argc, char *argv[])
{

	int option = 0;
	char * p1_interface = 0, * p2_interface = 0;

	while ((option = getopt(argc, argv,"h l 1:2:")) != -1) {
		switch (option) {
			case '1' :
				p1_interface = optarg;
				break;
			case '2' :
				p2_interface = optarg;
				break;
			case 'l':
				list_interfaces();
				exit(0);
			case 'h':
			default: print_usage();
				exit(EXIT_FAILURE);
		}
	}

	printf("PORT 1 => %s\n", p1_interface);
	printf("PORT 2 => %s\n", p2_interface);



	pcap_t *handle_1, *handle_2;	
	char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */
	struct bpf_program fp;		/* The compiled filter */
	char filter_exp[] = "";	/* The filter expression */

	struct pcap_pkthdr header;	/* The header that pcap gives us */
	const u_char *packet_1, *packet_2;		/* The actual packet */


	handle_1 = pcap_create(p1_interface, errbuf);
	if ( pcap_activate(handle_1)){
		printf("Failed to open interface %s\n", p1_interface);
		exit(-1);
	}   

    handle_2 = pcap_create(p2_interface, errbuf);
	if ( pcap_activate(handle_2) ){
		printf("Failed to open interface %s\n", p2_interface);
		exit(-1);
	}

	while (1) {

		/* Grab a packet */
		packet_1 = pcap_next(handle_1, &header);
		if (packet_1) {
			printf("%u\n", header.len);
			my_log("Got frame with size [%u]\n", header.len);
			Frame * f = add_frame((u_char*)packet_1, header.len);
			//print_eth(f);
			pcap_inject(handle_2, packet_1, 30);
		}
	}

	/* And close the session */
	pcap_close(handle_1);
	return(0);
}
