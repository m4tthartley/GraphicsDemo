
uniform mat4 uProjMatrix;
uniform mat4 uRotationMatrix;
uniform mat4 uTransform;
uniform mat4 cameraTransform;

uniform vec4 lightPosition;

uniform samplerCube shadowMap;

#ifdef VERTEX_SHADER

	layout(location = 0) in vec4 vertex;
	layout(location = 1) in vec3 inNormal;
	// layout(location

	// out vec4 vColor;
	out float light;
	out vec4 colorModifier;
	out vec3 cameraVector;
	out vec3 vNormal;
	out vec4 vertexInterp;

	void main () {
		gl_Position = uProjMatrix * (vertex * uTransform * cameraTransform);
		// vColor = gl_Color;
		// vColor = vec4(0.75f, 0.75f, 0.75f, 1.0f);

		vertexInterp = vertex * uTransform;

		colorModifier = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		light = 1.0f;

		vec4 lightDir = normalize(lightPosition);
		vec4 vertexNormal = vec4(inNormal, 0.0f);

		vec4 rotatedNormal = normalize(vertexNormal * uRotationMatrix);
		float lightDot = dot(rotatedNormal, lightDir);
		light = (lightDot * 0.5f) + 0.5f;

		/*float attenuation = length((vertex * uTransform) - lightPosition) / 20.0f;
		light *= 1.0f - attenuation;*/

		float lightDistance = length((vertex * uTransform) - lightPosition);
		float attenuation = 1.0f / (1.0f * pow(lightDistance, 1.1f));
		light *= 1.0f;

		// if (lightDot < 0.0f) {
		// 	colorModifier = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		// } else if (lightDot > 1.0f) {
		// 	colorModifier = vec4(0.0f, 0.0f, 1.0f, 1.0f);
		// } else {
		// 	light = clamp(lightDot, 0.0f, 1.0f);
		// }

		cameraVector = normalize(-(vertex * uTransform)).xyz;
		vNormal = rotatedNormal.xyz;
	}

#endif

#ifdef FRAGMENT_SHADER

	layout(location = 0) out vec4 fragmentResult;

	// in vec4 vColor;
	in float light;
	in vec4 colorModifier;
	in vec3 cameraVector;
	in vec3 vNormal;
	in vec4 vertexInterp;

	void main () {
		// float epsilon = 0.000001;
		float bias = 0.02;
		vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);

		vec4 lightDiff = vertexInterp - lightPosition;
		bool inShadow = texture(shadowMap, normalize(lightDiff.xyz)).r < length(lightDiff.xyz + bias);
		if (inShadow) {
			fragmentResult = ambient;
		} else {
			vec3 lightDir = normalize(lightPosition.xyz);

			vec3 halfVector = normalize(lightDir + cameraVector);
			vec4 specularColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
			float shininess = 50.0f;
			vec4 specular = specularColor * pow(max(dot(halfVector, vNormal), 0.0f), shininess);
			specular *= 1.0f - (light);

			vec4 colorModifier = vec4(0.75f, 0.75f, 0.75f, 1.0f);
			fragmentResult = max((colorModifier * light) + specular, ambient);
		}
	}

#endif
