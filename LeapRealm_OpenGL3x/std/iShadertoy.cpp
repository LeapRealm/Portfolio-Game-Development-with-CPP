#include "iShadertoy.h"

#include "iStd.h"

iShadertoy::iShadertoy()
{
	init();
}

iShadertoy::~iShadertoy()
{
	free();
}

// vert shader
const char* strVert = "							  \
	#version 150								\n\
	in vec4 position;							\n\
	void main()									\n\
	{											\n\
		gl_Position = position;					\n\
	}											\n\
";

const char* strFragHead = "						  \
	#version 150								\n\
												\n\
	uniform vec3      iResolution;				\n\
	uniform float     iTime;					\n\
	uniform float     iTimeDelta;				\n\
	uniform int       iFrame;              		\n\
	uniform float     iChannelTime[4];			\n\
	uniform vec3      iChannelResolution[4];	\n\
	uniform vec4      iMouse;					\n\
	uniform sampler2D iChannel0;        		\n\
	uniform sampler2D iChannel1;        		\n\
	uniform sampler2D iChannel2;        		\n\
	uniform sampler2D iChannel3;        		\n\
	uniform vec4      iDate;					\n\
	uniform float     iSampleRate;				\n\
	uniform float	  iActionKeys[5];			\n\
	uniform float	  iAxisKeys[5];				\n\
												\n\
	out vec4 fragColor;							\n\
";

const char* strFragTail = "						  \
	void main()									\n\
	{											\n\
		mainImage(fragColor, gl_FragCoord.xy);	\n\
	}											\n\
";

void iShadertoy::init()
{
	memset(programID, 0x00, sizeof(GLuint) * 5);

	memset(texFbo, 0x00, sizeof(Texture*) * 4 * 2);
	memset(toggle, false, sizeof(bool) * 4);

	memset(texs, 0x00, sizeof(Texture*) * 5 * 4);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			bufs[i][j] = -1;
			filter[i][j] = Linear;
			wrap[i][j] = Clamp;
		}
	}

	iTime = 0.0f;
	iFrame = 0;
	memset(iMouse, 0x00, sizeof(float) * 4);
	memset(iChannelTime, 0x00, sizeof(float) * 4);
	memset(iChannelResolution, 0x00, sizeof(iSize3D) * 4);
	iMouse[2] = -5;
}

void iShadertoy::load(iSTInfo* info)
{
	free();
	init();

	for (int i = 0; i < 5; i++)
	{
		iSTInfo* bi = &info[i];

		if (bi->fileNameFrag == nullptr) continue;

#define vertFile "vert.glsl"
#define fragFile "frag.glsl"

		// vert.glsl
		saveFile(vertFile, strVert, strlen(strVert) + 1);

		// frag.glsl
		int lenBody;
		char* strFragBody = loadFile(bi->fileNameFrag, lenBody);

		int lenHead = strlen(strFragHead);
		int lenTail = strlen(strFragTail);

		int len = lenHead + lenBody + lenTail + 3;
		char* strFrag = new char[len];
		sprintf(strFrag, "%s\n%s\n%s", strFragHead, strFragBody, strFragTail);
		delete strFragBody;

		saveFile(fragFile, strFrag, len);

		delete strFrag;

		programID[i] = createProgramID(vertFile, fragFile);

		removeFile(vertFile);
		removeFile(fragFile);

		if (i < 4)
		{
			for (int j = 0; j < 2; j++)
				texFbo[i][j] = createTexture(devSize.width, devSize.height);
		}

		for (int j = 0; j < 4; j++)
		{
			if (bi->fileNameTex[j])
				texs[i][j] = getPathTexture(bi->fileNameTex[j]);
			else if (bi->buf[j] != BN)
				bufs[i][j] = bi->buf[j];

			filter[i][j] = bi->filter[j];
			wrap[i][j] = bi->wrap[j];
		}
	}
}

void iShadertoy::free()
{
	for (int i = 0; i < 5; i++)
	{
		if (programID[i] == 0) continue;
		freeProgramID(programID[i]);

		if (i < 4)
		{
			for (int j = 0; j < 2; j++)
				freeTexture(texFbo[i][j]);
		}

		for (int j = 0; j < 4; j++)
		{
			if (texs[i][j])
				freeTexture(texs[i][j]);
		}
	}
}

void iShadertoy::paint(float dt)
{
	float position[16] = {
		-1, +1, 0, 1,		+1, +1, 0, 1,
		-1, -1, 0, 1,		+1, -1, 0, 1,
	};

	for (int i = 0; i < 5; i++)
	{
		GLuint id = programID[i];

		if (id == 0) continue;

		if (i < 4)
			fbo->bind(texFbo[i][toggle[i] ? 1 : 0]);

		glUseProgram(id);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(float), position);

		GLuint attrPosition = glGetAttribLocation(id, "position");
		glEnableVertexAttribArray(attrPosition);
		glVertexAttribPointer(attrPosition, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)0);

		GLint uid = glGetUniformLocation(id, "iResolution");
		glUniform3f(uid, devSize.width, devSize.height, 0);

		uid = glGetUniformLocation(id, "iTime");
		glUniform1f(uid, iTime);

		uid = glGetUniformLocation(id, "iTimeDelta");
		glUniform1f(uid, dt);

		uid = glGetUniformLocation(id, "iFrame");
		glUniform1f(uid, iFrame);

		uid = glGetUniformLocation(id, "iChannelTime");
		glUniform1fv(uid, 4, iChannelTime);

		if (i < 4)
			iChannelTime[i] += dt;

		uid = glGetUniformLocation(id, "iChannelResolution");
		glUniform3fv(uid, 4, (GLfloat*)iChannelResolution);

		uid = glGetUniformLocation(id, "iMouse");
		glUniform4fv(uid, 1, iMouse);

		const char* iChannels[4] = { "iChannel0", "iChannel1", "iChannel2", "iChannel3" };
		for (int j = 0; j < 4; j++)
		{
			Texture* tex = nullptr;

			if (texs[i][j])
				tex = texs[i][j];
			else if (bufs[i][j] != BN)
				tex = texFbo[bufs[i][j]][toggle[i] ? 0 : 1];

			if (tex == nullptr)
				continue;

			glActiveTexture(GL_TEXTURE0 + j);
			GLint uid = glGetUniformLocation(id, iChannels[j]);
			glUniform1i(uid, j);
			glBindTexture(GL_TEXTURE_2D, tex->texID);
		}

		float iDate[4] = { 0, 0, 0, 0 };
		uid = glGetUniformLocation(id, "iDate");
		glUniform1fv(uid, 4, iDate);

		float iSampleRate = 1.0f;
		uid = glGetUniformLocation(id, "iSampleRate");
		glUniform1f(uid, iSampleRate);

#if 1
		float iKeyDowns[5] = 
		{
			(float)getKeyDown(KEY_LEFT),
			(float)getKeyDown(KEY_UP),
			(float)getKeyDown(KEY_RIGHT),
			(float)getKeyDown(KEY_DOWN),
			(float)getKeyDown(KEY_SPACE),
		};
		uid = glGetUniformLocation(id, "iKeyDowns");
		glUniform1fv(uid, 5, iKeyDowns);

		float iKeys[5] =
		{
			(float)getKey(KEY_LEFT),
			(float)getKey(KEY_UP),
			(float)getKey(KEY_RIGHT),
			(float)getKey(KEY_DOWN),
			(float)getKey(KEY_SPACE),
		};
		uid = glGetUniformLocation(id, "iKeys");
		glUniform1fv(uid, 5, iKeys);
#endif

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);

		glDisableVertexAttribArray(attrPosition);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (i < 4)
		{
			fbo->unbind();
			toggle[i] = !toggle[i];
		}
	}

	iTime += dt;
	iFrame++;
}

void iShadertoy::key(iKeyState state, iPoint p)
{
	switch (state)
	{
	case iKeyStateBegan:
	{
		memcpy(&iMouse[0], &p, sizeof(iPoint));
		memcpy(&iMouse[2], &p, sizeof(iPoint));
		break;
	}

	case iKeyStateMoved:
	{
		if (iMouse[2] < 0)
			break;
		memcpy(&iMouse[0], &p, sizeof(iPoint));
		break;
	}

	case iKeyStateEnded:
	{
		iMouse[2] = -5;
		break;
	}
	}
}

static PathTexture* pt = nullptr;
static int ptNum, ptMax;

void cleanAllTexture()
{
	if (pt == nullptr)
		return;	

	for (int i = 0; i < ptNum; i++)
	{
		PathTexture* p = &pt[i];
		delete p->path;
		freeTexture(p->tex);
	}

	delete pt;
	pt = nullptr;
}

Texture* getPathTexture(const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szFormat, szText);

	if (pt == nullptr)
	{
		ptMax = 100;
		pt = new PathTexture[ptMax];
		ptNum = 0;
	}
	else if (ptNum == ptMax)
	{
		int num = ptMax + 50;
		PathTexture* p = new PathTexture[num];
		memcpy(p, pt, sizeof(PathTexture) * ptMax); 
		ptMax = num;

		delete pt;
		pt = p;
	}

	for (int i = 0; i < ptNum; i++)
	{
		PathTexture* p = &pt[i];
		if (strcmp(p->path, szText) == 0)
		{
			p->tex->retainCount++;
			return p->tex;
		}
	}

	PathTexture* p = &pt[ptNum];
	ptNum++;
	p->path = new char[strlen(szText) + 1];
	strcpy(p->path, szText);
	p->tex = createTexture(szText);

	return p->tex;
}
