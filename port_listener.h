#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

u_char brodcast_mac[] = {'\xff', '\xff', '\xff', '\xff', '\xff', '\xff'};

int forward_frame(Port * ppp, Frame * f) {
	if (!apply_rules(f, ppp, R_OUT)) {
		sprintf(log_b, "\tPort %i (%s) OUTbound rules blocked this frame", ppp->id, ppp->name);
		my_log(log_b);
		return 0;
	}
	int sent_bytes = pcap_inject(ppp->handle, f->eth_header, f->length);
	if (sent_bytes == f->length) {
		update_stats(f, ppp, R_OUT);
		sprintf(log_b, "\t< -- Port %i (%s) sending %i bytes", ppp->id, ppp->name, sent_bytes);
		my_log(log_b);
	} else {
		my_log("Failed to send frame with specific size :(");
	}
	return sent_bytes;
}

void * port_listener(void * arg) {
	Frame * f;
	Port * p, * tmp;
	p = (Port *)arg;
	Record * r;
	int sent_bytes;
	struct pcap_pkthdr * header;
	const u_char * packet;
	int forward, check, filter_check;

	sprintf(log_b, "Hello from thread on %s", p->name);
	my_log(log_b);

	while (1) {
		check = pcap_next_ex(p->handle, &header, &packet);
		pthread_mutex_lock(&mutex);
		if (check) {

			sprintf(log_b, "-- > Port %i (%s)\tgot %i bytes",
			p->id, p->name, header->len);
			my_log(log_b);

			// more like parse frame..
			f = add_frame((u_char*)packet, header->len, p, R_IN);
			print_frame(f);

			// check INBOUNT rules
			// filter_check = apply_rules(f, p, R_IN);
			// if (filter_check) {
			//
			// }
			sprintf(log_b, "\tUpdating MAC table with mac %s", get_src_mac(f));
			my_log(log_b);
			mac_table_insert(get_src_mac(f), p);




			// if (strcasecmp( get_src_mac(f), "10:DD:B1:EB:36:10" ) == 0
			//  || strcasecmp( get_src_mac(f), "10:9a:dd:42:8c:86" ) == 0
			// 	 || strcasecmp( get_dst_mac(f), "10:9a:dd:42:8c:86" ) == 0
			// 	 || strcasecmp( get_dst_mac(f), "10:9a:dd:42:8c:86" ) == 0 ) {
			// 	forward = 0;
			// }
			if (is_broadcast(f)) {
				my_log("\tBroadcast");
				r = 0;
			} else {
				r = mac_table_search(get_dst_mac(f));
				sprintf(log_b, "\tSearching MAC table for (dst) %s", get_dst_mac(f));
				my_log(log_b);
			}

			if(r){
				tmp = r->p;
				sprintf(log_b, "\tMAC was found on port %i (%s)", tmp->id, tmp->name);
				my_log(log_b);
				if(tmp == p){
					my_log("\tDo not send to the same port!");
				} else {
					forward_frame(r->p, f);
				}

			} else {
				sprintf(log_b, "\tMAC was NOT found - broadcast");
				my_log(log_b);
				if (p == p1) {
					forward_frame(p2, f);
				} else {
					forward_frame(p1, f);
				}
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
