#pragma once

#include <Shader.hpp>

#include <glad/gl.h>

class ShaderGL final: public Shader
{
public:
	ShaderGL(const Path& shaderfile);
	~ShaderGL();

	void Bind() const override;
	void Unbind() const override;

	void SetDouble(const String& name, double v) const override;
	void SetFloat(const String& name, float v) const override;
	void SetInt(const String& name, int v) const override;
	void SetUInt(const String& name, unsigned int v) const override;
	void SetColor(const String& name, Color color) const override;
	void SetTransform(const String& name, const Transform& transform) const override;

	U32 GetID() const override;

private:
	void ReadFile(const Path& shaderfile, String& source);
	void Preprocess(const String& source, String& vsout, String& fsout);
	void Compile(const String& vsource, const String& fsource);
	void Postprocess();

	GLenum ShaderTypeMap(ShaderType type);

private:
	U32                    mProgramID;
	HashMap<String, GLint> mUniformLocations;
};
