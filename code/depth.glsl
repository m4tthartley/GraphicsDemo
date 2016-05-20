
uniform mat4 uProjMatrix;
uniform mat4 uRotationMatrix;
uniform mat4 uTransform;
uniform mat4 cameraTransform;

uniform vec4 lightPosition;

#ifdef VERTEX_SHADER

	layout(location = 0) in vec4 vertex;
	layout(location = 1) in vec3 normal;

	out vec4 vertexInterp;

	void main () {
		gl_Position = uProjMatrix * ((vertex) * uTransform * cameraTransform);
		vertexInterp = (vertex * uTransform * cameraTransform);
	}

#endif

#ifdef FRAGMENT_SHADER

	layout(location = 0) out float fragmentResult;

	in vec4 vertexInterp;

	void main () {
		fragmentResult = length(vertexInterp - lightPosition);
	}

#endif