#ifndef ORGWORLD_H
#define ORGWORLD_H

#include "emp/Evolve/World.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"

#include "Org.h"

class OrgWorld : public emp::World<Organism> {

    emp::Random &random;
    emp::Ptr<emp::Random> random_ptr;

    public:

    OrgWorld(emp::Random &_random) : emp::World<Organism>(_random), random(_random) {
        random_ptr.New(_random);
    }

    ~OrgWorld() {
    }

    //This function removes the organism at position i and returns it
    emp::Ptr<Organism> ExtractOrganism(int i) {
        emp::Ptr<Organism> org = pop[i];
        pop[i] = nullptr;
        return org;
    }

    int moveOrg(int i) {
        /*
            Moves the organism at position i according to its species.
            grass can't move
            goats eat the grass at the cell they move to (if there is any grass there)
        */
        emp::Ptr<Organism> org = ExtractOrganism(i);
        int species = org->GetSpecies();

        if (species == 1) { //goat
            emp::WorldPosition newPos = GetRandomNeighborPos(i);
            int newPosInt = newPos.GetIndex(); //like 50% sure this works
            if(IsOccupied(newPos)) {
                emp::Ptr<Organism> neighborOrg = pop[newPosInt];
                if (neighborOrg->GetSpecies() == 0) {
                    //if there is grass there, eat the grass
                    int grassPoints = neighborOrg->GetPoints();
                    org->AddPoints(grassPoints);
                    ExtractOrganism(newPosInt);
                    AddOrgAt(org, newPos); 
                    return newPosInt;
                } else {
                //if neighborOrg is a goat, stay in place
                AddOrgAt(org, i);
                return i;
                }
            }
            //if new position is open, move there
            else {
                AddOrgAt(org, newPos);
                return newPosInt;}
        } else {
            //grass doesn't move
            AddOrgAt(org, i);
            return i;
        }
    }

    void checkDeath(int i) {
        /*Check if the organism at position i is dead. If it is, remove it.*/
        emp::Ptr<Organism> org = pop[i];
        if (org->GetPoints() <= 0 && org->GetSpecies() == 1) { //only goats die
            ExtractOrganism(i);
        }
    }

    void Update() {
        // Reset movement flags
        for (size_t i = 0; i < GetSize(); ++i) {
            if (IsOccupied(i)) {
                pop[i]->SetHasMoved(false);
            }
        }
        emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());
        for (int i : schedule) {
            if(!IsOccupied(i)) {continue;}
            emp::Ptr<Organism> org = pop[i];
            //check that we haven't already handled this organism
            if (org->HasMoved()) continue;
            org->Process();
            int new_org_pos = moveOrg(i);
            checkDeath(new_org_pos); //the organism might now be in a different position
            org->SetHasMoved(true);
        }

        schedule = emp::GetPermutation(random, GetSize());
        //I want to abstact this to a function
        //THINK ABOUT: MIGHT LOOP OVER RECENT OFFSPRING TOO - IS THIS AN ISSUE OR NOT? IF YES, USE HAS_REPRODUCED VAR
        for (int i : schedule) {
            if(!IsOccupied(i)) {continue;}
            emp::Ptr<Organism> offspring = pop[i]->CheckReproduction();

            if(offspring) {
                emp::WorldPosition newPos = GetRandomNeighborPos(i);
                if(!IsOccupied(newPos)) {AddOrgAt(offspring, newPos);} //I don't love this way of doing it - parent looses resources but doesn't get a kid
                //maybe I just call it a miscarriage...
                //I actually want a chech like above here, it can be placed on grass and eat it, but not on another goat - cause overcrowding
                else {
                    //if the new position is occupied, check if it is a goat or grass - this can be a helper since I use it twice
                    emp::Ptr<Organism> neighborOrg = pop[newPos.GetIndex()];
                    if (neighborOrg->GetSpecies() == 0) {
                        int grassPoints = neighborOrg->GetPoints();
                        offspring->AddPoints(grassPoints);
                        ExtractOrganism(newPos.GetIndex());
                        AddOrgAt(offspring, newPos);
                    }
                }
            }
        }


    }

};
#endif
