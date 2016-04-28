
#pragma pack(push, 1)
struct Vec2 {
	float x;
	float y;
};

union Vec3 {
	struct {
		float x;
		float y;
		float z;
	};
	Vec2 xy;
};

union Vec4 {
	struct {
		float x;
		float y;
		float z;
		float w;
	};
	Vec2 xy;
	Vec3 xyz;
};

union Mat4 {
	float m[16];
};
#pragma pack(pop)

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

typedef HGLRC WINAPI wglCreateContextAttribsARB_proc (HDC hdc, HGLRC sharedContext, const int *attribList);
wglCreateContextAttribsARB_proc *wglCreateContextAttribsARB;

/*typedef void gluPerspective_proc (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
gluPerspective_proc *gluPerspective;*/

void createWin32OpenglContext (HWND windowHandle) {
	HDC hdc = GetDC(windowHandle);
	HGLRC glContext;

	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	// pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (pixelFormat) {
		// Not sure if you should do the DescribePixelFormat thing here or not
		BOOL result = SetPixelFormat(hdc, pixelFormat, &pfd);
		if (result) {
			HGLRC tempContext = wglCreateContext(hdc);
			wglMakeCurrent(hdc, tempContext);

			char *version = (char*)glGetString(GL_VERSION);
			OutputDebugString(version);
			int major = version[0] - '0';
			int minor = version[2] - '0';

			if (major > 3 || (major == 3 && minor > 1)) {
				int attribs[] = {
					WGL_CONTEXT_MAJOR_VERSION_ARB, major,
					WGL_CONTEXT_MINOR_VERSION_ARB, minor,
					WGL_CONTEXT_FLAGS_ARB, 0, // WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
					WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, // WGL_CONTEXT_CORE_PROFILE_BIT_ARB
					0};

				wglCreateContextAttribsARB = (wglCreateContextAttribsARB_proc*)wglGetProcAddress("wglCreateContextAttribsARB");
				if (wglCreateContextAttribsARB) {
					glContext = wglCreateContextAttribsARB(hdc, 0, attribs);
				}

				wglMakeCurrent(null, null);
				wglDeleteContext(tempContext);

				wglMakeCurrent(hdc, glContext);

				// gluPerspective = (gluPerspective_proc*)wglGetProcAddress("gluPerspective");
			} else {
				assert(false);
			}
		} else {
			assert(false);
		}
	} else {
		assert(false);
	}
}

void initOpengl (HWND windowHandle) {
	createWin32OpenglContext(windowHandle);

	glEnable(GL_DEPTH_TEST);

	// wglSwapIntervalEXT(0);
}

/*
	OpenGL perspective matrix

	f = cotangent(fovy / 2)

	f / aspect, 0, 0                              ,	0                                  ,
	0         ,	f, 0                              ,	0                                  ,
	0         ,	0, (zFar + zNear) / (zNear - zFar), (2 × zFar × zNear) / (zNear - zFar),
	0         ,	0, -1                             ,	0                                  ,
*/
Mat4 createPerspectiveMatrix (float fov, float aspect, float near, float far) {
	float f = 1.0f / tanf(fov / 2.0f);
	Mat4 matrix = {
		f / aspect, 0, 0, 0,
		0, f, 0, 0,
		0, 0, (far + near) / (near - far), -1,
		0, 0, (2.0f * far * near) / (near - far), 0,
	};

	return matrix;
}

void drawOpengl (HWND windowHandle) {
	HDC hdc = GetDC(windowHandle);
	glClearColor(0.4f, 0.6f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float sw = 1 / tanf((70.0f / 2.0f) * (PI / 180.0f));
	float sh = 1 / tanf((70.0f / 2.0f) * (PI / 180.0f));
	/*float sw = 1 / tanf((70.0f / 2.0f) * (1280.0f / 720.0f));
	float sh = 1 / tanf((70.0f / 2.0f) * (720.0f / 1280.0f));*/
	float n = 0.1f;
	float f = 100.0f;
	float aspect = (1280.0f / 720.0f);
	/*float projectionMatrix[] = {
		sw/aspect, 0.0f, 0.0f, 0.0f,
		0.0f, sh, 0.0f, 0.0f,
		0.0f, 0.0f, -(f / (f - n)), -1.0f,
		0.0f, 0.0f, -((f * n) / (f - n)), 0.0f,
	};*/

	Mat4 proj = createPerspectiveMatrix(70, 1280.0f/720.0f, 0.1f, 100.0f);

	glViewport(0, 0, 1280, 720);

	// gluPerspective(70, 1280.0f/720.0f, 0.1f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf((float*)&proj);
	// glOrtho(-1.0f, 1.0f, 1.0f, -1.0f, 0.1f, 100.0f);
	// gluPerspective(70, 1280.0f/720.0f, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*glBegin(GL_QUADS);
	{
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex3f(-0.5f, 0.5f, -2);
		glVertex3f(0.5f, 0.5f, -2);
		glVertex3f(0.5f, -0.5f, -2);
		glVertex3f(-0.5f, -0.5f, -2);
	}
	glEnd();*/

	#define white 1.0f, 1.0f, 1.0f, 1.0f
	#define yellow 1.0f, 1.0f, 0.2f, 1.0f
	#define red 1.0f, 0.2f, 0.2f, 1.0f
	#define blue 0.2f, 0.2f, 1.0f, 1.0f
	#define purple 1.0f, 0.2f, 1.0f, 1.0f
	#define green 0.2f, 1.0f, 0.2f, 1.0f
	#define black 0.0f, 0.0f, 0.0f, 1.0f
	#define terq 0.2f, 1.0f, 1.0f, 1.0f

	static float xRotation = 0.0f;
	static float yRotation = 0.0f;
	static float zRotation = 0.0f;

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.0f);
	glRotatef((180.0f / PI) * xRotation, 1.0f, 0.0f, 0.0f);
	glRotatef((180.0f / PI) * yRotation, 0.0f, 1.0f, 0.0f);
	glRotatef((180.0f / PI) * zRotation, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);

	glColor4f(white); glVertex3f(-0.5f,  0.5f, 0.5f);
	glColor4f(yellow); glVertex3f(0.5f,  0.5f, 0.5f);
	glColor4f(red); glVertex3f(0.5f, -0.5f, 0.5f);
	glColor4f(purple); glVertex3f(-0.5f, -0.5f, 0.5f);

	glColor4f(green); glVertex3f(0.5f,  0.5f,  -0.5f);
	glColor4f(terq); glVertex3f(-0.5f,  0.5f,  -0.5f);
	glColor4f(blue); glVertex3f(-0.5f, -0.5f,  -0.5f);
	glColor4f(black); glVertex3f(0.5f, -0.5f,  -0.5f);

	glColor4f(yellow); glVertex3f(0.5f,  0.5f, 0.5f);
	glColor4f(green); glVertex3f(0.5f,  0.5f,  -0.5f);
	glColor4f(black); glVertex3f(0.5f, -0.5f,  -0.5f);
	glColor4f(red); glVertex3f(0.5f, -0.5f, 0.5f);

	glColor4f(terq); glVertex3f(-0.5f,  0.5f,  -0.5f);
	glColor4f(white); glVertex3f(-0.5f,  0.5f, 0.5f);
	glColor4f(purple); glVertex3f(-0.5f, -0.5f, 0.5f);
	glColor4f(blue); glVertex3f(-0.5f, -0.5f,  -0.5f);

	/*glColor4f(); glVertex3f();
	glColor4f(); glVertex3f();
	glColor4f(); glVertex3f();
	glColor4f(); glVertex3f();*/

	glEnd();

	glPopMatrix();

	xRotation -= 0.005f;
	yRotation += 0.005f;
	zRotation += 0.005f;

	/*v4 white = {1.0f, 1.0f, 1.0f, 1.0f};
	v4 yellow = {1.0f, 1.0f, 0.2f, 1.0f};
	v4 red = {1.0f, 0.2f, 0.2f, 1.0f};
	v4 blue = {0.2f, 0.2f, 1.0f, 1.0f};
	v4 purple = {1.0f, 0.2f, 1.0f, 1.0f};
	v4 green = {0.2f, 1.0f, 0.2f, 1.0f};
	v4 black = {0.0f, 0.0f, 0.0f, 1.0f};
	v4 terq = {0.2f, 1.0f, 1.0f, 1.0f};
	r_vertex Vertices[]=
	{
		// front
		{ {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, white },
		{ { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, yellow },
		{ { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, red },
		
		{ {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, white },
		{ { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, red },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, purple },

		

		// back
		{ { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, green },
		{ {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, terq },
		{ {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, blue },
		
		{ { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, green },
		{ {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, blue },
		{ { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, black },

		// right
		{ { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, yellow },
		{ { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, green },
		{ { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, black },
		
		{ { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, yellow },
		{ { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, black },
		{ { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, red },

		// left
		{ {-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, terq },
		{ {-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, white },
		{ {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, purple },
		
		{ {-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, terq },
		{ {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, purple },
		{ {-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, blue },

		// top
		{ {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, terq },
		{ { 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, green },
		{ { 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, yellow },
		
		{ {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, terq },
		{ { 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, yellow },
		{ {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, white },

		// bottom
		{ { 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, black },
		{ {-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, blue },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, purple },
		
		{ { 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, black },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, purple },
		{ { 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, red },
	};*/

	SwapBuffers(hdc);
}