#include "Player.h"
#include "CDirectInput.h"
#include  "CPhysx.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "Bullet.h"
using namespace Dix;
void CPlayer::Start()
{
	SetName();
	if (m_pos.IsExist() == NULL)
		m_pos = Holder->GetWeakComponent<CTransform>();
	
	m_rb = Holder->GetWeakComponent<CRigidbody>();

	//m_rb->SetRigid(m_controller->getActor());
	//CPhysx::SetActor(m_rb->GetRigidDynamic());
	//m_rb->SetActor(CPhysx::GetActor());


}

void CPlayer::Move()
{
	//インプット情報が取得されたら
	bool inputFg = false;
	bool inputAngleFg = false;
	float speed = 15.5f;

	//移動ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	XMFLOAT3 inputSpeed = XMFLOAT3(0.0f, 0.0f, 0.0f);

	CDirectInput::GetInstance().GetMouseState();
	if (!CDirectInput::GetInstance().GetMouseRButtonCheck()) {
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)){
			inputSpeed.z = speed;
			
			inputFg = true;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)){
			inputSpeed.x = -speed;
			
			inputFg = true;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S)){
			inputSpeed.z = -speed;
			
			inputFg = true;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D)){
			inputSpeed.x = speed;
			
			inputFg = true;
		}
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
			inputSpeed.y = 500.0f;
			inputFg = true;
		}


	}
	//--------------------------------------------------------------------------------------

	if (CDirectInput::GetInstance().GetMouseLButtonTrigger()) {
		//Shot();
	}
	//----------------------------回転-----------------------------------------------------
	
	//m_pos->m_angle.y += CDirectInput::GetInstance().GetMouseAxisX()/5.0f;
	//m_pos->m_angle.x += CDirectInput::GetInstance().GetMouseAxisY()/5.0f;

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP)) {
		m_pos->m_angle.y += 5.f;
	}
	//---------------------------------------------------------------------------------------
	//入力された場合
		if (m_rb.IsExist()) {
			if (m_rb->GetRigidDynamic() != nullptr) {
				if (m_pos->m_angle.x < -40.0f) {
					m_pos->m_angle.x = -40.0f;
				}
				if (m_pos->m_angle.x > 35.0f) {
					m_pos->m_angle.x = 35.0f;
				}
				XMFLOAT3 _vx, _vz;

				//ローカル軸基準でいどうするため（Y軸考慮すると飛んじゃう）
				_vx.x = m_zikuX.x*inputSpeed.x;
				_vx.y = 0;
				_vx.z = m_zikuX.z*inputSpeed.x;

				_vz.x = m_zikuZ.x*inputSpeed.z;
				_vz.y = 0;
				_vz.z = m_zikuZ.z*inputSpeed.z;

				inputSpeed = XMFLOAT3(_vx.x + _vz.x, _vx.y + _vz.y+inputSpeed.y, _vx.z + _vz.z);
				m_rb->GetRigidDynamic()->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z );
				if (m_rb->GetRigidDynamic()->getLinearVelocity().x > 15.0f) {
					inputSpeed.x = 0;
				}
				if (m_rb->GetRigidDynamic()->getLinearVelocity().z > 15.0f) {
					inputSpeed.x = 0;
				}
				m_rb->GetRigidDynamic()->addForce(physx::PxVec3(inputSpeed.x, inputSpeed.y, inputSpeed.z),physx::PxForceMode::eFORCE);
			}
		}
	
	
}

void CPlayer::Update()
{
	m_zikuX = XMFLOAT3(m_pos->m_mat._11, m_pos->m_mat._12, m_pos->m_mat._13);
	m_zikuY = XMFLOAT3(m_pos->m_mat._21, m_pos->m_mat._22, m_pos->m_mat._23);
	m_zikuZ = XMFLOAT3(m_pos->m_mat._31, m_pos->m_mat._32, m_pos->m_mat._33);
	DX11Vec3Normalize(m_zikuX, m_zikuX);
	DX11Vec3Normalize(m_zikuY, m_zikuY);
	DX11Vec3Normalize(m_zikuZ, m_zikuZ);

	Move();	
}

void CPlayer::ImGuiDraw()
{

}

void CPlayer::Shot()
{
	CObject* bullet=new CObject;
	bullet = new CObject;
	auto _trans = bullet->AddComponent<CTransform>();
	_trans->SetPos(m_pos->GetDirectPos());
	auto _rigid = bullet->AddComponent<CRigidbody>();
	auto _render = bullet->AddComponent<CMeshRenderer>();
	bullet->AddComponent<Bullet>();
	_render->SphereInit(3.0f);
	_rigid->InitDynamic();
	bullet->SetName(std::string("bullet"));
	bullet->SetTag(std::string("Default"));
	SceneManager::GetInstance()->AddObject(bullet);

	XMFLOAT3 _vy, _vz;
	m_zikuX = XMFLOAT3(m_pos->m_mat._11, m_pos->m_mat._12, m_pos->m_mat._13);
	m_zikuY = XMFLOAT3(m_pos->m_mat._21, m_pos->m_mat._22, m_pos->m_mat._23);
	m_zikuZ = XMFLOAT3(m_pos->m_mat._31, m_pos->m_mat._32, m_pos->m_mat._33);
	DX11Vec3Normalize(m_zikuX, m_zikuX);
	DX11Vec3Normalize(m_zikuY, m_zikuY);
	DX11Vec3Normalize(m_zikuZ, m_zikuZ);

	//ローカル軸基準でいどうするため（Y軸考慮すると飛んじゃう）

	_vz.x = m_zikuZ.x*8000.0f;
	_vz.y = m_zikuZ.y*8000.0f;
	_vz.z = m_zikuZ.z*8000.0f;

	_rigid->GetRigidDynamic()->addForce(physx::PxVec3(_vz.x, _vz.y, _vz.z));
}
