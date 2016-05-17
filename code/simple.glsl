
uniform mat4 uProjMatrix;
uniform mat4 uRotationMatrix;
uniform mat4 uTransform;

#ifdef VERTEX_SHADER

	layout(location = 0) in vec3 position;
	layout(location = 1) in vec3 normal;

	out vec3 color;

	void main () {
		gl_Position = uProjMatrix * (vec4(position, 1) * uTransform);
		color = normal;
	}

#endif

#ifdef FRAGMENT_SHADER

	layout(location = 0) out vec4 fragment;

	in vec3 color;

	void main () {
		// fragment = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		fragment = vec4(color, 1.0f);
	}

#endif