#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

u_char brodcast_mac[] = {'\xff', '\xff', '\xff', '\xff', '\xff', '\xff'};

int forward_frame(Port * p, Frame * f) {
	int sent_bytes = pcap_inject(p->handle, f->data, f->length);
	if (sent_bytes) {
		update_stats(f, p, R_OUT);
		sprintf(log_b, "Sent %i bytes via port 2 (%s)", sent_bytes, p->name);
		//my_log(log_b);
	} else {
		my_log("Failed to send frame :(");
	}
	return sent_bytes;
}

void * port_listener(void * arg) {
	Frame * f;
	Port * p;
	p = (Port *)arg;
	int sent_bytes;
	struct pcap_pkthdr * header;
	const u_char * packet;
	int forward, check;

	sprintf(log_b, "Hello from thread on %s", p->name);
	my_log(log_b);

	while (1) {
		check = pcap_next_ex(p->handle, &header, &packet);
		pthread_mutex_lock(&mutex);
		if (check) {
			sprintf(log_b, "Recieved frame on port %d (%s)", p->id, p->name);
			//my_log(log_b);

			// more like parse frame..
			f = add_frame((u_char*)packet, header->len, p, R_IN);
			print_frame(f);

			// check INBOUNT rules
			forward = apply_rules(f, p, R_IN);

			// if rules are satisfied
			if (forward ) {
				update_stats(f, p, R_IN);

				if(mac_table_search(get_src_mac(f)) == 0){
					sprintf(log_b, "MAC (%s) was not found - forward as brodcast", get_src_mac(f));
					my_log(log_b);
					memcpy((void *) (packet + 6), brodcast_mac, 6);
				}
				mac_table_insert(get_src_mac(f), p);

				// to create malformed packets
				//memcpy((void *) (packet + 6), brodcast_mac, 6);
				if (p->id == 1) {
					forward_frame(p2, f);
				} else {
					forward_frame(p1, f);
				}

			} else {
				my_log("Frame blocked by rule");
			}
		} else {
			sprintf(log_b, "Failed to get frame: pcpa_next_ex returned: %i", check);
			my_log(log_b);
		}
		pthread_mutex_unlock (&mutex);
	}

	pcap_close(p->handle);
	return 0;
}


// 94:44:52:7B:2C:10 p1
// 74:D4:35:14:46:62 p2
// 5C:F8:A1:23:7A:CE p3
// DC:A9:71:3F:A5:57 p4
