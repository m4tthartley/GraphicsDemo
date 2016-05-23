
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

// struct Model {
// 	Model_Vertex vertices[4096*16];
// 	int indices[4096*16];
// 	int vertexCount;
// 	int indexCount;
// 	float renderScale;
// };
struct Model {
	Model_Vertex *vertices;
	int *indices;
	int vertexCount;
	int indexCount;
	float renderScale;

	GLuint vertexBuffer;
	GLuint indexBuffer;
};

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

Model loadModel (char *file, float renderScale, char *objectName) {
	

	// file_data modelFile = FileRead(file);
	gj_Data modelData = gj_readFile(file, &globalMemStack);
	char *objStr = (char*)modelData.mem;

	int unusedVertexCount = 0;
	int unusedNormalCount = 0;

	{
		// Preproccess counts
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

	int vertexMemoryNeeded = 0;
	int normalMemoryNeeded = 0;
	int faceMemoryNeeded = 0;

	char *preStr = objStr;
	while (preStr - modelData.mem < modelData.size) {
		char *token = readObjToken(&preStr);

		if (!strcmp(token, "v")) {
			++vertexMemoryNeeded;
		}
		if (!strcmp(token, "vn")) {
			++normalMemoryNeeded;
		}
		if (!strcmp(token, "f")) {
			++faceMemoryNeeded;
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

	Model model = {};
	int modelVertexMemory = faceMemoryNeeded*3;
	// int modelNormalMemory = 
	model.vertices = (Model_Vertex*)gj_pushMemStack(&globalMemStack, modelVertexMemory*sizeof(Model_Vertex));
	model.indices = (int*)gj_pushMemStack(&globalMemStack, faceMemoryNeeded*3*sizeof(int));
	Vec3 *tempVertices = (Vec3*)gj_pushMemStack(&globalMemStack, vertexMemoryNeeded);
	Vec3 *tempNormals = (Vec3*)gj_pushMemStack(&globalMemStack, normalMemoryNeeded);

	// Model *tempModel = (Model*)gj_pushMemStack(&globalMemStack, sizeof(Model));
	int vertexCount = 0;
	int normalCount = 0;
	// int indexCount = 0;

	while (objStr - modelData.mem < modelData.size) {
		char *token = readObjToken(&objStr);

		if (!strcmp(token, "v")) {
			Vec3 *vert = &tempVertices[vertexCount];
			token = readObjToken(&objStr);
			vert->x = atof(token);
			token = readObjToken(&objStr);
			vert->y = atof(token);
			token = readObjToken(&objStr);
			vert->z = atof(token);
			++vertexCount;

			// if (vertexCount >= arraySize(tempModel->vertices)) {
			// 	assert(false);
			// }
		} else if (!strcmp(token, "vn")) {
			Vec3 *norm = &tempNormals[vertexCount];
			token = readObjToken(&objStr);
			norm->x = atof(token);
			token = readObjToken(&objStr);
			norm->y = atof(token);
			token = readObjToken(&objStr);
			norm->z = atof(token);
			++normalCount;

			// if (normalCount >= arraySize(tempModel->vertices)) {
			// 	assert(false);
			// }
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

			model.indices[model.indexCount] = model.vertexCount;
			model.indices[model.indexCount+1] = model.vertexCount+1;
			model.indices[model.indexCount+2] = model.vertexCount+2;

			model.vertices[model.vertexCount].vertex = tempVertices[index1Vertex - unusedVertexCount];
			model.vertices[model.vertexCount+1].vertex = tempVertices[index2Vertex - unusedVertexCount];
			model.vertices[model.vertexCount+2].vertex = tempVertices[index3Vertex - unusedVertexCount];

			model.vertices[model.vertexCount].normal = tempNormals[index1Normal - unusedNormalCount];
			model.vertices[model.vertexCount+1].normal = tempNormals[index2Normal - unusedNormalCount];
			model.vertices[model.vertexCount+2].normal = tempNormals[index3Normal - unusedNormalCount];

			/*model->vertices[model->vertexCount].vertex = tempModel->vertices[index1Vertex].vertex;
			model->vertices[model->vertexCount].vertex = tempModel->vertices[index2Vertex].vertex;
			model->vertices[model->vertexCount].vertex = tempModel->vertices[index3Vertex].vertex;*/

			model.indexCount += 3;
			model.vertexCount += 3;

			// if (model->indexCount >= arraySize(model->indices) - 2) {
			// 	assert(false);
			// }
			// if (model->vertexCount >= arraySize(model->vertices) - 2) {
			// 	assert(false);
			// }

			/*indices[faceCount].x = faces[faceCount].points[0].vertexIndex;
			indices[faceCount].y = faces[faceCount].points[1].vertexIndex;
			indices[faceCount].z = faces[faceCount].points[2].vertexIndex;*/

			// ++faceCount;
		}

		
	}

	glGenBuffers(1, &model.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, model.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, modelVertexMemory*sizeof(Model_Vertex), model.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &model.indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceMemoryNeeded*3*sizeof(int), model.indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	model.renderScale = renderScale;
	return model;
}