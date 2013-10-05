#pragma once
#include "Animation.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include <vector>
#include <string>
using namespace std;



struct AnimationEvent
{
	string eventID;
	RECT   collision;
};


class CAnchorAnimation : public CAnimation
{

public:
	friend class CFrame;
	////////////////////////////////////////////////////////////
	//Purpose:	creates a tAnchor structure and adds it to the animation
	//
	//In:		
	//			source:		the section of the image to draw when rendering this frame
	//			anchor:		represents the actual position of the object
	//			duration:	how much time to spend on this frame
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	void CreateAnchor(RECT source,POINT anchor, float duration, vector<AnimationEvent*> events);

	void CreateFromFile( char* filename );

	RECT GetRect( int i );

	////////////////////////////////////////////////////////////
	//Purpose:	sets the image id
	//
	//In:		the index into the texture manager that this
	//			animation should render from
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	void Init(int id);

	////////////////////////////////////////////////////////////
	//Purpose:	draws this animation to the screen
	//
	//In:		
	//		frame:		which frame this animation is currently on
	//		x:			the position on the screen to draw the animation
	//		y:			the position on the screen to draw the animation
	//		scalex:		scales the width by this amount
	//		scaley:		scales the height by this amount
	//		dwColor:	tints the animation this color
	//		flipped:	renders the animation backwards if true
	//		playing:	renders the animation if true
	//		looping:	NOT USED
	//
	//Return:
	////////////////////////////////////////////////////////////
	void Render(int frame, int x, int y, float scalex, float scaley, DWORD dwColor, bool flipped = false, bool playing = true, bool looping = true);
	
	////////////////////////////////////////////////////////////
	//Purpose:	determines which frame to play next
	//
	//In:
	//		fElapsed:	change in time
	//		animInfo:	structure that stores and updates the animations 
	//					updateble info
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	void Update(float fElapsed, CFrame& animInfo);

	////////////////////////////////////////////////////////////
	//Purpose:	determine if a collision has happend
	//
	//In:
	//		pBase:	object to check collision against
	//		key:	the animation info of the object checking collision
	//
	//Return:	true if collision happend
	////////////////////////////////////////////////////////////
	bool CheckCollision(CBaseObject* pBase, CFrame key);

	////////////////////////////////////////////////////////////
	//Purpose:	determine if a collision has happened
	//
	//In:
	//		pBase:	the animation info of the object to check collision against
	//		key:	the animation info of the object checking collision
	//
	//Return:	true if collision happend
	////////////////////////////////////////////////////////////
	bool CheckCollision(CFrame* pBase, CFrame key);

	////////////////////////////////////////////////////////////
	//Purpose:	constructor
	//
	//In:		None
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	CAnchorAnimation(void);

	////////////////////////////////////////////////////////////
	//Purpose:	destructor
	//
	//In:		None
	//
	//Return:	None
	////////////////////////////////////////////////////////////
	~CAnchorAnimation(void);

	int GetXAnchor(int frame)	{ return m_vAnchors[frame]->Anchor.x; }
	int GetYAnchor(int frame)	{ return m_vAnchors[frame]->Anchor.y; }
	int GetWidth(int frame)		{ return m_vAnchors[frame]->Bounds.right - m_vAnchors[frame]->Bounds.left; }
	int GetHeight(int frame)	{ return m_vAnchors[frame]->Bounds.bottom - m_vAnchors[frame]->Bounds.top; }

private:
	
	int m_nImageID;// holds the texture manager image id of the sprite sheet

	//std::string m_szFileName;//	

	CSGD_TextureManager* m_pTM;// pointer to the taxture manager

	struct tAnchor// anchor struct to hold the source rect
	{
		RECT  Bounds;
		POINT Anchor;
		float Duration;
		vector<AnimationEvent*> Events;
		tAnchor(RECT source,POINT anchor, float duration, vector<AnimationEvent*> events)
		{
			SetRect(&Bounds,source.left,source.top,source.right,source.bottom);
			Anchor.x = anchor.x;
			Anchor.y = anchor.y;
			Duration = duration;
			Events	 = events;
		}

		~tAnchor()
		{
			for(size_t i = 0; i < Events.size(); i++ )
			{
				AnimationEvent* temp = Events[i];
				delete temp;
				Events[i] = nullptr;
			}
			Events.erase( Events.begin(), Events.end() );
		}
	};

	vector<tAnchor*> m_vAnchors;// vector of anchors to store the animation info

	public:
	
	////////////////////////////////////////////////////////////
	//Purpose:	allow quick acces to the animations frames
	//
	//In:
	//		curr_frame:	the index into the frame list
	//
	//Return:	the frame at that index
	////////////////////////////////////////////////////////////
	tAnchor* operator[](int curr_frame){return m_vAnchors[curr_frame];}
};

