#include "Camera.h"
#include "CDirectInput.h"
#include "DirectX11Manager.h"

#include "UnityExportModel.h"
#include  "UnityExportSkinnedModel.h"

using namespace Egliss::ComponentSystem;


void CCamera::Start()
{
	m_name = "Camera";
	if (!m_pos.IsExist())
		m_pos = Holder->GetWeakComponent<CTransform>();

	Init(1.0f, 10000.0f, XM_PIDIV2, SCREEN_X, SCREEN_Y, m_eye, m_lookat, m_up);
	m_pos->SetPos(XMFLOAT3(0, 0, -30.0f));
	m_radius = 30.0f;

	//==========--コンスタントバッファー生成======================================================================================

	

	XMVECTOR eyePos = XMVectorSet(m_eye.x, m_eye.y, m_eye.z, 0);
	XMVECTOR targetPos = XMVectorSet(m_lookat.x, m_lookat.y, m_lookat.z, 0.f);
	XMVECTOR upVector = XMVectorSet(m_up.x, m_up.y, m_up.z, 0);
	DirectX11Manager::m_constantBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(eyePos, targetPos, upVector));


	//===========================================================================================================================
}

void CCamera::Update()
{
	//設定されたオブジェクトに追尾
	if (m_trackingFg) {
		
		if (m_trackingObj.IsExist()) {
			//UpdateSphereCamera(XMFLOAT3(m_trackingObj->m_mat._41, m_trackingObj->m_mat._42, m_trackingObj->m_mat._43), m_elevation, m_azimuth);

			CCamera::SetLookat(m_trackingObj->GetDirectPos());
			//m_pos->SetPos(CCamera::GetEye());
			//SetEye(m_pos->m_position);

			XMFLOAT4X4 forward_;
			//===================================正面方向のベクトル===================
			DX11MtxTranslation(XMFLOAT3(0, 30.0f, -40.0f), forward_);
			DX11MtxMultiply(forward_, forward_, m_trackingObj->m_mat);
			//========================================================================
			SetEye(XMFLOAT3(forward_._41, forward_._42, forward_._43));
			SetUp(XMFLOAT3(forward_._21, forward_._22, forward_._23));
			m_pos->SetPos(CCamera::GetEye());
			CreateCameraMatrix();
		}
	}
	else if(m_freeCameraFg)
	{
		XMFLOAT3 inputSpeed = XMFLOAT3(0.0f, 0.0f, 0.0f);
		float speed = 0.5f;
		//右クリック押しながらだと動くようにしている
		if (DirectX11Manager::input.Mouse()->IsRDown()) {
			
			if (DirectX11Manager::input.Keyboard()->ChkKeyDown(DIK_W)) {
				inputSpeed.z = speed;
			}
			if (DirectX11Manager::input.Keyboard()->ChkKeyDown(DIK_A)) {
				inputSpeed.x = -speed;
			}
			if (DirectX11Manager::input.Keyboard()->ChkKeyDown(DIK_S)) {
				inputSpeed.z = -speed;
			}
			if (DirectX11Manager::input.Keyboard()->ChkKeyDown(DIK_D)) {
				inputSpeed.x = speed;
			}
			if (DirectX11Manager::input.Keyboard()->ChkKeyDown(DIK_Q)) {
				inputSpeed.y = -speed;
			}
			if (DirectX11Manager::input.Keyboard()->ChkKeyDown(DIK_E)) {
				inputSpeed.y = speed;
			}
			m_pos->m_angle.y += DirectX11Manager::input.Mouse()->MoveX() / 2;
			m_pos->m_angle.x += DirectX11Manager::input.Mouse()->MoveY() / 2;

		}
		m_pos->MoveForward(inputSpeed);
		//=========================注視点を進行方向の先にしている=========================
		SetEye(m_pos->GetDirectPos());
		XMFLOAT4X4 forward_;
		DX11MtxTranslation(XMFLOAT3(0, 0, 10.0f), forward_);
		DX11MtxMultiply(forward_, forward_, m_pos->m_mat);
		SetLookat(XMFLOAT3(forward_._41, forward_._42, forward_._43));
		SetUp(XMFLOAT3(forward_._21, forward_._22, forward_._23));
		CreateCameraMatrix();
		CreateProjectionMatrix();
		//================================================================================


		DirectX11Manager::m_constantBuffer.view = XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_camera));
		DirectX11Manager::m_constantBuffer.proj = XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_projection));

	}

}

void CCamera::Draw()
{

	m_pos->m_mat = GetCameraMatrix();

	// ビュー変換行列セット
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, m_pos->m_mat);

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, m_projection);

}

void CCamera::ImGuiDraw()
{
	ImGui::DragFloat(u8"方位角", &m_azimuth, true);
	ImGui::DragFloat(u8"仰角", &m_elevation, true);
	ImGui::DragFloat(u8"半径", &m_radius, true);
	if (ImGui::Checkbox(u8"追跡", &m_trackingFg)) {
		m_freeCameraFg = !m_freeCameraFg;
	}
	if (ImGui::Checkbox(u8"フリーカメラ", &m_freeCameraFg)) {
		m_trackingFg = !m_trackingFg;
	}
	
}


