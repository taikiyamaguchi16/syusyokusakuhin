#pragma once
#include "object.h"

namespace Egliss::ComponentSystem
{
	class CEnemy :public  CComponent
	{
		CTransform* m_pos = nullptr;
	public:

		CEnemy();
		~CEnemy();
		void Start()override;
		void Update()override;
	};
}

