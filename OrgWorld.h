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

    /**
     * Extracts the organism at the given index and returns a pointer to it.
     * Leaves the position empty.
     */
    emp::Ptr<Organism> ExtractOrganism(int i) {
        emp::Ptr<Organism> org = pop[i];
        pop[i] = nullptr;
        return org;
    }

    /**
     * Move an organism based on its species behavior.
     * Goats move, may eat grass, or stay in place.
     * Grass does not move.
     * Returns the new index of the organism.
     */
    int moveOrg(int index) {
        emp::Ptr<Organism> org = ExtractOrganism(index);
        int species = org->GetSpecies();

        if (species == 1) { //goat
            emp::WorldPosition newPos = GetRandomNeighborPos(index);
            int newPosInt = newPos.GetIndex();

            if(IsOccupied(newPos)) {
                emp::Ptr<Organism> neighborOrg = pop[newPosInt];

                if (neighborOrg->GetSpecies() == 0) {
                    //if there is grass there, eat the grass
                    int grassPoints = neighborOrg->GetPoints();
                    org->AddPoints(grassPoints);
                    ExtractOrganism(newPosInt); // remove the grass
                    AddOrgAt(org, newPos); 
                    return newPosInt;
                } else {
                //if neighborOrg is a goat, stay in place
                AddOrgAt(org, index);
                return index;
                }
            }

            //if new position is open, move there
            AddOrgAt(org, newPos);
            return newPosInt;
        }

        //grass must stay in place
        AddOrgAt(org, index);
        return index;
    }

    /**
     * Check if the organism at position i is dead (has 0 or fewer points). 
     * If it is, remove it.
     */
    void checkDeath(int i) {
        emp::Ptr<Organism> org = pop[i];
        if (org->GetPoints() <= 0 && org->GetSpecies() == 1) { //only goats die
            ExtractOrganism(i);
        }
    }

    /**
     * Attempt to place offspring in a neighboring cell.
     * May eat grass if necessary, but cannot replace goats.
     */
    void TryReproduce(int index) {
        // Try to reproduce
        emp::Ptr<Organism> offspring = pop[index]->CheckReproduction();
        if (!offspring) return;

        // place offspring in a random neighbor
        emp::WorldPosition newPos = GetRandomNeighborPos(index);
        int newIndex = newPos.GetIndex();


        if (!IsOccupied(newPos)) {
            AddOrgAt(offspring, newPos);
        } else {
            emp::Ptr<Organism> neighbor = pop[newIndex];
            if (neighbor->GetSpecies() == 0) {
                // If the neighbor is grass, eat it and place offspring there
                offspring->AddPoints(neighbor->GetPoints());
                ExtractOrganism(newIndex);
                AddOrgAt(offspring, newPos);
            }
            // No reproduction if the neighbor is another goat - overcrowding
        }
    }

    /**
     * Main update loop for the world.
     * Processes movement, death, and reproduction.
     */
    void Update() {
        // Reset movement status
        for (size_t i = 0; i < GetSize(); ++i) {
            if (IsOccupied(i)) {
                pop[i]->SetHasMoved(false);
            }
        }

        // Movement + processing phase
        emp::vector<size_t> move_schedule = emp::GetPermutation(random, GetSize());
        for (int i : move_schedule) {
            if (!IsOccupied(i)) continue;

            emp::Ptr<Organism> org = pop[i];
            if (org->HasMoved()) continue;

            org->Process();
            int new_index = moveOrg(i);
            checkDeath(new_index);
            org->SetHasMoved(true);
        }

        // Reproduction phase
        emp::vector<size_t> reproduction_schedule = emp::GetPermutation(random, GetSize());
        for (int i : reproduction_schedule) {
            if (!IsOccupied(i)) continue;
            TryReproduce(i);
        }
    }

};
#endif
