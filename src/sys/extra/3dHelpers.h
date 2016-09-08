#pragma once

struct vertex 
{
	float x, y, z;

	vertex::vertex()
	{
	}

	vertex::vertex(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void vertex::SetFloats(float *f)
	{
		f[0] =x;
		f[1] =y;
		f[2] =z;
	}

	vertex vertex::operator+ (vertex param)
	{

	}

	/*
	float* vertex::floats()
	{
		float **f = new float*[3];
		f[0] = &x;
		f[1] = &y;
		f[2] = &z;
		return *f;
	}*/

	/*
	static vertex::makeVertex(float x, float y, float z)
	{
		return new vertex{x,0,0};
	}*/
};

struct vertex4
{
	float x, y, z, w;

	vertex4::vertex4()
	{
	}

	vertex4::vertex4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	void vertex4::SetFloats(float *f)
	{
		f[0] =x;
		f[1] =y;
		f[2] =z;
		f[3] =w;
	}

	vertex4 vertex4::operator+ (vertex4 param)
	{

	}
};

struct vertexTexCoord
{
	float u;
	float v;
};


struct quadTex
{
	float tx1, ty1;
	float tx2, ty2;
};


struct vertexWithTexCoord 
{
	vertex v;
	vertexTexCoord tc;
};

struct quad
{
	vertex vertices[4];
};

struct nquad
{
	vertex vertices[4];
	vertex normals[4];
	vertex fnormals[4];
};

struct color4f
{
	float r, g, b, a;
};

struct color3f
{
	float r, g, b;
};