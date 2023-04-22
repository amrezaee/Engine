#pragma once

#include <Common.hpp>
#include <RenderDevice.hpp>
#include <Renderer.hpp>
#include <Scene.hpp>
#include <SceneManager.hpp>
#include <Vector2.hpp>
#include <Window.hpp>

class Application
{
public:
	Application(String name, Path working_dir);
	virtual ~Application();

	void Run();  // Run the game loop
	void Terminate();

	RenderDevice& GetRenderDevice()
	{
		return *mRenderDevice;
	}
	Window& GetWindow()
	{
		return *mWindow;
	}
	Renderer& GetRenderer()
	{
		return *mRenderer;
	}
	SceneManager& GetSceneManager()
	{
		return mSceneManager;
	}

protected:
	// Don't call this methods in derived class!!
	virtual void Initialize() = 0;
	virtual void FixedUpdate(double fdt)
	{
		(void)fdt;
	}
	virtual void Update(double dt) = 0;
	// Called on exiting
	virtual void OnExit() {}
	// Called on gain/lose focus
	virtual void OnFocus(bool focus)
	{
		(void)focus;
	}
	// Called on Framebuffer resize
	virtual void OnResize(vec2ui resolution)
	{
		(void)resolution;
	}

private:
	bool OnWindowClose();
	bool OnWindowFocus(bool b);
	bool OnFramebuffer(vec2ui resolution);

private:
	String mName;
	Path   mWorkingDir;
	Scene* mScene {nullptr};

	bool mRunning {true};
	bool mFocus {true};

	// for lowering frame spikes impacts
	std::array<double, 10> mDeltaTimeArray {};

	const double mMinDeltaTime {0.000001};
	const double mMaxDeltaTime {0.250000};
	const u32    mMaxFixedIterations {8};
	double       mFixedDeltaTime {1.0 / 60.0};
	double       mDeltaTime {0.0};

protected:
	RenderDevicePtr     mRenderDevice;
	WindowPtr           mWindow;
	UniquePtr<Renderer> mRenderer;
	SceneManager        mSceneManager;
};

extern UniquePtr<Application> CreateApp();
