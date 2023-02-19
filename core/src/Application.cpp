#include <Application.hpp>

#include <MathFunctions.hpp>
#include <Timer.hpp>

Application::Application(const String& name, const Path& workig_dir)
        : mName(name), mWorkingDir(workig_dir)
{
}

Application::~Application()
{
}

void Application::Run()
{
	if(!mWorkingDir.empty() && fs::exists(mWorkingDir))
		fs::current_path(mWorkingDir);

	mRenderDevice = RenderDevice::Create();
	mWindow       = Window::Create(WindowSettings(mName, {960, 540}, {400, 200}));
	mRenderDevice->Initialize();
	mRenderer = MakeUptr<Renderer>(*mRenderDevice, 3072);

	mWindow->CloseSignal.Connect(this, &Application::OnWindowClose);
	mWindow->FocusSignal.Connect(this, &Application::OnWindowFocus);
	mWindow->FramebufferSignal.Connect(this, &Application::OnFramebuffer);

	Initialize();

	Timer timer;

	while(mRunning)
	{
		mDeltaTimeArray[mDeltaTimeIndex++] = Min(timer.Seconds(), mMinDeltaTime);
		mDeltaTimeIndex %= ArraySize(mDeltaTimeArray);
		mDeltaTime =
		        std::accumulate(mDeltaTimeArray,
		                        mDeltaTimeArray + ArraySize(mDeltaTimeArray), 0.0) /
		        ArraySize(mDeltaTimeArray);
		timer.Reset();

		mDeltaTimeAccumulator += mDeltaTime;

		mWindow->PollEvents();

		if(mFocus)
		{
			while(mDeltaTimeAccumulator >= mFixedUpdateTime)
			{
				FixedUpdate(mFixedUpdateTime);
				mDeltaTimeAccumulator -= mFixedUpdateTime;
			}

			Update(mDeltaTime);
			Draw(mDeltaTime);
		}

		mWindow->SwapBuffers();
	}

	OnExit();
}

void Application::Terminate()
{
	mWindow->CloseSignal.Disconnect(this, &Application::OnWindowClose);
	mWindow->FocusSignal.Disconnect(this, &Application::OnWindowFocus);
	mWindow->FramebufferSignal.Disconnect(this, &Application::OnFramebuffer);
}

void Application::Rerun()
{
	Terminate();
	Run();
}

void Application::Exit()
{
	mRunning = false;
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
	return true;
}
