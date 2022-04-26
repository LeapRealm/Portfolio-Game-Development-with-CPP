#include "iImage.h"

#include "iStd.h"

iImage::iImage()
{
	texs = new iLinkedList(methodDeleteTexture);
	tex = nullptr;
	position = iPointZero;
	frame = 0;
	currAnimDt = 0.0f;
	animDt = default_animDt;
	isAnimation = false;
	currRepeatNum = repeatNum = 0; // inf
	method = nullptr;
	param = nullptr;

	alpha = 1.0f;
	degree = 0.0f;
	scale = 1.0f;
	reverse = REVERSE_NONE;
}

iImage::~iImage()
{
	delete texs;
}

void iImage::methodDeleteTexture(void* tex)
{
	freeTexture((Texture*)tex);
}
 
void iImage::paint(float dt, iPoint p)
{
	if (isAnimation)
	{
		currAnimDt += dt;
		if (currAnimDt >= animDt)
		{
			currAnimDt -= animDt;

			frame++;
			if (frame > texs->count - 1)
			{
				if (repeatNum == 0)
				{
					frame = 0;
				}
				else
				{
					currRepeatNum++;
					if (currRepeatNum < repeatNum)
					{
						frame = 0;
					}
					else
					{
						frame = texs->count - 1;
						isAnimation = false;
						if (method)
							method(param);
					}
				}
			}
		}
	}

	float r, g, b, a;
	getRGBA(r, g, b, a);
	setRGBA(r, g, b, a * alpha);

	tex = (Texture*)texs->getObjectByIndex(frame);
	p += position;

	p.x += (1.0f - scale) / 2 * tex->width;
	p.y += (1.0f - scale) / 2 * tex->height;

	drawTexture(tex, p.x, p.y, scale, scale, TOP | LEFT,
		0, 0, tex->width, tex->height,
		2, degree, reverse);
	setRGBA(r, g, b, a);
}

void iImage::addTexture(Texture* tex)
{
	if (this->tex == nullptr)
		this->tex = tex;
	texs->addObject(tex);
	tex->retainCount++;
}

void iImage::startAnimation(const CBAnimation m, void* p)
{
	isAnimation = true;
	frame = 0;
	currAnimDt = 0.0f;
	method = m;
	param = p;

	if (repeatNum)
		currRepeatNum = 0;
}

void iImage::stopAnimation()
{
	isAnimation = false;
}

iRect iImage::rect(iPoint p, iSize s)
{
	return iRectMake(position.x + p.x - s.width / 2, 
					 position.y + p.y - s.height / 2, 
					 tex->width + s.width, 
					 tex->height + s.height);
}

iPoint iImage::center(iPoint p)
{
	iRect rt = rect(p);
	return iPointMake(rt.origin.x + rt.size.width / 2,
					  rt.origin.y + rt.size.height / 2);
}

iImage* iImage::clone()
{
	iImage* sprite = new iImage();

	memcpy(sprite, this, sizeof(iImage));

	sprite->texs = new iLinkedList(methodDeleteTexture);
	for (int i = 0; i < texs->count; i++)
		sprite->addTexture((Texture*)texs->getObjectByIndex(i));

	return sprite;
}
