#include "ParticleEmitter.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include <sstream>

#include "..\SGD Wrappers\SGD_Math.h"
#include "..\SGD Wrappers\CSGD_DirectInput.h"

#include "Camera.h"

CParticleEmitter::CParticleEmitter(void)
{
	m_nImageID = -1;
	m_vPos.fX = 0;
	m_vPos.fY = 0;

	m_fAge = 0;
	m_fDuration = 0;
	m_bIsLooping = false;

	m_vTotalForce.fX = 0;
	m_vTotalForce.fY = 0;
	
	m_rParticleRect.left = 0;
	m_rParticleRect.top = 0;
	m_rParticleRect.right = 0;
	m_rParticleRect.bottom= 0;
	
	m_bIsBirth = true;
	m_bIsBurst = false;

	m_nPartsActive = 0;
	m_nDeadCuttoff = 0;

	m_fParticleRadius = 0;
	m_AnchorToTarget = nullptr;

	m_nDestBlendMode = D3DBLEND_ZERO;
	m_nSourceBlendMode = D3DBLEND_ZERO;

	m_fSizeScalar = 1.0f;
	m_bUseCamera = true;
}

CParticleEmitter::~CParticleEmitter(void)
{

}

void CParticleEmitter::AddParticle(tParticle &p)
{
	m_vParticles.push_back(p);
}

// Will Finalize the Particle Collection into the Emitter
void CParticleEmitter::Init( int imageID, float fParticleDuration, float fDuration, Emitter_Shape nShape, 
		tVector2D vEmitterCenterPos, bool isLooping, bool isBurst, tVector2D vExternalForce, 
		float fParticleRadius, float rectWidth, float rectHeight,
		CBaseObject* boAnchorToTarget)
{
	m_nImageID = imageID;
	m_vPos = vEmitterCenterPos;
	m_AnchorToTarget = boAnchorToTarget;

	m_nShape = nShape;

	m_vTotalForce.fX = vExternalForce.fX;
	m_vTotalForce.fY = vExternalForce.fY;

	m_fParticleRadius = fParticleRadius;

	m_rParticleRect.left = (LONG)(m_vPos.fX-rectWidth/2.0f);
	m_rParticleRect.top = (LONG)(m_vPos.fY-rectHeight/2.0f);
	m_rParticleRect.right = (LONG)(m_vPos.fX+rectWidth/2.0f);
	m_rParticleRect.bottom = (LONG)(m_vPos.fY+rectHeight/2.0f);

	

	if( m_nShape == PES_RECTANGLE || m_nShape == PES_CIRCLE )
	{
		for(decltype(m_vParticles.size()) i = 0; i < m_vParticles.size(); ++i)
		{
			tParticle* p = &m_vParticles[i];
			RandParticleInRect(p);
		}

		if( m_nShape == PES_CIRCLE )
		{
			m_rParticleRect.left = (LONG)(m_vPos.fX - m_fParticleRadius);
			m_rParticleRect.right = (LONG)(m_vPos.fX + m_fParticleRadius);
			m_rParticleRect.top = (LONG)(m_vPos.fY - m_fParticleRadius);
			m_rParticleRect.bottom = (LONG)(m_vPos.fY + m_fParticleRadius);

		}
	}

	
	m_bIsLooping = isLooping;
	m_bIsBurst = isBurst;
	m_fDuration = fDuration;
	m_fParticleDuration = fParticleDuration;

	if(m_bIsBurst) m_nPartsActive = (int)m_vParticles.size();

	//for(decltype(m_vParticles.size()) i = 0; i < m_vParticles.size(); ++i)
	//{
	//	tParticle* p = &m_vParticles[i];
	//	for(decltype(p->m_vColorCollection.size()) a = 0; a < p->m_vColorCollection.size(); ++a)
	//	{
	//		for(decltype(p->m_vColorCollection.size()) b = 0; b < p->m_vColorCollection.size(); ++b)
	//		{
	//			//DECENDING
	//			if(p->m_vColorCollection[a].m_fPosOnLifeSpan 
	//				< p->m_vColorCollection[b].m_fPosOnLifeSpan)
	//			{
	//				std::swap( 	p->m_vColorCollection[a], p->m_vColorCollection[b]);
	//			}
	//		}
	//	}

	//	for(decltype(p->m_vSizeCollection.size()) a = 0; a < p->m_vSizeCollection.size(); ++a)
	//	{
	//		for(decltype(p->m_vSizeCollection.size()) b = 0; b < p->m_vSizeCollection.size(); ++b)
	//		{
	//			//DECENDING
	//			if(p->m_vSizeCollection[a].m_fPosOnLifeSpan 
	//				< p->m_vSizeCollection[b].m_fPosOnLifeSpan)
	//			{
	//				std::swap( 	p->m_vSizeCollection[a], p->m_vSizeCollection[b]);
	//			}

	//		}
	//	}
	//}
	//int x = 0;
}

void CParticleEmitter::Update(float fElapsed)
{
	if(m_fDuration >= 0.0000f)
		m_fAge += fElapsed;

	if(m_AnchorToTarget != nullptr)
	{
		m_vPos.fX = m_AnchorToTarget->GetX();
		m_vPos.fY = m_AnchorToTarget->GetY();
	}

	if( m_nShape == PES_CIRCLE || m_nShape == PES_RECTANGLE)
	{
		float rectWidth = (float)(m_rParticleRect.right-m_rParticleRect.left);
		float rectHeight = (float)(m_rParticleRect.bottom-m_rParticleRect.top);
		m_rParticleRect.left = (LONG)(m_vPos.fX-rectWidth/2.0f);
		m_rParticleRect.top = (LONG)(m_vPos.fY-rectHeight/2.0f);
		m_rParticleRect.right = (LONG)(m_vPos.fX+rectWidth/2.0f);
		m_rParticleRect.bottom = (LONG)(m_vPos.fY+rectHeight/2.0f);
	}


	if(m_bIsBirth && !m_bIsBurst)	
	{
		m_nPartsActive = int(m_fAge/m_fParticleDuration*(int)m_vParticles.size());
		if(m_fAge >= m_fParticleDuration)
		{
			m_bIsBirth = false;
			m_nPartsActive = (int)m_vParticles.size();
		}
	}

	if(m_bIsBurst)
		m_nPartsActive = m_vParticles.size();
	if(m_bIsBurst && m_bIsBirth)
	{
		for(decltype(m_vParticles.size()) i = 0; i < m_vParticles.size(); ++i)
		{
			m_vParticles[i].m_vPos.fX = m_vPos.fX;
			m_vParticles[i].m_vPos.fY = m_vPos.fY;
		}
	}


	if( m_nPartsActive >= (int)m_vParticles.size() )
		m_bIsBirth = false;
	
	for(int i = m_nDeadCuttoff; i < m_nPartsActive; ++i)
	{ 
		// this is my particle, there are many like it
		// but this one is mine
		tParticle* p = &m_vParticles[i];\

		if(m_vParticles[i].m_fAge >= m_fParticleDuration || m_vParticles[i].m_fAge == 0.0f)
		{
			if(m_bIsLooping)
				SpawnParticle(&m_vParticles[i]);
			else if(m_vParticles[i].m_fAge >= m_fParticleDuration)
				++m_nDeadCuttoff;
		}

		

		if((unsigned int)m_vParticles[i].m_nCurColorStop < m_vParticles[i].m_nColorIndices.size()-1)
		{
			float diff = 0.0f;
			int curColorStop = m_vParticles[i].m_nColorIndices[m_vParticles[i].m_nCurColorStop];
			int nextColorStop = m_vParticles[i].m_nColorIndices[m_vParticles[i].m_nCurColorStop+1];

			diff = (m_vUniqueColors[nextColorStop].m_fPosOnLifeSpan)
				-(m_vUniqueColors[curColorStop].m_fPosOnLifeSpan);

			float curPos = 0.0f;
			curPos = m_vParticles[i].m_fAge/m_fParticleDuration-m_vUniqueColors[curColorStop].m_fPosOnLifeSpan;

			float coef = (diff != 0) ? curPos/diff : 1.0f; // 1.0f for no diff because it IS at the same posOnLifeSpan

			D3DXCOLOR curColor = m_vUniqueColors[curColorStop].m_d3Color;
			D3DXCOLOR nextColor = m_vUniqueColors[nextColorStop].m_d3Color;

			m_vParticles[i].color.a = curColor.a + (nextColor.a - curColor.a) * coef;
			m_vParticles[i].color.r = curColor.r + (nextColor.r - curColor.r) * coef;
			m_vParticles[i].color.g = curColor.g + (nextColor.g - curColor.g) * coef;
			m_vParticles[i].color.b = curColor.b + (nextColor.b - curColor.b) * coef;
			

			if(m_vParticles[i].m_fAge/m_fParticleDuration >= m_vUniqueColors[nextColorStop].m_fPosOnLifeSpan)
				++m_vParticles[i].m_nCurColorStop;
		}

		if((unsigned int)m_vParticles[i].m_nCurSizeStop < m_vParticles[i].m_nSizeIndices.size()-1)
		{
			float diff = 0.0f;
			int curSizeStop = m_vParticles[i].m_nSizeIndices[m_vParticles[i].m_nCurSizeStop];
			int nextSizeStop = m_vParticles[i].m_nSizeIndices[m_vParticles[i].m_nCurSizeStop+1];
			
			diff = (m_vUniqueSizes[nextSizeStop].m_fPosOnLifeSpan)
				-(m_vUniqueSizes[curSizeStop].m_fPosOnLifeSpan);

			float curPos = 0.0f;
			curPos = m_vParticles[i].m_fAge/m_fParticleDuration-m_vUniqueSizes[curSizeStop].m_fPosOnLifeSpan;

			float coef = (diff != 0) ? curPos/diff : 1.0f;

			float curWidth = m_vUniqueSizes[curSizeStop].m_fWidth;
			float curHeight = m_vUniqueSizes[curSizeStop].m_fHeight;

			float nextWidth = m_vUniqueSizes[nextSizeStop].m_fWidth;
			float nextHeight = m_vUniqueSizes[nextSizeStop].m_fHeight;

			m_vParticles[i].m_fWidth = (curWidth + (nextWidth - curWidth) * coef) * m_fSizeScalar;
			m_vParticles[i].m_fHeight = (curHeight + (nextHeight - curHeight) * coef) * m_fSizeScalar;

			if(m_vParticles[i].m_fAge/m_fParticleDuration >= m_vUniqueSizes[nextSizeStop].m_fPosOnLifeSpan)
				++m_vParticles[i].m_nCurSizeStop;
		}
		
		if((unsigned int)m_vParticles[i].m_nCurVelocityStop < m_vParticles[i].m_nVelocityIndices.size()-1)
		{
			float diff = 0.0f;
			int curVelStop = m_vParticles[i].m_nVelocityIndices[m_vParticles[i].m_nCurVelocityStop];
			int nextVelStop = m_vParticles[i].m_nVelocityIndices[m_vParticles[i].m_nCurVelocityStop+1];

			diff = (m_vUniqueVelocities[nextVelStop].m_fPosOnLifeSpan)
				-(m_vUniqueVelocities[curVelStop].m_fPosOnLifeSpan);

			float curPos = 0.0f;
			curPos = m_vParticles[i].m_fAge/m_fParticleDuration-m_vUniqueVelocities[curVelStop].m_fPosOnLifeSpan;

			float coef = (diff != 0) ? curPos/diff : 1.0f;

			float curVelX = m_vUniqueVelocities[curVelStop].m_vVel.fX;
			float curVelY = m_vUniqueVelocities[curVelStop].m_vVel.fY;

			float nextVelX = m_vUniqueVelocities[nextVelStop].m_vVel.fX;
			float nextVelY = m_vUniqueVelocities[nextVelStop].m_vVel.fY;

			m_vParticles[i].m_vVel.fX = (curVelX + (nextVelX - curVelX) * coef);
			m_vParticles[i].m_vVel.fY = (curVelY + (nextVelY - curVelY) * coef);

			if(m_vParticles[i].m_fAge/m_fParticleDuration >= m_vUniqueVelocities[nextVelStop].m_fPosOnLifeSpan)
				++m_vParticles[i].m_nCurVelocityStop;
		}
		

		m_vParticles[i].m_vPos.fX += (m_vParticles[i].m_vVel.fX + m_vTotalForce.fX) * fElapsed;
		m_vParticles[i].m_vPos.fY += (m_vParticles[i].m_vVel.fY + m_vTotalForce.fY) * fElapsed;

		m_vParticles[i].m_fAge += fElapsed;
	}


	
}

void CParticleEmitter::Render(void)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	float width = (float)(pTM->GetTextureWidth(m_nImageID));
	float height = (float)(pTM->GetTextureHeight(m_nImageID));

	LPDIRECT3DDEVICE9 device = CSGD_Direct3D::GetInstance()->GetDirect3DDevice();
	
	DWORD prevSourceBlend;
	DWORD prevDestBlend;
	device->GetRenderState( D3DRS_SRCBLEND, &prevSourceBlend);
	device->GetRenderState( D3DRS_DESTBLEND, &prevDestBlend);

	//if( CSGD_DirectInput::GetInstance()->KeyDown(DIK_T))
	{
		device->SetRenderState( D3DRS_SRCBLEND, m_nSourceBlendMode );
		device->SetRenderState( D3DRS_DESTBLEND, m_nDestBlendMode );
	}

	for(int i = m_nDeadCuttoff; i < m_nPartsActive; ++i)
	{
		tParticle* p = &m_vParticles[i];

		if(m_bUseCamera)
			pTM->Draw(m_nImageID, (int)((p->m_vPos.fX-width/2.0f*p->m_fWidth)-CCamera::GetInstance()->OffsetX()),
				(int)((p->m_vPos.fY-height/2.0f*p->m_fHeight)-CCamera::GetInstance()->OffsetY()),
				p->m_fWidth, p->m_fHeight, (RECT*)0, 0.0f, 0.0f, 0.0f, 
				D3DCOLOR_ARGB(int(p->color.a*255), int(p->color.r*255), int(p->color.g*255), int(p->color.b*255)) );
		else
			pTM->Draw(m_nImageID, (int)(p->m_vPos.fX-width/2.0f*p->m_fWidth),
				(int)(p->m_vPos.fY-height/2.0f*p->m_fHeight),
				p->m_fWidth, p->m_fHeight, (RECT*)0, 0.0f, 0.0f, 0.0f, 
				D3DCOLOR_ARGB(int(p->color.a*255), int(p->color.r*255), int(p->color.g*255), int(p->color.b*255)) );
		
		//std::wostringstream oss;
		//oss << _T("NUMBER: ") << i;
		//CSGD_Direct3D::GetInstance()->DrawTextW( (TCHAR*)oss.str().c_str(), p->m_vPos.fX, p->m_vPos.fY, 255, 255, 255);
	}

	//CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	device->SetRenderState( D3DRS_SRCBLEND, prevSourceBlend );
	device->SetRenderState( D3DRS_DESTBLEND, prevDestBlend );

	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	//pD3D->DrawRect(m_rParticleRect, 255,255,255 );

	int x = 0;
}

void CParticleEmitter::SpawnParticle(tParticle* particle)
{
	if( m_nShape == PES_POINT )
	{
		particle->m_vPos.fX = m_vPos.fX;
		particle->m_vPos.fY = m_vPos.fY;
	}
	else
	{
		RandParticleInRect(particle);
	}

	particle->m_nCurColorStop = 0;
	particle->m_nCurSizeStop = 0;
	particle->m_fAge = 0.0f;
	/*particle->color.a = 1.0f;
	particle->color.r = 1.0f;
	particle->color.g = 1.0f;
	particle->color.b = 1.0f;*/
	particle->m_fScale = 1.0f;
}

void CParticleEmitter::RandParticleInRect(tParticle* particle)
{
	for(;;)
	{
		particle->m_vPos.fX = (float)(rand()% (m_rParticleRect.right-m_rParticleRect.left+1)+m_rParticleRect.left);
		particle->m_vPos.fY = (float)(rand()% (m_rParticleRect.bottom-m_rParticleRect.top+1)+m_rParticleRect.top);

		if(m_nShape == PES_RECTANGLE || m_nShape == PES_POINT || 
			Distance(m_vPos, particle->m_vPos) < m_fParticleRadius/**2*/ ) break;
	}
}


float CParticleEmitter::Distance(tVector2D x, tVector2D y)
{
	return sqrt( pow((x.fX-y.fX), 2) + pow((x.fY-y.fY), 2) );
}

