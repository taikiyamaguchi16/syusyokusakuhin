#pragma once
#include "object.h"

using namespace Dix;
class CPlayer : public CComponent
{
	wp<CTransform> m_pos;
	wp<CRigidbody> m_rb;
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