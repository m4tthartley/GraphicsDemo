
uniform mat4 uProjMatrix;
uniform mat4 uTransform;

#ifdef VERTEX_SHADER

	layout(location = 0) in vec4 vertex;

	void main () {
		gl_Position = uProjMatrix * ((vertex) * uTransform);
	}

#endif

#ifdef FRAGMENT_SHADER

	layout(location = 0) out vec4 fragment;

	void main () {
		fragment = vec4(1.0f, 0.0f, 0.5f, 1.0f);
	}

#endif