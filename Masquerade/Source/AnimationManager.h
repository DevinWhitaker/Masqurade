#pragma once

#include <vector>
#include <map>
#include <string>

#include "Animation.h"
#include "Frame.h"
using namespace std;

class CAnimationManager 
{
private:

	vector<CFrame*>						m_vpFrames;

	std::map<std::string, CAnimation*>	m_vpAnimation;

	vector<string>						m_vFilenames;

	////////////////////////////////////////////////////////////
	//Purpose:	constructor
	//
	//In:		None
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	CAnimationManager(void);

	////////////////////////////////////////////////////////////
	//Purpose:	copy constructor
	//
	//In:		None
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	CAnimationManager(CAnimationManager&);

	////////////////////////////////////////////////////////////
	//Purpose:	private assignment operator
	//
	//In:		reference to a CAnimationManager
	//
	//Return:	reference to a CAnimationManager
	////////////////////////////////////////////////////////////
	CAnimationManager& operator=(CAnimationManager&);

	////////////////////////////////////////////////////////////
	//Purpose:	destructor
	//
	//In:		None
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	~CAnimationManager(void);

	////////////////////////////////////////////////////////////
	//Purpose:	DO NOT USE! updates the CFrames data based on the animation 
	//			it corresponds to
	//
	//In:
	//		animInfo:	the CFrame to update
	//		fElapsed:	the change over time
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	void Update(CFrame& animInfo, float fElapsed);

	////////////////////////////////////////////////////////////
	//Purpose:	DO NOT USE!	renders the animation that corresponds to the 
	//			CFrame
	//
	//In:	animInfo:	the CFrame containing the needed animation information
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	void Render(CFrame animInfo);

	////////////////////////////////////////////////////////////
	//Purpose:	DO NOT USE!	checks collision between an object and an animation
	//
	//In:
	//		pBase:	the object to check collision against
	//		key:	the info of the object checking collision
	//
	//Return:	true if a collison has happened
	////////////////////////////////////////////////////////////
	bool CheckCollision(CBaseObject* pBase, CFrame key);
	
	////////////////////////////////////////////////////////////
	//Purpose:	DO NOT USE!
	//
	//In:
	//		pBase:	the info of the object to check collision against
	//		key:	the info of the object checking collision
	//
	//Return:	true if a collison has happened
	////////////////////////////////////////////////////////////
	bool CheckCollision(CFrame* pBase, CFrame key);

public:
	
	////////////////////////////////////////////////////////////
	//Purpose:	allow quick access to animations in the list
	//
	//In:		id:	the index into the map of animations
	//
	//Return:	the animation at the selected index
	////////////////////////////////////////////////////////////
	CAnimation* operator[](string id);

	////////////////////////////////////////////////////////////
	//Purpose:	allows access to this singleton class
	//
	//In:		None
	//
	//Return:	pointer to a CAnimationManager
	////////////////////////////////////////////////////////////
	static CAnimationManager* GetInstance(void);

	////////////////////////////////////////////////////////////
	//Purpose:	updates all animations
	//
	//In:
	//		fElapsed:	the change over time
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	void UpdateAll( float fElapsed );

	////////////////////////////////////////////////////////////
	//Purpose:	renders all animations
	//
	//
	//In:	
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	void RenderAll( void );

	////////////////////////////////////////////////////////////
	//Purpose:	creates animations as detailed in the xml file
	//
	//In:
	//		filename:	the file that contains the animations details
	//
	//Return:	a vector of ids to all the animations found in the file
	////////////////////////////////////////////////////////////
	vector<string> CreateAnimationFromFile(const char* filename);

	////////////////////////////////////////////////////////////
	//Purpose:	clean up any dynamic memory
	//
	//In:		None
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	void Shutdown();

	////////////////////////////////////////////////////////////
	//Purpose:	Check collsion between every single active animation
	//
	//In:		None
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	void CheckAllCollision( void );

	////////////////////////////////////////////////////////////
	//Purpose:		adds a CFrame to the vector containing all CFrames
	//
	//In:
	//		pkey:	address of the CFrame to add
	//
	//Return:		None
	////////////////////////////////////////////////////////////
	void AddFrame(CFrame& pkey);

	////////////////////////////////////////////////////////////
	//Purpose:		removes a CFrame from the vector containing all CFrames
	//
	//In:
	//		pkey:	address of the CFrame to remove
	//
	//Return:		None
	////////////////////////////////////////////////////////////
	inline void RemoveFrame(CFrame* pkey)
	{
		for( size_t i = 0; i < m_vpFrames.size(); ++i)
		{
			if( m_vpFrames[i] == pkey )
				m_vpFrames.erase(m_vpFrames.begin()+i);
		}
	}

	////////////////////////////////////////////////////////////
	//Purpose:		empties the manager of all CFrames
	//
	//In:			None
	//		
	//
	//Return:		None
	////////////////////////////////////////////////////////////
	void Clear();
};
