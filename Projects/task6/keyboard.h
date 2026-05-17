void handle_keyboard(Screen &s) {
    if (IsKeyPressed(KEY_ESCAPE)) s.init();
    if (IsKeyPressed(KEY_ONE)) s.pType = s.ORTHO;
    if (IsKeyPressed(KEY_TWO)) s.pType = s.FRUSTUM;
    if (IsKeyPressed(KEY_THREE)) s.pType = s.PERSPECTIVE;

    if (IsKeyDown(KEY_W)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
            s.T = lookAt(Vec3(0, 0, -0.1), Vec3(0, 0, -0.2), Vec3(0, 1, 0)) * s.T;
		else s.T = lookAt(Vec3(0, 0, -1), Vec3(0, 0, -2), Vec3(0, 1, 0)) * s.T;
    }
    if (IsKeyDown(KEY_S)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
            s.T = lookAt(Vec3(0, 0, 0.1), Vec3(0, 0, 0), Vec3(0, 1, 0)) * s.T;
		else s.T = lookAt(Vec3(0, 0, 1), Vec3(0, 0, 0), Vec3(0, 1, 0)) * s.T;
    }
    if (IsKeyDown(KEY_A)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
            s.T = lookAt(Vec3(-0.1, 0, 0), Vec3(-0.1, 0, -1), Vec3(0, 1, 0)) * s.T;
		else s.T = lookAt(Vec3(-1, 0, 0), Vec3(-1, 0, -1), Vec3(0, 1, 0)) * s.T;
    }
    if (IsKeyDown(KEY_D)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
            s.T = lookAt(Vec3(0.1, 0, 0), Vec3(0.1, 0, -1), Vec3(0, 1, 0)) * s.T;
		else s.T = lookAt(Vec3(1, 0, 0), Vec3(1, 0, -1), Vec3(0, 1, 0)) * s.T;
    }
    if (IsKeyDown(KEY_R)) {
        Vec3 v = Mat3(rotate(0.1, Vec3(0, 0, 1))) * Vec3(0, 1, 0);
        s.T = lookAt(Vec3(0, 0, 0), Vec3(0, 0, -1), v) * s.T;
    }
    if (IsKeyDown(KEY_T)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            Mat4 M = rotateP(0.1, Vec3(1, 0, 0), Vec3(0, 0, -s.dist));
            Vec3 v = Mat3(M) * Vec3(0, 1, 0);
            Vec3 S1 = normalize(M * Vec4(0, 0, 0, 1));
            s.T = lookAt(S1, Vec3(0, 0, -s.dist), v) * s.T;
        }
		else {
            Mat4 M = rotate(0.1, Vec3(1, 0, 0));
            Vec3 v = Mat3(M) * Vec3(0, 1, 0);
            Vec3 P1 = normalize(M * Vec4(0, 0, -1, 1));
			Vec3 z = Vec3(0, 0, 0);
            s.T = lookAt(z, P1, v) * s.T;
        }
    }
    if (IsKeyDown(KEY_I)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) s.t--;
		else s.t++;
	}
    if (IsKeyDown(KEY_J)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) s.l--;
		else s.l++;
	}
    if (IsKeyDown(KEY_Y)) s.T = rotate(0.1, Vec3(0, 0, 1)) * s.T;
    if (IsKeyDown(KEY_G)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
            s.T = rotateP(0.1, Vec3(1, 0, 0), s.P) * s.T;
		else s.T = rotate(0.1, Vec3(1, 0, 0)) * s.T;
    }
    if (IsKeyDown(KEY_F)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
            s.T = rotateP(-0.1, Vec3(0, 1, 0), s.P) * s.T;
		else s.T = rotate(-0.1, Vec3(0, 1, 0)) * s.T;
    }
    if (IsKeyDown(KEY_H)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
            s.T = rotateP(0.1, Vec3(0, 1, 0), s.P) * s.T;
		else s.T = rotate(0.1, Vec3(0, 1, 0)) * s.T;
    }
    if (IsKeyDown(KEY_K)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) s.b--;
        else s.b++;
    }
    if (IsKeyDown(KEY_L)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) s.r--;
        else s.r++;
    }
    if (IsKeyDown(KEY_U)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) s.n = max<float>(0.1, s.n - 0.2);
        else s.n = min<float>(s.f - 0.1, s.n + 0.2);
    }
    if (IsKeyDown(KEY_O)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) s.f -= 0.2;
        else s.f += 0.2;
        s.f = max<float>(s.n + 0.1, s.f);
    }
    if (IsKeyDown(KEY_B)) {
        Vec3 view_direction = norm(s.P - s.S);
        float step = 0.2;
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            if (length(s.P - s.S) - step >= 0.1) {
                s.S = s.S + view_direction * step;
                s.init(); 
            }
        }
		else {
            s.S = s.S - view_direction * step;
            s.init();
        }
    }
    if (IsKeyDown(KEY_Z)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) s.fovy_work = max<float>(0.3, s.fovy_work - 0.1);
        else s.fovy_work = min<float>(3, s.fovy_work + 0.1);
    }
    if (IsKeyDown(KEY_X)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) s.aspect_work -= 0.05;
        else s.aspect_work += 0.05;
        s.aspect_work = max<float>(0.01, s.aspect_work);
    }
} 
