
#extension GL_NV_shadow_samplers_cube : enable

uniform mat4 uProjMatrix;
uniform mat4 uRotationMatrix;
uniform mat4 uTransform;
uniform mat4 cameraTransform;

uniform samplerCube cubeMap;

#ifdef VERTEX_SHADER

	layout(location = 0) in vec4 vertex;
	layout(location = 1) in vec3 normal;

	out vec3 vertexInterp;
	out vec3 normalInterp;

	void main () {
		gl_Position = uProjMatrix * (vertex * uTransform * cameraTransform);
		vertexInterp = vertex.xyz;
		normalInterp = normal;
	}

#endif

#ifdef FRAGMENT_SHADER

	// layout(location = 0) out vec4 fragment;
	out vec4 fragment;

	in vec3 vertexInterp;
	in vec3 normalInterp;

	void main () {
		float sample = texture(cubeMap, vertexInterp).r;
		fragment = vec4(sample * 0.2f, 0.0f, 0.0f, 1.0f);
	}

#endif