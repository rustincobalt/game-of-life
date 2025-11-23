#include "..\include\App.hpp"

App::App(int y, int x, float pixelSize, int targetFPS, string windowName, Color bgColor):
y(y), x(x), pixelSize(pixelSize), targetFPS(targetFPS), windowName(windowName), bgColor(bgColor)
{}

void App::runAppLoop(){
    
    Simulation simulationGoL = Simulation(y, x, pixelSize);
    simulationGoL.InitRandom(200'000); // Temporary, just for testing
    
    string messageText;

    #ifdef TIMEDEBUG
	int iteration = 0;
	float timeSpent = 0;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
	#endif


    while(!WindowShouldClose()){
        #ifdef TIMEDEBUG
		begin = std::chrono::steady_clock::now();
		#endif



        // Event Handling

        // Update State
        simulationGoL.Update();

        messageText = 
			"Alive cells: " + 
			std::to_string(simulationGoL.GetNumberAliveCells());
        
        // Draw
        BeginDrawing();
            ClearBackground(bgColor);
            simulationGoL.DrawAliveCells();
            DrawText(messageText.c_str(),0, 0, 10, {255, 255, 255, 128});
        EndDrawing();


        #ifdef TIMEDEBUG
		end = std::chrono::steady_clock::now();
		timeSpent += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		iteration++;
		#endif
    }

    #ifdef TIMEDEBUG
	std::cout << "Average time of a loop in seconds: " << (timeSpent/iteration)/1000 << "\n"; 
	#endif
}

void App::Launch(){

    InitWindow(x*pixelSize, y*pixelSize, windowName.c_str());
    SetTargetFPS(targetFPS);
    
    this->runAppLoop();

    CloseWindow();

}