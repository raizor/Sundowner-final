/* coherent noise function over 1, 2 or 3 dimensions */
/* (copyright Ken Perlin) */

/*
This source provides a C++ wrapper around Ken Perlin's noise function. 
I know there already is a Perlin noise function on the COTD collection, 
but this one serves a specific purpose. 
The C++ wrapper allows you to create multiple instances of perlin noise 
functions so you can have completely determinstic and distinct noise textures.
Each instance of the 'Perlin' class can be sampled independently of the other,
always returning the same randomized results. 
You construct an instance of Perlin as follows:
Perlin *perlin = new Perlin(4,4,1,94); 
The first parameter is the number of octaves, this is how noisy or smooth the function is. 
This is valid between 1 and 16. A value of 4 to 8 octaves produces fairly conventional noise results. 
The second parameter is the noise frequency. Values betwen 1 and 8 are reasonable here. 
You can try sampling the data and plotting it to the screen to see what numbers you like. 
The last parameter is the amplitude. Setting this to a value of 1 will return randomized samples between -1 and +1. 
The last parameter is the random number seed. This number is what causes this instance of the Perlin to be 
deterministic and distanct from any other instance. The perlin noise function creates some random number tables
that are sampled during queries. This random number seed determines the contents of those tables so that you will
get the same exact results every time you use it. To retrieve a sample you simply call the method 'Get' and pass
it the X and Y sample point to query. X and Y should be in the ranges of 0 to 1. So if you are sampling for a bitmap
be sure and scale the pixel co-ordinates down into normalized values. Each instance of Perlin contains it's own random
number tables and sampling values. This class is extremely convenient if you just need a quick and dirty way to do some
kind of distribution pattern that looks halfway decent. John
*/

//#include <math.h>
#include "../msys.h"
#include "perlin.h"

#define B SAMPLE_SIZE
#define BM (SAMPLE_SIZE-1)

#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

#define s_curve(t) ( t * t * (3.0f - 2.0f * t) )
#define lerp(t, a, b) ( a + t * (b - a) )

// setup modified with the 1000 mult/div stuff to cope with bad float precision without libc/intrinsics, hack but functional...
#define setup(i,b0,b1,r0,r1)\
	t = vec[i] + N;\
	iv = ((int)(t*1000))/1000;\
	b0 = (iv) & BM;\
	b1 = (b0+1) & BM;\
	r0 = t - iv;\
	r1 = r0 - 1.0f;

float Perlin::noise1(float arg)
{
	int bx0, bx1;
	float rx0, rx1, sx, t, u, v, vec[1];
	int iv;

	vec[0] = arg;

	if (mStart)
	{
		//srand(mSeed);
		mStart = false;
		init();
	}

	setup(0, bx0,bx1, rx0,rx1);

	sx = s_curve(rx0);

	u = rx0 * g1[ p[ bx0 ] ];
	v = rx1 * g1[ p[ bx1 ] ];

	return lerp(sx, u, v);
}

float Perlin::noise2(float vec[2])
{
	int bx0, bx1, by0, by1, b00, b10, b01, b11;
	float rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
	int i, j, iv;

	if (mStart)
	{
		//srand(mSeed);
		mStart = false;
		init();
	}

	setup(0,bx0,bx1,rx0,rx1);
	setup(1,by0,by1,ry0,ry1);

	i = p[bx0];
	j = p[bx1];

	b00 = p[i + by0];
	b10 = p[j + by0];
	b01 = p[i + by1];
	b11 = p[j + by1];

	sx = s_curve(rx0);
	sy = s_curve(ry0);

  #define at2(rx,ry) ( rx * q[0] + ry * q[1] )

	q = g2[b00];
	u = at2(rx0,ry0);
	q = g2[b10];
	v = at2(rx1,ry0);
	a = lerp(sx, u, v);

	q = g2[b01];
	u = at2(rx0,ry1);
	q = g2[b11];
	v = at2(rx1,ry1);
	b = lerp(sx, u, v);

	return lerp(sy, a, b);
}

float Perlin::noise3(float vec[3])
{
	int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	float rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	int i, j, iv;

	if (mStart)
	{
		//srand(mSeed);
		mStart = false;
		init();
	}

	setup(0, bx0,bx1, rx0,rx1);
	setup(1, by0,by1, ry0,ry1);
	setup(2, bz0,bz1, rz0,rz1);

	i = p[ bx0 ];
	j = p[ bx1 ];

	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];

	t  = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

  #define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

	q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
	q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
	a = lerp(t, u, v);

	q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
	q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
	b = lerp(t, u, v);

	c = lerp(sy, a, b);

	q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
	q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
	a = lerp(t, u, v);

	q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
	q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
	b = lerp(t, u, v);

	d = lerp(sy, a, b);

	return lerp(sz, c, d);
}

void Perlin::normalize2(float v[2])
{
	float s;

	s = msys_sqrtf(v[0] * v[0] + v[1] * v[1]);
	s = 1.0f/s;
	v[0] = v[0] * s;
	v[1] = v[1] * s;
}

void Perlin::normalize3(float v[3])
{
	float s;

	s = msys_sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	s = 1.0f/s;

	v[0] = v[0] * s;
	v[1] = v[1] * s;
	v[2] = v[2] * s;
}

void Perlin::init(void)
{
	int i, j, k;
	for (i = 0 ; i < B ; i++)
	{
		p[i] = i;
		g1[i] = (float)((msys_rand(&mSeed) % (B + B)) - B) / B;
		for (j = 0 ; j < 2 ; j++)
			g2[i][j] = (float)((msys_rand(&mSeed) % (B + B)) - B) / B;
		normalize2(g2[i]);
		for (j = 0 ; j < 3 ; j++)
			g3[i][j] = (float)((msys_rand(&mSeed) % (B + B)) - B) / B;
		normalize3(g3[i]);
	}

	while (--i)
	{
		k = p[i];
		p[i] = p[j = msys_rand(&mSeed) % B];
		p[j] = k;
	}

	for (i = 0 ; i < B + 2 ; i++)
	{
		p[B + i] = p[i];
		g1[B + i] = g1[i];
		for (j = 0 ; j < 2 ; j++)
			g2[B + i][j] = g2[i][j];
		for (j = 0 ; j < 3 ; j++)
			g3[B + i][j] = g3[i][j];
	}

}


float Perlin::perlin_noise_2D(float vec[2])
{
	int terms    = mOctaves;
	float freq   = mFrequency;
	float result = 0.0f;
	float amp = mAmplitude;

	vec[0]*=mFrequency;
	vec[1]*=mFrequency;
	
	for( int i=0; i<terms; i++ )
	{
		result += noise2(vec)*amp;
		vec[0] *= 2.0f;
		vec[1] *= 2.0f;
		amp*=0.5f;
	}

	//if (result <0) result = 0;
	return result;
}



Perlin::Perlin(int octaves,float freq,float amp,int seed)
{
	mOctaves = octaves;
	mFrequency = freq;
	mAmplitude = amp;
	mSeed = seed;
	mStart = true;
}

