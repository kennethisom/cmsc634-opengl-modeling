// vertex shader for simple terrain demo
#version 400 core

// per-frame data
layout(std140)                  // use standard layout
uniform SceneData {             // uniform struct name
    mat4 viewMatrix, viewInverse;
    mat4 projectionMatrix, projectionInverse;
    vec3 lightpos;
};

// per-vertex input
in vec3 vPosition, vNormal;

// output to fragment shader
out vec4 position, light;
out vec3 normal;

void main() {
    // surface and light position in view space
    position = viewMatrix * vec4(vPosition, 1);
    light = viewMatrix * vec4(lightpos, 1);

    // transform normal to view space
    normal = normalize(vNormal * mat3(viewInverse));

    // rendering position
    gl_Position = projectionMatrix * position;
}
