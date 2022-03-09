#include "State.h"

#ifndef NODE_H
#define NODE_H

class Node {
public:
	State state;
	Node* next_node;
	Node* previous_node;

	Node(){}

};

#endif
