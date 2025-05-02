#ifndef GOAT_H
#define GOAT_H

#include "Org.h"

/**
 * Goat species - mobile herbivore.
 * Consumes energy over time, eats grass to survive and reproduce.
 */
class Goat : public Organism {
public:
    Goat(emp::Ptr<emp::Random> _random, double _points = 0.0)
        : Organism(_random, _points) {}

    /** 
     * Input: None
     * 
     * Output: None
     * 
     * Goats lose energy each update cycle. */
    void Process() override {
        AddPoints(-50);  
    }

    /** 
     * Input: None
     * 
     * Output: 1
     * 
     * Species ID for goats is 1. */
    int GetSpecies() const override {
        return 1;
    }

    /** 
     * Input: None
     * 
     * Output: produced offspring or nullptr
     * 
     * Reproduce if enough energy is available. Offspring starts with some energy. */
    emp::Ptr<Organism> CheckReproduction() override {
        if (points > 1000) {
            emp::Ptr<Goat> offspring = new Goat(*this);
            offspring->SetPoints(150);  // Give offspring a small starting reserve
            points -= 1000;
            SetPoints(points);
            ::std::cout << "Reproducing!" << std::endl;
            return offspring;
        }
        return nullptr;
    }
};

#endif
