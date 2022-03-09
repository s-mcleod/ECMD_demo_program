#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <iostream>
#include <queue>
#include <vector>

#include "Domain.h"
#include "State.h"
#include "Node.h"

using namespace std;


class compare_function
{
     public:
     bool operator()(State& s1, State& s2)
    {
        return (s1.ub < s2.ub);
    }
};

class Priority_Queue {
public:

	priority_queue< State, vector<State>, compare_function > pq;

	Priority_Queue(){
	}

	int empty(){
		return pq.empty();
	}

	int size(){
		return pq.size();
	}

	void push(State state){
		pq.push(state.deep_copy());
	}

	State get_head(){
		State head_state = pq.top();
		pq.pop();
		return head_state;
	}

	~Priority_Queue(){
	}

};

#endif
