#include <ShaderGL.hpp>

#include <Assert.hpp>
#include <Logger.hpp>

ShaderGL::ShaderGL(const Path& shaderfile): mProgramID(0)
{
	String source;
	String vsource;
	String fsource;

	ReadFile(shaderfile, source);
	Preprocess(source, vsource, fsource);
	Compile(vsource, fsource);
	Postprocess();
}

ShaderGL::~ShaderGL()
{
	glDeleteProgram(mProgramID);
}

void ShaderGL::Bind() const
{
	glUseProgram(mProgramID);
}

void ShaderGL::Unbind() const
{
	glUseProgram(0);
}

void ShaderGL::SetDouble(const String& name, double v) const
{
	auto it = mUniformLocations.find(name);
	ASSERT(it != mUniformLocations.cend());

	glProgramUniform1d(mProgramID, it->second, v);
}

void ShaderGL::SetFloat(const String& name, float v) const
{
	auto it = mUniformLocations.find(name);
	ASSERT(it != mUniformLocations.cend());

	glProgramUniform1f(mProgramID, it->second, v);
}

void ShaderGL::SetInt(const String& name, int v) const
{
	auto it = mUniformLocations.find(name);
	ASSERT(it != mUniformLocations.cend());

	glProgramUniform1i(mProgramID, it->second, v);
}

void ShaderGL::SetUInt(const String& name, unsigned int v) const
{
	auto it = mUniformLocations.find(name);
	ASSERT(it != mUniformLocations.cend());

	glProgramUniform1ui(mProgramID, mUniformLocations.at(name), v);
}

void ShaderGL::SetColor(const String& name, Color color) const
{
	auto it = mUniformLocations.find(name);
	ASSERT(it != mUniformLocations.cend());

	float t[4];
	color.GetColors(t);
	glProgramUniform4fv(mProgramID, it->second, 4, t);
}

void ShaderGL::SetTransform(const String& name, const Transform& transform) const
{
	auto it = mUniformLocations.find(name);
	ASSERT(it != mUniformLocations.cend());

	glProgramUniformMatrix3x2fv(mProgramID, it->second, 1, GL_TRUE,
	                            transform.GetPtr());
}

U32 ShaderGL::GetID() const
{
	return mProgramID;
}

void ShaderGL::ReadFile(const Path& shaderfile, String& source)
{
	std::ifstream in(shaderfile, std::ios::ate);

	if(in)
	{
		size_t size = in.tellg();
		if(size != -1)
		{
			source.resize(size);
			in.seekg(0, std::ios::beg);
			in.read(&source[0], size);
		}
		else
		{
			ERROR("Could not read from file {}",
			      shaderfile.filename().generic_string());
			return;
		}
	}
	else
	{
		ERROR("Could not open file {}", shaderfile.filename().generic_string());
		return;
	}
}

void ShaderGL::Preprocess(const String& source, String& vsout, String& fsout)
{
	std::regex vsregex("\\s*#\\s*type\\s+vertex\\s*\\r?\\n");
	std::regex fsregex("\\s*#\\s*type\\s+fragment\\s*\\r?\\n");

	std::smatch vsmatch;
	std::smatch fsmatch;

	if(!std::regex_search(source, vsmatch, vsregex))
	{
		ERROR("\"#type vertex\" specifier not found.");
		return;
	}

	if(!std::regex_search(source, fsmatch, fsregex))
	{
		ERROR("\"#type fragment\" specifier not found.");
		return;
	}

	size_t vpos = vsmatch.position(0);
	size_t fpos = fsmatch.position(0);
	if(vpos < fpos)
	{
		vsout = source.substr(vpos + vsmatch.length(0),
		                      fpos - (vpos + vsmatch.length(0)));
		fsout = fsmatch.suffix();
	}
	else
	{
		fsout = source.substr(fpos + fsmatch.length(0),
		                      vpos - (fpos + fsmatch.length(0)));
		vsout = vsmatch.suffix();
	}
}

void ShaderGL::Compile(const String& vsource, const String& fsource)
{
	GLuint vsid = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsid = glCreateShader(GL_FRAGMENT_SHADER);
	mProgramID  = glCreateProgram();

	const char* vsrc_ptr = vsource.c_str();
	const char* fsrc_ptr = fsource.c_str();

	glShaderSource(vsid, 1, &vsrc_ptr, nullptr);
	glCompileShader(vsid);

	glShaderSource(fsid, 1, &fsrc_ptr, nullptr);
	glCompileShader(fsid);

	glAttachShader(mProgramID, vsid);
	glAttachShader(mProgramID, fsid);

	glLinkProgram(mProgramID);
	glDeleteShader(vsid);
	glDeleteShader(fsid);
}

void ShaderGL::Postprocess()
{
	GLint count = 0;
	glGetProgramInterfaceiv(mProgramID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &count);
	const GLenum properties[3] = {GL_BLOCK_INDEX, GL_NAME_LENGTH, GL_LOCATION};

	for(U32 i = 0; i < U32(count); ++i)
	{
		GLint values[3];
		glGetProgramResourceiv(mProgramID, GL_UNIFORM, i, 3, properties, 3, nullptr,
		                       values);

		// Skip any uniforms that are in a block.
		if(values[0] != -1)
			continue;

		String name;
		name.resize(size_t(values[1]) - 1);  // resize to retrieved size minus 1
		// because we don't consider null termination
		glGetProgramResourceName(mProgramID, GL_UNIFORM, i, size_t(values[1]),
		                         nullptr, &name[0]);
		mUniformLocations[name] = values[2];
	}
}

GLenum ShaderGL::ShaderTypeMap(ShaderType type)
{
	switch(type)
	{
	case ShaderType::Vertex: return GL_FRAGMENT_SHADER;
	case ShaderType::Fragment: return GL_VERTEX_SHADER;
	default: return 0;
	}
}
