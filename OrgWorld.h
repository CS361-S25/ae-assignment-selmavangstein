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
     * Input: An integer index corresponding to a cell in the world.
     * 
     * Output: The extracted organism.
     * 
     * Purpose: Extracts the organism at the given index and returns a pointer to it.
     * Leaves the position empty.
     */
    emp::Ptr<Organism> ExtractOrganism(int i) {
        emp::Ptr<Organism> org = pop[i];
        pop[i] = nullptr;
        return org;
    }

    /** 
     * Input: An integer index corresponding to a cell in the world.
     * 
     * Output: New position index of the organism.
     * 
     * Purpose: Move an organism based on its species behavior.
     * Goats move, may eat grass, or stay in place.
     * Grass does not move.
     * Returns the new index of the organism.
     */
    int moveOrg(int index) {
        emp::Ptr<Organism> org = ExtractOrganism(index);
        int species = org->GetSpecies();

        if (species == 1) {
            return MoveGoat(org, index);
        }

        // grass must stay in place
        int newPos = StayPut(org, index);
        return newPos;
    }

    /** 
     * Input: A pointer to a goat organism and its index in the world.
     * 
     * Output: The new position index of the goat.
     * 
     * Purpose: The goat attempts to move to a random neighboring cell. It may:
     * - Eat grass if the cell contains grass.
     * - Stay in place if the cell contains another goat.
     * - Move to the empty cell otherwise.
     */
    int MoveGoat(emp::Ptr<Organism> goat, int index) {
        emp::WorldPosition newPos = GetRandomNeighborPos(index);
        int newPosInt = newPos.GetIndex();

        if (IsOccupied(newPos)) {
            return HandleOccupiedCell(goat, index, newPos, newPosInt);
        }

        return MoveToEmpty(goat, newPos, newPosInt);
    }

    /**
     * Input: A pointer to a goat organism, its index, the new position, and the new position index.
     * 
     * Output: The new position index of the goat.
     * 
     * Purpose: If the occupant is grass (species 0), the goat eats it and moves there.
     * If it's another goat, the goat stays put.
     */
    int HandleOccupiedCell(emp::Ptr<Organism> goat, int index, const emp::WorldPosition& newPos, int newPosInt) {
        emp::Ptr<Organism> neighborOrg = pop[newPosInt];

        if (neighborOrg->GetSpecies() == 0) {
            return EatAndMove(goat, newPos, newPosInt, neighborOrg);
        } else {
            return StayPut(goat, index);
        }
    }

    /**
     * Input: A pointer to a goat organism, the new position, the new position index, and a pointer to the grass organism.
     * 
     * Output: The new position index of the goat.
     * 
     * Purpose: The goat gains points when it eats the grass, which is then removed from the world.
     * The goat finally takes the grass's position.
     */
    int EatAndMove(emp::Ptr<Organism> goat, const emp::WorldPosition& newPos, int newPosInt, emp::Ptr<Organism> grass) {
        goat->AddPoints(grass->GetPoints());
        ExtractOrganism(newPosInt);
        AddOrgAt(goat, newPos);
        return newPosInt;
    }

    /**
     * Input: A pointer to a goat organism, the new position, and the new position index.
     * 
     * Output: The new position index of the goat.
     * 
     * Purpose: Moves the goat to an empty neighboring cell.
     */
    int MoveToEmpty(emp::Ptr<Organism> goat, const emp::WorldPosition& newPos, int newPosInt) {
        AddOrgAt(goat, newPos);
        return newPosInt;
    }

    /**
     * Input: A pointer to an organism and its index.
     * 
     * Output: The index of the organism.
     * 
     * Purpose: Puts organism back in the same place it was
     */
    int StayPut(emp::Ptr<Organism> org, int index) {
        AddOrgAt(org, index);
        return index;
    }


    /**
     * Input: An integer index corresponding to a cell in the world.
     * 
     * Output: None.
     * 
     * Purpose: Check if the organism at position i is dead (has 0 or fewer points). 
     * If it is, remove it.
     */
    void checkDeath(int i) {
        emp::Ptr<Organism> org = pop[i];
        if (org->GetPoints() <= 0 && org->GetSpecies() == 1) { //only goats die
            ExtractOrganism(i);
        }
    }

    /**
     * Input: An integer index corresponding to a cell in the world.
     * 
     * Output: None.
     * 
     * Purpose: Attempt to reproduce the organism at the given index.
     * If successful, place the offspring in a random neighboring cell.
     * If the neighbor is grass, eat it and place offspring there.
     * If the neighbor is another goat, do not reproduce (overcrowding).
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
     * Input: None
     * 
     * Output: None
     * 
     * Purpose: Main update loop for the world.
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
