//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	ゲーム処理
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <Crtdbg.h>
#include <DirectXMath.h>
#include "DX11util.h"
#include "CModel.h"
#include "game.h"
#include "CDirectInput.h"
#include "DX11Settransform.h"
#include "dx11mathutil.h"
#include "shader.h"
#include "Player.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "CLight.h"

#include "ImGuiControl.h"
#include <math.h>
#include <vector>
#include "CPhysx.h"
#include "Title.h"

#include "CObject2D.h"
using namespace DirectX;


CCamera* _cama;
//******************************************************************************
//	End of file.
//******************************************************************************

bool CGame::Init()
{
	AddObjects();
	return true;
}

void CGame::UnInit()
{
	for (auto item : m_obj_list) {
		delete item;
	}
	m_obj_list.clear();
	for (auto came : m_subCameras) {
		delete came;
	}
	m_subCameras.clear();
	delete m_mainCamera;
}

void CGame::Update()
{
	// ターゲットバッファクリア	
	float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; //red,green,blue,alpha

	// レンダリング前処理
	DX11BeforeRender(ClearColor);


	CDirectInput::GetInstance().GetKeyBuffer();			// キー入力取得

	for (auto item = m_obj_list.begin(); item != m_obj_list.end();) {
		(*item)->Update();
		if (!(*item)->GetLife()) {
			delete *item;
			item = m_obj_list.erase(item);
			continue;
		}
		item++;
	}

	for (auto item2 : m_subCameras) {
		CCamera* caca = item2->GetComponent<CCamera>();
		CTransform* tra1 = item2->GetComponent<CTransform>();
		CTransform* tra = m_active_obj->GetComponent<CTransform>();

		ImGuiControl::GetInstance()->Draw3DGuizm(tra1->m_mat,caca->GetProjectionMatrix(), tra->m_mat,tra->m_angle);

		item2->Update();
	}
	m_mainCamera->Update();

	for (auto item = m_obj_list.begin(); item != m_obj_list.end(); item++) {
		(*item)->LateUpdate();
	}
	//=============================デバッグモード用のGUI表示====================================================

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_Y))
	{
		ImGuiControl::GetInstance()->m_debugMode = !ImGuiControl::GetInstance()->m_debugMode;
	}
	if (ImGuiControl::GetInstance()->m_debugMode) {
		ImGuiControl::GetInstance()->HierarchyDraw(this);
		ImGuiControl::GetInstance()->InspectorDraw(m_active_obj, m_Components);
		ImGuiControl::GetInstance()->ConsoleDraw();
	}
	//==========================================================================================================
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_1)) {
		m_activeCamera = m_mainCamera;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_2)) {
		m_activeCamera = m_subCameras[0];
	}

	CPhysx::StepPhysics(60.0f);

	CDirectInput::GetInstance().GetMouseState();
}

void CGame::MainLoop()
{
	CGame::Update();
	CGame::Draw();
}

void CGame::Draw()
{
	// イミィディエイトコンテキスト
	ID3D11DeviceContext* devcontext = GetDX11DeviceContext();

	for (auto item : m_obj_list) {
		item->Draw();
	}

	m_activeCamera->Draw();

	// レンダリング後処理
	DX11AfterRender();
}


void CGame::AddObjects()
{
	CObject* AirPlane;
	CObject* SkyDome;
	CObject* CameraObj;
	CObject* SubCamera;

	CObject* enemy;
	CObject* sphere;
	CObject* sphere2;
	CObject* box;
	CObject* box2;
	CObject* box3;
	CObject* box4;
	CObject* box5;

	float _plane = 600.0f;
	float _color[4] = { 255.0f,255.0f,255.0f,255.0f };

	m_Components.emplace_back(createInstance<CPlayer>());
	m_Components.emplace_back(createInstance<CRigidbody>());
	m_Components.emplace_back(createInstance<CMeshRenderer>());

	for (auto item : m_Components) {
		item->SetName();
	}

	CTransform* _trans;
	CRigidbody* _rigid;
	CMeshRenderer* _render;
	CCamera* _camera;
	//==========================================================================================
	//AddComponentはTransformから！！！！！！！！！！！！！！
	//その次にRigidbody
	//==========================================================================================
	/*SkyDome = new CObject;
	SkyDome->AddComponent<CTransform>();
	_render = SkyDome->AddComponent<CMeshRenderer>();
	_render->SetModel("skydome");
	SkyDome->SetName(std::string("SkyDome"));
	SkyDome->SetTag(std::string("Default"));
	m_obj_list.emplace_back(SkyDome);
*/

	SubCamera = new CObject;
	SubCamera->AddComponent<CTransform>();
	_cama = SubCamera->AddComponent<CCamera>();
	SubCamera->AddComponent<CLight>();
	SubCamera->SetName(std::string("SubCamera"));
	SubCamera->SetTag(std::string("Camera"));
	m_subCameras.emplace_back(SubCamera);


	CameraObj = new CObject;
	CameraObj->AddComponent<CTransform>();
	_camera = CameraObj->AddComponent<CCamera>();
	CameraObj->AddComponent<CLight>();
	CameraObj->SetName(std::string("Camera"));
	CameraObj->SetTag(std::string("Camera"));
	m_mainCamera = CameraObj;
	_camera->SetMainCameraFg(true);
	m_activeCamera = CameraObj;


	AirPlane = new CObject;
	_trans = AirPlane->AddComponent<CTransform>();
	_trans->SetPos(XMFLOAT3(60.0f, 100.0f, 0));
	_rigid = AirPlane->AddComponent<CRigidbody>();
	_rigid->SetMass(0.1f);
	AirPlane->AddComponent<CPlayer>();
	AirPlane->SetMyFps(60);
	_render = AirPlane->AddComponent<CMeshRenderer>();
	_trans->SetScale(XMFLOAT3(30.f, 30.f, 30.f));
	_render->BoxInit(XMFLOAT3(3.0f, 3.0f, 3.0f));
	_rigid->InitDynamic();

	AirPlane->SetName(std::string("AirPlane"));
	AirPlane->SetTag(std::string("Player"));
	m_obj_list.emplace_back(AirPlane);

	_camera->SetTrackingObj(_trans);

	//enemy = new CObject;
	//enemy->AddComponent<CTransform>();
	//auto com3 = enemy->AddComponent<CMeshRenderer>();
	////com3->LoadModel("assets/TIE_Fighter.x.dat", "shader/vs.fx", "shader/ps.fx");
	//com3->SetModel("TIE_Fighter");
	//enemy->SetName(std::string("enemy"));
	//enemy->SetTag(std::string("Default"));
	//m_obj_list.emplace_back(enemy);


	sphere = new CObject;
	_trans = sphere->AddComponent<CTransform>();
	_trans->SetPos(XMFLOAT3(15.0f, 20.0f, 0.0f));
	_trans->SetScale(XMFLOAT3(5.f, 5.f,5.f ));
	_rigid = sphere->AddComponent<CRigidbody>();
	_render = sphere->AddComponent<CMeshRenderer>();
	_render->SphereInit(3.0f);
	_rigid->SetGeometryType(GEOMETRYTYPE::SPHERE);
	_rigid->InitDynamic();
	sphere->SetName(std::string("sphere"));
	sphere->SetTag(std::string("Default"));
	m_obj_list.emplace_back(sphere);


	/*sphere2 = new CObject;
	_trans = sphere2->AddComponent<CTransform>();
	_trans->SetPos(XMFLOAT3(15.0f, 20.0f, 0.0f));
	_rigid = sphere2->AddComponent<CRigidbody>();
	_render = sphere2->AddComponent<CMeshRenderer>();
	_render->SphereInit(8.0f);
	_rigid->SetRadius(8.0f);
	_rigid->InitDynamic();
	sphere2->SetName(std::string("sphere2"));
	sphere2->SetTag(std::string("Default"));
	m_obj_list.emplace_back(sphere2);*/


	box = new CObject;
	_trans = box->AddComponent<CTransform>();
	_trans->SetPos(XMFLOAT3(0.0f, 0.0f, 0.0f));
	_rigid = box->AddComponent<CRigidbody>();
	_render = box->AddComponent<CMeshRenderer>();
	//_rigid->SetBoxSize(XMFLOAT3(_plane, 5.0f, _plane));
	_trans->SetScale(XMFLOAT3(_plane, 5.0f, _plane));
	_render->SetColor(255.0f, 0.0f, 0.0f, 255.0f);
	_rigid->InitStatic();
	box->SetName(std::string("box"));
	box->SetTag(std::string("Default"));
	m_obj_list.emplace_back(box);



	box2 = new CObject;
	_trans = box2->AddComponent<CTransform>();
	_trans->SetPos(XMFLOAT3(_plane / 2.0f, _plane / 2.0f, .0f));
	_rigid = box2->AddComponent<CRigidbody>();
	_render = box2->AddComponent<CMeshRenderer>();
	//_rigid->SetBoxSize(XMFLOAT3(5.0f, _plane, _plane));
	_trans->SetScale(XMFLOAT3(5.0f, _plane, _plane));
	_render->SetColor(0.0f, 255.0f, 0.0f, 255.0f);
	_rigid->InitStatic();
	box2->SetName(std::string("box2"));
	box2->SetTag(std::string("Default"));
	m_obj_list.emplace_back(box2);


	box3 = new CObject;
	_trans = box3->AddComponent<CTransform>();
	_trans->SetPos(XMFLOAT3(-_plane / 2.0f, _plane / 2.0f, .0f));
	_rigid = box3->AddComponent<CRigidbody>();
	_render = box3->AddComponent<CMeshRenderer>();
	//_rigid->SetBoxSize(XMFLOAT3(5.0f, _plane, _plane));
	_trans->SetScale(XMFLOAT3(5.0f, _plane, _plane));
	_render->SetColor(0.0f, 0.0f, 255.0f, 255.0f);
	_rigid->InitStatic();
	box3->SetName(std::string("box3"));
	box3->SetTag(std::string("Default"));
	m_obj_list.emplace_back(box3);


	box4 = new CObject;
	_trans = box4->AddComponent<CTransform>();
	_trans->SetPos(XMFLOAT3(0.0f, _plane / 2.0f, _plane / 2.0f));
	_rigid = box4->AddComponent<CRigidbody>();
	_render = box4->AddComponent<CMeshRenderer>();
	//_rigid->SetBoxSize(XMFLOAT3(_plane, _plane, 5.0f));
	_trans->SetScale(XMFLOAT3(_plane, _plane, 5.0f));
	_render->SetColor(255.0f, 255.0f, 0.0f, 255.0f);
	_rigid->InitStatic();
	box4->SetName(std::string("box4"));
	box4->SetTag(std::string("Default"));
	m_obj_list.emplace_back(box4);


	box5 = new CObject;
	_trans = box5->AddComponent<CTransform>();
	_trans->SetPos(XMFLOAT3(0.0f, _plane / 2.0f, -_plane / 2.0f));
	_rigid = box5->AddComponent<CRigidbody>();
	_render = box5->AddComponent<CMeshRenderer>();
	//_rigid->SetBoxSize(XMFLOAT3(_plane, _plane, 5.0f));
	_trans->SetScale(XMFLOAT3(_plane, _plane, 5.0f));
	_render->SetColor(255.0f, 0.0f, 255.0f, 255.0f);
	_rigid->InitStatic();
	box5->SetName(std::string("box5"));
	box5->SetTag(std::string("Default"));
	m_obj_list.emplace_back(box5);


	m_active_obj = AirPlane;
}
