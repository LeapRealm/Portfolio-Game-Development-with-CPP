#include "ST.h"

#include "iStd.h"

static iShadertoy* st;

void loadST()
{
	st = new iShadertoy();
	st->load(stInfoTest);
}

void freeST()
{
	delete st;
}

void drawST(float dt)
{
	glDisable(GL_BLEND);
	st->paint(dt);
	glEnable(GL_BLEND);
}

void keyST(iKeyState state, iPoint p)
{
	st->key(state, iPointMake(p.x, devSize.height - p.y));
}

iSTInfo stInfoTest[5] =
// original link : https://www.shadertoy.com/view/XdycWG
{
	{
		"assets/st/mountA.glsl",
		{"assets/st/mount.jpg",	nullptr,	nullptr,	nullptr},
		{BN,							BA,			BN,			BN},
		{Mipmap,						Linear,		Linear,		Linear},
		{Repeat,						Clamp,		Clamp,		Clamp},
	},
	{
		"assets/st/mountB.glsl",
		{nullptr,					nullptr,	nullptr,	nullptr},
		{BA,							BN,			BN,			BN},
		{Mipmap,						Linear,		Linear,		Linear},
		{Clamp,							Clamp,		Clamp,		Clamp},
	},
	{
		"assets/st/mountC.glsl",
		{nullptr,					nullptr,	nullptr,	nullptr},
		{BB,							BN,			BN,			BN},
		{Mipmap,						Linear,		Linear,		Linear},
		{Clamp,							Clamp,		Clamp,		Clamp},
	},
	{
		nullptr,
	},
	{
		"assets/st/mountI.glsl",
		{nullptr,					nullptr,	nullptr,	nullptr},
		{BA,							BC,			BN,			BN},
		{Mipmap,						Linear,		Linear,		Linear},
		{Clamp,							Clamp,		Clamp,		Clamp},
	},
};
