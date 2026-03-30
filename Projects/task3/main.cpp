#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>

#include "raylib.h"
#include "raymath.h"
#include "label.h"
#include "textbox.h"
#include "matrix.h"

using namespace std;

#define pb push_back

Color font_color = Color({86, 19, 124, 255});
Color textbox_color = Color({234, 216, 243, 255});

struct Path {
	vector<Vec2> vertices;
	Color color;
	float thickness;

	Path(vector<Vec2> vertices, Color color, float thickness) {
		this->vertices = vertices;
		this->color = color;
		this->thickness = thickness;
	}
};

struct Figure {
	vector<Path> paths;
	float Vx, Vy;

	Figure() {}
	Figure(vector<Path> &paths, float Vx, float Vy) {
		this->paths = paths;
		this->Vx = Vx;
		this->Vy = Vy;
	}
};

bool to_ignore(string &line) {
	return (line.empty() || line.find_first_not_of(" \t\r\n") == string::npos || line[0] == '#');
}

Figure read_from_file(const char *filename, bool &fl) {
    ifstream in(filename);
    Figure figure;
    int r, g, b;
    float thickness;
    string line;
    while (in) {
        getline(in, line);
        if (to_ignore(line)) continue;
        stringstream sin(line);
		string command;
		sin >> command;
		if (command == "frame") {
			sin >> figure.Vx >> figure.Vy;
		}
		else if (command == "color") {
			sin >> r >> g >> b;
		}
		else if (command == "thickness") {
			sin >> thickness;
		}
		else if (command == "path") {
			vector<Vec2> vertices;
			int n;
			sin >> n;
			string s;
			while (n != 0) {
				getline(in, s);
				if (to_ignore(s)) continue;
				float x, y;
				stringstream sin1(s);
				sin1 >> x >> y;
				vertices.pb(Vec2(x, y));
				n--;
			}
			figure.paths.pb(Path(vertices, Color({r, g, b, 255}), thickness));
		}
		else if (command == "mirrorX") fl = 1;
	}
	return figure;
}

int main() {
	float width = 480;
	float height = 600;
	float center_x = width / 2;
	float center_y = height / 2;
	float window_aspect = width / height;

	Figure figure;
	Mat3 T = unit();
	Mat3 initT;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Potapkina-graphics");
	SetExitKey(KEY_NULL);
    SetTargetFPS(60);

	Label label = Label("Filename:", 0, 0, GetFontDefault(), 30, textbox_color);
	TextBox input_file = TextBox(label.getLength() + 10, 0, width - label.getLength() - 10, 30, font_color, textbox_color);

	auto op = [&](Mat3 tr) { // для операций, где надо менять центр
		T = translate(-center_x, -center_y) * T;
		T = tr * T;
		T = translate(center_x, center_y) * T;
	};

	while (!WindowShouldClose()) {
		BeginDrawing();
		width = GetScreenWidth();
		height = GetScreenHeight(); 
		center_x = width / 2;
		center_y = height / 2;
		window_aspect = width / height;

        ClearBackground(BLACK);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { 
			input_file.setCursor();
		}
		if (input_file.isActive()) {
			input_file.handleKeyboard();
			if (IsKeyPressed(KEY_ENTER)) {
				auto filename = input_file.getValue();
				bool fl = 0;
				figure = read_from_file(filename, fl);
                float figure_aspect = figure.Vx / figure.Vy;
                float S = (figure_aspect < window_aspect ? width / figure.Vy : width / figure.Vx);
                float Ty = S * figure.Vy;
                if (fl) initT = translate(0, 50) * scale(S, S);
				else initT = translate(0, Ty + 50) * scale(S, -S);
                T = initT;
			}
		}
		else {
			// повороты на 0.01 рад.
			if (IsKeyDown(KEY_E)) op(rotate(0.01f));
			if (IsKeyDown(KEY_Q)) op(rotate(-0.01f));
			// сдвиги на 1 пиксель
			if (IsKeyDown(KEY_W)) T = translate(0, -1) * T;
			if (IsKeyDown(KEY_S)) T = translate(0, 1) * T;
			if (IsKeyDown(KEY_A)) T = translate(-1, 0) * T;
			if (IsKeyDown(KEY_D)) T = translate(1, 0) * T;
			// повороты на 0.05 рад.
			if (IsKeyDown(KEY_R)) op(rotate(0.05f));
			if (IsKeyDown(KEY_Y)) op(rotate(-0.05f));
			// сдвиги на 10 пикселей
			if (IsKeyDown(KEY_T)) T = translate(0, -10) * T;
			if (IsKeyDown(KEY_G)) T = translate(0, 10) * T;
			if (IsKeyDown(KEY_F)) T = translate(-10, 0) * T;
			if (IsKeyDown(KEY_H)) T = translate(10, 0) * T;
			// увеличение и уменьшение в 1.1 раза
			if (IsKeyDown(KEY_Z)) op(scale(1.1, 1.1));
			if (IsKeyDown(KEY_X)) op(scale(1.0 / 1.1, 1.0 / 1.1));
			// отражение по горизонтали и вертикали
			if (IsKeyPressed(KEY_U)) op(mirrorX());
			if (IsKeyPressed(KEY_J)) op(mirrorY());
			// растяжение и сжатие по горизонтали
			if (IsKeyDown(KEY_I)) op(scale(1.1, 1.0));
			if (IsKeyDown(KEY_K)) op(scale(1.0 / 1.1, 1.0));
			// растяжение и сжатие по вертикали
			if (IsKeyDown(KEY_O)) op(scale(1.0, 1.1));
			if (IsKeyDown(KEY_L)) op(scale(1.0, 1.0 / 1.1));
			// к исходному изображению
			if (IsKeyPressed(KEY_ESCAPE)) T = initT;
		}
		input_file.render();
		label.render();
		Mat3 M = T * initT;
		for (auto &lines : figure.paths) {
            Vec2 start = normalize(T * Vec3(lines.vertices[0], 1));
            for (auto &line : lines.vertices) {
                Vec2 end = normalize(T * Vec3(line, 1));
                DrawLineEx({start.x, start.y}, {end.x, end.y}, lines.thickness, lines.color);
                start = end;
            }
        }
		EndDrawing();
	}
    CloseWindow();
}
