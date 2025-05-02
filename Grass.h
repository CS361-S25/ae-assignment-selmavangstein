#ifndef GRASS_H
#define GRASS_H

#include "Org.h"

/**
 * Grass species - passive energy-gathering organism.
 * Grows passively over time and reproduces when energy exceeds a threshold.
 */
class Grass : public Organism {
public:
    Grass(emp::Ptr<emp::Random> _random, double _points = 0.0)
        : Organism(_random, _points) {}

    /** 
     * Input: None
     * 
     * Output: None
     * 
     * Grass gains energy each tick (simulates growth). */
    void Process() override {
        AddPoints(200);
    }

    /** 
     * Input: None
     * 
     * Output: 0
     * 
     * Species ID for grass is 0. */
    int GetSpecies() const override {
        return 0;
    }

    /** 
     * Input: None
     * 
     * Output: produced offspring or nullptr
     * Reproduce if enough energy is available. */
    emp::Ptr<Organism> CheckReproduction() override {
        if (points > 400) {
            emp::Ptr<Grass> offspring = new Grass(*this);
            offspring->SetPoints(0);
            points -= 400;
            SetPoints(points);
            return offspring;
        }
        return nullptr;
    }
};

#endif
