#include "..\include\raylib.h"
#include "..\include\Simulation.hpp"

int main(void){

	int rows = 500;
	int cols = 600;
	float cellSizePixels = 2; 

	const float windowWidth = cols * cellSizePixels;
	const float windowHeight =  rows * cellSizePixels;

	int FPS = 12;

	InitWindow(windowWidth, windowHeight, "Game of Life");
	SetTargetFPS(FPS); // doesn't gurantee the fps, just sets the max fps

	Color bgColor = BLACK;

	Simulation simul = Simulation(rows, cols, cellSizePixels);
	simul.InitRandom();

	// Simulation loop
	while(!WindowShouldClose()){
		
		
		// 1. Event Handling

		// 2. Update State
		simul.Update();
		// 3. Drawing
		BeginDrawing(); // creates blank canvas
		ClearBackground(bgColor);
		simul.DrawAliveCells();
		EndDrawing(); // ends the canvas drawing
		
	}

	CloseWindow();
	return 0;
}
