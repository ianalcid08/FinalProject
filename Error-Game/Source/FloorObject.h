#ifndef _FLOOROBJECT_H_
#define _FLOOROBJECT_H_

#include "ibaseobject.h"
class CFloorObject : public IBaseObject
{
	float m_fTime;

public:
	CFloorObject(void);
	virtual ~CFloorObject(void);

	void Update(float fDelta);
	bool CheckCollision(IBaseObject* obj);
	void CollisionResponse(IBaseObject* obj);
	void TakeDamage(IBaseObject* _pIn);
	void Render();
};

#endif