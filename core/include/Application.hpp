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
	void Rerun();
	// void Tick();  // Run the game loop one time
	void Exit();  // Exit game

	Window&       GetWindow();
	Renderer&     GetRenderer();
	RenderDevice& GetRenderDevice();

protected:
	// Don't call this methods in derived class!!
	virtual void Initialize()    = 0;
	virtual void Draw(double dt) = 0;
	virtual void FixedUpdate(double fdt) { UNUSED(fdt); }
	virtual void Update(double dt) = 0;

	virtual void OnExit() {}                             // Called on exiting
	virtual void OnFocus(bool focus) { UNUSED(focus); }  // Called on gain/lose focus

private:
	bool OnWindowClose();
	bool OnWindowFocus(bool b);
	bool OnFramebuffer(Vec2ui resolution);

private:
	String          mName;
	Path            mWorkingDir;
	RenderDevicePtr mRenderDevice;
	WindowPtr       mWindow;
	Uptr<Renderer>  mRenderer;

	bool mRunning {true};
	bool mFocus {true};

	double mDeltaTime {0.0};
	double mMinDeltaTime {0.25};

	// for lowering framespikes impacts
	double mDeltaTimeArray[10] {};
	U32    mDeltaTimeIndex {0};

	double mFixedUpdateTime {0.016666};
	double mDeltaTimeAccumulator {0.0};
};

extern Uptr<Application> CreateApp();
