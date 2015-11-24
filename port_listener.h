#include <pthread.h>
#include <stdio.h>
#include <unistd.h>



/* This is our thread function.  It is like main(), but for a thread*/
void *port_listener(void * arg)
{
	Port * p;
	p = (Port *)arg;

	struct pcap_pkthdr header;
	const u_char * packet;

	printf("Hello from thread :) %s\n", p->name);


	while (1) {
		/* Grab a packet */
		packet = pcap_next(p->handle, &header);
		if (packet) {
			pthread_mutex_lock(&mutex);


				Frame * f = add_frame((u_char*)packet, header.len);
				//print_eth(f);


				char * port = find_port(get_src_mac(f));
				if(strlen(port) == 0){
					insert( get_src_mac(f), p->name);
					//printf("New MAC added:%s\n", get_src_mac(f));
				}

				// if (p->id == 1) // is is eth0
				// {
				// 	pcap_inject(p2->handle, packet, 30);
				// }

			pthread_mutex_unlock (&mutex);
		}
	}

	pcap_close(p->handle);
	return 0;
}


// 94:44:52:7B:2C:10 p1
// 74:D4:35:14:46:62 p2
// 5C:F8:A1:23:7A:CE p3
// DC:A9:71:3F:A5:57 p4
