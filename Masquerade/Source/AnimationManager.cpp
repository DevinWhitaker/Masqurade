#include "AnimationManager.h"
#include "AnchorAnimation.h"
#include "CellAnimation.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../tinyxml/tinyxml.h"
#include <sstream>


#include "Camera.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"


typedef std::vector<CFrame*>::iterator iter;

////////////////////////////////////////////////////////////
/////////////*/*/*CHANGES TO BE MADE*/*/*///////////////////
////////////////////////////////////////////////////////////
//
//
//	X	AnimationManager should contain a vector of 
//		all cFrames in the game.
//
//	X	Animations should be mapped by animation name not 
//		the name of their image file.
//
//	X	CheckCollsion function should handle all collisions
//		between two animations
//
//	*	Animations need to be able to throw events when 
//		rendering as well as upon collsion 
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//Purpose:	constructor
//
//In:		None
//
//Return:	None
////////////////////////////////////////////////////////////
CAnimationManager::CAnimationManager(void)
{
	m_vpAnimation.clear();
}

////////////////////////////////////////////////////////////
//Purpose:	destructor
//
//In:		None
//
//Return:	None
////////////////////////////////////////////////////////////
CAnimationManager::~CAnimationManager(void)
{
	map<std::string, CAnimation*>::iterator i;

	for( i = m_vpAnimation.begin(); i != m_vpAnimation.end(); i++ )
	{
		CAnchorAnimation* temp = (CAnchorAnimation*)m_vpAnimation[i->first];
		delete temp;
		m_vpAnimation[i->first] = nullptr;
	}

	m_vpAnimation.clear();
}

////////////////////////////////////////////////////////////
//Purpose:	allows access to this singleton class
//
//In:		None
//
//Return:	pointer to a CAnimationManager
////////////////////////////////////////////////////////////
CAnimationManager* CAnimationManager::GetInstance(void)
{
	static CAnimationManager s_Instance;

	return &s_Instance;
}

////////////////////////////////////////////////////////////
//Purpose:	updates all animations
//
//In:
//		fElapsed:	the change over time
//
//Return:	None
////////////////////////////////////////////////////////////
void CAnimationManager::UpdateAll( float fElapsed )
{
	iter anime;
	CCamera* cam = CCamera::GetInstance();

#if _DEVIN
	int i = 0;
#endif

	for(anime = m_vpFrames.begin(); anime != m_vpFrames.end(); anime++)
	{
		//Culling
		if( (((*anime))->GetX() + ((*anime))->GetWidth()) <= 0.0f || ((*anime))->GetX() > (cam->OffsetX() + cam->GetScreenSpaceX() + 0.0f)
			|| (((*anime))->GetY() + ((*anime))->GetHeight()) <= 0.0f || ((*anime))->GetY() > (cam->OffsetY() + cam->GetScreenSpaceY() + 0.0f) )
			continue;

		m_vpAnimation[(*anime)->GetAnimationID()]->Update(fElapsed,**anime);

#if _DEVIN
		i++;
#endif
	}

#if _DEVIN
	printf("%i animations updated\n",i);
#endif
}

////////////////////////////////////////////////////////////
//Purpose:	updates the CFrames data based on the animation 
//			it corresponds to
//
//In:
//		animInfo:	the CFrame to update
//		fElapsed:	the change over time
//
//Return:	None
////////////////////////////////////////////////////////////
void CAnimationManager::Update(CFrame& animInfo, float fElapsed)
{
	m_vpAnimation[animInfo.m_nAnimationID]->Update(fElapsed,animInfo);
}

////////////////////////////////////////////////////////////
//Purpose:	renders all animations
//
//
//In:	
//
//Return:	None
////////////////////////////////////////////////////////////
void CAnimationManager::RenderAll( void )
{
	iter anime;

	for(anime = m_vpFrames.begin(); anime != m_vpFrames.end(); anime++)
	{
		m_vpAnimation[(*anime)->m_nAnimationID]->Render((*anime)->m_nCurrFrame,
			(int)*(*anime)->m_nX,(int)*(*anime)->m_nY, (*anime)->m_fScaleX, (*anime)->m_fScaleY, 
			(*anime)->dwColor,(*anime)->m_bIsFlipped, 
			(*anime)->m_bIsPlaying,(*anime)->m_bIsLooping);
#if FALSE//_DEVIN
		CCamera* cam = CCamera::GetInstance();
		RECT test;
		SetRect(&test,
			(*anime)->GetRect().left   - cam->OffsetX(),
			(*anime)->GetRect().top    - cam->OffsetY(),
			(*anime)->GetRect().right  - cam->OffsetX(),
			(*anime)->GetRect().bottom - cam->OffsetY()
			);
		CSGD_Direct3D::GetInstance()->DrawRect( test, 0, 0, 255 );
#endif
	}
}

////////////////////////////////////////////////////////////
//Purpose:	renders the animation that corresponds to the 
//			CFrame
//
//In:	animInfo:	the CFrame containing the needed animation information
//
//Return:	None
////////////////////////////////////////////////////////////
void CAnimationManager::Render(CFrame animInfo)
{
	m_vpAnimation[animInfo.m_nAnimationID]->Render(animInfo.m_nCurrFrame,
		(int)*animInfo.m_nX,(int)*animInfo.m_nY, animInfo.m_fScaleX, animInfo.m_fScaleY, 
		animInfo.dwColor,animInfo.m_bIsFlipped, 
		animInfo.m_bIsPlaying,animInfo.m_bIsLooping);
}

////////////////////////////////////////////////////////////
//Purpose:	creates animations as detailed in the xml file
//
//In:
//		filename:	the file that contains the animations details
//
//Return:	a vector of ids to all the animations found in the file
////////////////////////////////////////////////////////////
vector<string> CAnimationManager::CreateAnimationFromFile(const char* filename)
{
	TiXmlDocument doc;
	vector<string> ids;
	ids.clear();
	bool isCreated = false;
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	if( doc.LoadFile( filename ) == false )
		return ids;

	TiXmlElement* pRoot = doc.RootElement();
	if( pRoot == nullptr )
		return ids;

	for( size_t i = 0; i < m_vFilenames.size(); i++ )
	{
		if( strcmp( m_vFilenames[i].c_str(), filename ) == 0 )
		{
			isCreated = true;
			break;
		}
	}

	if( !isCreated )
	{

		TiXmlElement* pAnimation = pRoot->FirstChildElement( "animation_info" );

		while( pAnimation != nullptr )
		{
			string imageFile = pAnimation->Attribute("image_file");
			//int pos = imageFile.find_last_of("/");
			string animationID = pAnimation->Attribute("name");
			//string animationID = imageFile.substr();
			ids.push_back(animationID);

			int r = atoi(pAnimation->Attribute("r"));
			int g = atoi(pAnimation->Attribute("g"));
			int b = atoi(pAnimation->Attribute("b"));

			wostringstream oss;
			oss << imageFile.c_str();

			int id;

			if( r ==-1 || g == -1 || b == -1 )
				id = pTM->LoadTexture( oss.str().c_str() );
			else
				id = pTM->LoadTexture( oss.str().c_str(), D3DCOLOR_XRGB(r,g,b) );

			if( strcmp("anchor" ,pAnimation->Attribute("type")) == 0 )
			{
				CAnchorAnimation* pNewAnimation = new CAnchorAnimation;

				//int id = pTM->LoadTexture( oss.str().c_str() );

				pNewAnimation->Init(id);

				int count = atoi(pAnimation->Attribute("num_anchors"));
				TiXmlElement* pAnchor = pAnimation->FirstChildElement("anchor");
				for( int i = 0; i < count; i++ )
				{
					TiXmlElement* pRect = pAnchor->FirstChildElement("rect");
					RECT src;
					src.left = atoi(pRect->Attribute("left"));
					src.right = atoi(pRect->Attribute("right"));
					src.top = atoi(pRect->Attribute("top"));
					src.bottom = atoi(pRect->Attribute("bottom"));

					TiXmlElement* pPoint = pAnchor->FirstChildElement("point");
					POINT anchr;
					anchr.x = atoi(pPoint->Attribute("x"));
					anchr.y = atoi(pPoint->Attribute("y"));

					TiXmlElement* pDuration = pAnchor->FirstChildElement("duration");
					float duration = (float)atof(pDuration->Attribute("value"));

					vector<AnimationEvent*> events;

					int numEvents = atoi(pAnchor->Attribute("events"));
					TiXmlElement* pEvent = pAnchor->FirstChildElement("event");

					for(int i =0; i < numEvents; i++)
					{
						AnimationEvent* e = new AnimationEvent;

						int left	= atoi(pEvent->Attribute("left"));
						int right	= atoi(pEvent->Attribute("right"));
						int top		= atoi(pEvent->Attribute("top"));
						int bottom	= atoi(pEvent->Attribute("bottom"));

						SetRect(&e->collision,left,top,right,bottom);
						e->eventID = pEvent->Attribute("id");

						events.push_back(e);

						pEvent = pEvent->NextSiblingElement("event");
					}

					pNewAnimation->CreateAnchor(src,anchr,duration,events);

					pAnchor = pAnchor->NextSiblingElement("anchor");
				}

				m_vpAnimation[animationID] = pNewAnimation;
			}
			else if( strcmp("cell",pAnimation->Attribute("type")) == 0 )
			{
				CCellAnimation* pNewAnimation = new CCellAnimation;
				//int id = pTM->LoadTexture( oss.str().c_str() );

				TiXmlElement* pCell = pAnimation->FirstChildElement("cell");
				int width = atoi(pCell->Attribute("width"));
				int height = atoi(pCell->Attribute("height"));

				TiXmlElement* pFrames = pAnimation->FirstChildElement("frames");
				int max_frames = atoi(pFrames->Attribute("max"));

				TiXmlElement* pSprite_sheet = pAnimation->FirstChildElement("sprite_sheet");
				int rows = atoi(pSprite_sheet->Attribute("rows"));
				int cols = atoi(pSprite_sheet->Attribute("cols"));

				pNewAnimation->Init(id,width,height,max_frames,rows,cols);

				m_vpAnimation[animationID] = pNewAnimation;
			}

			pAnimation = pAnimation->NextSiblingElement("animation_info");

		}

		m_vFilenames.push_back( filename );

	}
	else
	{
		TiXmlElement* pAnimation = pRoot->FirstChildElement( "animation_info" );

		while( pAnimation != nullptr )
		{
			string animationID = pAnimation->Attribute("name");
			
			ids.push_back(animationID);

			pAnimation = pAnimation->NextSiblingElement("animation_info");
		}
	}

	return ids;
}

////////////////////////////////////////////////////////////
//Purpose:	clean up any dynamic memory
//
//In:		None
//
//Return:	None
////////////////////////////////////////////////////////////
void CAnimationManager::Shutdown()
{
	m_vFilenames.clear();

	m_vpFrames.clear();

	map<std::string, CAnimation*>::iterator i;
	for(i = m_vpAnimation.begin(); i != m_vpAnimation.end(); i++ )
	{
		CAnimation* temp = m_vpAnimation[i->first];
		delete temp;
		m_vpAnimation[i->first] = nullptr;
	}
	m_vpAnimation.clear();
}

////////////////////////////////////////////////////////////
//Purpose:	DO NOT USE!
//			checks collision between an object and an animation
//
//In:
//		pBase:	the object to check collision against
//		key:	the info of the object checking collision
//
//Return:	true if a collison has happened
////////////////////////////////////////////////////////////
bool CAnimationManager::CheckCollision(CBaseObject* pBase, CFrame key)
{
	return m_vpAnimation[key.GetAnimationID()]->CheckCollision(pBase,key);
}

////////////////////////////////////////////////////////////
//Purpose:	DO NOT USE!
//
//In:
//		pBase:	the info of the object to check collision against
//		key:	the info of the object checking collision
//
//Return:	true if a collison has happened
////////////////////////////////////////////////////////////
bool CAnimationManager::CheckCollision(CFrame* pBase, CFrame key)
{
	return m_vpAnimation[key.GetAnimationID()]->CheckCollision(pBase,key);
}

////////////////////////////////////////////////////////////
//Purpose:	allow quick access to animations in the list
//
//In:		id:	the index into the map of animations
//
//Return:	the animation at the selected index
////////////////////////////////////////////////////////////
CAnimation* CAnimationManager::operator[](string id)
{
	return m_vpAnimation[id];
}

////////////////////////////////////////////////////////////
//Purpose:	Check collsion between every single active animation
//
//In:		None
//
//Return:	None
////////////////////////////////////////////////////////////
void CAnimationManager::CheckAllCollision( void )
{
	iter animation1;
	iter animation2;
	CCamera* cam = CCamera::GetInstance();

	for(animation1 = m_vpFrames.begin(); animation1 != m_vpFrames.end(); animation1++)
	{
		for(animation2 = m_vpFrames.begin(); animation2 != m_vpFrames.end(); animation2++)
		{
			if( abs( (*animation1)->GetX() - (*animation2)->GetX() ) > cam->GetScreenSpaceX() / 3  )
				continue;

			if(animation1 != animation2)
			{//need the double dereference so that the signature matches the CFrame on CFrame collision function
				if(m_vpAnimation[(*animation1)->GetAnimationID()]->CheckCollision(*animation2,**animation1))
				{}
			}
		}
	}
}

////////////////////////////////////////////////////////////
//Purpose:		adds a CFrame to the vector containing all CFrames
//
//In:
//		pkey:	address of the CFrame to add
//
//Return:		None
////////////////////////////////////////////////////////////
void CAnimationManager::AddFrame(CFrame& pkey)
{
	m_vpFrames.push_back( &pkey );
}

////////////////////////////////////////////////////////////
//Purpose:		empties the manager of all CFrames
//
//In:			None
//		
//
//Return:		None
////////////////////////////////////////////////////////////
void CAnimationManager::Clear()
{
	m_vpFrames.clear();
}