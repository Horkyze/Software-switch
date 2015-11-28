#define R_ANY 0
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
	Port * port;
	int action; // ALLOW or DENY
	int direction; // IN or OUT
	int src_addr_type; // MAC or IP
	int dst_addr_type; // MAC or IP
	char * src_addr;
	char * dst_addr;
	int proto; // http, icmp, ...
}Rule;

LL * rules_ll = 0;

void mock_rule(){
    Rule * r = (Rule *) malloc(sizeof(Rule));
    r->src_addr = (char *) malloc(20);
    r->dst_addr = (char *) malloc(20);
    r->id = 1;
    r->port = p1;
    r->action = R_DENY;
    r->direction = R_OUT;
    r->dst_addr = "192.168.2.5";
    r->src_addr = "any";
    //strcpy("192.168.1.5", r->dst_addr);
    //strcpy("any", r->src_addr);
    r->proto = 80;
    r->src_addr_type = R_ANY;
    r->dst_addr_type = R_IP;

    if (rules_ll == 0){
        rules_ll = LL_init();
    }
    LL_add_to_front(rules_ll, (void *)r);
}

void create_rule(char * port, char * action, char * direction, char * dst, char * src, char * proto){
    Rule * r = (Rule *) malloc(sizeof(Rule));
    r->src_addr = (char *) malloc(20);
    r->dst_addr = (char *) malloc(20);


    if (strcasecmp(port, "1") == 0) {
        r->port = p1;
    } else if (strcasecmp(port, "2") == 0){
        r->port = p2;
    } else {
        printf("Invalid input at port: '%s' \n", port);
        return;
    }

    if (strcasecmp(action, "allow") == 0) {
        r->action = R_ALLOW;
    } else if (strcasecmp(action, "deny") == 0){
        r->action = R_DENY;
    } else {
        printf("Invalid input at action %s\n", action);
        return;
    }

    if (strcasecmp(direction, "in") == 0) {
        r->direction = R_IN;
    } else if (strcasecmp(direction, "out") == 0){
        r->direction = R_OUT;
    } else {
        printf("Invalid input at direction\n");
        return;
    }

    if (is_valid_mac(src)) {
        strcpy(src, r->src_addr);
        r->src_addr_type = R_MAC;
    } else if (is_valid_ip(src)) {
        strcpy(src, r->src_addr);
        r->src_addr_type = R_IP;
    } else {
        printf("Invalid input at src\n");
        return;
    }

    if (is_valid_mac(dst)) {
        strcpy(dst, r->dst_addr);
        r->dst_addr_type = R_MAC;
    } else if (is_valid_ip(dst)) {
        strcpy(dst, r->dst_addr);
        r->dst_addr_type = R_IP;
    } else {
        printf("Invalid input at dst\n");
        return;
    }

    r->proto = atoi(proto);

    if (rules_ll == 0){
        rules_ll = LL_init();
    }
    LL_add_to_front(rules_ll, (void *)r);

}

void print_rules(){
    if (rules_ll == 0){
        printf("No rules added yet, you can do so by entering config mode, press 'c' and hit Enter\n");
        return;
    }
 
    Item * curr = (Item *) rules_ll->head;
    int i = 0;
    printf("%d Rule(s) are saved\n ID \t PORT \t ALLOW/DENY \t IN/OUT \t SRC_ADDR \t DST_ADDR \t PROTO\n", rules_ll->number_of_items);
    printf("----------------------------------------------------------------------------------------\n");
    while(curr){
        printf("%2i \t %i \t %s \t\t %s \t\t %s \t\t %s \t %i\n", 
            i, 
            ((Rule *)curr->data)->port->id, 
            ( ((Rule *)curr->data)->action == R_ALLOW )? "allow" : "deny",
            ( ((Rule *)curr->data)->direction == R_IN )? "in" : "out",
            ((Rule *)curr->data)->src_addr,
            ((Rule *)curr->data)->dst_addr,
            ((Rule *)curr->data)->proto
        );
        curr = curr->next;
        i++;
    }

}
int apply_rules(Frame * f){
    int forward = 1;


    return forward;
}
