#pragma once

#include <Shader.hpp>

class ShaderGL final: public Shader
{
public:
	explicit ShaderGL(const Path& shaderfile);
	~ShaderGL() override;

	void Bind() const override;
	void Unbind() const override;

	void SetDouble(const String& name, double v) const override;
	void SetFloat(const String& name, float v) const override;
	void SetInt(const String& name, int v) const override;
	void SetUInt(const String& name, unsigned int v) const override;
	void SetColor(const String& name, Color color) const override;
	void SetTransform(const String& name, const Transform& transform) const override;

	u32 GetID() const override;

private:
	static void ReadFile(const Path& shaderfile, String& source);
	static void Preprocess(const String& source, String& vsout, String& fsout);
	void        Compile(const String& vsource, const String& fsource);
	void        Postprocess();

	static GLenum ShaderTypeMap(ShaderType type);

private:
	u32                    mProgramID;
	HashMap<String, GLint> mUniformLocations;
};
