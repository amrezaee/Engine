#type vertex
#version 460 core

// Vertex Attributes
layout (location = 0) in vec2  aPosition;
layout (location = 1) in vec2  aUV;
layout (location = 2) in vec4  aColor;
layout (location = 3) in float aTexID;

// View-Projection matrix
uniform mat3x2 uViewProjection;

struct VertexOutput
{
	vec2  UV;
	vec4  Color;
	float TexID;
};

layout (location = 0) out VertexOutput Output;

void main()
{
	Output.UV = aUV;
	Output.Color = aColor;
	Output.TexID = aTexID;

	gl_Position = vec4(uViewProjection * vec3(aPosition, 1.0f), 0.0f, 1.0f);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D uTextures[32];

struct VertexOutput
{
	vec2  UV;
	vec4  Color;
	float TexID;
};

layout (location = 0) in VertexOutput Input;

void main()
{
	switch(int(Input.TexID))
	{
	case 0: outColor =  Input.Color * texture(uTextures[0],  Input.UV); break;
	case 1: outColor =  Input.Color * texture(uTextures[1],  Input.UV); break;
	case 2: outColor =  Input.Color * texture(uTextures[2],  Input.UV); break;
	case 3: outColor =  Input.Color * texture(uTextures[3],  Input.UV); break;
	case 4: outColor =  Input.Color * texture(uTextures[4],  Input.UV); break;
	case 5: outColor =  Input.Color * texture(uTextures[5],  Input.UV); break;
	case 6: outColor =  Input.Color * texture(uTextures[6],  Input.UV); break;
	case 7: outColor =  Input.Color * texture(uTextures[7],  Input.UV); break;
	case 8: outColor =  Input.Color * texture(uTextures[8],  Input.UV); break;
	case 9: outColor =  Input.Color * texture(uTextures[9],  Input.UV); break;
	case 10: outColor = Input.Color * texture(uTextures[10], Input.UV); break;
	case 11: outColor = Input.Color * texture(uTextures[11], Input.UV); break;
	case 12: outColor = Input.Color * texture(uTextures[12], Input.UV); break;
	case 13: outColor = Input.Color * texture(uTextures[13], Input.UV); break;
	case 14: outColor = Input.Color * texture(uTextures[14], Input.UV); break;
	case 15: outColor = Input.Color * texture(uTextures[15], Input.UV); break;
	case 16: outColor = Input.Color * texture(uTextures[16], Input.UV); break;
	case 17: outColor = Input.Color * texture(uTextures[17], Input.UV); break;
	case 18: outColor = Input.Color * texture(uTextures[18], Input.UV); break;
	case 19: outColor = Input.Color * texture(uTextures[19], Input.UV); break;
	case 20: outColor = Input.Color * texture(uTextures[20], Input.UV); break;
	case 21: outColor = Input.Color * texture(uTextures[21], Input.UV); break;
	case 22: outColor = Input.Color * texture(uTextures[22], Input.UV); break;
	case 23: outColor = Input.Color * texture(uTextures[23], Input.UV); break;
	case 24: outColor = Input.Color * texture(uTextures[24], Input.UV); break;
	case 25: outColor = Input.Color * texture(uTextures[25], Input.UV); break;
	case 26: outColor = Input.Color * texture(uTextures[26], Input.UV); break;
	case 27: outColor = Input.Color * texture(uTextures[27], Input.UV); break;
	case 28: outColor = Input.Color * texture(uTextures[28], Input.UV); break;
	case 29: outColor = Input.Color * texture(uTextures[29], Input.UV); break;
	case 30: outColor = Input.Color * texture(uTextures[30], Input.UV); break;
	case 31: outColor = Input.Color * texture(uTextures[31], Input.UV); break;
	}
}