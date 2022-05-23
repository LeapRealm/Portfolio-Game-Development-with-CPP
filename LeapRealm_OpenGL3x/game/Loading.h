#pragma once

#include "iStd.h"

enum iGameState
{
	iGameStateMenu = 0,
	iGameStateMap,

	iGameStateST,

	iGameStateNone

};

extern iGameState gs;

typedef void (*MethodLoading)();

void setLoading(iGameState gs, MethodLoading free, MethodLoading load);
void drawLoading(float dt);

#define loadingDelta 0.5f
