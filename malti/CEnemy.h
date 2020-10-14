#pragma once
#include "object.h"

class CEnemy :public  CComponent
{
	CTransform* m_pos = nullptr;
public:

	CEnemy();
	~CEnemy();
	void Start()override;
	void Update()override;
};

