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

        // set up the starting grid
        world.Resize(num_h_boxes, num_w_boxes);
        //create fcn seedWorld. More initial cells. Use random to place them
        seedWorld();
        /* world.AddOrgAt(new Grass(&random), 0);
        world.AddOrgAt(new Grass(&random), 2);
        world.AddOrgAt(new Grass(&random), 11);
        world.AddOrgAt(new Grass(&random), 13);
        world.AddOrgAt(new Goat(&random, 1000), 1); //a full adult - tune
        world.AddOrgAt(new Goat(&random, 1000), 19); //a full adult - tune */
        world.SetPopStruct_Grid(num_w_boxes, num_h_boxes);
    }

    void seedWorld() {
        //seed the world with some grass and goats
        for (int i = 0; i < num_w_boxes * num_h_boxes; i++) {
            if (random.GetDouble() < 0.1) {
                world.AddOrgAt(new Grass(&random), i);
            } else if (random.GetDouble() < 0.03) {
                world.AddOrgAt(new Goat(&random, 1000), i);
            }
        }
    }

    void DoFrame() override {
        /*apply the rules of the world*/
        canvas.Clear();

        if (update_count != 0){
            //move the world one step forward
            world.Update();
        }

        //abstract this at some point..?
        //draw the grid
        int org_num = 0;
        for (int x = 0; x < num_w_boxes; x++){
            for (int y = 0; y < num_h_boxes; y++) {
                if (world.IsOccupied(org_num)) {
                    Organism &org = world.GetOrg(org_num);
                    int species = org.GetSpecies();
                    //::std::cout << species << std::endl;
                    if (species == 0) {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "green", "black");
                    } else if (species == 1) {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "brown", "black");
                    } else if (species == -1){ //FOR DEBUGGING, REMOVE LATER
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "red", "black");
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