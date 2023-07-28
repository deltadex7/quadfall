#define SUPPORT_CUSTOM_FRAME_CONTROL 1

#include <random>
#include <chrono>
#include <iostream>

#include <raylib-cpp.hpp>

#include "tfield.hpp"

namespace chrono = std::chrono;

using QC = qf::CellColor;
using RC = raylib::Color;
using intervalTimePoint = chrono::steady_clock::time_point;
using durationDouble = chrono::duration<double>;

const int NANO_IN_SEC = 1000000000;

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
  // * logic update timer

  // clock with fixed interval in nanoseconds
  auto steadyClock = chrono::steady_clock();
  using steadyInterval = chrono::steady_clock::duration;

  const int targetUpdatePerSecond = 600;
  const auto targetUpdateTick = steadyInterval(NANO_IN_SEC/targetUpdatePerSecond);
  // timepoint since last update
  intervalTimePoint lastUpdate;
  // timepoint at current update
  intervalTimePoint currentUpdate;
  // timepoint at next update
  intervalTimePoint nextUpdate;

  //----------------
  // * graphics draw timer

  const int targetDrawPerSecond = 60;
  const auto targetDrawTick = steadyInterval(NANO_IN_SEC/targetDrawPerSecond);
  // timepoint since last draw
  intervalTimePoint lastDraw;
  // timepoint at current draw
  intervalTimePoint currentDraw;
  // timepoint at next draw
  intervalTimePoint nextDraw;

  //----------------
  // * init values
  
  qf::Row
    row1 {
      qf::Cell{1,QC::QC_GRAY},qf::Cell{1,QC::QC_RED},qf::Cell{1,QC::QC_GREEN},qf::Cell{0,QC::QC_GREEN},qf::Cell{1,QC::QC_CYAN},
      qf::Cell{1,QC::QC_PURPLE},qf::Cell{1,QC::QC_GRAY},qf::Cell{1,QC::QC_BLUE},qf::Cell{1,QC::QC_ORANGE},qf::Cell{1,QC::QC_YELLOW}
      },
    row2;
  
  // some place to store the current time
  intervalTimePoint currentTime;
  // some place to store the previous time
  intervalTimePoint lastTime;

  // get the clock simultaneously so that we have no discrepancy
  currentTime = lastUpdate = lastDraw = currentDraw = currentUpdate = nextUpdate = nextDraw = steadyClock.now();
  
  #pragma endregion init

  // * MAIN LOOP PHASE * //
  while (!w.ShouldClose()) // Detect window close button or ESC key
  {
    // TODO implement polling, buffer swap, and wait

    //----------------
    // * UPDATE
    #pragma region update
    
    const char *text;

    if (currentTime >= nextUpdate) {
      // perform update logic

      //----------------
      // * POLLING
      #pragma region polling
      PollInputEvents();
      #pragma endregion polling

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

      // update the update tick

      currentUpdate = steadyClock.now();
      auto updateTimeDif = chrono::duration_cast<durationDouble>(currentUpdate - lastUpdate);

      bool updateDirty = false;
      while (nextUpdate <= currentTime) {
        nextUpdate += targetUpdateTick;

        if (updateDirty)
          std::cout << "Skipping update tick, input may be dropped, logic is delayed.\n";
        
        updateDirty = true;
      }

      std::cout << "Time to update: " << updateTimeDif.count() * 1000 << "\n"
                << "Current UPS   : " << 1 / updateTimeDif.count() << "\n";

      lastUpdate = currentUpdate;
    }

    #pragma endregion update

    //----------------
    // * DRAW
    #pragma region draw
    if (currentTime >= nextDraw) {
      // perform update logic
      BeginDrawing();
      {
        ClearBackground(RC::LightGray());
        textColor.DrawText(text, 200, 150, 20);
        row1.draw(100,200,64);
      }
      EndDrawing();
      SwapScreenBuffer();

      // update the update tick

      currentDraw = steadyClock.now();
      auto drawTimeDif = chrono::duration_cast<durationDouble>(currentDraw - lastDraw);

      nextDraw = currentDraw + targetDrawTick;

      std::cout << "Time to draw: " << drawTimeDif.count() * 1000 << "\n"
                << "Current DPS   : " << 1 / drawTimeDif.count() << "\n";

      lastDraw = currentDraw;
    }
    #pragma endregion draw

    //----------------
    // * WAIT
    #pragma region wait
    auto compareUpdateDraw = nextUpdate.time_since_epoch().count() <=> nextDraw.time_since_epoch().count();
    currentTime = steadyClock.now();
    durationDouble waitTime;

    std::cout << "Current time: " << currentTime.time_since_epoch().count() << "\n"
              << "Next update : " << nextUpdate.time_since_epoch().count() << "\n"
              << "Next draw   : " << nextDraw.time_since_epoch().count() << "\n";

    if (compareUpdateDraw < 0) { // update first
      waitTime = chrono::duration_cast<durationDouble>(nextUpdate-currentTime);
      std::cout << "Next update in: ";
    } else if (compareUpdateDraw > 0) { // draw first
      waitTime = chrono::duration_cast<durationDouble>(nextDraw-currentTime);
      std::cout << "Next draw in: ";
    } else { // update and draw
      waitTime = chrono::duration_cast<durationDouble>(nextUpdate-currentTime);
      std::cout << "Next update+draw in: ";
    }
    auto wtc = waitTime.count();
    if (wtc > 0.0) {
      std::cout << wtc * 1000 << "\n";
      WaitTime(wtc);
    } else {
      std::cout << "0 (rectified from " << wtc * 1000 << ")\n";
    }
    currentTime = steadyClock.now();
    deltaTime = chrono::duration_cast<durationDouble>(currentTime - lastTime);
    std::cout << "Actual wait time:" << deltaTime.count() * 1000 << "\n";
    lastTime = currentTime;

    #pragma endregion wait
  }

  // * DEINIT PHASE * //
  #pragma region deinit

  #pragma endregion deinit

  return 0;
}