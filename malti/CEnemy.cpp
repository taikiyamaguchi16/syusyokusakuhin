#include "CEnemy.h"

using namespace Egliss::ComponentSystem;

CEnemy::CEnemy()
{
	
}


CEnemy::~CEnemy()
{
}

void CEnemy::Start()
{
	if (m_pos == nullptr)
		m_pos = Holder->GetComponent<Egliss::ComponentSystem::CTransform>();
}

void CEnemy::Update()
{
}
