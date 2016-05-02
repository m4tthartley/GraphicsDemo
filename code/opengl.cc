
#pragma pack(push, 1)
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

union Vec3 {
	struct {
		float x;
		float y;
		float z;
	};
	Vec2 xy;
};

Vec3 operator+ (Vec3 vec1, Vec3 vec2) {
	Vec3 result = {vec1.x+vec2.x, vec1.y+vec2.y, vec1.z+vec2.z};
	return result;
}

Vec3 operator* (Vec3 vec, float num) {
	Vec3 result = {vec.x*num, vec.y*num, vec.z*num};
	return result;
}

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

struct model_face {
	struct {
		int vertexIndex;
		int normalIndex;
	} points[3];
};

struct Model_Vertex {
	Vec3 vertex;
	Vec3 normal;
};

struct Model {
	Model_Vertex vertices[4096*16];
	int indices[4096*16];
	int vertexCount;
	int indexCount;
	float renderScale;
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

gj_Mem_Stack globalMemStack;

GLuint globalShaderProgram;
GLuint globalWireShader;

// Model *globalModel;
// Model *globalShipModel;
// Model *globalCylinderModel;
// Model *globalTestModel;
// Model *globalBigShipModel;
Model *models[5];
int selectedModel = 0;

Vec2 globalMousePos;
float globalScroll;

float globalZoom = -3.0f;
bool globalNormalVisualization = false;

static float xRotation = 0.0f;
static float yRotation = 0.0f;
static float zRotation = 0.0f;

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

GLuint createShader (char *file) {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	gj_Data shaderData = gj_readFile(file, &globalMemStack);

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

	OutputDebugString(file);
	OutputDebugString(" \nvertex shader output: \n");
	OutputDebugString(vertexShaderError);
	OutputDebugString(" fragment shader output: \n");
	OutputDebugString(fragmentShaderError);

	OutputDebugString(shaderData.mem);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	return program;
}

char *readObjToken (char **str) {
	while (**str == ' ' || **str == '\t') {
		++*str;
	}

	char *start = *str;

	while (**str != ' ' && **str != '\t' && **str != '\n' && **str != 0) {
		++*str;
	}

	**str = 0;
	++*str;

	return start;
}

Model *loadModel (char *file, float renderScale, char *objectName) {
	

	// file_data modelFile = FileRead(file);
	gj_Data modelData = gj_readFile(file, &globalMemStack);
	char *objStr = (char*)modelData.mem;

	int unusedVertexCount = 0;
	int unusedNormalCount = 0;

	{
		// Preprocess counts
		while (objStr - modelData.mem < modelData.size) {
			char *token = readObjToken(&objStr);

			if (!strcmp(token, "v")) {
				++unusedVertexCount;
				int x = 0;
			} else if (!strcmp(token, "vn")) {
				++unusedNormalCount;
				int x = 0;
			} else if (!strcmp(token, "o")) {
				token = readObjToken(&objStr);
				if (!strcmp(token, objectName)) {
					break;
				}
				int x = 0;
			} else if (!strcmp(token, "f")) {
				int x = 0;
			}
		}
	}

	// objStr = (char*)modelData.mem;

	/*s32 modelMemSize = sizeof(v3)*vertMax + sizeof(v3)*vertMax + sizeof(v3)*normMax + sizeof(model_face)*faceMax + sizeof(iv3)*faceMax;
	char *modelMem = MemPoolAlloc(assetMemPool, modelMemSize);
	verts = (v3*)modelMem;
	perVertNormals = verts + vertMax;
	norms = perVertNormals + vertMax;
	faces = (model_face*)(norms + normMax);
	indices = (iv3*)(faces + faceMax);
	s32 vertCount = 0;
	s32 normCount = 0;
	s32 faceCount = 0;*/

	Model *model = (Model*)gj_pushMemStack(&globalMemStack, sizeof(Model));
	Model *tempModel = (Model*)gj_pushMemStack(&globalMemStack, sizeof(Model));
	int vertexCount = 0;
	int normalCount = 0;
	// int indexCount = 0;

	while (objStr - modelData.mem < modelData.size) {
		char *token = readObjToken(&objStr);

		if (!strcmp(token, "v")) {
			Vec3 *vert = &tempModel->vertices[vertexCount].vertex;
			token = readObjToken(&objStr);
			vert->x = strtof(token, NULL);
			token = readObjToken(&objStr);
			vert->y = strtof(token, NULL);
			token = readObjToken(&objStr);
			vert->z = strtof(token, NULL);
			++vertexCount;

			if (vertexCount >= arraySize(tempModel->vertices)) {
				assert(false);
			}
		} else if (!strcmp(token, "vn")) {
			Vec3 *norm = &tempModel->vertices[normalCount].normal;
			token = readObjToken(&objStr);
			norm->x = strtof(token, NULL);
			token = readObjToken(&objStr);
			norm->y = strtof(token, NULL);
			token = readObjToken(&objStr);
			norm->z = strtof(token, NULL);
			++normalCount;

			if (normalCount >= arraySize(tempModel->vertices)) {
				assert(false);
			}
		} else if (!strcmp(token, "f")) {
			// token = readObjToken(&objStr);
			/*faces[faceCount].points[0].vertexIndex = atoi(token) - 1;
			token += 3;
			faces[faceCount].points[0].normalIndex = atoi(token) - 1;

			token = readObjToken(&objStr);
			faces[faceCount].points[1].vertexIndex = atoi(token) - 1;
			token += 3;
			faces[faceCount].points[1].normalIndex = atoi(token) - 1;

			token = readObjToken(&objStr);
			faces[faceCount].points[2].vertexIndex = atoi(token) - 1;
			token += 3;
			faces[faceCount].points[2].normalIndex = atoi(token) - 1;*/

			token = readObjToken(&objStr);
			int index1Vertex = atoi(token) - 1;
			while (*token != '/') {
				++token;
			}
			token += 2;
			int index1Normal = atoi(token) - 1;

			token = readObjToken(&objStr);
			int index2Vertex = atoi(token) - 1;
			while (*token != '/') {
				++token;
			}
			token += 2;
			int index2Normal = atoi(token) - 1;

			token = readObjToken(&objStr);
			int index3Vertex = atoi(token) - 1;
			while (*token != '/') {
				++token;
			}
			token += 2;
			int index3Normal = atoi(token) - 1;

			model->indices[model->indexCount] = model->vertexCount;
			model->indices[model->indexCount+1] = model->vertexCount+1;
			model->indices[model->indexCount+2] = model->vertexCount+2;

			model->vertices[model->vertexCount].vertex = tempModel->vertices[index1Vertex - unusedVertexCount].vertex;
			model->vertices[model->vertexCount+1].vertex = tempModel->vertices[index2Vertex - unusedVertexCount].vertex;
			model->vertices[model->vertexCount+2].vertex = tempModel->vertices[index3Vertex - unusedVertexCount].vertex;

			model->vertices[model->vertexCount].normal = tempModel->vertices[index1Normal - unusedNormalCount].normal;
			model->vertices[model->vertexCount+1].normal = tempModel->vertices[index2Normal - unusedNormalCount].normal;
			model->vertices[model->vertexCount+2].normal = tempModel->vertices[index3Normal - unusedNormalCount].normal;

			/*model->vertices[model->vertexCount].vertex = tempModel->vertices[index1Vertex].vertex;
			model->vertices[model->vertexCount].vertex = tempModel->vertices[index2Vertex].vertex;
			model->vertices[model->vertexCount].vertex = tempModel->vertices[index3Vertex].vertex;*/

			model->indexCount += 3;
			model->vertexCount += 3;

			if (model->indexCount >= arraySize(model->indices) - 2) {
				assert(false);
			}
			if (model->vertexCount >= arraySize(model->vertices) - 2) {
				assert(false);
			}

			/*indices[faceCount].x = faces[faceCount].points[0].vertexIndex;
			indices[faceCount].y = faces[faceCount].points[1].vertexIndex;
			indices[faceCount].z = faces[faceCount].points[2].vertexIndex;*/

			// ++faceCount;
		}

		
	}

	// model->vertexCount = vertexCount;

	int x = 0;

	/*fiz (faceMax) {
		perVertNormals[faces[i].points[0].vertexIndex] = norms[faces[i].points[0].normalIndex];
		perVertNormals[faces[i].points[1].vertexIndex] = norms[faces[i].points[1].normalIndex];
		perVertNormals[faces[i].points[2].vertexIndex] = norms[faces[i].points[2].normalIndex];
	}*/

	model->renderScale = renderScale;
	return model;
}

void initOpengl (HWND windowHandle) {
	globalMemStack = gj_initMemStack(megabytes(20));

	createWin32OpenglContext(windowHandle);

	glEnable(GL_DEPTH_TEST);

	globalShaderProgram = createShader("../code/shader.glsl");
	globalWireShader = createShader("../code/wire.glsl");

	/*fiz (globalModel->indexCount) {
		Vec3 vertex = globalModel->vertices[globalModel->indices[i]].vertex;
		Vec3 normal = globalModel->vertices[globalModel->indices[i]].normal;
		char str[256];
		sprintf(str, "v %f %f %f, n %f %f %f \n", vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z);
		OutputDebugString(str);
	}*/

	models[0] = loadModel("1v5.obj", 0.1f, "Ship_Cube.001");
	models[1] = loadModel("119.obj", 0.005f, "engine_starboard.001_Cylinder.003");
	models[2] = loadModel("cube.obj", 0.5f, "Cube");
	models[3] = loadModel("test.obj", 1.0f, "Sphere");
	models[4] = loadModel("cylinder.obj", 0.4f, "Cylinder_Cylinder.001");
	
	/*globalModel = loadModel("cube.obj", "Cube");
	globalShipModel = loadModel("1v5.obj", "Ship_Cube.001");
	globalCylinderModel = loadModel("cylinder.obj", "Cylinder_Cylinder.001");
	globalTestModel = loadModel("test.obj", "Sphere");
	globalBigShipModel = loadModel("119.obj", "engine_starboard.001_Cylinder.003");*/
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

Mat4 operator* (Mat4 mat1, Mat4 mat2) {
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
}

void drawModel (Model *model, float scale) {
	glUseProgram(globalShaderProgram);

	Mat4 rotationMatrix = xRotate(xRotation) * yRotate(yRotation) * zRotate(zRotation);
	Mat4 translationMatrix = translate(0.0f, 0.0f, globalZoom);
	glUniformMatrix4fv(glGetUniformLocation(globalShaderProgram, "uRotationMatrix"), 1, GL_FALSE, rotationMatrix.m);
	Mat4 transformMatrix = translationMatrix * rotationMatrix;
	glUniformMatrix4fv(glGetUniformLocation(globalShaderProgram, "uTransform"), 1, GL_FALSE, transformMatrix.m);

	glBegin(GL_TRIANGLES);

	fiz (model->indexCount) {
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		Vec3 vertex = model->vertices[model->indices[i]].vertex;
		Vec3 normal = model->vertices[model->indices[i]].normal;
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(vertex.x * scale, vertex.y * scale, vertex.z * scale);
	}

	glEnd();

	if (globalNormalVisualization) {
		glUseProgram(globalWireShader);
		glUniformMatrix4fv(glGetUniformLocation(globalWireShader, "uTransform"), 1, GL_FALSE, transformMatrix.m);

		glBegin(GL_LINES);
		fiz (model->vertexCount) {
			glColor4f(1.0f, 0.0f, 0.5f, 1.0f);
			Vec3 vertex = model->vertices[i].vertex * scale;
			glVertex3f(vertex.x, vertex.y, vertex.z);
			Vec3 normal = vertex + (model->vertices[i].normal * 0.1f);
			glVertex3f(normal.x, normal.y, normal.z);
		}
		glEnd();
	}
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
	// xRotation -= 0.01f;
	// yRotation += 0.01f;
	// zRotation += 0.01f;

	#define white 1.0f, 1.0f, 1.0f, 1.0f
	#define yellow 1.0f, 1.0f, 0.2f, 1.0f
	#define red 1.0f, 0.2f, 0.2f, 1.0f
	#define blue 0.2f, 0.2f, 1.0f, 1.0f
	#define purple 1.0f, 0.2f, 1.0f, 1.0f
	#define green 0.2f, 1.0f, 0.2f, 1.0f
	#define black 0.0f, 0.0f, 0.0f, 1.0f
	#define terq 0.2f, 1.0f, 1.0f, 1.0f

#if 0
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
#endif

	// drawModel(globalModel, 0.5f);
	// drawModel(globalCylinderModel, 0.15f);
	// drawModel(globalTestModel, 0.5f);

	// drawModel(globalShipModel, 0.15f);
	// drawModel(globalBigShipModel, 0.15f);

	drawModel(models[selectedModel], models[selectedModel]->renderScale);

	SwapBuffers(hdc);
}