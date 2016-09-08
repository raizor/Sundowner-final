#include "textureGen.h"
#include "../extra/perlin.h"
#include "../extra/RNG.h"

textureGen::textureGen(void)
{
}

/************************************************************************/
/* Generate a 512x512 perlin noise texture (cloudy looking thing)       */
/************************************************************************/
GLuint textureGen::GenerateNoiseTexture()
{
	GLuint textureId;
	GLint width=512, height=512;
	GLbyte *pixels = new GLbyte[width*height*4];

	RNG *rng = new RNG(msys_timerGet());

	
	Perlin *pn = new Perlin(8,1.5f/rng->rFloat(),1,94+rng->rInt(1,50));
	Perlin *pn2 = new Perlin(8,2.5f*rng->rFloat(),1,10+rng->rInt(1,50));
	Perlin *pn3 = new Perlin(8,3.0f/rng->rFloat(),2,33+rng->rInt(1,50));

	Perlin *pn4 = new Perlin(8,5.0f*rng->rFloat(),2,11+rng->rInt(1,50));
	Perlin *pn5 = new Perlin(8,6.0f/rng->rFloat(),2,62+rng->rInt(1,50));
	Perlin *pn6 = new Perlin(8,7.0f*rng->rFloat(),2,23+rng->rInt(1,50));


	/*
	Perlin *pn = new Perlin(8,1.5f,1,94);
	Perlin *pn2 = new Perlin(8,2.5f,1,10);
	Perlin *pn3 = new Perlin(8,3.0f,2,33);

	Perlin *pn4 = new Perlin(8,5.0f,2,11);
	Perlin *pn5 = new Perlin(8,6.0f,2,62);
	Perlin *pn6 = new Perlin(8,7.0f,2,23);*/

	int offset = 0;

	for(int x=0; x<width; x++)
	{
		for (int y=0; y<height; y++)
		{
			float xv = (float)x/width;
			float yv = (float)y/height;
			float val1 = pn->Get(xv, yv);
			float val2 = pn2->Get(xv, yv);
			float val3 = pn3->Get(xv, yv);
			float val4 = pn4->Get(xv, yv);
			float val5 = pn5->Get(xv, yv);
			float val6 = pn6->Get(xv, yv);
			val1 = msys_fabsf(val1);
			val2 = msys_fabsf(val2);
			val3 = msys_fabsf(val3);
			val4 = msys_fabsf(val4);
			val5 = msys_fabsf(val5);
			val6 = msys_fabsf(val6);
			float valFin = (val2+val3)*0.5f;
			valFin = (valFin + val2) / 2.0f;
			//float val4 = val1;

			valFin += val4*0.33f;
			valFin += val5*0.33f;
			valFin += val6*0.33f;

			valFin/=2+(x*0.001f);

			if (valFin > 1.0f) valFin = 1.0f;
			for(int i=0; i<3; i++)
			{
				pixels[offset++] = (GLbyte)(valFin*255);
			}
			// alpha
			pixels[offset++] = (GLbyte)255;
		}
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
	return textureId;
}

textureGen::~textureGen(void)
{
}
