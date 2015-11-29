#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

u_char brodcast_mac[] = {'\xff', '\xff', '\xff', '\xff', '\xff', '\xff'};

/* This is our thread function.  It is like main(), but for a thread*/
void *port_listener(void * arg)
{
	Frame * f;
	Port * p;
	p = (Port *)arg;
	int sent_bytes;
	struct pcap_pkthdr header;
	const u_char * packet;
	int forward;

	sprintf(log_b, "Hello from thread on %s", p->name);
	my_log(log_b);

	while (1) {
		packet = pcap_next(p->handle, &header);
		if (packet) {
			pthread_mutex_lock(&mutex);
				sprintf(log_b, "Recieved frame on port %d (%s)", p->id, p->name);
				//my_log(log_b);

				f = add_frame((u_char*)packet, header.len, p, R_IN);
				print_frame(f);

				forward = apply_rules(f, p, R_IN);
				if (forward != 1) {
					my_log("Disdarding frame ...");
					pthread_mutex_unlock (&mutex);
					continue;
				}
				update_stats(f, p, R_IN);


				if(mac_table_search(get_src_mac(f)) == 0){
					sprintf(log_b, "MAC (%s) was not found - forward as brodcast", get_src_mac(f));
					my_log(log_b);
					memcpy((void *) (packet + 6), brodcast_mac, 6);
				}

				mac_table_insert(get_src_mac(f), p);

				// to create malformed packets
				//header.len = 30;

				if (p->id == 1) {
					forward = apply_rules(f, p2, R_OUT);
					if (forward == 0) {
						my_log("Disdarding frame ...");
						pthread_mutex_unlock (&mutex);
						continue;
					}
					sent_bytes = pcap_inject(p2->handle, packet, header.len);
					update_stats(f, p2, R_OUT);
					//p2->out->l2_total++;
					sprintf(log_b, "Sent %i bytes via port 2 (%s)", sent_bytes, p2->name);
					//my_log(log_b);



				} else {
					header.len = 40;
					forward = apply_rules(f, p1, R_OUT);
					if (forward == 0) {
						my_log("Disdarding frame ...");
						pthread_mutex_unlock (&mutex);
						continue;
					}

					sent_bytes = pcap_inject(p1->handle, packet, header.len);
					update_stats(f, p1, R_OUT);
					sprintf(log_b, "Sent %i bytes via port 1 (%s)", sent_bytes, p1->name);
					//my_log(log_b);
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
