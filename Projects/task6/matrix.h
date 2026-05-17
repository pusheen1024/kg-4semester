#include "header.h"
using namespace std;

// ДВУМЕРНЫЕ ВЕКТОРА

struct Vec2 {
    float x = 0;
    float y = 0;

    Vec2() {}
    Vec2(float a, float b) {
		this->x = a;
		this->y = b;
	}
	float &operator[](int i) { 
		if (i == 0) return this->x;
		else if (i == 1) return this->y;
	}
};

float scalar(Vec2 a, Vec2 b) { // скалярное произведение
    return a[0] * b[0] + a[1] * b[1];
}

// ТРЁХМЕРНЫЕ ВЕКТОРА

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
	Vec3(Vec2 &v, float c) {
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

Vec3 operator *(Vec3 a, float x) { // умножение вектора на число
	return Vec3(a[0] * x, a[1] * x, a[2] * x);
}

Vec3 operator *(Vec3 a, Vec3 b) { // перемножение векторов
	return Vec3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

Vec3 operator +(Vec3 a, Vec3 b) { // сложение векторов
	return Vec3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

Vec3 operator -(Vec3 a, Vec3 b) { // вычитание векторов
	return Vec3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

float scalar(Vec3 a, Vec3 b) { // скалярное произведение
    Vec3 tmp = a * b;
    return tmp.x + tmp.y + tmp.z;
}

float length(Vec3 p) { // длина вектора
	return sqrtf(scalar(p, p));
}

Vec2 normalize(Vec3 v) { // переход от однородных координат к нормальным
	return Vec2(v[0] / v[2], v[1] / v[2]);
}

// ЧЕТЫРЁХМЕРНЫЕ ВЕКТОРА

struct Vec4 {
    float x = 0;
    float y = 0;
    float z = 0;
	float w = 0;

    Vec4() {}
    Vec4(float a, float b, float c, float d) {
		this->x = a;
		this->y = b;
		this->z = c;
		this->w = d;
	}
	Vec4(Vec3 v, float c) {
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = c;
	}
    float &operator[](int i) { 
		if (i == 0) return this->x;
		else if (i == 1) return this->y;
		else if (i == 2) return this->z;
		else if (i == 3) return this->w;
	}
};

Vec3 normalize(Vec4 v) { // переход от однородных координат к нормальным
	float div = (v[3] == 0 ? 1 : v[3]);
	return Vec3(v[0] / div, v[1] / div, v[2] / div);
}

Vec4 operator *(Vec4 a, Vec4 b) { // перемножение векторов
	return Vec4(a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]);
}

float scalar(Vec4 a, Vec4 b) { // скалярное произведение
    Vec4 tmp = a * b;
    return tmp.x + tmp.y + tmp.z + tmp.w;
}

// ЧЕТЫРЁХМЕРНЫЕ МАТРИЦЫ

struct Mat4 {
    Vec4 row1;
    Vec4 row2;
    Vec4 row3;
	Vec4 row4;

    Mat4() {}
    Mat4(Vec4 r1, Vec4 r2, Vec4 r3, Vec4 r4) {
		this->row1 = r1;
		this->row2 = r2;
		this->row3 = r3;
		this->row4 = r4;
	}
    Vec4 &operator[](int i) {
		if (i == 0) return row1;
		else if (i == 1) return row2;
		else if (i == 2) return row3;
		else if (i == 3) return row4;
	}
    Mat4 transpose() {
        Mat4 tmp(*this);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                (*this)[i][j] = tmp[j][i];
            }
        }
		return (*this);
    }
};

// ТРЁХМЕРНЫЕ МАТРИЦЫ

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
	Mat3(Mat4 m) {
        this->row1 = Vec3(m.row1.x, m.row1.y, m.row1.z);
        this->row2 = Vec3(m.row2.x, m.row2.y, m.row2.z);
        this->row3 = Vec3(m.row3.x, m.row3.y, m.row3.z);
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

Mat3 unit3() { // единичная трёхмерная матрица
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

Mat3 operator +(Mat3 a, Mat3 b) { // сложение матриц
	Mat3 res;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			res[i][j] = a[i][j] + b[i][j];
		}
	}
	return res;
}

Mat3 operator *(Mat3 a, float x) { // умножение матрицы на число
	Mat3 res;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			res[i][j] = a[i][j] * x;
		}
	}
	return res;
}

Mat3 translate(float Tx, float Ty) { // сдвиг
    Mat3 res = unit3();
    res[0][2] = Tx;
    res[1][2] = Ty;
    return res;
}

Mat3 scale(float Sx, float Sy) { //  машстабирование
    Mat3 res = unit3();
    res[0][0] = Sx;
    res[1][1] = Sy;
    return res;
}

Mat3 rotate(float theta) { // поворот
    Mat3 res = unit3();
    res[0][0] = cos(theta);
    res[1][1] = res[0][0];
    res[0][1] = sin(theta);
    res[1][0] = -res[0][1];
    return res;
}

Mat3 mirrorX() { // отражение по горизонтали
    Mat3 res = unit3();
    res[1][1] = -1;
    return res;
}

Mat3 mirrorY() { // отражение по вертикали
    Mat3 res = unit3();
    res[0][0] = -1;
    return res;
}

Mat3 crossM(Vec3 p) { // матрица для векторного произведения
	return Mat3(Vec3(0.f, -p.z, p.y),
				Vec3(p.z, 0.f, -p.x),
                Vec3(-p.y, p.x, 0.f));
}

Vec3 cross(Vec3 p, Vec3 q) { // векторное произведение
	return crossM(p) * q;
}

Vec3 norm(Vec3 p) {
	return normalize(Vec4(p, length(p)));
}

// ДВУМЕРНЫЕ МАТРИЦЫ

struct Mat2 {
    Vec2 row1{};
    Vec2 row2{};

    Mat2() {}
	Mat2(Mat3& m) {
        this->row1 = Vec2(m[0][0], m[0][1]);
        this->row2 = Vec2(m[1][0], m[1][1]);
    }
	Vec2 &operator[](int i) { 
		if (i == 0) return this->row1;
		else if (i == 1) return this->row2;
	}
	Mat2 &transpose() {
		Mat2 tmp(*this);
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				(*this)[i][j] = tmp[j][i];
			}
		}
        return *this;
    }
};

Vec2 operator *(Mat2 m, Vec2 v) { // умножение матрицы на вектор
	Vec2 res;
    for (int i = 0; i < 2; i++) {
		res[i] = scalar(m[i], v);
    }
    return res;
}

Mat2 operator *(Mat2 a, Mat2 b) { // перемножение матриц
	Mat2 res;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return res;
}

// ЧЕТЫРЁХМЕРНЫЕ МАТРИЦЫ

Mat4 unit4() { // единичная четырёхмерная матрица
	Mat4 mtx;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mtx[i][j] = (i == j ? 1 : 0);
		}
	}
	return mtx;
}

Mat4 operator *(Mat4 a, Mat4 b) { // перемножение матриц
	Mat4 res;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return res;
}

Vec4 operator *(Mat4 m, Vec4 v) { // умножение матрицы на вектор
	Vec4 res;
    for (int i = 0; i < 4; i++) {
		res[i] = scalar(m[i], v);
    }
    return res;
}

Mat4 operator +(Mat4 a, Mat4 b) { // сложение матриц
	Mat4 res;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res[i][j] = a[i][j] + b[i][j];
		}
	}
	return res;
}

Mat4 translate(float Tx, float Ty, float Tz) { // перенос
    Mat4 res = unit4();
    res[0][3] = Tx;
    res[1][3] = Ty;
    res[2][3] = Tz;
    return res;
}

Mat4 scale(float Sx, float Sy, float Sz) { // масштабирование
    Mat4 res = unit4();
    res[0][0] = Sx;
    res[1][1] = Sy;
    res[2][2] = Sz;
    return res;
}

Mat4 rotate(float theta, Vec3 n) { // поворот
    Vec3 n_norm = norm(n);
    Mat3 n_cross = crossM(n_norm);
    Mat3 E = unit3();
    Mat3 R = E + n_cross * sin(theta) + (n_cross * n_cross) * (1 - cos(theta));
    Mat4 res = unit4();
    res[0][0] = R[0][0];
    res[0][1] = R[0][1];
    res[0][2] = R[0][2];
    res[1][0] = R[1][0];
    res[1][1] = R[1][1];
    res[1][2] = R[1][2];
    res[2][0] = R[2][0];
    res[2][1] = R[2][1];
    res[2][2] = R[2][2];
    return res;
}

Mat4 rotateP(float theta, Vec3 n, Vec3 P) {
    return translate(P.x, P.y, P.z) * (rotate(theta, n) * translate(-P.x, -P.y, -P.z));
}

Mat4 lookAt(Vec3 S, Vec3 P, Vec3 u) {
    Vec3 e3 = norm(S - P);
    Vec3 e1 = norm(cross(u, e3));
    Vec3 e2 = cross(e3, e1);
    Mat4 R = unit4();
    R[0][0] = e1.x;
    R[0][1] = e1.y;
    R[0][2] = e1.z;
    R[1][0] = e2.x;
    R[1][1] = e2.y;
    R[1][2] = e2.z;
    R[2][0] = e3.x;
    R[2][1] = e3.y;
    R[2][2] = e3.z;
    Mat4 T = translate(-S.x, -S.y, -S.z);
    return R * T;
}

// прямоугольная проекция

inline Mat4 ortho(float l, float r, float b, float t, float zn, float zf) {
    return Mat4(Vec4(2.0 / (r - l), 0, 0, -(r + l) / (r - l)),
                Vec4(0, 2.0 / (t - b), 0, -(t + b) / (t - b)),
                Vec4(0, 0, -2.0 / (zf - zn), -(zf + zn) / (zf - zn)),
                Vec4(0, 0, 0, 1));
}

// перспективная проекция

Mat4 frustum(float l, float r, float b, float t, float n, float f) {
    return Mat4(Vec4(2.0 * n / (r - l), 0, (r + l) / (r - l), 0),
                Vec4(0, 2.0 * n / (t - b), (t + b) / (t - b), 0),
                Vec4(0, 0, -(f + n) / (f - n), -2.0 * f * n / (f - n)),
                Vec4(0, 0, -1, 0));
}

Mat4 perspective(float fovy, float aspect, float n, float f) {
    float ctg_fovy_2 = 1.0 / tan(fovy / 2.0);
    return Mat4(Vec4(ctg_fovy_2 / aspect, 0, 0, 0), Vec4(0, ctg_fovy_2, 0, 0),
                Vec4(0, 0, -(f + n) / (f - n), -2 * f * n / (f - n)),
                Vec4(0, 0, -1, 0));
}

Mat3 cadrRL(Vec2 Vc, Vec2 V, Vec2 Wc, Vec2 W) {
	return translate(Wc.x, Wc.y) * (scale(W.x / V.x, -W.y / V.y) * translate(-Vc.x, -Vc.y));
}
