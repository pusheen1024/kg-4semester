struct Screen {
    float left = 30, right = 30, top = 30, bottom = 30;
    float minX, maxX;
    float minY, maxY;
    float Wx, Wy, Wcx, Wcy;
    float Rx, Ry, Rcx, Rcy;

    Mat4 T = unit4();
    Mat4 initT = unit4();
    Vec3 S, P, u;

    float dist;  
    float fovy, aspect;
    float fovy_work, aspect_work;
    float near, far;
    float n, f;
    float l, r, t, b;
    enum projType { ORTHO, FRUSTUM, PERSPECTIVE } pType = ORTHO;

    void update() {
        float width = GetScreenWidth();
		float height = GetScreenHeight();
		minX = left;
		minY = right;
		Ry = min((width - left - right) / 2, height - top - bottom);
		Rx = Ry * 2;
		Rcx = minX;
		Rcy = minY + Ry;
		Wcx = width / 2.0;
		Wcy = height / 2.0;		
        if (Ry != 0) aspect_work = Rx / Ry;
    }

    void init() {
        n = near;
        f = far; 
        fovy_work = fovy;
        float Vy = 2 * n * tan(fovy / 2.0);
        float Vx = aspect_work * Vy;
        l = -Vx / 2.0;
        r = Vx / 2.0;
        b = -Vy / 2.0;
        t = Vy / 2.0;
        dist = length(P - S);
        T = lookAt(S, P, u);
    }
};
