#pragma once

// stolen from: https://www.quantstart.com/articles/Random-Number-Generation-via-Linear-Congruential-Generators-in-C/

#include <vector>

class cRandomNumberGenerator {
protected:
    unsigned long init_seed;  // Initial random seed value
    unsigned long cur_seed;   // Current random seed value
    //unsigned long num_draws;  // Dimensionality of the RNG

public:
    cRandomNumberGenerator(unsigned long _init_seed)
        : init_seed(_init_seed), cur_seed(_init_seed) {};
    virtual ~cRandomNumberGenerator() {};

    virtual unsigned long get_random_seed() const { return cur_seed; }
    virtual void set_random_seed(unsigned long _seed) { cur_seed = _seed; }
    virtual void reset_random_seed() { cur_seed = init_seed; }
    //virtual void set_num_draws(unsigned long _num_draws) { num_draws = _num_draws; }

    // Obtain a random integer (needed for creating random uniforms)
    virtual unsigned long get_random_integer() = 0;

    // Fills a vector with uniform random variables on the open interval (0,1)
    virtual void get_uniform_draws(std::vector<double>& draws, unsigned int num_draws) = 0;
};
