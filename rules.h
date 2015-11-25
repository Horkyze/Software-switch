#define R_ALLOW 1
#define R_DENY 2
#define R_IN 3
#define R_OUT 4
#define R_IP 5
#define R_MAC 6
#define R_HTTP 7
#define R_TCP 7
#define R_ICMP 7



typedef struct Rule {
	int id;
	pcap_t * port;
	int action; // ALLOW or DENY
	int direction; // IN or OUT
	int src_addr_type; // MAC or IP
	int dst_addr_type; // MAC or IP
	char * src_addr;
	char * dst_addr;
	int proto; // http, icmp, ...
}Rule;

LL * rules_ll = 0;

Rule * create_rule(char * action, char * direction, char * dst, char * src, char * proto){
    Rule * r = (Rule *) malloc(sizeof(Rule));
    r->src_addr = (char *) malloc(20);
    r->dst_addr = (char *) malloc(20);

    if (strcasecmp(action, "allow") == 0) {
        r->action = R_ALLOW;
    } else if (strcasecmp(action, "deny")){
        r->action = R_DENY;
    } else {
        printf("Invalid input.. arboting..\n");
        return 0;
    }

    if (strcasecmp(direction, "in") == 0) {
        r->direction = R_IN;
    } else if (strcasecmp(direction, "out")){
        r->direction = R_OUT;
    } else {
        printf("Invalid input.. arboting..\n");
        return 0;
    }

    if (is_valid_mac(src)) {
        strcpy(src, r->src_addr);
        r->src_addr_type = R_MAC;
    } else if (is_valid_ip(src)) {
        strcpy(src, r->src_addr);
        r->src_addr_type = R_IP;
    } else {
        printf("Invalid input.. arboting..\n");
        return 0;
    }

    if (is_valid_mac(dst)) {
        strcpy(dst, r->dst_addr);
        r->dst_addr_type = R_MAC;
    } else if (is_valid_ip(dst)) {
        strcpy(dst, r->dst_addr);
        r->dst_addr_type = R_IP;
    } else {
        printf("Invalid input.. arboting..\n");
        return 0;
    }

    if (strcasecmp(proto, "http") == 0) {
        r->proto = R_HTTP;
    } else {
        printf("Invalid input.. arboting..\n");
        return 0;
    }

    return r;

}

int apply_rules(Frame * f){
    int forward = 0;


    return forward;
}
