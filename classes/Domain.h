#include <stdio.h>
#include <iostream>

#ifndef DOMAIN_H
#define DOMAIN_H

class Domain{
public:
  // Two parameters defining the size of the domain.
	double left_domain;
	double right_domain;

  Domain(){}

  // Must specify the size of the domain during initialisation because it depends
  // on the image plane size.
	Domain(double left_domain, double right_domain){
		this->left_domain = left_domain;
		this->right_domain = right_domain;
	}

	// Splitting the domain into 2 sub-domains.
	Domain* split(){

		Domain* partition = new Domain[2];
    double mid_domain = middle_of_domain();

    partition[0] = Domain(left_domain, mid_domain);
    partition[1] = Domain(mid_domain, right_domain);

		return partition;
	}

  // Centre of the domian.
  double middle_of_domain(){
    return(0.5*(left_domain+right_domain));
  }

};

#endif
