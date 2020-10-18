#pragma once
#include "object.h"

using namespace Dix;
class CPlayer : public CComponent
{
	sp< CTransform > sp1;
	CTransform* m_pos = nullptr;
	CRigidbody* m_rb = nullptr;
	physx::PxBoxController* m_controller;

	XMFLOAT3 m_zikuX, m_zikuY, m_zikuZ;
	float m_time = 0.0f;
public:
	void Start();
	void Move();
	void Update();
	XMFLOAT3 GetPosition() { return m_pos->GetDirectPos(); }
	void ImGuiDraw()override;
	inline void SetName()override {
		m_name = "Player";
	}

	void Shot();
};