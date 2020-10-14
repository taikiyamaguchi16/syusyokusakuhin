#include "CEnemy.h"



CEnemy::CEnemy()
{
	
}


CEnemy::~CEnemy()
{
}

void CEnemy::Start()
{
	if (m_pos == nullptr)
		m_pos = Holder->GetComponent<CTransform>();
}

void CEnemy::Update()
{
}
