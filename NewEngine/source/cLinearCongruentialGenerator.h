#pragma once

#include "cRandomNumberGenerator.h"

class cLinearCongruentialGenerator : public cRandomNumberGenerator {
private:
	double max_multiplier;

public:
	cLinearCongruentialGenerator(unsigned long _init_seed = 1);
	virtual ~cLinearCongruentialGenerator() {};

	virtual unsigned long get_random_integer();
	virtual void get_uniform_draw(double& draw);
	virtual void get_uniform_draws(std::vector<double>& draws, unsigned int num_draws);
	virtual void get_uniform_draws2(std::vector<double>& draws, unsigned int num_draws);
};