#include "header.h"

using namespace std;

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
            A.x = A.x + (B.x - A.x) * (maxY - A.y) / (B.y - A.y)
            A.y = maxY;
        }
        codeA = code(A, minX, minY, maxX, maxY);
    }
    return true;
}
