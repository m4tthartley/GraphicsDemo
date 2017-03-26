
uniform mat4 uProjMatrix;
uniform mat4 uRotationMatrix;
uniform mat4 uTransform;
uniform mat4 cameraTransform;

uniform vec4 lightPosition;

//uniform samplerCube shadowMap;

#ifdef VERTEX_SHADER

	layout(location = 0) in vec3 vertex;
	layout(location = 1) in vec3 inNormal;
	// layout(location

	// out vec4 vColor;
	// out float light;
	out vec4 colorModifier;
	out vec3 cameraVector;
	out vec3 vNormal;
	out vec4 vertexInterp;

	void main () {
		gl_Position = uProjMatrix * (vec4(vertex, 1.0f) * uTransform * cameraTransform);

		vertexInterp = vec4(vertex, 1.0f) * uTransform;

		cameraVector = normalize(-(vec4(vertex, 1.0f) * uTransform)).xyz;

		vec4 vertexNormal = vec4(inNormal, 0.0f);
		vec4 rotatedNormal = normalize(vertexNormal * uRotationMatrix);
		vNormal = inNormal/*rotatedNormal.xyz*/;
	}

#endif

#ifdef FRAGMENT_SHADER

	layout(location = 0) out vec4 fragmentResult;

	// in vec4 vColor;
	// in float light;
	in vec4 colorModifier;
	in vec3 cameraVector;
	in vec3 vNormal;
	in vec4 vertexInterp;

	void main () {
		float light = 1.0f;

		vec3 lightDir = normalize(lightPosition.xyz - vertexInterp.xyz);

		float lightDot = dot(vNormal, lightDir);
		// fragmentResult = vec4(1.0f, 1.0f, 1.0f, 1.0f) * lightDot;
		fragmentResult = vec4(vNormal/**0.5f + 0.5f*/, 1.0f);
	}

#endif
