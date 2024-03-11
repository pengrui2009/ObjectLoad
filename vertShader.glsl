#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normal;

uniform mat4 model_matrix;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec3 FragPos;
out vec2 TextCoord;
out vec3 FragNormal;
//out vec2 tc;

layout(binding = 0) uniform sampler2D samp;

void main()
{
	gl_Position = proj_matrix * mv_matrix * model_matrix * vec4(position, 1.f);
	FragPos = vec3(model_matrix * vec4(position, 1.0)); // 在世界坐标系中指定片元位置
	TextCoord = texCoords;
	mat3 normalMatrix = mat3(transpose(inverse(model_matrix)));
	FragNormal = normalMatrix * normal; // 计算法向量经过模型变换后值
}

