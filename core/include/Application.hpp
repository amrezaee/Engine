#pragma once

#include <Common.hpp>
#include <RenderDevice.hpp>
#include <Renderer.hpp>
#include <Vector2.hpp>
#include <Window.hpp>

class Application
{
public:
	Application(const String& name, const Path& workig_dir);
	virtual ~Application();

	void Run();  // Run the game loop
	void Terminate();

	Window&       GetWindow();
	Renderer&     GetRenderer();
	RenderDevice& GetRenderDevice();

protected:
	// Don't call this methods in derived class!!
	virtual void Initialize() = 0;
	virtual void FixedUpdate(double fdt) { (void)fdt; }
	virtual void Update(double dt) = 0;

	virtual void OnExit() {}                           // Called on exiting
	virtual void OnFocus(bool focus) { (void)focus; }  // Called on gain/lose focus
	// Called on Framebuffer resize
	virtual void OnResize(Vec2ui resolution) { (void)resolution; }

private:
	bool OnWindowClose();
	bool OnWindowFocus(bool b);
	bool OnFramebuffer(Vec2ui resolution);

private:
	String              mName;
	Path                mWorkingDir;
	RenderDevicePtr     mRenderDevice;
	WindowPtr           mWindow;
	UniquePtr<Renderer> mRenderer;

	bool mRunning {true};
	bool mFocus {true};

	double mDeltaTime {0.0};
	double mMaxDeltaTime {0.25};

	// for lowering framespikes impacts
	double mDeltaTimeArray[10] {};

	double mFixedDeltaTime {1.0 / 60.0};
	uword  mMaxFixedIterations {8};
	double mDeltaTimeAccumulator {0.0};
};

extern UniquePtr<Application> CreateApp();
