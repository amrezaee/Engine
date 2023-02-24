#include <Application.hpp>

#include <MathFunctions.hpp>
#include <Timer.hpp>

Application::Application(const String& name, const Path& workig_dir)
        : mName(name), mWorkingDir(workig_dir)
{
	if(!mWorkingDir.empty() && fs::exists(mWorkingDir))
		fs::current_path(mWorkingDir);
}

Application::~Application()
{
}

void Application::Run()
{
	mRenderDevice = RenderDevice::Create();
	mWindow       = Window::Create(WindowSettings(mName));
	mRenderDevice->Initialize();
	mRenderer = MakeUptr<Renderer>(*mRenderDevice, 2048);

	mWindow->CloseSignal.Connect(this, &Application::OnWindowClose);
	mWindow->FocusSignal.Connect(this, &Application::OnWindowFocus);
	mWindow->FramebufferSignal.Connect(this, &Application::OnFramebuffer);

	Initialize();

	Timer timer;
	U32   fixed_iterations = 0;

	while(mRunning)
	{
		mDeltaTimeArray[mDeltaTimeIndex++] = Min(timer.Seconds(), mMinDeltaTime);
		timer.Reset();
		mDeltaTimeIndex %= ArraySize(mDeltaTimeArray);
		mDeltaTime =
		        std::accumulate(mDeltaTimeArray,
		                        mDeltaTimeArray + ArraySize(mDeltaTimeArray), 0.0) /
		        ArraySize(mDeltaTimeArray);

		mDeltaTimeAccumulator += mDeltaTime;

		mWindow->PollEvents();

		if(mFocus)
		{
			while((mDeltaTimeAccumulator >= mFixedDeltaTime) &&
			      (fixed_iterations++ < mMaxFixedIterations))
			{
				FixedUpdate(mFixedDeltaTime);
				mDeltaTimeAccumulator -= mFixedDeltaTime;
			}
			fixed_iterations = 0;

			Update(mDeltaTime);
		}

		mWindow->SwapBuffers();
	}

	OnExit();
}

void Application::Terminate()
{
	mRunning = false;

	mWindow->CloseSignal.Disconnect(this, &Application::OnWindowClose);
	mWindow->FocusSignal.Disconnect(this, &Application::OnWindowFocus);
	mWindow->FramebufferSignal.Disconnect(this, &Application::OnFramebuffer);
}

Window& Application::GetWindow()
{
	return *mWindow;
}

Renderer& Application::GetRenderer()
{
	return *mRenderer;
}

RenderDevice& Application::GetRenderDevice()
{
	return *mRenderDevice;
}

bool Application::OnWindowClose()
{
	mRunning = false;
	return true;
}

bool Application::OnWindowFocus(bool b)
{
	OnFocus(b);
	mFocus = b;
	return true;
}

bool Application::OnFramebuffer(Vec2ui resolution)
{
	mRenderDevice->UpdateViewport(0, 0, resolution.x, resolution.y);
	OnResize(resolution);
	return true;
}
