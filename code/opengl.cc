
/*#pragma pack(push, 1)
union Vec2 {
	struct {
		float x;
		float y;
	};
	struct {
		float u;
		float v;
	};
	struct {
		float s;
		float t;
	};
};

Vec2 vec2 (float x, float y) {
	Vec2 result = {x, y};
	return result;
}

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
#pragma pack(pop)*/

Vec3 operator+ (Vec3 vec1, Vec3 vec2) {
	Vec3 result = {vec1.x+vec2.x, vec1.y+vec2.y, vec1.z+vec2.z};
	return result;
}

Vec3 operator* (Vec3 vec, float num) {
	Vec3 result = {vec.x*num, vec.y*num, vec.z*num};
	return result;
}

struct Depth_Cube_Frame_Buffer {
	GLuint id;
	GLuint depthTexture;
	GLuint cubeTexture;
};

enum Draw_Mode {
	DRAW_MODE_FINAL,
	DRAW_MODE_DEPTH,
	DRAW_MODE_DIFFUSE,
};

struct Camera {
	Vec3 position;
	Vec3 rotation;
	Mat4 perspective;
	Vec3 direction;
	Vec3 up;
};

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31

#define GL_FRAMEBUFFER 0x8D40
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5

#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT 0x8CD9
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT    0x8CDD

#define GL_RENDERBUFFER                   0x8D41
#define GL_DEPTH_ATTACHMENT               0x8D00

typedef char GLchar;

typedef HGLRC WINAPI wglCreateContextAttribsARB_proc (HDC hdc, HGLRC sharedContext, const int *attribList);
wglCreateContextAttribsARB_proc *wglCreateContextAttribsARB;

typedef GLuint __stdcall glCreateShader_proc (GLenum shaderType);
glCreateShader_proc *glCreateShader;

typedef void __stdcall glShaderSource_proc (GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length);
glShaderSource_proc *glShaderSource;

typedef void __stdcall glCompileShader_proc (GLuint shader);
glCompileShader_proc *glCompileShader;

typedef GLuint __stdcall glCreateProgram_proc ();
glCreateProgram_proc *glCreateProgram;

typedef void __stdcall glAttachShader_proc (GLuint program, GLuint shader);
glAttachShader_proc *glAttachShader;

typedef void __stdcall glLinkProgram_proc (GLuint program);
glLinkProgram_proc *glLinkProgram;

typedef void __stdcall glUseProgram_proc (GLuint program);
glUseProgram_proc *glUseProgram;

typedef void __stdcall glGetShaderInfoLog_proc (GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
glGetShaderInfoLog_proc *glGetShaderInfoLog;

typedef GLint __stdcall glGetUniformLocation_proc (GLuint program, const GLchar *name);
glGetUniformLocation_proc *glGetUniformLocation;

typedef void __stdcall glUniformMatrix4fv_proc (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
glUniformMatrix4fv_proc *glUniformMatrix4fv;

typedef void __stdcall glUniform1f_proc (GLint location, GLfloat v0);
glUniform1f_proc *glUniform1f;

typedef void __stdcall glUniform4f_proc (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
glUniform4f_proc *glUniform4f;

typedef void __stdcall glUniform1i_proc (GLint location, GLint v0);
glUniform1i_proc *glUniform1i;

typedef void __stdcall glGenFramebuffers_proc (GLsizei n, GLuint *ids);
glGenFramebuffers_proc *glGenFramebuffers;

typedef void __stdcall glBindFramebuffer_proc (GLenum target, GLuint framebuffer);
glBindFramebuffer_proc *glBindFramebuffer;

typedef void __stdcall glFramebufferTexture_proc (GLenum target, GLenum attachment, GLuint texture, GLint level);
glFramebufferTexture_proc *glFramebufferTexture;

typedef void __stdcall glFramebufferTexture2D_proc (GLenum target, GLenum attachment,GLenum textarget, GLuint texture, GLint level);
glFramebufferTexture2D_proc *glFramebufferTexture2D;

typedef void __stdcall glDrawBuffers_proc (GLsizei n, const GLenum *bufs);
glDrawBuffers_proc *glDrawBuffers;

typedef GLenum __stdcall glCheckFramebufferStatus_proc (GLenum target);
glCheckFramebufferStatus_proc *glCheckFramebufferStatus;

typedef void __stdcall glVertexAttribPointer_proc (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
glVertexAttribPointer_proc *glVertexAttribPointer;

typedef void __stdcall glEnableVertexAttribArray_proc (GLuint index);
glEnableVertexAttribArray_proc *glEnableVertexAttribArray;
 
typedef void __stdcall glDisableVertexAttribArray_proc (GLuint index);
glDisableVertexAttribArray_proc *glDisableVertexAttribArray;

typedef void (APIENTRY *DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void *userParam);
typedef void __stdcall glDebugMessageCallback_proc (DEBUGPROC callback, void * userParam);
glDebugMessageCallback_proc *glDebugMessageCallback;

typedef void __stdcall glDebugMessageControl_proc (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
glDebugMessageControl_proc *glDebugMessageControl;

typedef void __stdcall glActiveTexture_proc (GLenum texture);
glActiveTexture_proc *glActiveTexture;

typedef void glGenBuffers_proc (GLsizei n, GLuint * buffers);
glGenBuffers_proc *glGenBuffers;

typedef void glBufferData_proc (GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
glBufferData_proc *glBufferData;

typedef void glBindBuffer_proc (GLenum target, GLuint buffer);
glBindBuffer_proc *glBindBuffer;


gjMemStack globalMemStack;

Vec2 defaultViewport = {1280, 720};
float playerFov = 70;

GLuint globalShaderProgram;
GLuint globalWireShader;
GLuint depthShader;
GLuint simpleShader;
GLuint cubeMapShader;
GLuint pbr_shader;

#include "mesh.cc"

// Model *globalModel;
// Model *globalShipModel;
// Model *globalCylinderModel;
// Model *globalTestModel;
// Model *globalBigShipModel;
Model models[6];
int selectedModel = 0;

Vec2 globalMousePos;
float globalScroll;

float globalZoom = 3.0f;
bool globalNormalVisualization = false;

Mat4 globalProjMatrix;

static float xRotation = 0.0f;
static float yRotation = 0.0f;
static float zRotation = 0.0f;

Vec3 lightPosition = {-1.0f, 2.0f, 2.0f};

static Depth_Cube_Frame_Buffer shadowFrameBuffer;

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
	loadExtension(glGetUniformLocation);
	loadExtension(glUniformMatrix4fv);
	loadExtension(glUniform1f);
	loadExtension(glUniform4f);
	loadExtension(glUniform1i);

	loadExtension(glGenFramebuffers);
	loadExtension(glBindFramebuffer);
	loadExtension(glFramebufferTexture);
	loadExtension(glFramebufferTexture2D);
	loadExtension(glDrawBuffers);
	loadExtension(glCheckFramebufferStatus);

	loadExtension(glVertexAttribPointer);
	loadExtension(glEnableVertexAttribArray);
	loadExtension(glDisableVertexAttribArray);

	loadExtension(glDebugMessageCallback);
	loadExtension(glDebugMessageControl);

	loadExtension(glActiveTexture);

	loadExtension(glGenBuffers);
	loadExtension(glBufferData);
	loadExtension(glBindBuffer);
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
			int requestedMajor = 3;
			int requestedMinor = 2;

			#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
			if (major > requestedMajor || (major == requestedMajor && minor >= requestedMinor)) {
				int attribs[] = {
					WGL_CONTEXT_MAJOR_VERSION_ARB, major,
					WGL_CONTEXT_MINOR_VERSION_ARB, minor,
					WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB, // WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
					WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, // WGL_CONTEXT_CORE_PROFILE_BIT_ARB
					0};
				// GL_CONTEXT_FLAG_DEBUG_BIT 

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

GLuint createShader (char *file) {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	gjData shaderData = gjReadFile(file, &globalMemStack);

	char *vertexSource[2];
	char *fragmentSource[2];
	// if (!strcmp(file, "../code/shader.glsl") || !strcmp(file, "../code/simple.glsl")) {
		vertexSource[0] = "#version 330\n#define VERTEX_SHADER";
		vertexSource[1] = shaderData.mem;
		fragmentSource[0] = "#version 330\n#define FRAGMENT_SHADER";
		fragmentSource[1] = shaderData.mem;
	// } else {
	// 	vertexSource[0] = "#define VERTEX_SHADER";
	// 	vertexSource[1] = shaderData.mem;
	// 	fragmentSource[0] = "#define FRAGMENT_SHADER";
	// 	fragmentSource[1] = shaderData.mem;
	// }
	
	glShaderSource(vertexShader, 2, vertexSource, null); // The length can be null if the string is null terminated
	glShaderSource(fragmentShader, 2, fragmentSource, null);

	char vertexShaderError[1024] = {0};
	char fragmentShaderError[1024] = {0};

	glCompileShader(vertexShader);
	glGetShaderInfoLog(vertexShader, 1024, 0, vertexShaderError);
	glCompileShader(fragmentShader);
	glGetShaderInfoLog(fragmentShader, 1024, 0, fragmentShaderError);

	OutputDebugString(file);
	OutputDebugString(" \nvertex shader output: \n");
	OutputDebugString(vertexShaderError);
	OutputDebugString(" fragment shader output: \n");
	OutputDebugString(fragmentShaderError);

	// OutputDebugString(shaderData.mem);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	return program;
}

char *getFrameBufferError (GLuint id) {
	GLenum completeness = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (completeness != GL_FRAMEBUFFER_COMPLETE) {
		// char *error = glErrorStr(glGetError());
		switch (completeness) {
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: {
				return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n";
			} break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: {
				return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n";
			} break;
			case GL_FRAMEBUFFER_UNSUPPORTED: {
				return "L_FRAMEBUFFER_UNSUPPORTED\n";
			} break;
		}
	}

	return null;
}

Depth_Cube_Frame_Buffer createDepthCubeFrameBuffer () {
	Depth_Cube_Frame_Buffer frameBuffer = {};
	glGenFramebuffers(1, &frameBuffer.id);

	float windowWidth = 1024;
	float windowHeight = 1024;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &frameBuffer.depthTexture);	
	glBindTexture(GL_TEXTURE_2D, frameBuffer.depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &frameBuffer.cubeTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, frameBuffer.cubeTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_R32F, windowWidth, windowHeight, 0, GL_RED, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_R32F, windowWidth, windowHeight, 0, GL_RED, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_R32F, windowWidth, windowHeight, 0, GL_RED, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_R32F, windowWidth, windowHeight, 0, GL_RED, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_R32F, windowWidth, windowHeight, 0, GL_RED, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_R32F, windowWidth, windowHeight, 0, GL_RED, GL_FLOAT, 0);

 	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, frameBuffer.depthTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	// glDrawBuffers(1, drawBuffers);

	char *error = getFrameBufferError(frameBuffer.id);
	if (error) {
		OutputDebugString(error);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return frameBuffer;
}

void APIENTRY openglDebugCallback (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void *userParam) {
	char str[256];
	_snprintf(str, 256, "Opengl debug message: %s \n", message);
	OutputDebugString(str);

	// assert(false);
}

void initOpengl (HWND windowHandle) {
	globalMemStack = gjInitMemStack(megabytes(50));

	createWin32OpenglContext(windowHandle);

	glDebugMessageCallback(openglDebugCallback, null);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, null, GL_TRUE);

	glEnable(GL_DEPTH_TEST);

	globalShaderProgram = createShader("../code/shader.glsl");
	globalWireShader = createShader("../code/wire.glsl");
	depthShader = createShader("../code/depth.glsl");
	simpleShader = createShader("../code/simple.glsl");
	cubeMapShader = createShader("../code/cubemap.glsl");
	pbr_shader = createShader("../code/pbr.glsl");

	/*fiz (globalModel->indexCount) {
		Vec3 vertex = globalModel->vertices[globalModel->indices[i]].vertex;
		Vec3 normal = globalModel->vertices[globalModel->indices[i]].normal;
		char str[256];
		sprintf(str, "v %f %f %f, n %f %f %f \n", vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z);
		OutputDebugString(str);
	}*/

	models[0] = loadModel("1v5.obj", 0.1f, "Ship_Cube.001");
	models[1] = loadModel("119.obj", 0.005f, "engine_starboard.001_Cylinder.003");
	models[2] = loadModel("smooth.obj", 0.5f, "Sphere_Sphere.001");
	models[3] = loadModel("cube.obj", 0.5f, "Cube");
	models[4] = loadModel("test.obj", 1.0f, "Sphere");
	models[5] = loadModel("cylinder.obj", 0.4f, "Cylinder_Cylinder.001");
	
	/*globalModel = loadModel("cube.obj", "Cube");
	globalShipModel = loadModel("1v5.obj", "Ship_Cube.001");
	globalCylinderModel = loadModel("cylinder.obj", "Cylinder_Cylinder.001");
	globalTestModel = loadModel("test.obj", "Sphere");
	globalBigShipModel = loadModel("119.obj", "engine_starboard.001_Cylinder.003");*/

	shadowFrameBuffer = createDepthCubeFrameBuffer();
}

/*float rads (float degs) {
	float result = (degs / 180.0f) * PI;
	return result;
}*/

/*
	OpenGL perspective matrix

	f = cotangent(fovy / 2)

	f / aspect, 0, 0                              ,	0                                  ,
	0         ,	f, 0                              ,	0                                  ,
	0         ,	0, (zFar + zNear) / (zNear - zFar), (2 × zFar × zNear) / (zNear - zFar),
	0         ,	0, -1                             ,	0                                  ,
*/
Mat4 createPerspectiveMatrix (float fov, float aspect, float near, float far) {
	float f = 1.0f / tanf(rads(fov) / 2.0f);
	Mat4 matrix = {
		f / aspect, 0, 0, 0,
		0, f, 0, 0,
		0, 0, (far + near) / (near - far), -1,
		0, 0, (2.0f * far * near) / (near - far), 0,
	};

	return matrix;
}

Mat4 cameraPerspective (float fov) {
	Mat4 result = createPerspectiveMatrix(fov, defaultViewport.x/defaultViewport.y, 0.1f, 100.0f);
	return result;
}

Mat4 cameraCubeMapPerspective (float fov) {
	Mat4 result = createPerspectiveMatrix(fov, 1024.0f/1024.0f, 0.1f, 100.0f);
	return result;
}

Mat4 identityMatrix () {
	Mat4 result = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	return result;
}

Mat4 xRotate (float rads) {
	Mat4 result = {
		1,  0,             0,             0,
		0,  gj_cos(rads), -gj_sin(rads),  0,
		0,  gj_sin(rads),  gj_cos(rads),  0,
		0,  0,             0,             1,
	};
	return result;
}

Mat4 yRotate (float rads) {
	Mat4 result = {
		 gj_cos(rads),  0,  gj_sin(rads),  0,
		 0,             1,  0,             0,
		-gj_sin(rads),  0,  gj_cos(rads),  0,
		 0,             0,  0,             1,
	};
	return result;
}

Mat4 zRotate (float rads) {
	Mat4 result = {
		gj_cos(rads),  -gj_sin(rads),  0,  0,
		gj_sin(rads),   gj_cos(rads),  0,  0,
		0,             0,              1,  0,
		0,             0,              0,  1,
	};
	return result;
}

// Mat4 normalToMatrix (Vec3 normal) {
// 	Mat4 result = xRotate(zxRotation) * yRotate(yRotation) * zRotate(zRotation);
// }

Mat4 translate (float x, float y, float z) {
	Mat4 result = {
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1,
	};
	return result;
}

Mat4 translate (Vec3 pos) {
	Mat4 result = translate(pos.x, pos.y, pos.z);
	return result;
}

Mat4 scaleMatrix (float scale) {
	Mat4 result = {
		scale, 0,     0,     0,
		0,     scale, 0,     0,
		0,     0,     scale, 0,
		0,     0,     0,     1,
	};
	return result;
}

/*Mat4 operator* (Mat4 mat1, Mat4 mat2) {
	Mat4 result = {
		mat1.m[0]*mat2.m[0] + mat1.m[1]*mat2.m[4] + mat1.m[2]*mat2.m[8] + mat1.m[3]*mat2.m[12],
		mat1.m[0]*mat2.m[1] + mat1.m[1]*mat2.m[5] + mat1.m[2]*mat2.m[9] + mat1.m[3]*mat2.m[13],
		mat1.m[0]*mat2.m[2] + mat1.m[1]*mat2.m[6] + mat1.m[2]*mat2.m[10] + mat1.m[3]*mat2.m[14],
		mat1.m[0]*mat2.m[3] + mat1.m[1]*mat2.m[7] + mat1.m[2]*mat2.m[11] + mat1.m[3]*mat2.m[15],

		mat1.m[4]*mat2.m[0] + mat1.m[5]*mat2.m[4] + mat1.m[6]*mat2.m[8] + mat1.m[7]*mat2.m[12],
		mat1.m[4]*mat2.m[1] + mat1.m[5]*mat2.m[5] + mat1.m[6]*mat2.m[9] + mat1.m[7]*mat2.m[13],
		mat1.m[4]*mat2.m[2] + mat1.m[5]*mat2.m[6] + mat1.m[6]*mat2.m[10] + mat1.m[7]*mat2.m[14],
		mat1.m[4]*mat2.m[3] + mat1.m[5]*mat2.m[7] + mat1.m[6]*mat2.m[11] + mat1.m[7]*mat2.m[15],

		mat1.m[8]*mat2.m[0] + mat1.m[9]*mat2.m[4] + mat1.m[10]*mat2.m[8] + mat1.m[11]*mat2.m[12],
		mat1.m[8]*mat2.m[1] + mat1.m[9]*mat2.m[5] + mat1.m[10]*mat2.m[9] + mat1.m[11]*mat2.m[13],
		mat1.m[8]*mat2.m[2] + mat1.m[9]*mat2.m[6] + mat1.m[10]*mat2.m[10] + mat1.m[11]*mat2.m[14],
		mat1.m[8]*mat2.m[3] + mat1.m[9]*mat2.m[7] + mat1.m[10]*mat2.m[11] + mat1.m[11]*mat2.m[15],

		mat1.m[12]*mat2.m[0] + mat1.m[13]*mat2.m[4] + mat1.m[14]*mat2.m[8] + mat1.m[15]*mat2.m[12],
		mat1.m[12]*mat2.m[1] + mat1.m[13]*mat2.m[5] + mat1.m[14]*mat2.m[9] + mat1.m[15]*mat2.m[13],
		mat1.m[12]*mat2.m[2] + mat1.m[13]*mat2.m[6] + mat1.m[14]*mat2.m[10] + mat1.m[15]*mat2.m[14],
		mat1.m[12]*mat2.m[3] + mat1.m[13]*mat2.m[7] + mat1.m[14]*mat2.m[11] + mat1.m[15]*mat2.m[15],
	};
	return result;
}*/

float length (Vec3 v) {
	float result = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	return result;
}

Vec3 normalize (Vec3 v) {
	Vec3 result;
	if (length(v)) {
		result = {v.x/length(v), v.y/length(v), v.z/length(v)};
	} else {
		result = {0.0f, 0.0f, 0.0f};
	}
	return result;
}

Vec3 cross (Vec3 a, Vec3 b) {
	Vec3 result;
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
	return normalize(result);
}

Vec3 operator- (Vec3 v1, Vec3 v2) {
	Vec3 result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	return result;
}

Vec3 operator- (Vec3 v1) {
	Vec3 result = {-v1.x, -v1.y, -v1.z};
	return result;
}

float dot (Vec3 v1, Vec3 v2) {
	float result = (v1.x*v2.x +
					v1.y*v2.y +
					v1.z*v2.z);
	return result;
}

Mat4 cameraMatrix (Vec3 position, Vec3 direction, Vec3 up) {
	Vec3 x = {};
	Vec3 y = {};
	Vec3 z = {};

	z = -normalize(direction);
	y = up;
	x = cross(y, z);
	y = cross(z, x);
	x = normalize(x);
	y = normalize(y);

	Mat4 result = {
		x.x, x.y, x.z, -dot(x, position),
		y.x, y.y, y.z, -dot(y, position),
		z.x, z.y, z.z, -dot(z, position),
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}

void drawModel (Model model, float scale, Draw_Mode drawMode, Camera camera) {
	GLuint worldShader;
	if (drawMode == DRAW_MODE_FINAL) {
		worldShader = globalShaderProgram;
	} else if (drawMode == DRAW_MODE_DEPTH) {
		worldShader = depthShader;
	}

	glUseProgram(worldShader);

	/*Mat4 rotationMatrix = xRotate(camera.rotation.x) * yRotate(camera.rotation.y) * zRotate(camera.rotation.z);
	Mat4 worldRotationMatrix = identityMatrix();
	Mat4 cameraRotationMatrix = identityMatrix();
	if (drawMode == DRAW_MODE_FINAL) {
		cameraRotationMatrix = rotationMatrix;
	} else if (drawMode == DRAW_MODE_DEPTH) {
		worldRotationMatrix = rotationMatrix;
	}*/

	glUniformMatrix4fv(glGetUniformLocation(worldShader, "uProjMatrix"), 1, GL_FALSE, camera.perspective.m);
	Mat4 translationMatrix = identityMatrix();
	Mat4 transformMatrix = translationMatrix * scaleMatrix(scale);
	glUniformMatrix4fv(glGetUniformLocation(worldShader, "uTransform"), 1, GL_FALSE, transformMatrix.m);

	glUniformMatrix4fv(glGetUniformLocation(worldShader, "uRotationMatrix"), 1, GL_FALSE, identityMatrix().m/*rotationMatrix.m*/);

	// Mat4 cameraTranslation = translate(-camera.position.x, -camera.position.y, -camera.position.z);
	// if (drawMode == DRAW_MODE_DEPTH) {
	// 	cameraTranslation = translate(1.0f, -1.0f, -5.0f);
	// }

	/*Mat4 cameraMatrix = cameraTranslation * rotationMatrix;
	if (drawMode == DRAW_MODE_DEPTH) {
		cameraMatrix = rotationMatrix * cameraTranslation;
	}*/
	Mat4 matCamera = cameraMatrix(camera.position, camera.direction, camera.up);
	glUniformMatrix4fv(glGetUniformLocation(worldShader, "cameraTransform"), 1, GL_FALSE, matCamera.m/*cameraMatrix.m*/);

	glUniform4f(glGetUniformLocation(worldShader, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z, 1.0f);

	if (drawMode == DRAW_MODE_FINAL) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowFrameBuffer.cubeTexture);
		glUniform1i(glGetUniformLocation(worldShader, "shadowMap"), 0);
	}

#if 0
	glBegin(GL_TRIANGLES);

	fiz (model->indexCount) {
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		Vec3 vertex = model->vertices[model->indices[i]].vertex;
		Vec3 normal = model->vertices[model->indices[i]].normal;
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(vertex.x * scale, vertex.y * scale, vertex.z * scale);
	}

	glEnd();
#else
	
	glBindBuffer(GL_ARRAY_BUFFER, model.vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.indexBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model_Vertex), 0/*model.vertices*/);
	glEnableVertexAttribArray(1);
	char *data = (char*)model.vertices;
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model_Vertex), (void*)sizeof(Vec3)/*data + sizeof(Vec3)*/);

	glPushMatrix();
	// glTranslatef(0.0f, 0.0f, -8.0f);
	glDrawElements(GL_TRIANGLES, model.indexCount, GL_UNSIGNED_INT, 0/*model.indices*/);
	glPopMatrix();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


#endif

	if (drawMode == DRAW_MODE_FINAL) {
		if (globalNormalVisualization) {
			glUseProgram(globalWireShader);
			glUniformMatrix4fv(glGetUniformLocation(globalWireShader, "uProjMatrix"), 1, GL_FALSE, globalProjMatrix.m);
			glUniformMatrix4fv(glGetUniformLocation(globalWireShader, "uTransform"), 1, GL_FALSE, transformMatrix.m);

			glBegin(GL_LINES);
			fiz (model.vertexCount) {
				glColor4f(1.0f, 0.0f, 0.5f, 1.0f);
				Vec3 vertex = model.vertices[i].vertex * scale;
				glVertex3f(vertex.x, vertex.y, vertex.z);
				Vec3 normal = vertex + (model.vertices[i].normal * 0.1f);
				glVertex3f(normal.x, normal.y, normal.z);
			}
			glEnd();
		}
	}
}

void drawWorld (Draw_Mode drawMode, Camera camera) {
	GLuint worldShader;
	if (drawMode == DRAW_MODE_FINAL) {
		worldShader = globalShaderProgram;
	} else if (drawMode == DRAW_MODE_DEPTH) {
		worldShader = depthShader;
	}

	glUseProgram(worldShader);

#if 1
	Mat4 rotationMatrix = xRotate(camera.rotation.x) * yRotate(camera.rotation.y) * zRotate(camera.rotation.z);
	Mat4 worldRotationMatrix = identityMatrix();
	Mat4 cameraRotationMatrix = identityMatrix();
	if (drawMode == DRAW_MODE_FINAL) {
		cameraRotationMatrix = rotationMatrix;
	} else if (drawMode == DRAW_MODE_DEPTH) {
		worldRotationMatrix = rotationMatrix;
	}

	glUniformMatrix4fv(glGetUniformLocation(worldShader, "uProjMatrix"), 1, GL_FALSE, camera.perspective.m);
	Mat4 translationMatrix = identityMatrix();
	Mat4 transformMatrix = translationMatrix * scaleMatrix(2.0f);
	glUniformMatrix4fv(glGetUniformLocation(worldShader, "uTransform"), 1, GL_FALSE, transformMatrix.m);

	glUniformMatrix4fv(glGetUniformLocation(worldShader, "uRotationMatrix"), 1, GL_FALSE, identityMatrix().m/*rotationMatrix.m*/);
	Mat4 matCamera = cameraMatrix(camera.position, camera.direction, camera.up);
	glUniformMatrix4fv(glGetUniformLocation(worldShader, "cameraTransform"), 1, GL_FALSE, matCamera.m/*cameraMatrix.m*/);

	glUniform4f(glGetUniformLocation(worldShader, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z, 1.0f);

	if (drawMode == DRAW_MODE_FINAL) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowFrameBuffer.cubeTexture);
		glUniform1i(glGetUniformLocation(worldShader, "shadowMap"), 0);
	}

	Vec3 walls[] = {
		{-2.0f, 2.0f, -2.0f,},
		{2.0f, 2.0f, -2.0f,},
		{2.0f, -2.0f, -2.0f,},
		{-2.0f, -2.0f, -2.0f,},

		{2.0f, 2.0f, -2.0f,},
		{2.0f, 2.0f, 2.0f,},
		{2.0f, -2.0f, 2.0f,},
		{2.0f, -2.0f, -2.0f,},

		{-2.0f, -2.0f, -2.0f,},
		{2.0f, -2.0f, -2.0f,},
		{2.0f, -2.0f, 2.0f,},
		{-2.0f, -2.0f, 2.0f,},
	};

	Vec3 wallNormals[] = {
		{0.0f, 0.0f, 1.0f,},
		{0.0f, 0.0f, 1.0f,},
		{0.0f, 0.0f, 1.0f,},
		{0.0f, 0.0f, 1.0f,},

		{-1.0f, 0.0f, 0.0f,},
		{-1.0f, 0.0f, 0.0f,},
		{-1.0f, 0.0f, 0.0f,},
		{-1.0f, 0.0f, 0.0f,},

		{0.0f, 1.0f, 0.0f,},
		{0.0f, 1.0f, 0.0f,},
		{0.0f, 1.0f, 0.0f,},
		{0.0f, 1.0f, 0.0f,},
	};

	int fuckingIndices[] = {
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		8, 9, 10,
		10, 11, 8,
	};

#if 1
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0/*sizeof(Model_Vertex)*/, walls);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0/*sizeof(Model_Vertex)*/, wallNormals);
	glDrawElements(GL_TRIANGLES, arraySize(fuckingIndices), GL_UNSIGNED_INT, fuckingIndices);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
#endif

#endif

	drawModel(models[selectedModel], models[selectedModel].renderScale, drawMode, camera);
}

#if 0
void drawCubeIGuess () {
	#define white 1.0f, 1.0f, 1.0f, 1.0f
	#define yellow 1.0f, 1.0f, 0.2f, 1.0f
	#define red 1.0f, 0.2f, 0.2f, 1.0f
	#define blue 0.2f, 0.2f, 1.0f, 1.0f
	#define purple 1.0f, 0.2f, 1.0f, 1.0f
	#define green 0.2f, 1.0f, 0.2f, 1.0f
	#define black 0.0f, 0.0f, 0.0f, 1.0f
	#define terq 0.2f, 1.0f, 1.0f, 1.0f

	glPushMatrix();
	// glTranslatef(0.0f, 0.0f, -2.0f);
	// glRotatef((180.0f / PI) * xRotation, 1.0f, 0.0f, 0.0f);
	// glRotatef((180.0f / PI) * yRotation, 0.0f, 1.0f, 0.0f);
	// glRotatef((180.0f / PI) * zRotation, 0.0f, 0.0f, 1.0f);

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
}
#endif

void drawOpengl (HWND windowHandle) {
	Vec3 asd = {10.0f, 5.0f, 0.0f};
	Vec3 qwe = {5.0f, 10.0f, 0.0f};
	Vec3 test = cross(asd, qwe);

	Mat4 cam = cameraMatrix({3.0f, 0.0f, 3.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f});
	int x = 0;

	HDC hdc = GetDC(windowHandle);
	// glClearColor(0.4f, 0.6f, 0.9f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// float sw = 1 / tanf((70.0f / 2.0f) * (PI / 180.0f));
	// float sh = 1 / tanf((70.0f / 2.0f) * (PI / 180.0f));
	/*float sw = 1 / tanf((70.0f / 2.0f) * (1280.0f / 720.0f));
	float sh = 1 / tanf((70.0f / 2.0f) * (720.0f / 1280.0f));*/
	// float n = 0.1f;
	// float f = 100.0f;
	// float aspect = (1280.0f / 720.0f);
	/*float projectionMatrix[] = {
		sw/aspect, 0.0f, 0.0f, 0.0f,
		0.0f, sh, 0.0f, 0.0f,
		0.0f, 0.0f, -(f / (f - n)), -1.0f,
		0.0f, 0.0f, -((f * n) / (f - n)), 0.0f,
	};*/

	globalProjMatrix = createPerspectiveMatrix(70, defaultViewport.x/defaultViewport.y, 0.1f, 100.0f);

	glViewport(0, 0, defaultViewport.x, defaultViewport.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	globalZoom -= globalScroll * (globalZoom * 0.1f);

	POINT mousePos;
	GetCursorPos(&mousePos);
	SHORT mouseLeftDown = GetAsyncKeyState(VK_LBUTTON);
	Vec2 mouseMovement = {mousePos.x - globalMousePos.x, mousePos.y - globalMousePos.y};
	globalMousePos = {mousePos.x, mousePos.y};

	if (mouseLeftDown) {
		yRotation += mouseMovement.x * 0.01f;
		xRotation += mouseMovement.y * 0.01f;
	}

	// Rotate the light
	static float lightRotation = 0.0f;
	lightRotation += 0.01f;
	if (lightRotation > PI2) {
		lightRotation -= PI2;
	}
	lightPosition = {gj_sin(lightRotation)*-3.0f, 3.0f, gj_cos(lightRotation)*-3.0f};

#if 1
	struct Cube_Map_Side {
		GLenum target;
		Vec3 direction;
		Vec3 up;
	};

	Cube_Map_Side cubeMapSides[] = {
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, {0.0f, 0.0f, -1.0f}, {0.0f, -1.0f, 0.0f}},
		{GL_TEXTURE_CUBE_MAP_POSITIVE_Z, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_X, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{GL_TEXTURE_CUBE_MAP_POSITIVE_X, {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
		{GL_TEXTURE_CUBE_MAP_POSITIVE_Y, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
	};

	glViewport(0, 0, 1024, 1024);

	fiz (arraySize(cubeMapSides)) {
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffer.id);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubeMapSides[i].target, shadowFrameBuffer.cubeTexture, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Camera lightCamera = {};
		lightCamera.position = lightPosition;
		lightCamera.direction = cubeMapSides[i].direction;
		lightCamera.up = cubeMapSides[i].up;
		// lightCamera.rotation = {0.0f, 0.0f, 0.0f};
		lightCamera.perspective = cameraCubeMapPerspective(90);
		drawWorld(DRAW_MODE_DEPTH, lightCamera);
	}

	/*glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffer.id);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, shadowFrameBuffer.cubeTexture, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera lightCamera = {};
	lightCamera.position = lightPosition;
	lightCamera.rotation = {0.0f, 0.0f, 0.0f};
	lightCamera.perspective = cameraCubeMapPerspective(90);
	drawWorld(DRAW_MODE_DEPTH, lightCamera);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, shadowFrameBuffer.cubeTexture, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lightCamera.rotation = {0.0f, PI, 0.0f};
	drawWorld(DRAW_MODE_DEPTH, lightCamera);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, shadowFrameBuffer.cubeTexture, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lightCamera.rotation = {0.0f, PI * 0.5f, 0.0f};
	drawWorld(DRAW_MODE_DEPTH, lightCamera);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, shadowFrameBuffer.cubeTexture, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lightCamera.rotation = {0.0f, -(PI * 0.5f), 0.0f};
	drawWorld(DRAW_MODE_DEPTH, lightCamera);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, shadowFrameBuffer.cubeTexture, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lightCamera.rotation = {-(PI * 0.5f), 0.0f, 0.0f};
	drawWorld(DRAW_MODE_DEPTH, lightCamera);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, shadowFrameBuffer.cubeTexture, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lightCamera.rotation = {PI * 0.5f, 0.0f, 0.0f};
	drawWorld(DRAW_MODE_DEPTH, lightCamera);*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

	glViewport(0, 0, defaultViewport.x, defaultViewport.y);

	Camera playerCamera = {};
	/*playerCamera.position = lightPosition;
	playerCamera.direction = cubeMapSides[0].direction;
	playerCamera.up = cubeMapSides[0].up;*/
	playerCamera.position = {gj_sin(yRotation)*5.0f, 2.0f, gj_cos(yRotation)*-5.0f};
	playerCamera.direction = {gj_sin(yRotation)*-1.0f, -0.5f, gj_cos(yRotation)*1.0f};
	playerCamera.up = {0.0f, 1.0f, 0.0f};


	// playerCamera.position = {0.0f, 0.0f, globalZoom};
	// playerCamera.rotation = {xRotation, yRotation, zRotation};
	playerCamera.perspective = cameraPerspective(playerFov);
	drawWorld(DRAW_MODE_FINAL, playerCamera);

	char shit[256];
	sprintf(shit, "zoom %f xRot %f yRot %f fov %f \n", globalZoom, xRotation, yRotation, playerFov);
	OutputDebugString(shit);


#if 1
	{
		glUseProgram(cubeMapShader);

		glUniformMatrix4fv(glGetUniformLocation(cubeMapShader, "uProjMatrix"), 1, GL_FALSE, playerCamera.perspective.m);
		Mat4 translationMatrix = translate(lightPosition);
		Mat4 transformMatrix = translationMatrix * scaleMatrix(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(cubeMapShader, "uTransform"), 1, GL_FALSE, transformMatrix.m);

		glUniformMatrix4fv(glGetUniformLocation(cubeMapShader, "uRotationMatrix"), 1, GL_FALSE, identityMatrix().m/*rotationMatrix.m*/);
		Mat4 matCamera = cameraMatrix(playerCamera.position, playerCamera.direction, playerCamera.up);
		glUniformMatrix4fv(glGetUniformLocation(cubeMapShader, "cameraTransform"), 1, GL_FALSE, matCamera.m/*cameraMatrix.m*/);

		glUniform4f(glGetUniformLocation(cubeMapShader, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z, 1.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowFrameBuffer.cubeTexture);
		glUniform1i(glGetUniformLocation(cubeMapShader, "cubeMap"), 0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model_Vertex), models[2].vertices);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model_Vertex), ((char*)models[2].vertices) + sizeof(Vec3));
		glDrawElements(GL_TRIANGLES, models[2].indexCount, GL_UNSIGNED_INT, models[2].indices);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
#endif

	SwapBuffers(hdc);
}

void stuff() {
	glViewport(0, 0, defaultViewport.x, defaultViewport.y);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mat4 cam = cameraMatrix({0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f});
	Mat4 projection = createPerspectiveMatrix(70, defaultViewport.x/defaultViewport.y, 0.1f, 100.0f);
	Mat4 mat = identityMatrix();

	glUseProgram(pbr_shader);
	glUniformMatrix4fv(glGetUniformLocation(pbr_shader, "uProjMatrix"), 1, GL_FALSE, projection.m);
	glUniformMatrix4fv(glGetUniformLocation(pbr_shader, "uRotationMatrix"), 1, GL_FALSE, mat.m);
	glUniformMatrix4fv(glGetUniformLocation(pbr_shader, "uTransform"), 1, GL_FALSE, mat.m);
	glUniformMatrix4fv(glGetUniformLocation(pbr_shader, "cameraTransform"), 1, GL_FALSE, cam.m);
	glUniform4f(glGetUniformLocation(pbr_shader, "lightPosition"), 5.0f, 5.0f, 5.0f, 1.0f);
	
	float verts[] = {
		-1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
	};
	float norms[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};
	int indices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	glBindBuffer(GL_ARRAY_BUFFER, models[2].vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, models[2].indexBuffer);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// 2, 4
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model_Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model_Vertex), (void*)sizeof(Vec3));

	glDrawElements(GL_TRIANGLES, models[2].indexCount, GL_UNSIGNED_INT, 0);
}