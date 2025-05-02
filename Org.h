#ifndef ORG_H
#define ORG_H

#include "emp/math/Random.hpp"

/**
 * Base class for all organisms in the artificial ecology.
 * Defines shared interface and behavior.
 */
class Organism {
protected:
    double points;                          // Resource or energy level
    emp::Ptr<emp::Random> random;           // Shared random pointer
    bool has_moved = false;                 // Flag to prevent multiple moves in one update

public:
    Organism(emp::Ptr<emp::Random> _random, double _points = 0.0)
        : points(_points), random(_random) {}

    /** 
     * Input: a double value representing the amount of points
     * 
     * Output: None
     * 
     * Set current point level. */
    void SetPoints(double _pts) { points = _pts; }

    /** 
     * Input: a double value representing the amount of points
     * 
     * Output: None
     * 
     * Add to current energy/point level. */
    void AddPoints(double _pts) { points += _pts; }

    /** 
     * Input: None
     * 
     * Output: a double value representing the current point level
     * 
     * Get current energy/point level. */
    double GetPoints() const { return points; }

    /** 
     * Input: a boolean value
     * 
     * Output: None
     * 
     * Mark if this organism has moved this update cycle. */
    void SetHasMoved(bool val) { has_moved = val; }

    /** 
     * Input: None
     * 
     * Output: a boolean value representing if the organism has moved this update cycle
     * 
     * Check if organism has already moved. */
    bool HasMoved() const { return has_moved; }

    /** 
     * Input: None
     * 
     * Output: None
     * 
     * Handle processing behavior (e.g., metabolism, growth). Override in species. */
    virtual void Process() { AddPoints(1000); }

    /** 
     * Input: None
     * 
     * Output: Nullptr
     * 
     * Check if ready to reproduce. Override in species. */
    virtual emp::Ptr<Organism> CheckReproduction() { return nullptr; }

    /** 
     * Input: None
     * 
     * Output: -1
     * 
     * Return species ID. Override in species. */
    virtual int GetSpecies() const { return -1; }

    virtual ~Organism() = default;
};

#endif
