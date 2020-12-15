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

#include "game.h"

#include "dx11mathutil.h"
#include "shader.h"
#include "Player.h"
#include "MeshRenderer.h"
#include "CSkineMeshRenderer.h"
#include "Camera.h"
#include "CLight.h"

#include "ImGuiControl.h"
#include <math.h>
#include <vector>
#include "CPhysx.h"
#include "Title.h"

using namespace DirectX;
using namespace Egliss::ComponentSystem;

CCamera* _cama;
//******************************************************************************
//	End of file.
//******************************************************************************

bool CGame::Init()
{
	Egliss::Reflection::DynamicTypeManager::Initialize();
	AddObjects();
	return true;
}

void CGame::UnInit()
{
	for (auto item : m_obj_list) {
		item.Clear();
	}
	m_obj_list.clear();
	for (auto came : m_subCameras) {
		came.Clear();
	}
	m_subCameras.clear();
	m_mainCamera.Clear();
}

void CGame::Update()
{
	// ターゲットバッファクリア	
	float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; //red,green,blue,alpha

	// レンダリング前処理
	//DX11BeforeRender(ClearColor);
	DirectX11Manager::DrawBegin();

	//CDirectInput::GetInstance().GetKeyBuffer();			// キー入力取得


	for (auto item = m_obj_list.begin(); item != m_obj_list.end();) {
		(*item)->Update();
		if (!(*item)->GetLife()) {
			item->Clear();
			item = m_obj_list.erase(item);
			continue;
		}
		item++;
	}

	for (auto item2 : m_subCameras) {
		CCamera* caca = item2->GetComponent<CCamera>();
		CTransform* tra1 = item2->m_transform.GetPtr();
		CTransform* tra = m_active_obj->m_transform.GetPtr();
		//3dguizmの表示
		ImGuiControl::GetInstance()->Draw3DGuizm(tra1->m_mat,caca->GetProjectionMatrix(), tra->m_mat,tra->m_angle);

		item2->Update();
	}
	m_mainCamera->Update();

	for (auto item = m_obj_list.begin(); item != m_obj_list.end(); item++) {
		(*item)->LateUpdate();
	}
	//=============================デバッグモード用のGUI表示====================================================
	if(DirectX11Manager::input.Keyboard()->ChkKeyAction(DIK_Y))
	{
		ImGuiControl::GetInstance()->m_debugMode = !ImGuiControl::GetInstance()->m_debugMode;
	}

	if (ImGuiControl::GetInstance()->m_debugMode) {
		ImGuiControl::GetInstance()->HierarchyDraw(this);
		ImGuiControl::GetInstance()->InspectorDraw(m_active_obj, m_Components);
		ImGuiControl::GetInstance()->ConsoleDraw();
	}
	//==========================================================================================================

	if (DirectX11Manager::input.Keyboard()->ChkKeyAction(DIK_1))
	{
		m_activeCamera = m_mainCamera;
	}

	if (DirectX11Manager::input.Keyboard()->ChkKeyAction(DIK_2))
	{
		m_activeCamera = m_subCameras[0];
	}

	
	CPhysx::StepPhysics(60.0f);

	DirectX11Manager::input.InputUpdate();
}

void CGame::MainLoop()
{
	CGame::Update();
	CGame::Draw();
}

void CGame::Draw()
{
	// イミィディエイトコンテキスト
	ID3D11DeviceContext* devcontext = DirectX11Manager::m_pImContext.Get();
	if (m_activeCamera.IsExist()) {

		m_activeCamera->Draw();
	}
	for (auto item : m_obj_list) {
		item->Draw();
	}
	

	// レンダリング後処理
	DirectX11Manager::DrawEnd();
}


void CGame::AddObjects()
{

	sp<CObject> AirPlane;
	sp<CObject> CameraObj;
	sp<CObject> SubCamera;

	sp<CObject> sphere;

	sp<CObject> box;

	float _plane = 100.0f;
	float _color[4] = { 255.0f,255.0f,255.0f,255.0f };


	for (auto item : m_Components) {
		item->SetName();
	}

	CRigidbody* _rigid;
	CMeshRenderer* _render;
	CCamera* _camera;

		const std::string rigidName = "Rigidbody";
	//==========================================================================================
	//AddComponentはTransformから！！！！！！！！！！！！！！
	//その次にRigidbody
	//==========================================================================================


	SubCamera.SetPtr(new CObject);
	_cama = SubCamera->AddComponentByNameAs<CCamera>("Camera");
	SubCamera->SetName(std::string("SubCamera"));
	SubCamera->SetTag(std::string("Camera"));
	m_subCameras.emplace_back(SubCamera);


	CameraObj.SetPtr(new CObject);
	_camera = CameraObj->AddComponent<CCamera>();
	CameraObj->SetName(std::string("Camera"));
	CameraObj->SetTag(std::string("Camera"));
	sp<CObject>wark(CameraObj);
	m_mainCamera = wark;
	_camera->SetMainCameraFg(true);

	wp<CObject>wark_weak(CameraObj);
	m_activeCamera = wark_weak;


	AirPlane.SetPtr(new CObject);
	AirPlane->m_transform->SetPos(XMFLOAT3(0.0f, 60.0f, 0.0f));
	_rigid = AirPlane->AddComponentByNameAs<CRigidbody>(rigidName);
	AirPlane->AddComponent<CSkineMeshRenderer>();
	AirPlane->m_transform->SetScale(XMFLOAT3(3.f, 3.f, 3.f));
	_rigid->SetGeometryType(GEOMETRYTYPE::BOX);
	_rigid->InitDynamic();

	AirPlane->SetName(std::string("AirPlane"));
	AirPlane->SetTag(std::string("Player"));
	m_obj_list.emplace_back(AirPlane);

	_camera->SetTrackingObj(AirPlane->m_transform);

	sphere.SetPtr(new CObject);
	sphere->m_transform->SetPos(XMFLOAT3(30.0f, 30.0f, 0.0f));
	sphere->m_transform->SetScale(XMFLOAT3(5.f, 5.f,5.f ));
	_rigid = sphere->AddComponentByNameAs<CRigidbody>(rigidName);
	_render = sphere->AddComponent<CMeshRenderer>();
	_rigid->SetGeometryType(GEOMETRYTYPE::BOX);
	_rigid->InitDynamic();
	sphere->SetName(std::string("sphere"));
	sphere->SetTag(std::string("Default"));
	m_obj_list.emplace_back(sphere);


	box.SetPtr(new CObject);

	box->m_transform->SetPos(XMFLOAT3(0.0f, 0.0f, 0.0f));
	_rigid = box->AddComponent<CRigidbody>();
	_render = box->AddComponent<CMeshRenderer>();
	_render->BoxInit();
	box->m_transform->SetScale(XMFLOAT3(_plane, 5.0f, _plane));
	_rigid->InitStatic();
	box->SetName(std::string("box"));
	box->SetTag(std::string("Default"));
	m_obj_list.emplace_back(box);


	m_active_obj = AirPlane.GetPtr();
}
