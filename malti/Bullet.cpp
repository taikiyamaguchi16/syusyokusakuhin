#include "Bullet.h"

void Bullet::Start()
{
	m_name = "Bullet";
}

void Bullet::Delete()
{
	if (m_time > 300)
		Holder->SetLife(false);
}
