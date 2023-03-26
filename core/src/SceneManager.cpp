#include <SceneManager.hpp>

#include <Logger.hpp>
#include <Scene.hpp>

SceneManager::SceneManager(Application* app): mApp {app}, mCurrent {nullptr}
{
}

void SceneManager::Add(UniquePtr<Scene> scene)
{
	if(scene == nullptr)
		WARN("Scene is null");

	auto it = std::find(mScenes.cbegin(), mScenes.cend(), scene);
	if(it == mScenes.cend())
	{
		scene->SetApp(mApp);
		scene->Initialize();
		mScenes.push_back(std::move(scene));
	}
	else
		WARN("Scene already exists");
}

void SceneManager::Remove(UniquePtr<Scene> scene)
{
	if(scene == nullptr)
		WARN("Scene is null");

	auto it = std::find(mScenes.cbegin(), mScenes.cend(), scene);
	if(it != mScenes.cend())
		mScenes.erase(it);
	else
		WARN("Scene does not exist");
}

bool SceneManager::Switch(const String& name)
{
	return std::any_of(mScenes.cbegin(), mScenes.cend(),
	                   [this, &name](auto& s) -> bool
	                   {
		                   if(s->GetName() == name)
		                   {
			                   this->mCurrent = s.get();
			                   return true;
		                   }
		                   return false;
	                   });
}
