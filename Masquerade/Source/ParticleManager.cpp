#include "ParticleManager.h"

#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include "..\tinyxml\tinyxml.h"
#include "..\SGD Wrappers\SGD_String.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"

#include <fstream>
#include <sstream>

CParticleManager::CParticleManager(void)
{

}

CParticleManager::~CParticleManager(void)
{
}

CParticleManager* CParticleManager::GetInstance(void)
{
	static CParticleManager PManager;

	return &PManager;
} 

bool CParticleManager::Shutdown()
{
	RemoveAllEmitters();

	std::map< std::string, CParticleEmitter*>::iterator iter;
	for( iter = m_mEmitterTemplates.begin(); iter != m_mEmitterTemplates.end(); ++iter )
	{
		delete (*iter).second;
		(*iter).second = nullptr;
	}

	m_mEmitterTemplates.erase(m_mEmitterTemplates.begin(), m_mEmitterTemplates.end() );
	m_mEmitterTemplates.clear();

	return true;
}
void CParticleManager::UpdateAllParticles(float fElapsed)
{
	for(decltype(m_vEmitters.size()) i = 0; i < m_vEmitters.size(); ++i)
	{
		m_vEmitters[i]->Update(fElapsed);
		
		if(m_vEmitters[i]->m_fDuration > 0.0000f)
			if(m_vEmitters[i]->m_fAge >= m_vEmitters[i]->m_fDuration)
				RemoveEmitter(m_vEmitters[i]);
			else if (m_vEmitters[i]->m_bIsBurst 
				&& m_vEmitters[i]->m_fAge >= m_vEmitters[i]->m_fParticleDuration )
				RemoveEmitter(m_vEmitters[i]);
	}
}
void CParticleManager::RenderAllParticles()
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	IDirect3DDevice9* pDevice = nullptr;
	//pD3D->SetBlend();ToFix
	//LPDIRECT3DDEVICE9
	//pD3D->GetSprite()->GetDevice(&pDevice);
	//pDevice = pD3D->GetDevice();
	//LPDIRECT3DDEVICE9* ppDevice = dynamic_cast<LPDIRECT3DDEVICE9*>(pDevice);
	for(decltype(m_vEmitters.size()) i = 0; i < m_vEmitters.size(); ++i)
	{
		m_vEmitters[i]->Render();
	}

}
void CParticleManager::AddEmitter(CParticleEmitter* pEmitter)
{
	for(decltype(pEmitter->m_vParticles.size()) i = 0; i < pEmitter->m_vParticles.size(); ++i)
	{
		CParticleEmitter::tParticle* p = &pEmitter->m_vParticles[i];
		if(p->m_nColorIndices.size() == 1)
			p->color = pEmitter->m_vUniqueColors[p->m_nColorIndices[0]].m_d3Color;

		if(p->m_nSizeIndices.size() == 1)
		{
			p->m_fWidth = pEmitter->m_vUniqueSizes[p->m_nSizeIndices[0]].m_fWidth;
			p->m_fHeight = pEmitter->m_vUniqueSizes[p->m_nSizeIndices[0]].m_fHeight;
		}

		if(p->m_nVelocityIndices.size() == 1 )
		{
			p->m_vVel.fX = pEmitter->m_vUniqueVelocities[p->m_nVelocityIndices[0]].m_vVel.fX;
			p->m_vVel.fY = pEmitter->m_vUniqueVelocities[p->m_nVelocityIndices[0]].m_vVel.fY;
		}
	}
	m_vEmitters.push_back(pEmitter);
}
void CParticleManager::RemoveEmitter(CParticleEmitter* pEmitter)
{
	for(decltype(m_vEmitters.size()) i = 0; i < m_vEmitters.size(); ++i)
	{
		if(m_vEmitters[i] == pEmitter)
		{
			m_vEmitters[i]->m_vParticles.clear();
			delete m_vEmitters[i];
			m_vEmitters[i] = nullptr;
			m_vEmitters.erase( m_vEmitters.begin()+i);
			break;
		}
	}
}

void CParticleManager::RemoveEmitter(std::string emitterName )
{
	for(decltype(m_vEmitters.size()) i = 0; i < m_vEmitters.size(); ++i)
	{
		if(m_vEmitters[i]->m_szName == emitterName)
		{
			delete m_vEmitters[i];
			std::vector<CParticleEmitter*>::iterator iter = m_vEmitters.begin();
			m_vEmitters.erase(iter+i);
			break;
		}
	}
}


void CParticleManager::RemoveAllEmitters()
{
	for(decltype( m_vEmitters.size()) i = 0; i < m_vEmitters.size(); ++i)
	{
		m_vEmitters[i]->m_vParticles.clear();
		delete m_vEmitters[i];
	}
	m_vEmitters.clear();
}

void CParticleManager::SaveParticles(std::string filename)
{
	
	for(decltype(m_vEmitters.size()) i = 0; i < m_vEmitters.size(); ++i)
		*m_mEmitterTemplates[m_vEmitters[i]->m_szName] = *m_vEmitters[i]; 

	

	TiXmlDocument doc;
	
	TiXmlDeclaration* pDec = new TiXmlDeclaration( "1.0", "utf-8", "");
	doc.LinkEndChild( pDec );
//open
	TiXmlElement* Root = new TiXmlElement( "ParticleEmitters" );
	Root->SetAttribute( "NumberOfEmitters", m_vEmitters.size() );
	doc.LinkEndChild(  Root );

	std::vector<CParticleEmitter*>::iterator iter;
	for( iter = m_vEmitters.begin(); iter != m_vEmitters.end(); ++iter )
	{
		CParticleEmitter* e = (*iter);
	//open
		TiXmlElement* Emitter = new TiXmlElement( "Emitter" );
			
		Emitter->SetAttribute( "Name", e->m_szName.c_str() );
		Emitter->SetAttribute( "ImageName", e->m_szFilename.c_str() );
		Emitter->SetDoubleAttribute( "Duration", e->m_fDuration );
		Emitter->SetDoubleAttribute( "ParticleDuration", e->m_fParticleDuration );
		Emitter->SetAttribute( "IsLooping", (int)e->m_bIsLooping );
		Emitter->SetAttribute( "IsBurst", (int)e->m_bIsBurst );
		Emitter->SetDoubleAttribute( "xForce", e->m_vTotalForce.fX );
		Emitter->SetDoubleAttribute( "yForce", e->m_vTotalForce.fY );
		Emitter->SetAttribute( "rectWidth", e->m_rParticleRect.right-e->m_rParticleRect.left );
		Emitter->SetAttribute( "rectHeight", e->m_rParticleRect.bottom-e->m_rParticleRect.top );
		Emitter->SetDoubleAttribute( "Radius", e->m_fParticleRadius );
		Emitter->SetAttribute( "Shape", e->m_nShape );
		Emitter->SetAttribute( "SourceBlendMode", e->m_nSourceBlendMode );
		Emitter->SetAttribute( "DestinationBlendMode", e->m_nDestBlendMode );

	//open
		TiXmlElement* colors = new TiXmlElement( "UniqueColors" );

		std::vector<CParticleEmitter::tColorStop>::iterator iterc;
		for( iterc = e->m_vUniqueColors.begin(); iterc != e->m_vUniqueColors.end(); ++iterc)
		{
		//open
			TiXmlElement* c = new TiXmlElement( "Color" );
			
			c->SetDoubleAttribute( "Alpha", iterc->m_d3Color.a );
			c->SetDoubleAttribute( "Red", iterc->m_d3Color.r );
			c->SetDoubleAttribute( "Green", iterc->m_d3Color.g );
			c->SetDoubleAttribute( "Blue", iterc->m_d3Color.b );
			colors->LinkEndChild(c);
		//close
		//open
			//TiXmlElement* t = new TiXmlElement( "Time" );
			c->SetDoubleAttribute( "Coeficient", iterc->m_fPosOnLifeSpan );
			//colors->LinkEndChild(t);
		//close
		}
		Emitter->LinkEndChild(colors);
	//close
	//open
		TiXmlElement* sizes = new TiXmlElement( "UniqueSizes" );

		std::vector<CParticleEmitter::tSizeStop>::iterator iters;
		for( iters = e->m_vUniqueSizes.begin(); iters != e->m_vUniqueSizes.end(); ++iters)
		{
			TiXmlElement* s = new TiXmlElement( "Size" );
			s->SetDoubleAttribute( "Width", iters->m_fWidth );
			s->SetDoubleAttribute( "Height", iters->m_fHeight );
			sizes->LinkEndChild(s);

			//TiXmlElement* t = new TiXmlElement( "Time" );
			s->SetDoubleAttribute( "Coeficient", iters->m_fPosOnLifeSpan );
			//sizes->LinkEndChild(t);
		}
		Emitter->LinkEndChild(sizes);
	//close
	//open
		TiXmlElement* vels = new TiXmlElement( "UniqueVelocities" );
		vels->SetAttribute( "NumberOfVelocities", e->m_vUniqueVelocities.size() );

		std::vector<CParticleEmitter::tVelocityStop>::iterator iterv;
		for( iterv = e->m_vUniqueVelocities.begin(); iterv != e->m_vUniqueVelocities.end(); ++iterv)
		{
			TiXmlElement* v = new TiXmlElement( "Velocity" );
			v->SetDoubleAttribute( "xVelocity", iterv->m_vVel.fX );
			v->SetDoubleAttribute( "yVelocity", iterv->m_vVel.fY );
			vels->LinkEndChild(v);

			//TiXmlElement* t = new TiXmlElement( "Time" );
			v->SetDoubleAttribute( "Coeficient", iterv->m_fPosOnLifeSpan );
			//vels->LinkEndChild(t);
		}
		Emitter->LinkEndChild(vels);
	//close

	//open
		TiXmlElement* parts = new TiXmlElement( "Particles" );
		parts->SetAttribute( "NumberOfParticles", e->m_vParticles.size() );	
			
		std::vector<CParticleEmitter::tParticle>::iterator iterp;
		for( iterp = e->m_vParticles.begin(); iterp != e->m_vParticles.end(); ++iterp )
		{
		//open
			TiXmlElement* p = new TiXmlElement( "Particle" );
			
			std::vector<int>::iterator iteri;
			
			//Open
			TiXmlElement* ci = new TiXmlElement( "ColorIndices" );
			for(iteri = iterp->m_nColorIndices.begin(); iteri != iterp->m_nColorIndices.end(); ++iteri)
			{
				TiXmlElement* i = new TiXmlElement( "Index" );
				i->SetAttribute( "ci", *iteri );
				ci->LinkEndChild(i);
			}
			p->LinkEndChild(ci);
			//Close

			TiXmlElement* si = new TiXmlElement( "SizeIndices" );
			for(iteri = iterp->m_nSizeIndices.begin(); iteri != iterp->m_nSizeIndices.end(); ++iteri)
			{
				TiXmlElement* i = new TiXmlElement( "Index" );
				i->SetAttribute( "si", *iteri );
				si->LinkEndChild(i);
			}
			p->LinkEndChild(si);

			TiXmlElement* vi = new TiXmlElement( "VelocityIndices" );
			for(iteri = iterp->m_nVelocityIndices.begin(); iteri != iterp->m_nVelocityIndices.end(); ++iteri)
			{
				TiXmlElement* i = new TiXmlElement( "Index" );
				i->SetAttribute( "vi", *iteri );
				vi->LinkEndChild(i);
			}
			p->LinkEndChild(vi);

			parts->LinkEndChild(p);
		//close
		}
		Emitter->LinkEndChild(parts);
	//close
	Root->LinkEndChild( Emitter );
//close
	}
	doc.SaveFile( filename.c_str() );
}


bool CParticleManager::LoadParticles(std::string filename)
{
	TiXmlDocument doc;

	if( doc.LoadFile( filename.c_str() ) == false )
		return false;

	TiXmlElement* pRoot = doc.RootElement();
	if( pRoot == nullptr )
		return false;

	TiXmlElement* pEmitter = pRoot->FirstChildElement( "Emitter" );
	

	while( pEmitter != nullptr )
	{
		CParticleEmitter* e = new CParticleEmitter;

		e->m_szName	= pEmitter->Attribute( "Name" );
		e->m_szFilename	= pEmitter->Attribute( "ImageName" );

		std::string buffer;
		std::wostringstream woss;
		woss << "Resources/Images/Particles/" << e->m_szFilename.c_str();
		
		e->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture(woss.str().c_str());
		
		
		buffer = pEmitter->Attribute( "Duration"/*, &((double)e->m_fDuration)*/ );
		e->m_fDuration = (float)(atof(buffer.c_str()));

		buffer = pEmitter->Attribute( "ParticleDuration" );
		e->m_fParticleDuration =  (float)(atof(buffer.c_str()));

		buffer = pEmitter->Attribute( "IsLooping" );
		e->m_bIsLooping = (atoi(buffer.c_str())) ? TRUE : FALSE;

		buffer = pEmitter->Attribute( "IsBurst" );
		e->m_bIsBurst = (atoi(buffer.c_str())) ? true : false; 

		buffer = pEmitter->Attribute( "xForce" );
		e->m_vTotalForce.fX = (float)(atof(buffer.c_str()));

		buffer = pEmitter->Attribute( "yForce" );
		e->m_vTotalForce.fY = (float)(atof(buffer.c_str()));

		buffer = pEmitter->Attribute( "ParticleScalar" );
		e->m_fSizeScalar = (float)(atof(buffer.c_str()));

		RECT r;
		r.left = 0;
		r.top = 0;
		buffer = pEmitter->Attribute( "rectWidth" );
		r.right = atol(buffer.c_str());
		buffer = pEmitter->Attribute( "rectHeight" );
		r.bottom = atol(buffer.c_str());
		e->m_rParticleRect = r;

		buffer = pEmitter->Attribute( "Radius" );
		e->m_fParticleRadius = (float)(atof(buffer.c_str()));

		buffer = pEmitter->Attribute( "Shape" );
		e->m_nShape = atoi(buffer.c_str());

		buffer = pEmitter->Attribute( "SourceBlendMode" );
		e->m_nSourceBlendMode = atoi(buffer.c_str());
		buffer = pEmitter->Attribute( "DestinationBlendMode" );
		e->m_nDestBlendMode = atoi(buffer.c_str());

		TiXmlElement* cs = pEmitter->FirstChildElement( "UniqueColors" );
		
		TiXmlElement* c = cs->FirstChildElement( "Color" );

		while( c != nullptr )
		{
			CParticleEmitter::tColorStop t;
			buffer = c->Attribute( "Alpha" );
			t.m_d3Color.a = (float)(atof(buffer.c_str()));
			buffer = c->Attribute( "Red" );
			t.m_d3Color.r = (float)(atof(buffer.c_str()));
			buffer = c->Attribute( "Green" );
			t.m_d3Color.g = (float)(atof(buffer.c_str()));
			buffer = c->Attribute( "Blue" );
			t.m_d3Color.b = (float)(atof(buffer.c_str()));

			//TiXmlElement* ts = c->NextSiblingElement( "Time" );
			buffer = c->Attribute( "Coeficient" );
			t.m_fPosOnLifeSpan = (float)(atof(buffer.c_str()));

			e->m_vUniqueColors.push_back(t);

			c = c->NextSiblingElement( "Color" );
		}

		TiXmlElement* ss = cs->NextSiblingElement( "UniqueSizes" );

		TiXmlElement* s = ss->FirstChildElement( "Size" );

		while( s != nullptr )
		{
			CParticleEmitter::tSizeStop ts;
			buffer = s->Attribute( "Width" );
			ts.m_fWidth = (float)(atof(buffer.c_str()));
			buffer = s->Attribute( "Height" );
			ts.m_fHeight = (float)(atof(buffer.c_str()));

			//TiXmlElement* tc = s->NextSiblingElement( "Time" );
			buffer = s->Attribute( "Coeficient" );
			ts.m_fPosOnLifeSpan = (float)(atof(buffer.c_str()));

			e->m_vUniqueSizes.push_back(ts);

			s = s->NextSiblingElement( "Size" );
		}

		TiXmlElement* vs = cs->NextSiblingElement( "UniqueVelocities" );

		TiXmlElement* v = vs->FirstChildElement( "Velocity" );

		while( v != nullptr )
		{
			CParticleEmitter::tVelocityStop tv;
			buffer = v->Attribute( "xVelocity" );
			tv.m_vVel.fX = (float)(atof(buffer.c_str()));
			buffer = v->Attribute( "yVelocity" );
			tv.m_vVel.fY = (float)(atof(buffer.c_str()));

			//TiXmlElement* tc = v->NextSiblingElement( "Time" );
			buffer = v->Attribute( "Coeficient" );
			tv.m_fPosOnLifeSpan = (float)(atof(buffer.c_str()));

			e->m_vUniqueVelocities.push_back(tv);

			v = v->NextSiblingElement( "Velocity" );
		}

		TiXmlElement* particles = vs->NextSiblingElement( "Particles" );

		TiXmlElement* p = particles->FirstChildElement( "Particle" );
		
		while( p != nullptr )
		{
			CParticleEmitter::tParticle tPart;

			TiXmlElement* ci = p->FirstChildElement( "ColorIndices" );
			
			TiXmlElement* i = ci->FirstChildElement( "Index" );

			while( i != nullptr )
			{
				buffer = i->Attribute( "ci" );
				tPart.m_nColorIndices.push_back(atoi(buffer.c_str()));
				i = i->NextSiblingElement( "Index" );
			}

			TiXmlElement* si = ci->NextSiblingElement( "SizeIndices");

			i = si->FirstChildElement( "Index" );

			while( i !=nullptr )
			{
				buffer = i->Attribute( "si" );
				tPart.m_nSizeIndices.push_back(atoi(buffer.c_str()));
				i = i->NextSiblingElement( "Index" );
			}

			TiXmlElement* vi = ci->NextSiblingElement( "VelocityIndices" );

			i = vi->FirstChildElement( "Index" );

			while( i != nullptr )
			{
				buffer = i->Attribute( "vi" );
				tPart.m_nVelocityIndices.push_back(atoi(buffer.c_str()));
				i = i->NextSiblingElement( "Index" );
			}

			e->m_vParticles.push_back( tPart );

			p = p->NextSiblingElement( "Particle" );
		}


		m_mEmitterTemplates[e->m_szName] =  e;

		pEmitter = pEmitter->NextSiblingElement( "Emitter" );
	}

	std::ofstream ofs;

	ofs.open( "EmitterNames.txt" );

	if(ofs.is_open())
	{

		std::map< std::string, CParticleEmitter*>::iterator iter;
		for( iter = m_mEmitterTemplates.begin(); iter != m_mEmitterTemplates.end(); ++iter )
		{
			ofs << (*iter).second->m_szName << '\n';
		}

		ofs.close();
	}

	return true;
}

CParticleEmitter* CParticleManager::GetEmitter( std::string emitterName )
{
	for(decltype(m_vEmitters.size()) i = 0; i < m_vEmitters.size(); ++i)
	{
		if(m_vEmitters[i]->m_szName == emitterName)
			return m_vEmitters[i];
	}

	return nullptr;
}


/*
void CParticleManager::ConvertSystem(void)
{

	for(int j = 0; j < m_vEmitters.size(); ++j)
	// Emitters
	{
		
		
		for(int i = 0; i < m_vEmitters[j]->m_vParticles.size(); ++i)
		// Particles
		{
			CParticleEmitter::tParticle* p = &(m_vEmitters[j]->m_vParticles[i]);
			bool isUnique = true;

			for(int y = 0; y < p->m_vColorCollection.size(); ++y)
			// Color Collection
			{
				isUnique = true;
				for(int x = 0; x < m_vEmitters[j]->m_vUniqueColors.size(); ++x)
				// Unique Colors
				{
					if(m_vEmitters[j]->m_vUniqueColors[x] == p->m_vColorCollection[y])
					{
						p->m_nColorIndices.push_back(x);
						isUnique = false;
						break;
					}
				}
				if(isUnique)
				{
					p->m_nColorIndices.push_back(m_vEmitters[j]->m_vUniqueColors.size());
					m_vEmitters[j]->m_vUniqueColors.push_back(p->m_vColorCollection[y]);
				}
			}

			
			for(int y = 0; y < p->m_vSizeCollection.size(); ++y)
			// Size Collection
			{
				isUnique = true;
				for(int x = 0; x < m_vEmitters[j]->m_vUniqueSizes.size(); ++x)
				// Unique Sizes
				{
					if(m_vEmitters[j]->m_vUniqueSizes[x] == p->m_vSizeCollection[y])
					{
						p->m_nSizeIndices.push_back(x);
						isUnique = false;
						break;
					}
				}
				if(isUnique)
				{
					p->m_nSizeIndices.push_back(m_vEmitters[j]->m_vUniqueSizes.size());
					m_vEmitters[j]->m_vUniqueSizes.push_back(p->m_vSizeCollection[y]);
				}
			}

			isUnique = true;
			for(int y = 0; y < m_vEmitters[j]->m_vUniqueVelocities.size(); ++y)
			{
				if(m_vEmitters[j]->m_vUniqueVelocities[y].m_vVel.fX == p->m_vVel.fX &&
					m_vEmitters[j]->m_vUniqueVelocities[y].m_vVel.fY == p->m_vVel.fY )
				{
					p->m_nVelocityIndices.push_back(y);
					isUnique = false;
					break;
				}
			}
			if(isUnique)
			{
				p->m_nVelocityIndices.push_back(m_vEmitters[j]->m_vUniqueVelocities.size());
				CParticleEmitter::tVelocityStop vs;
				vs.m_fPosOnLifeSpan = 0.0f;
				vs.m_vVel.fX = p->m_vVel.fX;
				vs.m_vVel.fY = p->m_vVel.fY;

				m_vEmitters[j]->m_vUniqueVelocities.push_back(vs);
			}

		}
	}
}
*/
