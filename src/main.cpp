#include "..\lib\raylib\raylib.h"
#include "..\include\Simulation.hpp"
#include "..\include\MidiSoundMapper.hpp"

#define TIMEDEBUG
#define GoL

#ifdef TIMEDEBUG
#include <chrono>
#include <thread>
#include <iostream>

#endif


int main(void){

	// int rows = 4*800;
	// int cols = 4*1280;
	// float cellSizePixels = 0.25; 
	#ifdef GoL
	int y = 500;
	int x = 800;
	float pixelSize = 2;

	
	const float windowHeight =  y * pixelSize;
	const float windowWidth = x * pixelSize;

	int FPS = 120;

	InitWindow(windowWidth, windowHeight, "Game of Life");
	SetTargetFPS(FPS); // doesn't gurantee the fps, just sets the max fps

	Color bgColor = BLACK;

	Simulation simul = Simulation(y, x, pixelSize);

	simul.InitRandom(200'000);

	// Simulation loop
	#ifdef TIMEDEBUG
	int iteration = 0;
	float timeSpent = 0;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
	#endif
	std::string messageText;
	
	while(!WindowShouldClose()){
		#ifdef TIMEDEBUG
		begin = std::chrono::steady_clock::now();
		#endif
		// 1. Event Handling

		// 2. Update State
		simul.Update();
		messageText = 
			"Alive cells: " + 
			std::to_string(simul.GetNumberAliveCells());
		
		// std::this_thread::sleep_for(std::chrono::milliseconds(500));
		// 3. Drawing
		BeginDrawing(); // creates blank canvas
		ClearBackground(bgColor);
		simul.DrawAliveCells();
		DrawText(messageText.c_str(),0, 0, 10, {255, 255, 255, 128});
		EndDrawing(); // ends the canvas drawing
		#ifdef TIMEDEBUG
		end = std::chrono::steady_clock::now();
		timeSpent += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		iteration++;
		#endif
	}

	CloseWindow();

	#ifdef TIMEDEBUG
	std::cout << "Average time of a loop in seconds: " << (timeSpent/iteration)/1000 << "\n"; 
	#endif
	return 0;
	#endif

  	// MidiSoundMapper mapper;

	// mapper.ChoosePortInteractively();
	// mapper.ListenToMidi();
	
	return 0;
}
