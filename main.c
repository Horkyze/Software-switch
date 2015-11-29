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


#include "stats.h"
#include "port.h"

pthread_mutex_t mutex;
int pause_rendering = 0;

Port *p1, *p2;
char log_b[1024];

// custom includes
#include "functions.h"

#include "eth_parser.h"
#include "mac_table.h"
#include "rules.h"
#include "config.h"
#include "port_listener.h"




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
	my_log("Ctrl C - cya ;) ");
	printf("\033[?1049l"); // go back
	exit(0);
}

int main(int argc, char *argv[])
{

	signal (SIGINT, signal_handler);
	//printf("\033[?1049h\033[H");


	my_log("Software switch starting...");
	my_log("Clear log..");
	FILE * f;
	f = fopen(LOG_FILE, "w");
	fclose(f);


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
		sprintf(log_b, "Handle activated for %s", p1->name);
		my_log(log_b);
	}

	p2->handle = pcap_create(p2->name, errbuf);
	if ( pcap_activate(p2->handle)){
		printf("Failed to open interface %s\n", pcap_geterr(p2->handle));
		exit(-1);
	} else {
		sprintf(log_b, "Handle activated for %s", p2->name);
		my_log(log_b);
	}

	my_log("Deleting mac table..");
	clear_mac();
	
	my_log("Creating threads...");
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&(p1->thread), 0, port_listener, p1);
	pthread_create(&(p2->thread), 0, port_listener, p2);

	pthread_t config_thread;
	pthread_create(&config_thread, 0, config, 0);
	char c;

	while (1) {
		mac_delete_old_entries(5);
		if(pause_rendering == 1)
			continue;

		// render here
		system("clear");
		print_mac();
		print_rules();
		print_stats_header();
		print_stats(p1->in, "1 IN");
		print_stats(p1->out, "1 OUT");
		print_stats(p2->in, "2 IN");
		print_stats(p2->out, "2 OUT");
		sleep(1);

	}

	pthread_join(config_thread, 0);
	pthread_join(p1->thread, 0);
	pthread_join(p2->thread, 0);

	printf("\033[?1049l"); // go back

	return 0;
}
