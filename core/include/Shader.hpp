#pragma once

#include <Color.hpp>
#include <Common.hpp>
#include <Transform.hpp>

class Shader;
using ShaderPtr = SharedPtr<Shader>;

enum class ShaderType
{
	Vertex,
	Fragment
};

class Shader
{
public:
	virtual ~Shader() = default;

	static ShaderPtr Create(const Path& path);

	virtual void Bind() const   = 0;
	virtual void Unbind() const = 0;

	virtual void SetDouble(const String& name, double v) const     = 0;
	virtual void SetFloat(const String& name, float v) const       = 0;
	virtual void SetInt(const String& name, int v) const           = 0;
	virtual void SetUInt(const String& name, unsigned int v) const = 0;
	virtual void SetColor(const String& name, Color color) const   = 0;
	virtual void SetTransform(const String&    name,
	                          const Transform& transform) const    = 0;

	virtual uword GetID() const = 0;
};
