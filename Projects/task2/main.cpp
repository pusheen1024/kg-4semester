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

#define x first
#define y second

const int N = 30; // размер клетки

vector<vector<pair<float, float>>> vertices = {
	{{5, 5}, {8, 4}, {12, 4}, {15, 5}, {15, 6}, {12, 8}, {10, 9}, {8, 8}, {5, 6}, {5, 5}}, // голова
	{{6, 6}, {7, 5}, {9, 7}, {11, 7}, {13, 5}, {14, 6}}, // улыбка
	{{8, 4}, {8, 2}, {9, 1}, {10, 2}, {10, 4}, {10, 2}, {11, 1}, {12, 2}, {12, 4}}, // верхние глаза
	{{8.5, 4}, {8.5, 2.5}, {9, 2}, {9.5, 2.5}, {9.5, 4}, {10.5, 4}, {10.5, 2.5}, {11, 2}, {11.5, 2.5}, {11.5, 4}}, // средние глаза
	{{8.75, 4}, {8.75, 3.5}, {9, 3}, {9.25, 3.5}, {9.25, 4}, {10.75, 4}, {10.75, 3.5}, {11, 3}, {11.25, 3.5}, {11.25, 4}}, // нижние глаза 
	{{7, 8}, {7, 12}, {10, 9}, {13, 12}, {13, 8}, {10, 11}, {7, 8}}, // бантик
	{{8, 8}, {8, 9}}, {{12, 8}, {12, 9}}, // шея
	{{7.5, 11.5}, {7, 14}, {4, 14}, {6, 15}, {5, 17}, {7, 16}, {7, 18}, {9, 15},  {9.75, 10.75}}, // левая нога
	{{10.25, 10.75}, {11, 14}, {13, 18}, {13, 16}, {15, 17}, {14, 15}, {16, 14}, {13, 14}, {12.5, 11.5}}, // правая нога
	{{7, 9}, {4, 7}, {2, 8}, {3, 10}, {6, 14}, {7, 14}, {4, 9}}, // левое бедро
};

Vector2 scale(pair<float, float> pt) {
	return {pt.x * N, pt.y * N};
}

int main() {
	float width = N * 20;
	float height = N * 50;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Potapkina-graphics");
    SetTargetFPS(60);
	while (!WindowShouldClose()) {
		BeginDrawing();
		width = GetScreenWidth();
		height = GetScreenHeight();
        ClearBackground(SKYBLUE);
		for (auto v : vertices) {
			for (int i = 1; i < v.size(); i++) {
				DrawLineEx(scale(v[i - 1]), scale(v[i]), 5, BLACK);
			}
		}
		EndDrawing();
	}
    CloseWindow(); 
}
