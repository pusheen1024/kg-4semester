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
#include "screen.h"
#include "keyboard.h"

using namespace std;

#define pb push_back

Color font_color = Color({86, 19, 124, 255});
Color textbox_color = Color({234, 216, 243, 255});

struct Path {
	vector<Vec3> vertices;
	Color color;
	float thickness;

	Path(vector<Vec3> vertices, Color color, float thickness) {
		this->vertices = vertices;
		this->color = color;
		this->thickness = thickness;
	}
};

struct ModelF {
	vector<Path> paths;
	Mat4 M;

	ModelF(vector<Path> &paths, Mat4 M) {
		this->paths = paths;
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
	Screen s;
	float left = 30, top = 30, right = 30, bottom = 30;
	Mat3 T = unit4();
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

	auto read_from_file = [&](const char *filename) {
		ifstream in(filename);
		vector<ModelF> models;
		vector<Path> figure;
		int r, g, b;
		float thickness;
		Mat4 M = unit4();
		Mat4 initM;
		vector<Mat4> transforms;
		string line;
		while (in) {
			getline(in, line);
			if (to_ignore(line)) continue;
			stringstream sin(line);
			string command;
			sin >> command;
			if (command == "camera") {
            	sin >> s.S.x >> s.S.y >> s.S.z;
            	sin >> s.P.x >> s.P.y >> s.P.z;
            	sin >> s.u.x >> s.u.y >> s.u.z;
			}
			else if (command == "screen") {
				sin >> s.fovy_work >> s.aspect >> s.near >> s.far;
            	s.fovy = s.fovy_work / 180.0 * PI;
			}
			else if (command == "model") {
				float center_x, center_y, center_z, Vx, Vy, Vz;
				sin >> center_x >> center_y >> center_z >> Vx >> Vy >> Vz;
				float S = 2.0 / max({Vx, Vy, Vz});
				initM = scale(S, S, S) * translate(-center_x, -center_y, -center_z);
				figure.clear();	
			}
			else if (command == "color") sin >> r >> g >> b;
			else if (command == "thickness") sin >> thickness;
			else if (command == "path") {
				vector<Vec3> vertices;
				int n;
				sin >> n;
				string s;
				while (n != 0) {
					getline(in, s);
					if (to_ignore(s)) continue;
					float x, y, z;
					stringstream sin1(s);
					sin1 >> x >> y >> z;
					vertices.pb(Vec3(x, y, z));
					n--;
				}
				figure.pb(Path(vertices, Color({r, g, b, 255}), thickness));
			}
			else if (command == "figure") models.pb(ModelF(figure, M * initM));
			else if (command == "translate") {
				float Tx, Ty, Tz;
				sin >> Tx >> Ty >> Tz;
				M = translate(Tx, Ty, Tz) * M;
			}
			else if (command == "scale") {
				float S;
				sin >> S;
				M = scale(S, S, S) * M;
			}
			else if (command == "rotate") {
				float angle;
				float x, y, z;
				sin >> angle >> x >> y >> z;
				M = rotate(angle / 180 * PI, Vec3(x, y, z)) * M;
			}
			else if (command == "pushTransform") transforms.pb(M);
			else if (command == "popTransform") {
				M = transforms.back();
				transforms.pop_back();
			}
		}
		s.update();
		s.init();
		return models;
	};

	while (!WindowShouldClose()) {
		BeginDrawing();
		s.update();
		ClearBackground(color);
        DrawRectangleLinesEx({s.minX, s.minY, s.Rx, s.Ry}, 2, textbox_color);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) input_file.setCursor();
		if (input_file.isActive()) {
			input_file.handleKeyboard();
			if (IsKeyPressed(KEY_ENTER)) {
				auto filename = input_file.getValue();
				models = read_from_file(filename);
			}
		}
		else {
			if (IsKeyPressed(KEY_V)) color = DARKPURPLE;
			else if (IsKeyPressed(KEY_Q)) color = BLACK;
			else if (IsKeyPressed(KEY_P)) color = SKYBLUE;
			else handle_keyboard(s);
		}
		input_file.render();
		label.render();
		Mat4 proj;
        if (s.pType == s.ORTHO) proj = ortho(s.l, s.r, s.b, s.t, -s.n, -s.f);
		else if (s.pType == s.FRUSTUM) proj = frustum(s.l, s.r, s.b, s.t, s.n, s.f);
        else if (s.pType == s.PERSPECTIVE) proj = perspective(s.fovy_work, s.aspect_work, s.n, s.f);
        Mat3 cdr = cadrRL(Vec2(-1, 1), Vec2(2, 2), Vec2(s.minX, s.minY), Vec2(s.Rx, s.Ry));
        Mat4 C = proj * s.T;

        for (int i = 0; i < models.size(); i++) {
            Mat4 TM = C * models[i].M;
            for (auto &lines : models[i].paths) {
                Vec3 start3D = normalize(TM * Vec4(lines.vertices[0], 1));
                Vec2 start = normalize(cdr * Vec3(start3D[0], start3D[1], 1));
                for (const auto &line : lines.vertices) {
                    Vec3 end3D = normalize(TM * Vec4(line, 1));
                    Vec2 end = normalize(cdr * Vec3(end3D[0], end3D[1], 1));
                    Vec2 tmp = end;
                    if (coin_sazerlend(start, end, s.minX, s.minY, s.minX + s.Rx, s.minY + s.Ry)) {
                        DrawLineEx({start.x, start.y}, {end.x, end.y},
                                   lines.thickness, lines.color);
					}
                    start = tmp;
                }
            }
		}
		EndDrawing();
	}
    CloseWindow();
}
