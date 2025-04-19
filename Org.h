#ifndef ORG_H
#define ORG_H

#include "emp/math/Random.hpp"
#include "emp/tools/string_utils.hpp"

class Organism {
    private:
        double points;
        emp::Ptr<emp::Random> random;
        bool has_moved = false;
        //int species; //we remove instance variable approach to species

    public:
    Organism(emp::Ptr<emp::Random> _random, double _points=0.0) :
        points(_points), random(_random) {;}

    void SetPoints(double _pts) {points = _pts;}
    void AddPoints(double _pts) {points += _pts;} //GO THROUGH AND ADD THIS WHERE POSSIBLE
    int GetPoints() {return points;}

    void SetHasMoved(bool val) { has_moved = val; }
    bool HasMoved() const { return has_moved; }

    //these are just placeholders - will be implemented by each species
    virtual void Process(){
        SetPoints(GetPoints() + 1000);
    };
    virtual emp::Ptr<Organism> CheckReproduction(){
        return nullptr;
    };
    virtual int GetSpecies(){
        return -1;
    };

};
#endif