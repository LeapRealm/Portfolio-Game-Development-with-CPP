#pragma once

enum iGameState
{
	iGameStateMap = 0,
	iGameStateST,

	iGameStateNone

};

extern iGameState gs;

typedef void (*MethodLoading)();

void setLoading(iGameState gs, MethodLoading free, MethodLoading load);
void drawLoading(float dt);

#define loadingDelta 0.5f
