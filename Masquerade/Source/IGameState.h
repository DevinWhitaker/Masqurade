#pragma once
class IGameState
{
public:

	virtual ~IGameState(void) = 0
	{ /* Nothing */ }

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual bool Input() = 0;
	virtual void Update(float fElapsedTime) = 0;
	virtual void Render() = 0;
};

