#include "header.h"
using namespace std;

class Label {
	protected:
		char* text;
		float x;
		float y;
		Font font;
		int font_size;
		Color font_color;
		int spacing = 1;

	public:
		Label() {}

		Label(const char* info, float x, float y, Font font, int font_size, Color font_color) {
			this->setText(info);
			this->x = x;
			this->y = y;
			this->font = font;
			this->font_size = font_size;
			this->font_color = font_color;
		}
		
		int getLength() {
			return MeasureTextEx(font, text, font_size, spacing).x;
		}

		bool isInside(Vector2 coords) {
			return (x <= coords.x && coords.x <= x + getLength() && y <= coords.y && coords.y <= y + font_size);
		}

		void setText(const char* info) {
			text = new char[strlen(info) + 1];
            strcpy(this->text, info);
		}

		virtual void render() {
			DrawTextEx(font, text, {x, y}, font_size, spacing, font_color);
		}
};

