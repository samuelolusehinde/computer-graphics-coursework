#version 330 core
layout (location = 0) in vec3 inVertexPosition;
layout (location = 1) in vec3 inVertexNormal;
layout (location = 2) in vec2 inTextureCoordinate;
layout (location = 3) in vec3 inTangent;

out vec3 fragmentPosition;
out vec3 fragmentVertexNormal;
out vec2 fragmentTextureCoordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out mat3 TBN;


void main()
{
   fragmentPosition = vec3(model * vec4(inVertexPosition, 1.0));
   gl_Position = projection * view * model * vec4(inVertexPosition, 1.0f);
   
   fragmentVertexNormal = transpose(inverse(mat3(model))) *  inVertexNormal;
   fragmentTextureCoordinate = inTextureCoordinate;

   vec3 N = normalize(inVertexNormal);
   vec3 T = normalize(inTangent);
   vec3 B = normalize(cross(T, N));

   TBN = mat3(T, B, N);
}