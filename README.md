# Artificial Ecology Simulation

This is a simple artificial ecology simulation built using [Empirical](https://github.com/devosoft/Empirical), a C++ library for evolutionary computation and web-based visualization.

## Overview

The world is a 2D grid populated by two types of organisms: **grass** and **goats**. Each organism has its own behavior and energy system. The simulation runs in steps (updates), during which organisms move, interact, and potentially reproduce.

## Species

### Grass
- Grass is stationary and does not move.
- It gains energy each update.
- When it has enough energy, it reproduces by placing a new grass organism in a nearby empty cell.
- Grass can be eaten by goats, transferring its energy to them.

### Goats
- Goats move randomly to adjacent cells.
- Each update, goats lose a bit of energy to simulate the cost of living.
- If a goat moves onto a grass cell, it eats the grass and absorbs its energy.
- When a goat has accumulated enough energy, it may reproduce, placing a new goat in a neighboring cell.
- If a goat's energy drops to zero, it dies.

## How to Use

When you run the simulation in a browser:
- The world is visualized as a grid.
- Green squares represent grass.
- Brown squares represent goats.
- White squares are empty.

Use the **Start/Stop** button to start/stop the simulation and the **Step** button to advance it one update at a time.


[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/-GCLf3vW)
