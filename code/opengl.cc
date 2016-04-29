
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

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31

typedef char GLchar;

typedef HGLRC WINAPI wglCreateContextAttribsARB_proc (HDC hdc, HGLRC sharedContext, const int *attribList);
wglCreateContextAttribsARB_proc *wglCreateContextAttribsARB;

typedef GLuint glCreateShader_proc (GLenum shaderType);
glCreateShader_proc *glCreateShader;

typedef void glShaderSource_proc (GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length);
glShaderSource_proc *glShaderSource;

typedef void glCompileShader_proc (GLuint shader);
glCompileShader_proc *glCompileShader;

typedef GLuint glCreateProgram_proc ();
glCreateProgram_proc *glCreateProgram;

typedef void glAttachShader_proc (GLuint program, GLuint shader);
glAttachShader_proc *glAttachShader;

typedef void glLinkProgram_proc (GLuint program);
glLinkProgram_proc *glLinkProgram;

typedef void glUseProgram_proc (GLuint program);
glUseProgram_proc *glUseProgram;

typedef void glGetShaderInfoLog_proc (GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
glGetShaderInfoLog_proc *glGetShaderInfoLog;

gj_Mem_Stack globalMemStack;
GLuint globalShaderProgram;

void loadOpenglExtensions () {
	#define loadExtension(name) name = null; name = (name##_proc*)wglGetProcAddress(#name); assert(name);

	loadExtension(glCreateShader);
	loadExtension(glShaderSource);
	loadExtension(glCompileShader);
	loadExtension(glCreateProgram);
	loadExtension(glAttachShader);
	loadExtension(glLinkProgram);
	loadExtension(glUseProgram);
	loadExtension(glGetShaderInfoLog);
}

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

				loadOpenglExtensions();
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
	globalMemStack = gj_initMemStack(megabytes(10));

	createWin32OpenglContext(windowHandle);

	glEnable(GL_DEPTH_TEST);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	/*const char *vertexSource = "varying vec4 vColor;\
								void main () {\
									gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\
									vColor = gl_Color;\
								}";

	const char *fragmentSource =   "varying vec4 vColor;\
									void main () {\
										gl_FragColor = vColor * vec4(0.5f, 1.0f, 0.5f, 1.0f);\
									}";*/

	gj_Data shaderData = gj_readFile("../code/shader.glsl", &globalMemStack);

	char *vertexSource[2] = { "#define VERTEX_SHADER", shaderData.mem };
	char *fragmentSource[2] = { "#define FRAGMENT_SHADER", shaderData.mem };
	glShaderSource(vertexShader, 2, vertexSource, null); // The length can be null if the string is null terminated
	glShaderSource(fragmentShader, 2, fragmentSource, null);

	char vertexShaderError[1024] = {0};
	char fragmentShaderError[1024] = {0};

	glCompileShader(vertexShader);
	glGetShaderInfoLog(vertexShader, 1024, 0, vertexShaderError);
	glCompileShader(fragmentShader);
	glGetShaderInfoLog(fragmentShader, 1024, 0, fragmentShaderError);

	OutputDebugString(vertexShaderError);
	OutputDebugString(fragmentShaderError);

	globalShaderProgram = glCreateProgram();
	glAttachShader(globalShaderProgram, vertexShader);
	glAttachShader(globalShaderProgram, fragmentShader);
	glLinkProgram(globalShaderProgram);
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

	glUseProgram(globalShaderProgram);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.0f);
	glRotatef((180.0f / PI) * xRotation, 1.0f, 0.0f, 0.0f);
	glRotatef((180.0f / PI) * yRotation, 0.0f, 1.0f, 0.0f);
	glRotatef((180.0f / PI) * zRotation, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f); glColor4f(white); glVertex3f(-0.5f,  0.5f, 0.5f);
	glNormal3f(0.0f, 0.0f, 1.0f); glColor4f(yellow); glVertex3f(0.5f,  0.5f, 0.5f);
	glNormal3f(0.0f, 0.0f, 1.0f); glColor4f(red); glVertex3f(0.5f, -0.5f, 0.5f);
	glNormal3f(0.0f, 0.0f, 1.0f); glColor4f(purple); glVertex3f(-0.5f, -0.5f, 0.5f);

	glNormal3f(0.0f, 0.0f, -1.0f); glColor4f(green); glVertex3f(0.5f,  0.5f,  -0.5f);
	glNormal3f(0.0f, 0.0f, -1.0f); glColor4f(terq); glVertex3f(-0.5f,  0.5f,  -0.5f);
	glNormal3f(0.0f, 0.0f, -1.0f); glColor4f(blue); glVertex3f(-0.5f, -0.5f,  -0.5f);
	glNormal3f(0.0f, 0.0f, -1.0f); glColor4f(black); glVertex3f(0.5f, -0.5f,  -0.5f);

	glNormal3f(1.0f, 0.0f, 0.0f); glColor4f(yellow); glVertex3f(0.5f,  0.5f, 0.5f);
	glNormal3f(1.0f, 0.0f, 0.0f); glColor4f(green); glVertex3f(0.5f,  0.5f,  -0.5f);
	glNormal3f(1.0f, 0.0f, 0.0f); glColor4f(black); glVertex3f(0.5f, -0.5f,  -0.5f);
	glNormal3f(1.0f, 0.0f, 0.0f); glColor4f(red); glVertex3f(0.5f, -0.5f, 0.5f);

	glNormal3f(-1.0f, 0.0f, 0.0f); glColor4f(terq); glVertex3f(-0.5f,  0.5f,  -0.5f);
	glNormal3f(-1.0f, 0.0f, 0.0f); glColor4f(white); glVertex3f(-0.5f,  0.5f, 0.5f);
	glNormal3f(-1.0f, 0.0f, 0.0f); glColor4f(purple); glVertex3f(-0.5f, -0.5f, 0.5f);
	glNormal3f(-1.0f, 0.0f, 0.0f); glColor4f(blue); glVertex3f(-0.5f, -0.5f,  -0.5f);

	glNormal3f(0.0f, 1.0f, 0.0f); glColor4f(terq); glVertex3f(-0.5f,  0.5f,  -0.5f);
	glNormal3f(0.0f, 1.0f, 0.0f); glColor4f(green); glVertex3f( 0.5f,  0.5f,  -0.5f);
	glNormal3f(0.0f, 1.0f, 0.0f); glColor4f(yellow); glVertex3f( 0.5f,  0.5f, 0.5f);
	glNormal3f(0.0f, 1.0f, 0.0f); glColor4f(white); glVertex3f(-0.5f,  0.5f, 0.5f);

	glNormal3f(0.0f, -1.0f, 0.0f); glColor4f(black); glVertex3f( 0.5f, -0.5f,  -0.5f);
	glNormal3f(0.0f, -1.0f, 0.0f); glColor4f(blue); glVertex3f(-0.5f, -0.5f,  -0.5f);
	glNormal3f(0.0f, -1.0f, 0.0f); glColor4f(purple); glVertex3f(-0.5f, -0.5f, 0.5f);
	glNormal3f(0.0f, -1.0f, 0.0f); glColor4f(red); glVertex3f(0.5f, -0.5f, 0.5f);


	/*glColor4f(); glVertex3f();
	glColor4f(); glVertex3f();
	glColor4f(); glVertex3f();
	glColor4f(); glVertex3f();*/

	glEnd();

	glPopMatrix();

	xRotation -= 0.005f;
	yRotation += 0.005f;
	zRotation += 0.005f;

	SwapBuffers(hdc);
}