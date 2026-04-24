#include "header.h"
using namespace std;

struct Vec2 {
    float x = 0;
    float y = 0;

    Vec2() {}
    Vec2(float a, float b) {
		this->x = a;
		this->y = b;
	}
};

struct Vec3 {
    float x = 0;
    float y = 0;
    float z = 0;

    Vec3() {}
    Vec3(float a, float b, float c) {
		this->x = a;
		this->y = b;
		this->z = c;
	}
	Vec3(Vec2 v, float c) {
		this->x = v.x;
		this->y = v.y;
		this->z = c;
	}

    float &operator[](int i) { 
		if (i == 0) return this->x;
		else if (i == 1) return this->y;
		else if (i == 2) return this->z;
	}
};

Vec3 operator *(Vec3 a, Vec3 b) { // перемножение векторов
	return Vec3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

float scalar(Vec3 a, Vec3 b) {
    Vec3 tmp = a * b;
    return tmp.x + tmp.y + tmp.z;
}

Vec2 normalize(Vec3 v) { // переход от однородных координат к нормальным
	return Vec2(v[0] / v[2], v[1] / v[2]);
}

struct Mat3 {
    Vec3 row1;
    Vec3 row2;
    Vec3 row3;

    Mat3() {}
    Mat3(Vec3 r1, Vec3 r2, Vec3 r3) {
		this->row1 = r1;
		this->row2 = r2;
		this->row3 = r3;
	}

    Vec3 &operator[](int i) {
		if (i == 0) return row1;
		else if (i == 1) return row2;
		else if (i == 2) return row3;
	}
    Mat3 transpose() {
        Mat3 tmp(*this);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                (*this)[i][j] = tmp[j][i];
            }
        }
		return (*this);
    }
};

Mat3 unit() { // единичная матрица
	Mat3 mtx;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mtx[i][j] = (i == j ? 1 : 0);
		}
	}
	return mtx;
}

Vec3 operator *(Mat3 m, Vec3 v) { // умножение матрицы на вектор
	Vec3 res;
    for (int i = 0; i < 3; i++) {
		res[i] = scalar(m[i], v);
    }
    return res;
}

Mat3 operator *(Mat3 a, Mat3 b) { // перемножение матриц
	Mat3 res;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return res;
}

Mat3 translate(float Tx, float Ty) { // сдвиг
    Mat3 res = unit();
    res[0][2] = Tx;
    res[1][2] = Ty;
    return res;
}

Mat3 scale(float Sx, float Sy) { //  машстабирование
    Mat3 res = unit();
    res[0][0] = Sx;
    res[1][1] = Sy;
    return res;
}

Mat3 rotate(float theta) { // поворот
    Mat3 res = unit();
    res[0][0] = cos(theta);
    res[1][1] = res[0][0];
    res[0][1] = sin(theta);
    res[1][0] = -res[0][1];
    return res;
}

Mat3 mirrorX() { // отражение по горизонтали
    Mat3 res = unit();
    res[1][1] = -1;
    return res;
}

Mat3 mirrorY() { // отражение по вертикали
    Mat3 res = unit();
    res[0][0] = -1;
    return res;
}
