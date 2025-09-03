#include "..\include\raylib.h"

int main(void){

	const int windowWidth = 800;
	const int windowHeight = 800;
	int FPS = 12;

	InitWindow(windowWidth, windowHeight, "Game of Life");
	SetTargetFPS(FPS); // doesn't gurantee the fps, just sets the max fps

	Color bgColor = {100, 175, 150, 255};

	// Simulation loop
	while(!WindowShouldClose()){
		
		
		// 1. Event Handling

		// 2. Update State

		// 3. Drawing
		BeginDrawing(); // creates blank canvas
		ClearBackground(bgColor);
		EndDrawing(); // ends the canvas drawing
		
	}

	CloseWindow();
	return 0;
}
