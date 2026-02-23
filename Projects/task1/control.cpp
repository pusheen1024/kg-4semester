#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>

#include "raylib.h"
#include "raymath.h"

using namespace std;

int main() {
	float width = 600;
	float height = 480;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Potapkina-graphics");
    SetTargetFPS(60);
	while (!WindowShouldClose()) {
		BeginDrawing();
		width = GetScreenWidth();
		height = GetScreenHeight();
        ClearBackground(SKYBLUE);
		const int dx = 50;
		const int dy = 75;
		DrawLineEx({0, 0}, {width - dx, height - dy}, 6, RED);
		EndDrawing();
	}
    CloseWindow(); 
}
