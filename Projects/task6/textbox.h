#include "header.h"
using namespace std;

class TextBox {
	public:
		char buffer[50] = {0};
		int count = 0;
		bool cursor = 0;
		float x;
   		float y;
		float length;
		int font_size;
		Color font_color;
		Color background_color;
		
	public:
		TextBox() {}

		TextBox(float x, float y, float length, int font_size, Color font_color, Color background_color) {
			this->x = x;
			this->y = y;
			this->length = length;
			this->font_size = font_size;
			this->font_color = font_color;
			this->background_color = background_color;
		}
		
		void addSymbol(int key) {
			if (count >= 50) return;
			if (cursor) count--;
			buffer[count] = char(key);
			count++;
			if (cursor) {
				buffer[count] = '|';
				count++;
			}
			buffer[count] = '\0';
		}

		void removeSymbol() {
			if (! count) return;
			if (! cursor) count--;
			else if (count > 1) {
				count--;
				buffer[count - 1] = '|';
			}
			buffer[count] = '\0';
		}
			
	    bool isInside(Vector2 coords) {
			return (x <= coords.x && coords.x <= x + length &&
				   	y <= coords.y && coords.y <= y + font_size);
		}	
		
		bool isActive() {
			return cursor;
		}

		char* getValue() {
			return this->buffer;
		}

		void setCursor() {
			if (this->isInside(GetMousePosition())) {
				if (! cursor) {
					this->addSymbol(int('|'));
					cursor = 1;
				}
			}
			else {
				if (cursor) {
					cursor = 0;
					this->removeSymbol();
				}
			}
		}

		void handleKeyboard() {
			int key = GetCharPressed();
			if (key) this->addSymbol(key);
			if (IsKeyPressed(KEY_BACKSPACE)) {
				this->removeSymbol();
			}
			if (IsKeyPressed(KEY_ENTER)) {
				cursor = 0;
				this->removeSymbol();
			}
		}

		void render() {
			DrawRectangle(x, y, length, font_size, background_color);
			DrawText(buffer, x + 5, y, font_size, font_color);
		}
};
