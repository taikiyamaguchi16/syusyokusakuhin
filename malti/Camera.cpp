#include "Camera.h"
#include "CDirectInput.h"

void CCamera::Start()
{
	m_name = "Camera";
	if (m_pos == nullptr)
		m_pos = Holder->GetComponent<CTransform>();

	Init(1.0f, 10000.0f, XM_PIDIV2, SCREEN_X, SCREEN_Y, m_eye, m_lookat, m_up);
	m_pos->SetPos(XMFLOAT3(0, 0, -30.0f));
	m_radius = 30.0f;
}

void CCamera::Update()
{
	//�ݒ肳�ꂽ�I�u�W�F�N�g�ɒǔ�
	if (m_trackingFg) {
		
		if (m_trackingObj != nullptr) {
			//UpdateSphereCamera(XMFLOAT3(m_trackingObj->m_mat._41, m_trackingObj->m_mat._42, m_trackingObj->m_mat._43), m_elevation, m_azimuth);

			CCamera::SetLookat(m_trackingObj->GetDirectPos());
			//m_pos->SetPos(CCamera::GetEye());
			//SetEye(m_pos->m_position);

			XMFLOAT4X4 forward_;
			//===================================���ʕ����̃x�N�g��===================
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
		float speed = 2.0f;
		//�E�N���b�N�����Ȃ��炾�Ɠ����悤�ɂ��Ă���
		if (CDirectInput::GetInstance().GetMouseRButtonCheck()) {
			
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)) {
				inputSpeed.z = speed;
			}
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {
				inputSpeed.x = -speed;
			}
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S)) {
				inputSpeed.z = -speed;
			}
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D)) {
				inputSpeed.x = speed;
			}
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_Q)) {
				inputSpeed.y = -speed;
			}
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_E)) {
				inputSpeed.y = speed;
			}
			m_pos->m_angle.y += CDirectInput::GetInstance().GetMouseAxisX() / 5;
			m_pos->m_angle.x += CDirectInput::GetInstance().GetMouseAxisY() / 5;

		}
		m_pos->MoveForward(inputSpeed);
		//=========================�����_��i�s�����̐�ɂ��Ă���=========================
		SetEye(m_pos->GetDirectPos());
		XMFLOAT4X4 forward_;
		DX11MtxTranslation(XMFLOAT3(0, 0, 10.0f), forward_);
		DX11MtxMultiply(forward_, forward_, m_pos->m_mat);
		SetLookat(XMFLOAT3(forward_._41, forward_._42, forward_._43));
		SetUp(XMFLOAT3(forward_._21, forward_._22, forward_._23));
		CreateCameraMatrix();
		//================================================================================
	}
}

void CCamera::Draw()
{

	m_pos->m_mat = GetCameraMatrix();

	// �r���[�ϊ��s��Z�b�g
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, m_pos->m_mat);

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, m_projection);

}

void CCamera::ImGuiDraw()
{
	ImGui::DragFloat(u8"���ʊp", &m_azimuth, true);
	ImGui::DragFloat(u8"�p", &m_elevation, true);
	ImGui::DragFloat(u8"���a", &m_radius, true);
	if (ImGui::Checkbox(u8"�ǐ�", &m_trackingFg)) {
		m_freeCameraFg = !m_freeCameraFg;
	}
	if (ImGui::Checkbox(u8"�t���[�J����", &m_freeCameraFg)) {
		m_trackingFg = !m_trackingFg;
	}
	
}


