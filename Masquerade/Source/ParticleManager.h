#pragma once

#include <string>
#include <vector>
#include <map>

#include "ParticleEmitter.h"

class CParticleManager 
{
	
private:
	CParticleManager(void);
	std::vector<CParticleEmitter*> m_vEmitters;
	std::map< std::string, CParticleEmitter* > m_mEmitterTemplates;
public:
	
	~CParticleManager(void);
	static CParticleManager* GetInstance(void);

	bool Shutdown();
	void UpdateAllParticles(float fElapsed);
	void RenderAllParticles();
	void AddEmitter(CParticleEmitter* pEmitter);
	
	void RemoveEmitter(CParticleEmitter* pEmitter);
	
	//Will Remove the FIRST emitter found matching the emitter name provided
	void RemoveEmitter(std::string emitterName );

	void RemoveAllEmitters(void);
	void SaveParticles(std::string filename);
	bool LoadParticles(std::string filename);

	CParticleEmitter* CloneEmitterFromTemplate( std::string szClonefromEmitterName )
	{
		CParticleEmitter* Emitter = new CParticleEmitter();
		*Emitter = *m_mEmitterTemplates[szClonefromEmitterName];
		return Emitter;
	}

	CParticleEmitter* GetEmitter( std::string emitterName );
	
	//void ConvertSystem(void);
};