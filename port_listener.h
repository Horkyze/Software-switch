#include <pthread.h>
#include <stdio.h>
#include <unistd.h>



/* This is our thread function.  It is like main(), but for a thread*/
void *port_listener(void * arg)
{
	Port * p;
	p = (Port *)arg;
	int sent_bytes;
	struct pcap_pkthdr header;
	const u_char * packet;

	sprintf(log_b, "Hello from thread on %s", p->name);
	my_log(log_b);



	while (1) {
		/* Grab a packet */
		packet = pcap_next(p->handle, &header);
		if (packet) {
			pthread_mutex_lock(&mutex);
				sprintf(log_b, "Recieved frame on port %d (%s)", p->id, p->name);
				my_log(log_b);
				p->in++;
				Frame * f = add_frame((u_char*)packet, header.len);

				int forward = apply_rules(f);
				if (forward != 1) {
					// DISCARD Frame
					my_log("Disdarding frame ...");
					continue;
					pthread_mutex_unlock (&mutex);
				}


				char * port = find_port(get_src_mac(f));
				if(strlen(port) == 0){
					my_log("MAC was not found - forward as brodcast");
					insert( get_src_mac(f), p->name);
					sprintf(log_b, "New MAC added: %s\n", get_src_mac(f));
					my_log(log_b);
				}

				// to create malformed packets
				header.len = 30;

				if (p->id == 1) {
					//sent_bytes = pcap_inject(p2->handle, packet, header.len);
					p2->out++;
					sprintf(log_b, "Sent %i bytes via port 2 (%s)", sent_bytes, p2->name);
					my_log(log_b);
				} else {
					//sent_bytes = pcap_inject(p1->handle, packet, header.len);
					p2->out++;
					sprintf(log_b, "Sent %i bytes via port 1 (%s)", sent_bytes, p1->name);
					my_log(log_b);
				}

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
