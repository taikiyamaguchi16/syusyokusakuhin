#include "Scene.h"

#include <windows.h>
#include <stdio.h>
#include <Crtdbg.h>
#include <DirectXMath.h>
#include "DX11util.h"
#include "CModel.h"
#include "game.h"
#include "Title.h"
#include "CDirectInput.h"
#include "DX11Settransform.h"
#include "dx11mathutil.h"
#include "shader.h"
#include "Player.h"
#include "MeshRenderer.h"
#include "Camera.h"

bool SceneManager::Init(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	CPhysx::Init();
	bool		sts;

	
	sts = DX11SetTransform::GetInstance()->Init();
	if (!sts) {
		MessageBox(NULL, "SetTransform error", "Error", MB_OK);
		return false;
	}

	m_activeScene = sp<Scene>(new CGame());
	return	true;
}

void SceneManager::UnInit()
{
	m_activeScene.Clear();
}
