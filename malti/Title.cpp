#include "Title.h"

#include "Camera.h"
#include "game.h"

using namespace Egliss::ComponentSystem;

CCamera* _camaa;

bool Title::Init()
{
	AddObjects();
	return true;
}

void Title::UnInit()
{
	for (auto item : m_obj_list) {
		item.Clear();
	}
	m_obj_list.clear();
	for (auto came : m_subCameras) {
		came.Clear();
	}
	m_subCameras.clear();
}

void Title::MainLoop()
{
	Update();
	Draw();
}

void Title::Update()
{
	//CDirectInput::GetInstance().GetKeyBuffer();			// キー入力取得
	//for (auto item = m_obj_list.begin(); item != m_obj_list.end();) {
	//	(*item)->Update();
	//	if (!(*item)->GetLife()) {
	//		//delete item;
	//		item->Clear();
	//		item = m_obj_list.erase(item);
	//		continue;
	//	}
	//	item++;
	//}

	//for (auto item2 : m_subCameras) {
	//	item2->Update();
	//}

	//if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_L)) {
	//	// レンダリング後処理
	//	DX11AfterRender();
	//	SceneManager::GetInstance()->ChangeScene(sp<Scene>(new CGame()));
	//}
	//CDirectInput::GetInstance().GetMouseState();
}

void Title::Draw()
{

	// ターゲットバッファクリア	
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

}

void Title::AddObjects()
{
	sp<CObject> AirPlane;
	sp<CObject> SkyDome;
	sp<CObject> CameraObj;
	sp<CObject> SubCamera;

	float _plane = 600.0f;
	float _color[4] = { 255.0f,255.0f,255.0f,255.0f };


	CCamera* _camera;

	CTransform* _trans;
	//==========================================================================================
	//AddComponentはTransformから！！！！！！！！！！！！！！
	//その次にRigidbody
	//==========================================================================================


	CameraObj.SetPtr(new CObject);
	CameraObj->AddComponent<CTransform>();
	_camaa = _camera = CameraObj->AddComponent<CCamera>();
	CameraObj->SetName(std::string("Camera"));
	CameraObj->SetTag(std::string("Camera"));

	m_subCameras.emplace_back(CameraObj);
	_camera->SetMainCameraFg(true);


	AirPlane.SetPtr(new CObject);
	_trans = AirPlane->AddComponent<CTransform>();
	_trans->SetPos(XMFLOAT3(60.0f, 100.0f, 0));
	AirPlane->SetMyFps(60);
	
	AirPlane->SetName(std::string("AirPlane"));
	AirPlane->SetTag(std::string("Player"));
	m_obj_list.emplace_back(AirPlane);



	m_active_obj = AirPlane;
}
