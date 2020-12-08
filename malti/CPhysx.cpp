#include "CPhysx.h"
#include "UserData.h"

PxDefaultAllocator CPhysx::m_allocator;
PxDefaultErrorCallback CPhysx::m_errorCallback;
PxFoundation* CPhysx::m_foundation = nullptr;
PxPhysics* CPhysx::m_physics = nullptr;
PxDefaultCpuDispatcher* CPhysx::m_dispatcher = nullptr;
PxScene* CPhysx::m_scene = nullptr;
PxPvd* CPhysx::m_pvd = nullptr;
//PxControllerManager* CPhysx::m_controllerManager = nullptr;
CSimulationEventCallback* CPhysx::m_eventCallbac;
//PxSimulationFilterShader CPhysx::m_filterFlag;
//CSimulationFilterCallback* CPhysx::m_filterCallbac;

void CPhysx::Init()
{
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);

	// PVD(PhysX Visual Debugger) setting. To use PVD, we need to build the project as Debug mode.
	m_pvd = PxCreatePvd(*m_foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);


	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale(), true, m_pvd);

	// Scene setting
	
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);          // Right-hand coordinate system, Y-UP.
	m_dispatcher = PxDefaultCpuDispatcherCreate(1);         // The number of worker threads is one.
	sceneDesc.cpuDispatcher = m_dispatcher;
	sceneDesc.broadPhaseType = PxBroadPhaseType::eABP;
	//連続衝突検知ON
	//sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;
	
	//sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.filterShader = TestFilterShader;
	sceneDesc.filterCallback= m_filterCallbac;

	m_scene = m_physics->createScene(sceneDesc);
	m_eventCallbac = new CSimulationEventCallback();
	m_scene->setSimulationEventCallback(m_eventCallbac);
	// PVD setting
	PxPvdSceneClient* pvdClient = m_scene->getScenePvdClient();
	//コールバック関数を呼ぶタイミング
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

}

void CPhysx::UnInit()
{
	m_scene->release();
	m_scene = nullptr;

	m_physics->release();
	m_physics = nullptr;

	//m_controllerManager = nullptr;

	m_foundation->release();
	m_foundation = nullptr;
	

	delete m_eventCallbac;
}

//filterGroupに自身のレイヤー	filterMaskに当たり判定をとりたくないレイヤーを記述
void CPhysx::setupFiltering(PxRigidActor * actor, PxU32 filterGroup, PxU32 filterMask)
{
	PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask;  // word1 = ID mask to filter pairs that trigger a
									// contact callback;
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)malloc(sizeof(PxShape*)*numShapes);
	actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	free(shapes);
}

unsigned int CPhysx::GetFilterGroup(unsigned int filter)
{
	//ここに記述しておくことで当たり判定をとりたくないレイヤーがreturnで帰ってくるので
	//setupFilteringの際に間違えない
	unsigned int mask = 0;
	switch (filter)
	{
	case FilterGroup::eDEFAULT:
		break;
	case FilterGroup::ePLAYER:
		mask = FilterGroup::eFLOOR;
		break;
	case FilterGroup::eENEMY:
		mask = FilterGroup::eFLOOR;
		break;
	case FilterGroup::eFLOOR:
		mask = FilterGroup::ePLAYER | FilterGroup::eENEMY;
		break;
	case FilterGroup::eWALL:
		break;
	default:
		mask = 0;
		break;
	}
	return mask;
}


PxFilterFlags TestFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags & pairFlags, const void* constantBlock,PxU32 constantBlockSize)
{
	pairFlags = PxPairFlag::eDETECT_DISCRETE_CONTACT;
	//どちらかがtriggerの場合
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		//pairFlagsによってコールバック関数が呼び出されるタイミングが変わる
		pairFlags = PxPairFlag::eNOTIFY_TOUCH_LOST|PxPairFlag::eNOTIFY_TOUCH_FOUND| PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		return PxFilterFlag::eCALLBACK;
	}
	pairFlags = PxPairFlag::eDETECT_DISCRETE_CONTACT;
	pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eCONTACT_DEFAULT;
	

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		//pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT;
	}

	return PxFilterFlag::eCALLBACK;
}




void CSimulationEventCallback::onContact(const PxContactPairHeader & pairHeader, const PxContactPair * pairs, PxU32 nbPairs)
{
	//自分で設定したトリガーのフラグをtrueにする
	USERDATA* user = (USERDATA*)pairHeader.actors[0]->userData;
	USERDATA* otherUser = (USERDATA*)pairHeader.actors[1]->userData;
	//pairs->triggerActor->userData = user;

	if (pairs->events&PxPairFlag::eNOTIFY_TOUCH_FOUND) {
		user->obj->OnCollisionEnter(otherUser->obj);
	}
	else if (pairs->events&PxPairFlag::eNOTIFY_TOUCH_LOST) {
		user->obj->OnCollisionExit(otherUser->obj);
	}
	else if (pairs->events&PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
		user->obj->OnCollisionStay(otherUser->obj);
	}
	user->Status = NULL;
}

void CSimulationEventCallback::onTrigger(PxTriggerPair * pairs, PxU32 count)
{
	//自分で設定したトリガーのフラグをtrueにする
	USERDATA* user = (USERDATA*)pairs->triggerActor->userData;
	USERDATA* otherUser = (USERDATA*)pairs->otherActor->userData;
	//pairs->triggerActor->userData = user;

	if (pairs->status&PxPairFlag::eNOTIFY_TOUCH_FOUND) {
		user->obj->OnTriggerEnter(otherUser->obj);
	}
	else if (pairs->status&PxPairFlag::eNOTIFY_TOUCH_LOST) {
		user->obj->OnTriggerExit(otherUser->obj);
	}
	else if (pairs->status&PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
		user->obj->OnTriggerStay(otherUser->obj);
	}
	user->Status = NULL;
}
