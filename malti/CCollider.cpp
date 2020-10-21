#include "CCollider.h"
#include "CPhysx.h"
#include <cassert>

void CCollider::Start()
{
	//rigidbodyがないと異常終了する
	m_rb = Holder->GetComponent<CRigidbody>();
	assert(m_rb != nullptr);
	m_transform= Holder->GetComponent<CTransform>();

	SetName();
}

void CCollider::Init()
{
	m_scale = XMFLOAT3(1.f, 1.f, 1.f);
	m_angle = XMFLOAT3(0.f, 0.f, 0.f);
}

void CBoxCollider::Start()
{
	CCollider::Start();
}

void CBoxCollider::Draw()
{
	DX11MtxIdentity(m_mat);
	XMFLOAT4X4 scaleMatrix;
	XMFLOAT4X4 rotMatrix;
	XMFLOAT4X4 positionMatrix;

	XMFLOAT4X4 objMatrix;
	XMFLOAT4X4 inverseMatrix;

	XMFLOAT3 local_scale = XMFLOAT3(m_scale.x*m_transform->GetScale().x, m_scale.y*m_transform->GetScale().y, m_scale.z*m_transform->GetScale().z);
	XMFLOAT3 local_pos = XMFLOAT3(m_ofset.x + m_transform->GetDirectPos().x, m_ofset.y + m_transform->GetDirectPos().y, m_ofset.z + m_transform->GetDirectPos().z);
	DX11MtxScale(local_scale, scaleMatrix);
	DX11MtxTranslation(m_ofset, positionMatrix);
	DX11MtxRotation(m_angle, rotMatrix);
	DX11MtxScale(m_transform->GetDirectScale(), inverseMatrix);
	DX11MtxInverse(inverseMatrix, inverseMatrix);
	// ワールド変換行列
	DX11MtxMultiply(m_mat, scaleMatrix, rotMatrix, positionMatrix);
	//スケールの逆行列をかけて親の行列をスケール抜きのものにしている
	DX11MtxMultiply(objMatrix, inverseMatrix, m_transform->m_mat);
	//子の行列を生成
	DX11MtxMultiply(m_mat, m_mat, objMatrix);

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_mat);
	TurnWire();
	m_box->Draw();
	TurnSolid();
}


void CBoxCollider::ImGuiDraw()
{
	ImGui::DragFloat3(u8"スケール", &m_scale.x, true);
	ImGui::DragFloat3(u8"オフセット", &m_ofset.x, true);
	physx::PxVec3 Scale = physx::PxVec3(m_scale.x*m_transform->GetScale().x, m_scale.y*m_transform->GetScale().y, m_scale.z*m_transform->GetScale().z);
	m_shape->setGeometry(physx::PxBoxGeometry(Scale / 2.f));
}

void CBoxCollider::Init()
{
	CCollider::Init();
	const physx::PxMaterial* mmm = m_rb->GetMaterial();
	float  ss = mmm->getRestitution();
	m_shape = CPhysx::GetPhysics()->createShape(physx::PxBoxGeometry(m_transform->GetScale() / 2.f),*m_rb->GetMaterial(), true);

	PxShape* shapes[128];
	//シェイプの数取得
	const PxU32 nbShapes = m_rb->GetActor()->getNbShapes();
	m_rb->GetActor()->getShapes(shapes, nbShapes);
	m_rb->GetActor()->detachShape(*shapes[0]);
	m_rb->GetActor()->attachShape(*m_shape);
	m_box = new CBox();
	m_box->Init(XMFLOAT3(1.f, 1.f, 1.f));
	

	m_shape->setFlag(PxShapeFlag::eVISUALIZATION, false);
}

CBoxCollider::~CBoxCollider()
{
	m_box->Exit();
	m_shape->release();
}

void CSphereCollider::Start()
{
	CCollider::Start();
	m_radius = 1.0f;
}

void CSphereCollider::Init()
{
	CCollider::Init();
	m_shape = CPhysx::GetPhysics()->createShape(physx::PxSphereGeometry(m_transform->GetScale().x), *m_rb->GetMaterial(), true);

	PxShape* shapes[128];
	//シェイプの数取得
	const PxU32 nbShapes = m_rb->GetActor()->getNbShapes();
	m_rb->GetActor()->getShapes(shapes, nbShapes);
	m_rb->GetActor()->detachShape(*shapes[0]);

	m_rb->GetActor()->attachShape(*m_shape);

	m_sphere = new CSphere();
	m_sphere->Init(1.f);
}

CSphereCollider::~CSphereCollider()
{
	m_sphere->Exit();
	m_shape->release();
}

void CSphereCollider::Draw()
{
	DX11MtxIdentity(m_mat);
	XMFLOAT4X4 scaleMatrix;
	XMFLOAT4X4 rotMatrix;
	XMFLOAT4X4 positionMatrix;

	XMFLOAT4X4 objMatrix;
	XMFLOAT4X4 inverseMatrix;

	XMFLOAT3 local_scale = XMFLOAT3(m_radius*m_transform->GetScale().x, m_radius*m_transform->GetScale().y, m_radius*m_transform->GetScale().z);
	XMFLOAT3 local_pos = XMFLOAT3(m_ofset.x + m_transform->GetDirectPos().x, m_ofset.y + m_transform->GetDirectPos().y, m_ofset.z + m_transform->GetDirectPos().z);
	DX11MtxScale(local_scale, scaleMatrix);
	DX11MtxTranslation(m_ofset, positionMatrix);
	DX11MtxRotation(m_angle, rotMatrix);
	DX11MtxScale(m_transform->GetDirectScale(), inverseMatrix);
	DX11MtxInverse(inverseMatrix, inverseMatrix);
	// ワールド変換行列
	DX11MtxMultiply(m_mat, scaleMatrix, rotMatrix, positionMatrix);
	//スケールの逆行列をかけて親の行列をスケール抜きのものにしている
	DX11MtxMultiply(objMatrix, inverseMatrix, m_transform->m_mat);
	//子の行列を生成
	DX11MtxMultiply(m_mat, m_mat, objMatrix);

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_mat);
	TurnWire();
	m_sphere->Draw();
	TurnSolid();
}


void CSphereCollider::ImGuiDraw()
{
	ImGui::DragFloat(u8"半径", &m_radius, true);
	ImGui::DragFloat3(u8"オフセット", &m_ofset.x, true);
	
	m_shape->setGeometry(physx::PxSphereGeometry(m_radius*m_transform->GetScale().x));


}
