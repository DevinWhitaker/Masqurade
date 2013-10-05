/*
	File:		IBaseObject.h
	Course:		SGD 1204
	Author:		afoy
	Purpose:	Define the interface for Entities
*/ 
#pragma once
#include <Windows.h>

class IBaseObject
{
private:

public:
	virtual ~IBaseObject(void) = 0 { }

	virtual void Update(float fElapsedTime) = 0;
	virtual void SetScalar(float timeScalar) = 0;
	virtual void Render(void) = 0;

	// "Smart" pointer interface
	virtual void AddRef(void) = 0;
	virtual void Release(void) = 0;

	virtual int GetType(void) = 0;

	virtual RECT GetRect() = 0;
	virtual bool CheckCollision(IBaseObject * pBase) = 0;

};

