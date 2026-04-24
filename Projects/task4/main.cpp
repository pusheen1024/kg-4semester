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

struct ModelF {
	Figure figure;
	Mat3 M;

	ModelF(Figure figure, Mat3 M) {
		this->figure = figure;
		this->M = M;
	}
};

int code(Vec2 P, float minX, float minY, float maxX, float maxY) {
    int code = 0;
    if (P.x < minX) code++;
    else if (P.x > maxX) code += 2;
    if (P.y < minY) code += 4;
    else if (P.y > maxY) code += 8;
    return code;
}

bool coin_sazerlend(Vec2 &A, Vec2 &B, float minX, float minY, float maxX, float maxY) {
    int codeA = code(A, minX, minY, maxX, maxY);
    int codeB = code(B, minX, minY, maxX, maxY);
    while (codeA | codeB) {
        if (codeA & codeB) return false;
        if (codeA == 0) {
            swap(A, B);
            swap(codeA, codeB);
        }
        if (codeA & 1) {
            A.y = A.y + (B.y - A.y) * (minX - A.x) / (B.x - A.x);
            A.x = minX;
        }
		else if (codeA & 2) {
            A.y = A.y + (B.y - A.y) * (maxX - A.x) / (B.x - A.x);
            A.x = maxX;
        }
	   	else if (codeA & 4) {
            A.x = A.x + (B.x - A.x) * (minY - A.y) / (B.y - A.y);
            A.y = minY;
        }
		else {
            A.x = A.x + (B.x - A.x) * (maxY - A.y) / (B.y - A.y);
            A.y = maxY;
        }
        codeA = code(A, minX, minY, maxX, maxY);
    }
    return true;
}

bool to_ignore(string &line) {
	return (line.empty() || line.find_first_not_of(" \t\r\n") == string::npos || line[0] == '#');
}

int main() {
	float width = 480;
	float height = 600;

	vector<ModelF> models;
	float left = 30, top = 30, right = 30, bottom = 30;
	Mat3 T = unit();
	Mat3 initT;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Potapkina-graphics");
	SetExitKey(KEY_NULL);
    SetTargetFPS(60);


	Color color = SKYBLUE;
	Label label = Label("Filename:", left, 0, GetFontDefault(), 25, textbox_color);
	TextBox input_file = TextBox(label.getLength() + left + 10, 0, 
							     width - label.getLength() - left - right - 10,
								 25, font_color, textbox_color);

	auto read_from_file = [&](const char *filename, float rectX, float rectY, float rect_aspect) {
		ifstream in(filename);
		vector<ModelF> models;
		int r, g, b;
		float thickness;
		Mat3 M = unit();
		Mat3 initM;
		vector<Mat3> transforms;
		Figure figure;
		string line;
		while (in) {
			getline(in, line);
			if (to_ignore(line)) continue;
			stringstream sin(line);
			string command;
			sin >> command;
			if (command == "model") {
				float center_x, center_y, Vx, Vy;
				sin >> center_x >> center_y >> Vx >> Vy;
				float S = (Vx / Vy < 1 ? 2 / Vy : 2 / Vx);
				initM = scale(S, S) * translate(-center_x, -center_y);
				figure.paths.clear();	
			}
			else if (command == "frame") {
				float Vx, Vy;
				sin >> Vx >> Vy;
				float figure_aspect = Vx / Vy;
				float S = (figure_aspect < rect_aspect ? rectY / Vy : rectX / Vx);
				Mat3 T1 = translate(-Vx / 2, -Vy / 2);
				Mat3 S1 = scale(S, -S);
				Mat3 T2 = translate(rectX / 2 + left, rectY / 2 + top);
				initT = T2 * (S1 * T1);
				T = initT;
			}
			else if (command == "color") sin >> r >> g >> b;
			else if (command == "thickness") sin >> thickness;
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
			else if (command == "figure") models.pb(ModelF(figure, M * initM));
			else if (command == "translate") {
				float Tx, Ty;
				sin >> Tx >> Ty;
				M = translate(Tx, Ty) * M;
			}
			else if (command == "scale") {
				float S;
				sin >> S;
				M = scale(S, S) * M;
			}
			else if (command == "rotate") {
				float angle;
				sin >> angle;
				M = rotate(-angle / 180 * PI) * M;
			}
			else if (command == "pushTransform") transforms.pb(M);
			else if (command == "popTransform") {
				M = transforms.back();
				transforms.pop_back();
			}
		}
		return models;
	};

	while (!WindowShouldClose()) {
		BeginDrawing();
		width = GetScreenWidth();
		height = GetScreenHeight(); 
	
		float rectY = min((width - left - right) / 2, height - top - bottom);
		float rectX = rectY * 2;
		float minX = left, maxX = left + rectX;
		float minY = top, maxY = top + rectY;
		float rect_aspect = rectX / rectY;

		Wcy = top + rectY;
		float center_x = left + rectX / 2;
		float center_y = top + rectY / 2;

		auto op = [&](Mat3 tr) { // для операций, где надо менять центр
			T = translate(-center_x, -center_y) * T;
			T = tr * T;
			T = translate(center_x, center_y) * T;
		};

		ClearBackground(color);
		DrawRectangleLinesEx({minX, minY, rectX, rectY}, 2, textbox_color);
		
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { 
			input_file.setCursor();
		}
		if (input_file.isActive()) {
			input_file.handleKeyboard();
			if (IsKeyPressed(KEY_ENTER)) {
				auto filename = input_file.getValue();
				models = read_from_file(filename, rectX, rectY, rect_aspect);	
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

			if (IsKeyPressed(KEY_V)) color = DARKPURPLE;
			if (IsKeyPressed(KEY_B)) color = BLACK;
			if (IsKeyPressed(KEY_P)) color = SKYBLUE;
		}
		input_file.render();
		label.render();

		for (int i = 0; i < models.size(); i++) {
			Figure figure = models[i].figure;
			Mat3 TM = T * models[i].M;
			for (auto &lines : figure.paths) {
				Vec2 start = normalize(TM * Vec3(lines.vertices[0], 1));
				for (auto &line : lines.vertices) {
					Vec2 end = normalize(TM * Vec3(line, 1));
					Vec2 tmp = end;
					if (coin_sazerlend(start, end, minX, minY, maxX, maxY)) {
						DrawLineEx({start.x, start.y}, {end.x, end.y}, lines.thickness, lines.color);
					}
					start = tmp;
				}
			}
		}
		EndDrawing();
	}
    CloseWindow();
}
