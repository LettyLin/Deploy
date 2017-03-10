#include "deploy.h"
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

typedef struct bd_suplus {
	int start_node_num;
	int suplus;
}Suplus;

typedef struct Link {
	int bandwidth;
	int unit_price;
	int node1_num;
	int node2_num;
	Suplus suplus[2];
	Link(){}
	Link(int bw, int unit_pri, int num1, int num2) {
		bandwidth = bw;
		unit_price = unit_pri;
		suplus[0].start_node_num = node1_num = num1;
		suplus[1].start_node_num = node2_num = num2;
		suplus[0].suplus = suplus[1].suplus = bw;
	}
	void minus_bw(int start_num, int consume) {
		if (start_num == suplus[0].start_node_num&&suplus[0].suplus >= consume)
			suplus[0].suplus -= consume;
		else if (start_num == suplus[1].start_node_num&&suplus[1].suplus >= consume)
			suplus[1].suplus -= consume;
	}
}Link;

typedef struct Node {
	int id;   //The id of the netword node.
	vector<Link> links;   //The vector of the links connecting the node.
	int consumer_id = -1;    //The consumer id if the network node is connected to a consumer node
	int consumption = 0;   //The consumption of the consumer node.
	bool isSever = false;  //Whether the node is deployed a server.
	Node(){}
	Node(int node_id, Link link) {  //Constructor
		id = node_id;
		links.push_back(link);
	}
	void add_link(Link link) {
		links.push_back(link);
	}
	void set_consumer_id(int id) {
		consumer_id = id;
	}
	void set_server() {
		isSever = true;
	}
	int get_degree() {
		return links.size();
	}
};

int node_sum;
int consumer_sum;
int link_sum;
int server_price;
Link * link_list = NULL;
Node * node_list = NULL;

void add_node(int node_id, Link link, int * node_is_created) {
	if (node_is_created[node_id] == -1) {
		Node node = Node(node_id, link);
		node_is_created[node_id] = 0;
		node_list[node_id] = node;
	}
	else {
		node_list[node_id].add_link(link);
	}
}


void init(char *topo[MAX_EDGE_NUM], int line_num) {
	//Initialize all types of sums.
	char * first_line = topo[0];
	char * p = strtok(first_line, " ");
	node_sum = atoi(p);
	p = strtok(NULL, " ");
	link_sum = atoi(p);
	p = strtok(NULL, " ");
	consumer_sum = atoi(p);
	server_price = atoi(topo[2]);
	int * node_is_created = new int[node_sum];
	for (int i = 0; i < node_sum; i++)
		node_is_created[i] = -1;
	node_list = new Node[node_sum];
	link_list = new Link[link_sum];
	//Initialize the network nodes and links.
	for (int i = 4; i < link_sum+4; i++) {
		char * line = topo[i];
		char * cp = strtok(line, " ");
		int node1_id = atoi(cp);
		cp = strtok(NULL, " ");
		int node2_id = atoi(cp);
		cp = strtok(NULL, " ");
		int bandwidth = atoi(cp);
		cp = strtok(NULL, " ");
		int unit_price = atoi(cp);
		Link link = Link(bandwidth, unit_price, node1_id, node2_id);
		link_list[i-4] = link;
		add_node(node1_id, link, node_is_created);
		add_node(node2_id, link, node_is_created);
	}
	//Initialize the consumer nodes.
	for (int i = link_sum + 5; i < line_num; i++) {
		char * line = topo[i];
		char * pc = strtok(line, " ");
		int consumer_id = atoi(pc);
		pc = strtok(NULL, " ");
		int node_id = atoi(pc);
		pc = strtok(NULL, " ");
		int consumption = atoi(pc);
		node_list[node_id].consumer_id = consumer_id;
		node_list[node_id].consumption = consumption;
	}
}

//You need to complete the function 
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	init(topo, line_num);
	// Output demo
	char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	write_result(topo_file, filename);
	delete node_list;
	delete link_list;
}


