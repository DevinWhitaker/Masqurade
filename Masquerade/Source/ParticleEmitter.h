#pragma once

#include <string>
#include <vector>
#include "..\SGD Wrappers\SGD_Math.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include <Windows.h>

#include "BaseObject.h"

class CParticleEmitter : public CBaseObject
{
public:
	
	enum Emitter_Shape { PES_RECTANGLE, PES_CIRCLE, PES_POINT };

	friend class CParticleManager;

	struct tSizeStop
	{
		float m_fWidth;
		float m_fHeight;
		float m_fPosOnLifeSpan;

		tSizeStop(void)
		{
			m_fWidth = 1.0f;
			m_fHeight = 1.0f;
			m_fPosOnLifeSpan = 0.0f;
		}

		bool operator==(tSizeStop& ss)
		{
			if(ss.m_fHeight == this->m_fHeight &&
				ss.m_fWidth == this->m_fWidth &&
				ss.m_fPosOnLifeSpan == this->m_fPosOnLifeSpan )
				return true;
			else
				return false;
		}
	};

	struct tColorStop
	{
		D3DXCOLOR m_d3Color;
		float m_fPosOnLifeSpan;
		
		tColorStop(void)
		{
			m_d3Color.a = 1.0f;
			m_d3Color.r = 1.0f;
			m_d3Color.g = 1.0f;
			m_d3Color.b = 1.0f;
			m_fPosOnLifeSpan = 0.0f;
		}

		bool operator==(tColorStop& cs)
		{
			if(cs.m_d3Color == this->m_d3Color && 
				cs.m_fPosOnLifeSpan == this->m_fPosOnLifeSpan)
				return true;
			else
				return false;
		}
	};

	struct tVelocityStop
	{
		tVector2D m_vVel;
		float m_fPosOnLifeSpan;

		tVelocityStop(void)
		{
			m_vVel.fX = 0.0f;
			m_vVel.fY = 0.0f;
			m_fPosOnLifeSpan = 0.0f;
		}

		bool operator==(tVelocityStop& vs)
		{
			if(vs.m_vVel.fX == this->m_vVel.fX &&
				vs.m_vVel.fX == this->m_vVel.fX && 
				vs.m_fPosOnLifeSpan == this->m_fPosOnLifeSpan)
				return true;
			else
				return false;
		}
	};
	
	struct tParticle
	{
		tVector2D m_vPos;
		tVector2D m_vVel;
		float m_fAge;
		D3DXCOLOR color;
		float m_fScale;
		float m_fWidth;
		float m_fHeight;
		int m_nCurColorStop;
		int m_nCurSizeStop;
		int m_nCurVelocityStop;
		
		std::vector<int> m_nColorIndices;
		std::vector<int> m_nSizeIndices;
		std::vector<int> m_nVelocityIndices;


		tParticle(void)
		{
			m_vPos.fX = 0;
			m_vPos.fY = 0;
			m_vVel.fX = 0;
			m_vVel.fY = 0;
			m_fAge = 0;
			color.a = 1.0f;
			color.r = 1.0f;
			color.g = 1.0f;			
			color.b = 1.0f;
			m_fScale = 1.0f;
			m_fWidth = 1.0f;
			m_fHeight = 1.0f;
			m_nCurColorStop = 0;
			m_nCurSizeStop = 0;
			m_nCurVelocityStop = 0;
		}
	};
public:
	

private:
	std::vector<tParticle> m_vParticles;
	std::vector<tColorStop> m_vUniqueColors;
	std::vector<tSizeStop> m_vUniqueSizes;
	std::vector<tVelocityStop> m_vUniqueVelocities;
	int m_nImageID;
	std::string m_szName;
	tVector2D m_vPos;
	std::string m_szFilename;
	float m_fAge;
	float m_fDuration;
	float m_fParticleDuration;
	bool m_bIsLooping;
	tVector2D m_vTotalForce;
	float m_fSizeScalar;
	RECT m_rParticleRect;
	float m_fParticleRadius;
	int m_nShape;
	CBaseObject* m_AnchorToTarget;
	bool m_bIsBirth;
	bool m_bIsBurst;
	int m_nSourceBlendMode;
	int m_nDestBlendMode;
	int m_nPartsActive;
	int m_nDeadCuttoff;
	bool m_bUseCamera;

	void SpawnParticle(tParticle* particle);
	void RandParticleInRect(tParticle* particle);

	float Distance(tVector2D x, tVector2D y);
public:
	CParticleEmitter(void);

	~CParticleEmitter(void);

	void AddParticle(tParticle &p);

	// call this BEFORE adding to the Particle manager
	//
	// Will Finalize the Particle Emitter before you place into the Manager
	//
	// ATTENTION:
	//
	// This ONLY needs to be called if you are setting up a particle
	// Emitter from scratch (and not from clone ) *OR* you would like to 
	// change all the parameters of a particle emitter you created in one shot
	void Init( int imageID, float fParticleDuration, float fDuration, Emitter_Shape nShape, 
		tVector2D vEmitterCenterPos, bool isLooping, bool isBurst, tVector2D vExternalForce, 
		float fParticleRadius = 0.0f, float rectWidth = 0.0f, float rectHeight = 0.0f,
		CBaseObject* boAnchorToTarget = nullptr);

	void SetUseCamera(bool b)		{ m_bUseCamera = b; }
	bool &GetUseCamera(void)		{ return m_bUseCamera; }

	void Update(float fElapsed);
	void Render(void);

	void SetLooping(bool isLooping)		{m_bIsLooping = isLooping;}
	bool GetLooping(void)				{return m_bIsLooping; }

	void SetBurst(bool isBurst)			{m_bIsBurst = isBurst;}
	bool GetBurst(void)					{return m_bIsBurst; }

	void SetExternalForce(float x, float y) { m_vTotalForce.fX = x; m_vTotalForce.fY = y; }
	tVector2D GetExternalForce(void)	{return m_vTotalForce;}

	void SetImageID(int ID)				{m_nImageID = ID;}
	int GetImageID(void)				{return m_nImageID;}

	void SetParticleDuration(float f)	{m_fParticleDuration = f;}
	float GetParticleDuration(void)		{return m_fParticleDuration;}

	void SetEmitterDuration(float f)	{m_fDuration = f;}
	float GetEmitterDuration(void)		{return m_fDuration;}

	void SetEmitterShape(int nShape)	{m_nShape = nShape;}
	int GetEmitterShape(void)			{return m_nShape;}

	void SetEmitterRadius(float f)		{m_fParticleRadius = f;}
	float GetEmitterRadius(void)		{return m_fParticleRadius;}

	void SetEmitterRect(RECT r)			{m_rParticleRect = r;}
	RECT GetEmitterRect(void)			{return m_rParticleRect;}

	void SetAnchor(CBaseObject* anchor) {m_AnchorToTarget = anchor;}
	CBaseObject* GetAnchor(void)		{return m_AnchorToTarget;}

	void SetName(char* szName)			{m_szName = szName;}
	const char* GetName(void)			{return m_szName.c_str(); }

	void SetImageName(char* szFilename)	{m_szFilename = szFilename;}
	const char* GetImageName(void)		{return m_szFilename.c_str();}

	void SetPos(float xPos, float yPos ) {m_vPos.fX = xPos; m_vPos.fY = yPos;}
	tVector2D GetPos()					{return m_vPos;}
	
};