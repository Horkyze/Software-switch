/**
A simple packet analyzer utility using pcap library.
Created as a school assigment for Computer Networks curse.
Matej Bellus

**/
#pragma pack(1)

#include <pcap.h>
#include <arpa/inet.h>
#include <getopt.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

// custom includes
#include "functions.h"

#define LOG_FILE "log.txt"
// number or ports on switch - one port binds to one interface
#define PORTS 2

void print_usage(){
	printf("Software switch implementation by Matej Bellus (c) \n");
	printf("Usage: s_switch [-l] \n\n");
	printf("   -l 			List available interfaces\n");
}

#include <pcap.h>
#include <stdio.h>

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

	while ((option = getopt(argc, argv,"h l: 1: 2:")) != -1) {
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

	

	pcap_t *handle;			/* Session handle */
	char *dev;			/* The device to sniff on */
	char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */
	struct bpf_program fp;		/* The compiled filter */
	char filter_exp[] = "";	/* The filter expression */
	bpf_u_int32 mask;		/* Our netmask */
	bpf_u_int32 net;		/* Our IP */
	struct pcap_pkthdr header;	/* The header that pcap gives us */
	const u_char *packet;		/* The actual packet */

	/* Define the device */
	dev = interface;
	if (dev == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		return(2);
	}

	handle = pcap_create(interface, errbuf);
	pcap_activate(handle);
    pcap_inject(handle, "asdasd", 3);

	/* Grab a packet */
	packet = pcap_next(handle, &header);
	/* Print its length */
	printf("Jacked a packet with length of [%d]\n", header.len);
	/* And close the session */
	pcap_close(handle);
	return(0);
}
