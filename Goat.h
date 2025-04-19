#ifndef GOAT_H
#define GOAT_H

#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"

#include "Org.h"

class Goat : public Organism{

    public:
        Goat(emp::Ptr<emp::Random> _random, double _points=0.0) : Organism(_random, _points) {;}

        void Process() override {
            AddPoints(-50); //looses some energy each step - needs to eat to survive
        }

        int GetSpecies() override {return 1;}

        emp::Ptr<Organism> CheckReproduction() override{
            int points = GetPoints();
            if (points > 1000) {
                emp::Ptr<Goat> offspring = new Goat(*this);
                offspring->SetPoints(150); //needs some time to find food before it is killed off
                points -= 1000;
                SetPoints(points);
                ::std::cout << "Reproducing!" << std::endl;
                return offspring;
            } else {
                return nullptr;
            }
            
        }
};

#endif