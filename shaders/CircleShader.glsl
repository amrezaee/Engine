#type vertex
#version 460 core

layout(location = 0) in vec2 aWorldPosition;
layout(location = 1) in vec2 aLocalPosition;
layout(location = 2) in vec4 aColor;
layout(location = 3) in float aThickness;
layout(location = 4) in float aFade;

uniform mat3x2 uViewProjection;

struct VertexOutput
{
	vec2 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout (location = 0) out VertexOutput Output;

void main()
{
	Output.LocalPosition = aLocalPosition;
	Output.Color = aColor;
	Output.Thickness = aThickness;
	Output.Fade = aFade;

	gl_Position = vec4(uViewProjection * vec3(aWorldPosition, 1.0f), 0.0f, 1.0f);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 outColor;

struct VertexOutput
{
	vec2 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout (location = 0) in VertexOutput Input;

void main()
{
    float distance = 1.0 - length(Input.LocalPosition);
    float circle = smoothstep(0.0, Input.Fade, distance);
    circle *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

	if (circle <= 0.0)
		discard;

    outColor = Input.Color;
	outColor.a *= circle;
}