#pragma once

enum EGameEvent
{
	EGE_Error = 0,
	EGE_Player1_Jump,
	EGE_Player2_Jump,
	EGE_Player3_Jump,
	EGE_Player4_Jump,
	EGE_Pause
};

enum EGameState
{
	EGS_DISABLED,
	EGS_STARTED,
	EGS_PAUSED,
	EGS_ENDED
};

struct Event
{
	EGameEvent event;
	EGameState state;
};


