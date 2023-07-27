#define SUPPORT_CUSTOM_FRAME_CONTROL 1

#include <random>
#include <iostream>

#include <raylib-cpp.hpp>

#include "tfield.hpp"


using QC = qf::CellColor;
using RC = raylib::Color;

int main() {
  
  // * INITIALIZATION PHASE * //
  #pragma region init

  //----------------
  // * window properties

  int screenWidth = 1280;
  int screenHeight = 720;

  raylib::Color textColor(RC::DarkGray());
  raylib::Window w(screenWidth, screenHeight, "quadfall raylib c++: custom update/draw ticking");
  
  #ifndef SUPPORT_CUSTOM_FRAME_CONTROL
  SetTargetFPS(60);
  #endif // !SUPPORT_CUSTOM_FRAME_CONTROL

  //----------------
  // * game properties
  
  // delay time (in update ticks) to update cells
  int timerDelayFrames = 150;
  int timer = timerDelayFrames;
  durationDouble deltaTime;
  // randomizer engine using Mersenne Twister algorithm
  // seeded with system clock's epoch
  auto rand = std::mt19937_64(chrono::system_clock::now().time_since_epoch().count());


  //----------------
  // * init values
  
  qf::Row
    row1 {
      qf::Cell{1,QC::QC_GRAY},qf::Cell{1,QC::QC_RED},qf::Cell{1,QC::QC_GREEN},qf::Cell{0,QC::QC_GREEN},qf::Cell{1,QC::QC_CYAN},
      qf::Cell{1,QC::QC_PURPLE},qf::Cell{1,QC::QC_GRAY},qf::Cell{1,QC::QC_BLUE},qf::Cell{1,QC::QC_ORANGE},qf::Cell{1,QC::QC_YELLOW}
      },
    row2;
  
  #pragma endregion init

  // * MAIN LOOP PHASE * //
  while (!w.ShouldClose()) // Detect window close button or ESC key
  {

    //----------------
    // * UPDATE
    #pragma region update
    
    const char *text;

      // perform update logic

      timer--;
      std::cout << timer << "\n";
      while (timer <= 0) {
        timer += timerDelayFrames;
        int randomCell = rand() % 10;
        auto cell = &row1.content[randomCell];
        std::cout << randomCell << "\n";
        cell->weight = !cell->weight; 
        cell->color = (qf::CellColor)(rand() % 16);
        if (timerDelayFrames == 0) break;
      }

      text = row1.full() ? "row is full" : "row is not full";


    #pragma endregion update

    //----------------
    // * DRAW
    #pragma region draw
      // perform update logic
      BeginDrawing();
      {
        ClearBackground(RC::LightGray());
        textColor.DrawText(text, 200, 150, 20);
        row1.draw(100,200,64);
      }
      EndDrawing();

    #pragma endregion draw

    }
  // * DEINIT PHASE * //
  #pragma region deinit

  #pragma endregion deinit

  return 0;
}