#ifndef GRASS_H
#define GRASS_H

#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"

#include "Org.h"

class Grass : public Organism{

    public:
        Grass(emp::Ptr<emp::Random> _random, double _points=0.0) : Organism(_random, _points) {;}

        void Process() override {
            AddPoints(200);
        }

        int GetSpecies() override {return 0;}

        emp::Ptr<Organism> CheckReproduction() override{
            int points = GetPoints();
            if (points > 400) {
                emp::Ptr<Grass> offspring = new Grass(*this);
                offspring->SetPoints(0);
                points -= 400;
                SetPoints(points);
                //::std::cout << "Reproducing!" << std::endl;
                return offspring;
            } else {
                return nullptr;
            }
            
        }
};

#endif