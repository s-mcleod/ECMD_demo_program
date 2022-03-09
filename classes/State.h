#include "Domain.h"

#ifndef STATE_H
#define STATE_H

class State {
public:
  Domain domain;
	double ub;

	State(){
	}

	State(double ub, Domain d){
		this->ub = ub;
		this->domain = d;
	}

	State deep_copy(){
		State state = State();
		state.ub = this->ub;
		state.domain = this->domain;
		return (state);
	}

};

#endif
