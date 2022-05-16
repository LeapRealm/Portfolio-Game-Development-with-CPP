#include "ST.h"

#include "iStd.h"

static iShadertoy* st;
int stIndex;

void loadST()
{
	st = new iShadertoy();
	st->load(stInfoTest[0]);
	stIndex = 0;
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

	if (getActionKeyPressed(KEY_SPACE))
	{
		stIndex = (stIndex + 1) % 2;
		st->load(stInfoTest[stIndex]);
	}
}

void keyST(iKeyState state, iPoint p)
{
	st->key(state, iPointMake(p.x, devSize.height - p.y));
}

iSTInfo stInfoTest[2][5] =
{
	// [0] type
	{
		{
			nullptr,
		},
		{
			nullptr,
		},
		{
			nullptr,
		},
		{
			nullptr,
		},
		{
			"assets/st/testI.glsl",
			{nullptr, nullptr, nullptr, nullptr},
			{BN, BN, BN, BN},
			{Linear,	Linear,	Linear,	Linear},
			{Clamp, Clamp,	Clamp, Clamp},
		}
	},

	// [1] type : https://www.shadertoy.com/view/XdycWG
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
	},
};