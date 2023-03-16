#pragma once

#include <Common.hpp>

class Scene;
class Application;

class SceneManager
{
public:
	SceneManager(Application* app);
	SceneManager(const SceneManager&)            = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	void   Add(UniquePtr<Scene> scene);
	void   Remove(UniquePtr<Scene> scene);
	bool   Switch(const String& name);
	Scene* GetCurrent() { return mCurrent; }

private:
	Application*           mApp;
	List<UniquePtr<Scene>> mScenes;
	Scene*                 mCurrent;
};
