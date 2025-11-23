#include "..\include\App.hpp"


int main(){
    App GameOfLife = App(500, 800, 2, 120);
    GameOfLife.Launch();
    return 0;
}