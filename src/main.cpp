#include "..\include\raylib.h"
#include "..\include\Field.h"

int main(void){

	int rows = 200;
	int cols = 200;
	int cellSizePixels = 4; 

	const int windowWidth = rows * cellSizePixels;
	const int windowHeight = cols * cellSizePixels;

	int FPS = 12;

	InitWindow(windowWidth, windowHeight, "Game of Life");
	SetTargetFPS(FPS); // doesn't gurantee the fps, just sets the max fps

	Color bgColor = {100, 175, 150, 255};

	Field field = Field(rows, cols, cellSizePixels);
	field.initRandom();

	// Simulation loop
	while(!WindowShouldClose()){
		
		
		// 1. Event Handling

		// 2. Update State

		// 3. Drawing
		BeginDrawing(); // creates blank canvas
		ClearBackground(bgColor);
		field.draw();
		EndDrawing(); // ends the canvas drawing
		
	}

	CloseWindow();
	return 0;
}
