#pragma once
#include "object.h"

typedef struct _USERDATA
{
	bool TriggerFlag = false;
	bool CollisionFlag = false;
	unsigned int Status;
	CObject* obj;

}USERDATA;