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

mt19937 rnd(time(NULL));

const char* LETTERS =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" \
"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~" \
"абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
Font font;

void load_font() {
	int cnt = 0;
	int *codepoints = LoadCodepoints(LETTERS, &cnt);
	font = LoadFontEx("../../Assets/segoeprint_bold.ttf", 100, codepoints, cnt);
}

int main() {
	float width = 600;
	float height = 480;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Potapkina-graphics");
	load_font();
    SetTargetFPS(60);
	while (!WindowShouldClose()) {
		BeginDrawing();
		width = GetScreenWidth();
		height = GetScreenHeight();
        ClearBackground(SKYBLUE);
		DrawLineEx({0, 0}, {width, height}, 6, RED);
		DrawLineEx({90, 50}, {width, 80}, 10, BLUE);
		DrawLineEx({0, height / 3}, {2 * width / 3, 0}, 5, GREEN);
		DrawLineEx({2 * width / 3, 0}, {width, 2 * height / 3}, 5, GREEN);
		DrawLineEx({width, 2 * height / 3}, {width / 3, height}, 5, GREEN);
		DrawLineEx({width / 3, height}, {0, height / 3}, 5, GREEN);
		DrawTextEx(font, "Надпись на форме", {width * 0.1f, height * 0.8f}, 35, 0, BLACK);
		EndDrawing();
	}
    CloseWindow(); 
}
