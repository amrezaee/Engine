#include <Application.hpp>

#include <Timer.hpp>

Application::Application(String name, Path working_dir)
        : mName(std::move(name)), mWorkingDir(std::move(working_dir)),
          mSceneManager(this)
{
	if(!mWorkingDir.empty() && fs::exists(mWorkingDir))
		fs::current_path(mWorkingDir);
}

Application::~Application() = default;

void Application::Run()
{
	mWindow       = Window::Create(WindowSettings(mName));
	mRenderDevice = RenderDevice::Create();
	mRenderer     = MakeUnique<Renderer>(mRenderDevice, 2048);

	mWindow->CloseSignal.Connect(this, &Application::OnWindowClose);
	mWindow->FocusSignal.Connect(this, &Application::OnWindowFocus);
	mWindow->FramebufferSignal.Connect(this, &Application::OnFramebuffer);

	Initialize();

	Timer  timer;
	uword  fixed_iterations = 0;
	uword  index            = 0;
	double dt_accu          = 0.0;

	while(mRunning)
	{
		mDeltaTimeArray[index++] =
		        std::clamp(timer.Seconds(), mMinDeltaTime, mMaxDeltaTime);
		timer.Reset();
		index %= (uword)mDeltaTimeArray.size();
		mDeltaTime = std::accumulate(mDeltaTimeArray.cbegin(),
		                             mDeltaTimeArray.cend(), 0.0) /
		             (double)mDeltaTimeArray.size();
		dt_accu += mDeltaTime;

		mScene = mSceneManager.GetCurrent();

		mWindow->PollEvents();

		if(mFocus)
		{
			while((dt_accu >= mFixedDeltaTime) &&
			      (fixed_iterations++ < mMaxFixedIterations))
			{
				FixedUpdate(mFixedDeltaTime);
				mScene->FixedUpdate(mFixedDeltaTime);
				dt_accu -= mFixedDeltaTime;
			}
			fixed_iterations = 0;

			Update(mDeltaTime);
			mScene->Update(mDeltaTime);
			mScene->Render(dt_accu / mFixedDeltaTime);
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
	mScene->Resize(resolution);
	OnResize(resolution);
	return true;
}
