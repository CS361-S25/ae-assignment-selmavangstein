#include "emp/math/Random.hpp"
#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include "OrgWorld.h"
#include "Org.h"
#include "Grass.h"
#include "Goat.h"

emp::web::Document doc{"target"};

class AEAnimator : public emp::web::Animate {

    // arena setup
    const int num_h_boxes = 10;
    const int num_w_boxes = 10;
    const double RECT_SIDE = 20;
    const double width{num_w_boxes * RECT_SIDE};
    const double height{num_h_boxes * RECT_SIDE};
    int update_count = 0;

    //create a canvas
    emp::web::Canvas canvas{width, height, "canvas"};
    //create a place to put information aboutt the simulation
    emp::web::Div info_div; 

    private:
        emp::Random random{5};
        OrgWorld world{random};

    public:

    AEAnimator() {
        // shove canvas into the div
        // along with a control button
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        // Add info panel
        doc << info_div;
        info_div << "<h3>Species Info</h3>";
        info_div << "Here we have a simple world with two species: grass and goats. ";
        info_div << "The grass is green and the goats are brown. The goats eat the grass to collect enough energy to stay alive and replicate. ";
        info_div << "<h4>Grass</h4>";
        info_div << "Grass is green and grows in empty tiles. It cannot move around, but it can reproduce if it has enough points. ";
        info_div << "Specifically, the grass gains 200 points each update, and it needs 400 points to reproduce. ";
        info_div << "<h4>Goats</h4>";
        info_div << "Goats are brown and move randomly around the world. If they move onto a tile with grass, they eat it. ";
        info_div << "They eat grass to gain energy, and they gain the amount of points that the grass had. ";
        info_div << "The goats reproduce if they have more than 1000 points. ";
        info_div << "They then create an offspring that starts with 150 points. ";
        info_div << "The goats also lose 50 points each update, so they need to eat grass to survive. ";
        info_div << "If they reach 0 points, they die. ";
        info_div << "<h4>Simulation</h4>";
        info_div << "The simulation runs at 60 frames per second. You can pause it and step through it one frame at a time. ";
        info_div << "<h4>Controls</h4>";
        info_div << "Click the 'Start' button to start the simulation. Click it again to pause it. ";
        info_div << "Click the 'Step' button to step through the simulation one frame at a time. ";
        

        // set up the starting grid
        world.Resize(num_h_boxes, num_w_boxes);
        //seed the world with some grass and goats to start
        seedWorld();
        world.SetPopStruct_Grid(num_w_boxes, num_h_boxes);
    }

    /**
     * Input: None
     * 
     * Output: None
     * 
     * Purpose: seed the world with some grass and goats to start according to some initial probabilities
     */
    void seedWorld() {
        float prob_grass = 0.1;
        float prob_goat = 0.03;
        for (int i = 0; i < num_w_boxes * num_h_boxes; i++) {
            if (random.GetDouble() < prob_grass) {
                world.AddOrgAt(new Grass(&random), i);
            } else if (random.GetDouble() < prob_goat) {
                world.AddOrgAt(new Goat(&random, 1000), i); //start the goats with 1000 points to give it a chance to survive
            }
        }
    }

    /**
     * Input: None
     * 
     * Output: None
     * 
     * Purpose: apply the rules of the world
     */
    void DoFrame() override {
        canvas.Clear();

        if (update_count != 0){
            //move the world one step forward
            world.Update();
        }

        //draw the grid
        int org_num = 0;
        for (int x = 0; x < num_w_boxes; x++){
            for (int y = 0; y < num_h_boxes; y++) {
                if (world.IsOccupied(org_num)) {
                    Organism &org = world.GetOrg(org_num);
                    int species = org.GetSpecies();
                    if (species == 0) {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "green", "black");
                    } else if (species == 1) {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "brown", "black");
                    }
                    
                } else {
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "white", "black");
                }
                org_num++;
            }
        }
        
        update_count++;
    }

};

AEAnimator animator;

int main() {animator.Step();}