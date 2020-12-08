#include "Scene.h"

#include <windows.h>
#include <stdio.h>
#include <Crtdbg.h>
#include <DirectXMath.h>
#include "game.h"
#include "Title.h"
#include "shader.h"
#include "Player.h"
#include "MeshRenderer.h"
#include "Camera.h"

bool SceneManager::Init(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	CPhysx::Init();

	m_activeScene = sp<Scene>(new CGame());
	m_activeScene->Init();
	return	true;
}

void SceneManager::UnInit()
{
	m_activeScene.Clear();
}
