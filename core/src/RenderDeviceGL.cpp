#include <RenderDeviceGL.hpp>

#include <Assert.hpp>
#include <Logger.hpp>

void RenderDeviceGL::Initialize()
{
	TRACE("RenderDevice initializing...");

	int s = gladLoadGL(glfwGetProcAddress);
	ASSERT(s != 0, "OpenGL initialization failed");

	int mj = GLAD_VERSION_MAJOR(s);
	int mi = GLAD_VERSION_MINOR(s);

	ASSERT(mj == 4 && mi == 6, "OpenGL 4.6 required");

#ifdef ENGINE_DEBUG_BUILD
	void GLDebugOutput(GLenum, GLenum, GLuint, GLenum, GLsizei length, const char*,
	                   const void*);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GLDebugOutput, nullptr);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
	                      0, nullptr, GL_FALSE);
#endif  // ENGINE_DEBUG_BUILD

	{
		String r                  = (const char*)glGetString(GL_RENDERER);
		size_t spos               = r.find(' ');
		mInfo.Name                = r.substr(spos + 1);
		mInfo.Vendor              = r.substr(0, spos);
		mInfo.DriverVersion.major = mj;
		mInfo.DriverVersion.minor = mi;
		mInfo.ASTC                = GLAD_GL_KHR_texture_compression_astc_ldr;
		mInfo.S3TC                = GLAD_GL_EXT_texture_compression_s3tc;
		mInfo.ETC1                = GLAD_GL_OES_compressed_ETC1_RGB8_texture;
		mInfo.ETC2                = GLAD_GL_ARB_ES3_compatibility;
		mInfo.PVRTC               = GLAD_GL_IMG_texture_compression_pvrtc;
		mInfo.BC4BC5 = mInfo.DriverVersion.major >= 3;  // supported in opengl 3.0+
		mInfo.BC6HBC7 =
		        GLAD_GL_ARB_texture_compression_bptc;  // supported in opengl 4.2+

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (int*)&mInfo.NumTextureUnits);
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, (int*)&mInfo.MaxTextureWidth);
		mInfo.MaxTextureHeight = mInfo.MaxTextureWidth;
		glGetIntegerv(GL_MAX_SAMPLES, (int*)&mInfo.NumSamples);

		INFO("Render API: OpenGL");
		INFO("Render API Ver.: {}", (const char*)glGetString(GL_VERSION));
		INFO("Render API Shader Ver.: {}",
		     (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
		INFO("Render Device: {}", mInfo.Name);
		INFO("Render Device Vendor: {}", mInfo.Vendor);
		INFO("Max Texture Size: {}x{}", mInfo.MaxTextureWidth,
		     mInfo.MaxTextureHeight);
		INFO("Number of Texture Units: {}", mInfo.NumTextureUnits);
		INFO("Number of Samples: {}", mInfo.NumSamples);

		INFO("ASTC  Support: {}", mInfo.ASTC);
		INFO("S3TC  Support: {}", mInfo.S3TC);
		INFO("ETC1  Support: {}", mInfo.ETC1);
		INFO("ETC2  Support: {}", mInfo.ETC2);
		INFO("PVRTC Support: {}", mInfo.PVRTC);
		INFO("BC4_5 Support: {}", mInfo.BC4BC5);
		INFO("BC6_7 Support: {}", mInfo.BC6HBC7);
	}

	EnableBlending(true);
	SetBlendFunc(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);
	glEnable(GL_LINE_SMOOTH);

	TRACE("RenderDevice initialized");
}

void RenderDeviceGL::SetClearColor(Color color)
{
	float c[4];
	color.GetColors(c);
	glClearColor(c[0], c[1], c[2], c[3]);
}

void RenderDeviceGL::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderDeviceGL::EnableBlending(bool enable)
{
	if(enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}

void RenderDeviceGL::SetBlendFunc(BlendFunc src, BlendFunc dst, Color color)
{
	GLenum sfactor = GLenum(BlendFuncMap(src));
	GLenum dfactor = GLenum(BlendFuncMap(dst));

	glBlendFunc(sfactor, dfactor);

	float c[4];
	color.GetColors(c);
	glBlendColor(c[0], c[1], c[2], c[3]);
}

bool RenderDeviceGL::IsBlendingEnable() const
{
	return glIsEnabled(GL_BLEND) == GL_TRUE;
}

void RenderDeviceGL::UpdateViewport(uword x, uword y, uword width, uword height)
{
	glViewport(GLint(x), GLint(y), GLsizei(width), GLsizei(height));
}

void RenderDeviceGL::SetPointSize(float size)
{
	glPointSize(size);
}

void RenderDeviceGL::DrawIndexed(const VertexArrayPtr& va, uword index_count)
{
	uword count = index_count ? index_count : va->GetIndexBuffer()->GetCount();
	va->Bind();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

word RenderDeviceGL::BlendFuncMap(BlendFunc func)
{
	switch(func)
	{
	case BlendFunc::Zero: return GL_ZERO;
	case BlendFunc::One: return GL_ONE;
	case BlendFunc::SrcColor: return GL_SRC_COLOR;
	case BlendFunc::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
	case BlendFunc::DstColor: return GL_DST_COLOR;
	case BlendFunc::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
	case BlendFunc::SrcAlpha: return GL_SRC_ALPHA;
	case BlendFunc::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
	case BlendFunc::DstAlpha: return GL_DST_ALPHA;
	case BlendFunc::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
	case BlendFunc::ConstantColor: return GL_CONSTANT_COLOR;
	case BlendFunc::OneMinusConstantColor: return GL_ONE_MINUS_CONSTANT_COLOR;
	case BlendFunc::ConstantAlpha: return GL_CONSTANT_ALPHA;
	case BlendFunc::OneMinusConstantAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
	default: return -1;
	}
}

#ifdef ENGINE_DEBUG_BUILD
void GLDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                   GLsizei length, const char* message, const void* userparam)
{
	(void)userparam;
	(void)length;
	(void)id;

	const char *src, *tp;
	switch(source)
	{
	case GL_DEBUG_SOURCE_API: src = "API"; break;
	case GL_DEBUG_SOURCE_OTHER: src = "Other Sources"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: src = "Third Party Software"; break;
	case GL_DEBUG_SOURCE_APPLICATION: src = "Application"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: src = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: src = "Shader Compiler"; break;
	default: src = "Unknown"; break;
	}

	switch(type)
	{
	case GL_DEBUG_TYPE_ERROR: tp = "Error"; break;
	case GL_DEBUG_TYPE_OTHER: tp = "Other Type"; break;
	case GL_DEBUG_TYPE_MARKER: tp = "Marker"; break;
	case GL_DEBUG_TYPE_POP_GROUP: tp = "Pop Group"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP: tp = "Push Group"; break;
	case GL_DEBUG_TYPE_PORTABILITY: tp = "Portability Issue"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: tp = "Performance Issue"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: tp = "Undefined Behavior"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: tp = "Deprecated Behavior"; break;
	default: tp = "Unknown"; break;
	}

	switch(severity)
	{
	case GL_DEBUG_SEVERITY_HIGH: FATAL("GL {} in {}: {}", tp, src, message); return;
	case GL_DEBUG_SEVERITY_MEDIUM:
		ERROR("GL {} in {}: {}", tp, src, message);
		return;
	case GL_DEBUG_SEVERITY_LOW: WARN("GL {} in {}: {}", tp, src, message); return;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		TRACE("GL {} in {}: {}", tp, src, message);
		return;
	}
}
#endif  // ENGINE_DEBUG_BUILD
