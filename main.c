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
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"





typedef struct Port {
	int id;
	char * name;
	pcap_t * handle;
	pthread_t thread;
}Port;
pthread_mutex_t mutex;
int pause_rendering = 0;

Port *p1, *p2;

// custom includes
#include "functions.h"
#include "eth_parser.h"
#include "mac_table.h"
#include "config.h"
#include "rules.h"



#include "port_listener.h"





Port * create_port_struct(int i){
	Port * p;
	p = (Port *) malloc(sizeof(Port));
	p->id = i;
	p->name = (char *) calloc(1, 100);
	p->handle = 0;
	p->thread = 0;
	return p;
}

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

void signal_handler(){
	printf("Ctrl C - cya ;) \n");
	printf("\033[?1049l"); // go back
	exit(0);
}

int main(int argc, char *argv[])
{

	signal (SIGINT, signal_handler);
	printf("\033[?1049h\033[H");
	printf("Software switch implementation.\n");
	// for (i = 5; i; i--) {
	// 	printf("\rgoing back in %d...", i);
	// 	fflush(stdout);
	// 	sleep(1);
	// }


	int option = 0;
	char * p1_interface = 0, * p2_interface = 0;
	pthread_t p1_listener;
	char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */
	p1 = create_port_struct(1);
	p2 = create_port_struct(2);

	while ((option = getopt(argc, argv,"h l 1:2:")) != -1) {
		switch (option) {
			case '1' :
				strcpy(p1->name, optarg);
				break;
			case '2' :
				strcpy(p2->name, optarg);
				break;
			case 'l':
				list_interfaces();
				exit(0);
			case 'h':
			default: print_usage();
				exit(EXIT_FAILURE);
		}
	}


	p1->handle = pcap_create(p1->name, errbuf);
	if ( pcap_activate(p1->handle)){
		printf("Failed to open interface %s\n", pcap_geterr(p1->handle));
		exit(-1);

	} else {
		printf("Handle activated for %s\n", p1->name);
	}

	p2->handle = pcap_create(p2->name, errbuf);
	if ( pcap_activate(p2->handle)){
		printf("Failed to open interface %s\n", pcap_geterr(p2->handle));
		exit(-1);
	} else {
		printf("Handle activated for %s\n", p2->name);
	}

	printf("Deleting mac table..\n");
	clear_mac();
	printf("Creating threads...\n");
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&(p1->thread), 0, port_listener, p1);
	pthread_create(&(p2->thread), 0, port_listener, p2);

	pthread_t config_thread;
	pthread_create(&config_thread, 0, config, 0);
	char c;

	while (1) {
		if(pause_rendering == 1)
			continue;

		// render here
		system("clear");
		print_mac();
		sleep(2);

	}

	pthread_join(config_thread, 0);
	pthread_join(p1->thread, 0);
	pthread_join(p2->thread, 0);

	printf("\033[?1049l"); // go back

	return 0;
}
