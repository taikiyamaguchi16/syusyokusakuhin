#pragma once
#include "object.h"
#include "CBox.h"
#include "Sphere.h"

class CCollider :public CComponent
{
protected:
	wp<CRigidbody> m_rb;
	wp<CTransform> m_transform;
	XMFLOAT3 m_ofset;
	XMFLOAT3 m_scale;
	XMFLOAT3 m_angle;
	physx::PxShape* m_shape;

	XMFLOAT4X4 m_mat;
public:
	inline void SetName()override {
		m_name = "CCollider";
	}
	void Start()override;
	virtual void Init();
};

class CBoxCollider :public CCollider
{
private:
	CBox*  m_box = nullptr;
public:
	void Start()override;

	void Draw()override;
	void ImGuiDraw()override;
	void Init()override;

	inline void SetName()override {
		m_name = "BoxCollider";
	}
	~CBoxCollider();
};

class CSphereCollider :public CCollider
{
private:
	CSphere*  m_sphere = nullptr;
	float m_radius;
public:
	void Start()override;

	void Draw()override;
	void ImGuiDraw()override;
	void Init()override;

	inline void SetName()override {
		m_name = "SphereCollider";
	}
	~CSphereCollider();
};
