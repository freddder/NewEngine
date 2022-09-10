#include "cLinearCongruentialGenerator.h"

// This uses the Park & Miller algorithm found in "Numerical Recipes"

// Define the constants for the Park & Miller algorithm

const unsigned long a = 16807;       // 7^5
const unsigned long m = 2147483647;  // 2^32 - 1 (and thus prime)

// Schrage's algorithm constants

const unsigned long q = 127773;
const unsigned long r = 2836;

// Parameter constructor
cLinearCongruentialGenerator::cLinearCongruentialGenerator( unsigned long _init_seed) :
    cRandomNumberGenerator(_init_seed) {

    if (_init_seed == 0) {
        init_seed = 1;
        cur_seed = 1;
    }

    max_multiplier = 1.0 / (1.0 + (m - 1));
}

// Obtains a random unsigned long integer
unsigned long cLinearCongruentialGenerator::get_random_integer() {
    unsigned long k = 0;
    k = cur_seed / q;
    cur_seed = a * (cur_seed - k * q) - r * k;

    if (cur_seed < 0) {
        cur_seed += m;
    }

    return cur_seed;
}

// Create a vector of uniform draws between (0,1)
void cLinearCongruentialGenerator::get_uniform_draws(std::vector<double>& draws, unsigned int num_draws)
{
    for (unsigned long i = 0; i < num_draws; i++) {
        draws.push_back(get_random_integer() * max_multiplier);
    }
}

// Create a vector of uniform draws between (-1,1)
void cLinearCongruentialGenerator::get_uniform_draws2(std::vector<double>& draws, unsigned int num_draws)
{
    for (unsigned long i = 0; i < num_draws; i++) {
        draws.push_back(get_random_integer() * max_multiplier * 2 - 1);
    }
}
