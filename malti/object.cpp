#include "object.h"
#include  "Scene.h"
#include "UserData.h"
#include "nameof.hpp"
#include "ImGuiControl.h"
#include "CCollider.h"

using namespace Egliss::ComponentSystem;

void CObject::Update()
{
	if (SceneManager::GetInstance()->m_fpsCount % (60 / m_myFps) == 0) {
		for (const auto& com : m_ComponentList)
			com->Update();
	}
}

void CObject::LateUpdate()
{
	if (SceneManager::GetInstance()->m_fpsCount % (60 / m_myFps) == 0) {
		for (const auto& com : m_ComponentList)
			com->LateUpdate();
	}
}

void CObject::Draw()
{
	for (const auto& com : m_ComponentList)
		com->Draw();
}

void CTransform::Update()
{
	DX11MtxIdentity(m_mat);
	XMFLOAT4X4 scaleMatrix;
	XMFLOAT4X4 velcityMatrix;
	XMFLOAT4X4 rotMatrix;
	XMFLOAT4X4 positionMatrix;

	DX11MtxScale(m_scale, scaleMatrix);
	DX11MtxTranslation(m_trans->p, positionMatrix);
	DX11MtxRotation(m_angle, rotMatrix);
	//Rigidbody�����Ă���ꍇ
	if (m_rb.IsExist()) {

		DX11MtxMultiply(m_mat, scaleMatrix, velcityMatrix, rotMatrix, positionMatrix);

	}
	else {
		DX11MtxMultiply(m_mat, scaleMatrix, rotMatrix, positionMatrix);
	}

	if (m_rb.IsExist()) {
		if (m_rb->GetRigidDynamic() != nullptr || m_rb->GetRigidStatic() != nullptr) {
			XMFLOAT4X4 workmat;
			//�V�F�C�v����s������o���ꍇ�͂�����
			//PxShape* shapes[128];
			////�V�F�C�v�̐��擾
			//const PxU32 nbShapes = m_rb->GetActor()->getNbShapes();
			//m_rb->GetActor()->getShapes(shapes, nbShapes);
			//const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[0], *m_rb->GetActor()));

			const PxMat44 shapePose(m_rb->GetActor()->getGlobalPose());
			workmat._11 = shapePose.column0.x; workmat._12 = shapePose.column0.y; workmat._13 = shapePose.column0.z; workmat._14 = shapePose.column0.w;
			workmat._21 = shapePose.column1.x; workmat._22 = shapePose.column1.y; workmat._23 = shapePose.column1.z; workmat._24 = shapePose.column1.w;
			workmat._31 = shapePose.column2.x; workmat._32 = shapePose.column2.y; workmat._33 = shapePose.column2.z; workmat._34 = shapePose.column2.w;
			workmat._41 = shapePose.column3.x; workmat._42 = shapePose.column3.y; workmat._43 = shapePose.column3.z; workmat._44 = shapePose.column3.w;
			//�X�P�[����������
			DX11MtxMultiply(m_mat, scaleMatrix, workmat);
			//QuaternionToEulerAngles(m_angle, m_trans->q.getNormalized());
		}
		else {
		}
	}
	//���W�̉�����
	m_trans->p.x = m_mat._41;
	m_trans->p.y = m_mat._42;
	m_trans->p.z = m_mat._43;



	//�e�������ꍇ
	if (m_parent != nullptr) {
		DX11MtxMultiply(m_mat, m_mat, m_parent->m_mat);
	}
}

void CTransform::LateUpdate()
{
	//���W�̉�����
	m_trans->p.x = m_mat._41;
	m_trans->p.y = m_mat._42;
	m_trans->p.z = m_mat._43;

	XMFLOAT3 _scale;
	XMFLOAT3 _vec = XMFLOAT3(m_mat._11, m_mat._12, m_mat._13);
	DX11Vec3Length(_vec, _scale.x);

	_vec = XMFLOAT3(m_mat._21, m_mat._22, m_mat._23);
	DX11Vec3Length(_vec, _scale.y);

	_vec = XMFLOAT3(m_mat._31, m_mat._32, m_mat._33);
	DX11Vec3Length(_vec, _scale.z);

	m_scale = _scale;
}

void CTransform::MoveForward(XMFLOAT3 vec_)
{
	DX11MtxIdentity(m_mat);

	XMFLOAT4X4 scaleMatrix;
	XMFLOAT4X4 velcityMatrix;
	XMFLOAT4X4 rotMatrix;
	XMFLOAT4X4 positionMatrix;

	DX11MtxScale(m_scale, scaleMatrix);
	DX11MtxTranslation(m_trans->p, positionMatrix);
	DX11MtxRotation(m_angle, rotMatrix);

	DX11MtxTranslation(vec_, velcityMatrix);
	DX11MtxMultiply(m_mat, scaleMatrix, velcityMatrix, rotMatrix, positionMatrix);

	m_trans->p.x = m_mat._41;
	m_trans->p.y = m_mat._42;
	m_trans->p.z = m_mat._43;
}

void CTransform::Draw()
{

	//���W�̍ŏI����
	m_trans->p.x = m_mat._41;
	m_trans->p.y = m_mat._42;
	m_trans->p.z = m_mat._43;
	// ���[���h�ϊ��s��
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_mat);
}

void CTransform::Rotation(XMFLOAT4X4 & rotateMat_)
{
	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	// X�������o��
	axisX.x = rotateMat_._11;
	axisX.y = rotateMat_._12;
	axisX.z = rotateMat_._13;
	axisX.w = 0.0f;

	// Y�������o��
	axisY.x = rotateMat_._21;
	axisY.y = rotateMat_._22;
	axisY.z = rotateMat_._23;
	axisY.w = 0.0f;

	// Z�������o��
	axisZ.x = rotateMat_._31;
	axisZ.y = rotateMat_._32;
	axisZ.z = rotateMat_._33;
	axisZ.w = 0.0f;


	XMFLOAT4 qt;	// �N�I�[�^�j�I��

		// �s�񂩂�N�I�[�^�j�I���𐶐�
	DX11GetQtfromMatrix(rotateMat_, qt);

	XMFLOAT4 qtx, qty, qtz;		// �N�I�[�^�j�I��

	// �w�莲��]�̃N�I�[�^�j�I���𐶐�
	DX11QtRotationAxis(qtx, axisX, m_angle.x);
	DX11QtRotationAxis(qty, axisY, m_angle.y);
	DX11QtRotationAxis(qtz, axisZ, m_angle.z);

	// �N�I�[�^�j�I��������
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qtx);

	XMFLOAT4 tempqt2;
	DX11QtMul(tempqt2, qty, qtz);

	XMFLOAT4 tempqt3;
	DX11QtMul(tempqt3, tempqt1, tempqt2);

	// �N�I�[�^�j�I�����m�[�}���C�Y
	DX11QtNormalize(tempqt3, tempqt3);

	// �N�I�[�^�j�I������s����쐬
	DX11MtxFromQt(rotateMat_, tempqt3);

	// �s�񂩂�N�I�[�^�j�I���𐶐�
	DX11GetQtfromMatrix(rotateMat_, qt);
	// �N�I�[�^�j�I�����m�[�}���C�Y
	DX11QtNormalize(qt, qt);

	m_trans->q = physx::PxQuat(qt.x, qt.y, qt.z, qt.w);

}

void CTransform::Rotation(XMFLOAT3 _angle)
{
	physx::PxTransform _p = m_rb->GetRigidDynamic()->getGlobalPose();
	physx::PxQuat _q;
	//EulerAnglesToQuaternion();
	EulerAnglesToQuaternion(_q, _angle);
	m_trans->q = _q * _p.q;
	m_trans->q.normalize();
	m_rb->GetActor()->setGlobalPose(physx::PxTransform(GetPhysXPos(), physx::PxQuat(m_trans->q.x, m_trans->q.y, m_trans->q.z, m_trans->q.w)));
}

void CTransform::ImGuiDraw()
{
	ImGui::DragFloat3(u8"���W", &m_trans->p.x, true);
	ImGui::DragFloat3(u8"�p�x", &m_angle.x, true);
	//if (m_rb != nullptr) {
	if(m_rb.IsExist()){
		if (m_rb->GetRigidDynamic() != nullptr) {
			XMFLOAT4X4 rot;
			DX11MtxIdentity(rot);
			Rotation(rot);
			m_rb->GetActor()->setGlobalPose(physx::PxTransform(GetPhysXPos(), physx::PxQuat(m_trans->q.x, m_trans->q.y, m_trans->q.z, m_trans->q.w)),true);
		}
	}
	ImGui::DragFloat3(u8"�T�C�Y", &m_scale.x, true);
	ImGuiControl::GetInstance()->Select3DGuizm();

}

void CTransform::SetQuat(XMFLOAT4X4 & rotateMat_)
{
	/*XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;*/

	// X�������o��
	
	XMFLOAT4 qt;	// �N�I�[�^�j�I��

		// �s�񂩂�N�I�[�^�j�I���𐶐�
	DX11GetQtfromMatrix(rotateMat_, qt);


	// �N�I�[�^�j�I�����m�[�}���C�Y
	DX11QtNormalize(qt, qt);

	// �N�I�[�^�j�I������s����쐬
	//DX11MtxFromQt(rotateMat_, tempqt3);
	if (m_rb.IsExist()) {
		if (m_rb->GetRigidDynamic() != NULL) {
			m_trans->q = physx::PxQuat(qt.x, qt.y, qt.z, qt.w);
		}
	}
}

CRigidbody::~CRigidbody()
{
	CPhysx::DeleteActor(m_actor);
	m_actor->release();
	m_material->release();
	
}

void CRigidbody::Start()
{
	m_usegravity = true;
	m_transform = Holder->GetWeakComponent<CTransform>();
	wp<CRigidbody>wark_wp = Holder->GetWeakComponent<CRigidbody>();
	m_transform->SetRigidbody(wark_wp);
	m_name = "Rigidbody";
}

void CRigidbody::InitDynamic()
{
	m_material = CPhysx::GetPhysics()->createMaterial(0.6f, 0.6f, 0.f);
	physx::PxVec3 boxSize = m_transform->GetScale();
	CCollider* col;
	switch (m_geometryType)
	{
	case GEOMETRYTYPE::BOX:
		m_rigidDynamic = CPhysx::createDynamic(physx::PxTransform(Holder->GetComponent<CTransform>()->GetPhysXPos()), 
			physx::PxBoxGeometry(boxSize /2.f),
			m_material);

		CPhysx::SetActor(m_rigidDynamic);
		m_rigidDynamic->setMass(m_mass);
		
		m_rigidDynamic->setCMassLocalPose(physx::PxTransform(PxVec3(0.f)));
		m_rigidDynamic->setMassSpaceInertiaTensor(PxVec3(1.f, 1.f, 1.f));

		m_actor = CPhysx::GetActor();

		//�傫���ɂ������R���C�_�[�̎��t��
		Holder->AddComponentByName("BoxColider");
		break;
	case GEOMETRYTYPE::CAPSILE:

		break;

	case GEOMETRYTYPE::SPHERE:
		m_rigidDynamic = CPhysx::createDynamic(physx::PxTransform(Holder->GetComponent<CTransform>()->GetPhysXPos()),
			physx::PxSphereGeometry(boxSize.x),
			m_material);

		CPhysx::SetActor(m_rigidDynamic);
		m_rigidDynamic->setMass(m_mass);
		//m_rigidDynamic->setCMassLocalPose(physx::PxTransform(PxVec3(0.f,1.f,0.f)));
		m_rigidDynamic->setMassSpaceInertiaTensor(PxVec3(1.f));
		
		m_actor = CPhysx::GetActor();


		Holder->AddComponent<CSphereCollider>();
		break;
	default:
		break;
	}

	//�������_�̃V�F�C�v���폜
	PxShape* shapes[128];
	const PxU32 nbShapes = m_actor->getNbShapes();
	m_actor->getShapes(shapes, nbShapes);
	m_actor->detachShape(*shapes[0]);


	//============================���̂̃t���O�ݒ�==========================================
	m_actor->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);
	m_rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
	m_rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);
	//======================================================================================

	//�X���[�v��Ԃɓ��鎞��
	m_rigidDynamic->setSleepThreshold(0.5f);

	//�߂荞�݂�s���R�ȋ����̗}���̐��l�@���������@�E���߂荞�݁@
	//�f�t�H���g�l�@1,4
	m_rigidDynamic->setSolverIterationCounts(4, 4);

	//�evelcity�̒�R��
	m_rigidDynamic->setAngularDamping(0.4f);
	m_rigidDynamic->setLinearDamping(0.3f);
	
	USERDATA* use = new USERDATA();
	use->obj = Holder;
	m_actor->userData = use;
}

void CRigidbody::InitStatic()
{
	m_material = CPhysx::GetPhysics()->createMaterial(0.6f, 0.6f, 0.f);
	physx::PxVec3 boxSize = m_transform->GetScale();
	switch (m_geometryType)
	{
	case GEOMETRYTYPE::BOX:
		m_rigidStatic = CPhysx::createStatic(physx::PxTransform(Holder->GetComponent<CTransform>()->GetPhysXPos()),
			physx::PxBoxGeometry(boxSize / 2.0f),
			m_material);

		CPhysx::SetActor(m_rigidStatic);
		m_actor = CPhysx::GetStaticActor();

		Holder->AddComponent<CBoxCollider>();
		//col->Init();
		break;
	case GEOMETRYTYPE::CAPSILE:

		break;

	case GEOMETRYTYPE::SPHERE:
		m_rigidStatic = CPhysx::createStatic(physx::PxTransform(Holder->GetComponent<CTransform>()->GetPhysXPos()), 
			physx::PxSphereGeometry(boxSize.x),
			m_material);

		CPhysx::SetActor(m_rigidStatic);
		m_actor = CPhysx::GetStaticActor();

		Holder->AddComponent<CSphereCollider>();
		//col->Init();
		break;
	default:
		break;
	}

	//�������_�̃V�F�C�v���폜
	PxShape* shapes[128];
	const PxU32 nbShapes = m_actor->getNbShapes();
	m_actor->getShapes(shapes, nbShapes);
	m_actor->detachShape(*shapes[0]);

	m_actor->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);
	m_actor->userData = new USERDATA();
}

void CRigidbody::Update()
{
	if (m_rigidDynamic != nullptr) {
		
	}
}

void CRigidbody::LateUpdate()
{
	if (m_rigidDynamic != nullptr) {
		//��]�̂��߂�
		CTransform* _tra = Holder->GetComponent<CTransform>();
		_tra->GetTrans().q = m_rigidDynamic->getGlobalPose().q;

		physx::PxQuat _q = m_rigidDynamic->getGlobalPose().q;

		m_actor->setGlobalPose(physx::PxTransform(_tra->GetPhysXPos(), physx::PxQuat(_q.x, _q.y, _q.z, _q.w)), true);
	}
}

void CRigidbody::ImGuiDraw()
{
	if (ImGui::Checkbox(u8"�d�͓K�p", &m_usegravity)) {
		m_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !m_usegravity);
		//���x�O��
		m_rigidDynamic->setLinearVelocity(physx::PxVec3(0, 0, 0));
		m_rigidDynamic->setAngularVelocity(physx::PxVec3(0, 0, 0));
	}
	if (m_rigidDynamic != NULL) {
		physx::PxVec3 speed = m_rigidDynamic->getLinearVelocity();
		ImGui::Text(u8"���x x:%.3f  y:%.3f  z:%.3f", speed.x, speed.y, speed.z);
		speed = m_rigidDynamic->getAngularVelocity();
		ImGui::Text(u8"�p���x x:%.3f  y:%.3f  z:%.3f", speed.x, speed.y, speed.z);
		ImGui::DragFloat(u8"����", &m_mass);
		if (m_mass < 0.f)
		{
			m_mass = 0.1f;
		}
		m_rigidDynamic->setMass(m_mass);
	}
	if (ImGui::Checkbox(u8"�g���K�[�ɂ���", &m_trigger)) {
		if (m_trigger) {
			PxShape* shapes[128];
			//�V�F�C�v�̐��擾
			const PxU32 nbShapes = m_actor->getNbShapes();
			m_actor->getShapes(shapes, nbShapes);
			physx::PxShape* _shape;
			for (PxU32 i = 0; i < nbShapes; i++)
			{
				m_actor->getShapes(&_shape, i+1);
				_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
				_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
			}
		}
		else {
			PxShape* shapes[128];
			//�V�F�C�v�̐��擾
			const PxU32 nbShapes = m_actor->getNbShapes();
			m_actor->getShapes(shapes, nbShapes);
			physx::PxShape* _shape;
			for (PxU32 i = 0; i < nbShapes; i++)
			{
				m_actor->getShapes(&_shape, i+1);
				_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
				_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
			}

		}
	}
	if (ImGui::Checkbox(u8"�L�l�}�e�B�b�N", &m_simulation))
	{
		m_rigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, m_simulation);
	}

	const char* work[] = { "SphereColider",
		"BoxColider" };
	auto work_str = ImGuiControl::GetInstance()->SelectDropDown(work, IM_ARRAYSIZE(work));
	if (work_str != "null")
	{
		Holder->AddComponentByName(work_str);
	}

	//for (FilterGroup::Layer num : {FilterGroup::eDEFAULT, FilterGroup::eENEMY, FilterGroup::eFLOOR, FilterGroup::ePLAYER}) {

	//}
}

void CRigidbody::OnCollisionEnter(CObject * col)
{

}

void CRigidbody::OnCollisionStay(CObject * col)
{
	PxVec3 work = m_rigidDynamic->getLinearVelocity();
	PxVec3 work_a = m_rigidDynamic->getAngularVelocity();
	PxVec3 work_ab = work_a.abs();

	PxVec3 abs_work = work.abs();

	if (work.magnitude() < 1.3f)
	{
		m_rigidDynamic->clearForce();
	}

	if (work_a.magnitude() < 1.3f)
	{
		m_rigidDynamic->clearTorque();
	}
}


