#include "Loading.h"

iGameState gs;

static iGameState gsNew;
static MethodLoading methodFree, methodLoad;
static float loadingDt = 0.0f;

void setLoading(iGameState gs, MethodLoading free, MethodLoading load)
{
	if (loadingDt)
		return;

	gsNew = gs;
	methodFree = free;
	methodLoad = load;
	loadingDt = 0.000001f;
}

void drawLoading(float dt)
{
	if (loadingDt == 0.0f)
		return;

	float alpha = 1.0f;

	if (loadingDt < loadingDelta)
	{
		alpha = loadingDt / loadingDelta;
		loadingDt += dt;

		if (loadingDt >= loadingDelta)
		{
			alpha = 1.0f;

			methodFree();
			gs = iGameStateNone;
			loadingDt = loadingDelta;
		}
	}
	else if (loadingDt < loadingDelta + 0.7f)
	{
		alpha = 1.0f;
		loadingDt += dt;

		if (loadingDt >= loadingDelta + 0.7f)
		{
			methodLoad();
			gs = gsNew;
			loadingDt = loadingDelta + 0.7f;
		}
	}
	else
	{
		alpha = 1.0f - (loadingDt - loadingDelta - 0.7f) / loadingDelta;
		loadingDt += dt;

		if (loadingDt > loadingDelta * 2 + 0.7f)
			loadingDt = 0.0f;
	}

	setRGBA(0, 0, 0, alpha);
	fillRect(0, 0, devSize.width, devSize.height);
}
