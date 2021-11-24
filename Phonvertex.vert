layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNor;
layout(location = 2) in vec2 vertexUV;

out vec3 fragmentPosition;
out vec3 normalTransposed;
out vec2 UV;      //for textures

uniform mat4 matrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main() {
   fragmentPosition = vec3(matrix * vec4(vertexPos, 1.0));
   normalTransposed = mat3(transpose(inverse(matrix))) * vertexNor;
   UV = vertexUV;
   gl_Position = pMatrix * vMatrix * matrix * vec4(vertexPos   , 1.0);
}
