#include "iParticleSystem.h"

#include "iStd.h"

void iParticle::draw(float dt)
{
	float r = currDelta / delta;
	currDelta += dt;

	iPoint p = linear(positionStart, positionEnd, r);
	float s = linear(scaleStart, scaleEnd, r);
	iColor4f c;
	c.r = (colorStart.r * (1 - r) + colorEnd.r * r) / 255.f;
	c.g = (colorStart.g * (1 - r) + colorEnd.g * r) / 255.f;
	c.b = (colorStart.b * (1 - r) + colorEnd.b * r) / 255.f;
	c.a = (colorStart.a * (1 - r) + colorEnd.a * r) / 255.f;

	setRGBA(c.r, c.g, c.b, c.a);
	fillRect(p.x - s / 2, p.y - s / 2, s, s);
}
 
iParticleSystem::iParticleSystem()
{
	entireParticleMax = 1000;
	positionStartVar = iPointMake(40, 40);
	positionEndVar = iPointMake(400, 400);
	crossStartEnd = false;

	colorStart = iColor4bMake(0, 0, 0, 0);
	colorStartVar = iColor4bMake(255, 255, 255, 255);

	colorEnd = iColor4bMake(0, 0, 0, 0);
	colorEndVar = iColor4bMake(255, 255, 255, 255);

	scaleStart = 3;	scaleStartVar = 2;
	scaleEnd = 3; scaleEndVar = 2;

	delta = 0.1f;
	deltaVar = 0.1f;

	entireParticles = new iParticle[entireParticleMax];
	for (int i = 0; i < entireParticleMax; i++)
	{
		iParticle* p = &entireParticles[i];
		p->colorStart = p->colorEnd = iColor4bMake(0, 0, 0, 0);
		p->positionStart = p->positionEnd = iPointMake(0, 0);
		p->scaleStart = p->scaleEnd = 0.0f;
		p->currDelta = p->delta = 0.0f;
	}

	usingParticles = new iParticle * [entireParticleMax];
	usingParticleNum = 0;

	currCreateDt = 0;
}

iParticleSystem::iParticleSystem(const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szFormat, szText)

	entireParticles = nullptr;
	usingParticles = nullptr;
	load(szText);
}

iParticleSystem::~iParticleSystem()
{
	delete entireParticles;
	delete usingParticles;
}

#define createDt 0.01f

void iParticleSystem::paint(float dt)
{
	for (int i = 0; i < usingParticleNum; i++)
	{
		iParticle* p = usingParticles[i];
		p->draw(dt);

		if (p->currDelta >= p->delta)
		{
			usingParticleNum--;
			usingParticles[i] = usingParticles[usingParticleNum];
			i--;
		}
	}
}

void iParticleSystem::save(const char* fileName)
{
	FILE* fp = fopen(fileName, "wb");

	fwrite(&entireParticleMax, sizeof(int), 1, fp);

	fwrite(&positionStartVar, sizeof(float), 2, fp);
	fwrite(&positionEndVar, sizeof(float), 2, fp);
	fwrite(&crossStartEnd, sizeof(bool), 1, fp);

	fwrite(&colorStart, sizeof(iColor4b), 1, fp);
	fwrite(&colorStartVar, sizeof(iColor4b), 1, fp);

	fwrite(&colorEnd, sizeof(iColor4b), 1, fp);
	fwrite(&colorEndVar, sizeof(iColor4b), 1, fp);

	fwrite(&scaleStart, sizeof(int), 1, fp);
	fwrite(&scaleStartVar, sizeof(int), 1, fp);
	fwrite(&scaleEnd, sizeof(int), 1, fp);
	fwrite(&scaleEndVar, sizeof(int), 1, fp);

	fwrite(&delta, sizeof(float), 1, fp);
	fwrite(&deltaVar, sizeof(float), 1, fp);

	fclose(fp);
}

void iParticleSystem::load(const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");

	fread(&entireParticleMax, sizeof(int), 1, fp);

	if (entireParticles)
		delete entireParticles;

	entireParticles = new iParticle[entireParticleMax];
	for (int i = 0; i < entireParticleMax; i++)
	{
		iParticle* p = &entireParticles[i];
		p->colorStart = p->colorEnd = iColor4bMake(0, 0, 0, 0);
		p->positionStart = p->positionEnd = iPointMake(0, 0);
		p->scaleStart = p->scaleEnd = 0.0f;
		p->currDelta = p->delta = 0.0f;
	}

	if (usingParticles)
		delete usingParticles;

	usingParticles = new iParticle * [entireParticleMax];
	usingParticleNum = 0;

	fread(&positionStartVar, sizeof(float), 2, fp);
	fread(&positionEndVar, sizeof(float), 2, fp);
	fread(&crossStartEnd, sizeof(bool), 1, fp);

	fread(&colorStart, sizeof(iColor4b), 1, fp);
	fread(&colorStartVar, sizeof(iColor4b), 1, fp);

	fread(&colorEnd, sizeof(iColor4b), 1, fp);
	fread(&colorEndVar, sizeof(iColor4b), 1, fp);

	fread(&scaleStart, sizeof(int), 1, fp);
	fread(&scaleStartVar, sizeof(int), 1, fp);
	fread(&scaleEnd, sizeof(int), 1, fp);
	fread(&scaleEndVar, sizeof(int), 1, fp);

	fread(&delta, sizeof(float), 1, fp);
	fread(&deltaVar, sizeof(float), 1, fp);

	fclose(fp);
}

void iParticleSystem::startAnimation(iPoint pos)
{
	int num = 5 + rand() % 6;
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < entireParticleMax; j++)
		{
			iParticle* p = &entireParticles[j];

			if (p->currDelta < p->delta)
				continue;

			int sx = positionStartVar.x;
			int sy = positionStartVar.y;
			p->positionStart = iPointMake(pos.x - sx / 2 + rand() % sx,
										  pos.y - sy / 2 + rand() % sy);
			int ex = positionEndVar.x;
			int ey = positionEndVar.y;
			if (crossStartEnd)
			{
				p->positionEnd = iPointMake(pos.x - ex / 2 + rand() % ex,
											pos.y - ey / 2 + rand() % ey);
			}
			else
			{
				iRect rt = iRectMake(pos.x - sx / 2, pos.y - sy / 2, sx, sy);
				while (true)
				{
					p->positionEnd = iPointMake(pos.x - ex / 2 + rand() % ex,
												pos.y - ey / 2 + rand() % ey);
					if (containPoint(p->positionEnd, rt) == false)
						break;
				}
			}

			p->colorStart = iColor4bMake(colorStart.r + rand() % (colorStartVar.r + 1),
										 colorStart.g + rand() % (colorStartVar.g + 1),
										 colorStart.b + rand() % (colorStartVar.b + 1),
										 colorStart.a + rand() % (colorStartVar.a + 1));
			p->colorEnd = iColor4bMake(colorEnd.r + rand() % (colorEnd.r + 1),
									   colorEnd.g + rand() % (colorEnd.g + 1),
									   colorEnd.b + rand() % (colorEnd.b + 1),
									   colorEnd.a + rand() % (colorEnd.a + 1));
			p->scaleStart = scaleStart + rand() % (scaleStart + 1);
			p->scaleEnd = scaleEnd + rand() % (scaleEnd + 1);

			#define _DT 1000.f
			int DT = deltaVar * _DT;
			p->delta = delta + (rand() % DT) / _DT;
			p->currDelta = 0.0f;

			usingParticles[usingParticleNum] = p;
			usingParticleNum++;
			break;
		}
	}
}

void iParticleSystem::stopAnimation(bool immediately)
{
}
