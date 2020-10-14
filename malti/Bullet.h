#pragma once
#include "object.h"
class Bullet :public CComponent
{
	int m_time = 0;
public:

	void Start()override;

	inline void Update()override { m_time++; Delete(); };
	void Delete();
};

