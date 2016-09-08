#include "logo.h"
#include "julia.h"

GLuint logo::textureId;
GLuint logo::lookupTextureId;

extern julia *_julia;

logo::logo(void)
{
	for(int i=0; i<256; i++)
	{
		charWidths[i] = 9;
	}      
	seed = 0;
	charWidths[32] = 9;	
	pixelSize.x = 2.0f / 256.0f;
	pixelSize.y = 2.0f / 256.0f;	
	CheckGlError(false);
	oglGenBuffersARB(1, &vboVerts3);
	oglGenBuffersARB(1, &vboNormals);
	oglGenBuffersARB(1, &vboColors);
	oglGenBuffersARB(1, &vboTexCoords1);
	oglGenBuffersARB(1, &vboTexCoords2);
	oglGenBuffersARB(1, &vboTexCoords3);
	CheckGlError(false);
	GLint width, height, icomps;
	GLenum eFormat;
	GLbyte *pixels = gltLoadPNG(IDR_PNG_LOGOFONT, &width, &height, &icomps, &eFormat );
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, icomps, width, height, 0, eFormat, GL_UNSIGNED_BYTE, pixels );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
	CheckGlError(false);
	getBlocks(pixels, pixels);
	CheckGlError(false);
	vertex v, v2;

#if DEBUG
    _shaderIdLogo = new ShaderProg("src\\resources\\shaders\\logo_vs.glsl", "src\\resources\\shaders\\logo_fs.glsl", "greets");
    _shaderIdGreets = new ShaderProg("src\\resources\\shaders\\greets_vs.glsl", "src\\resources\\shaders\\greets_fs.glsl", "greets");
#else
    // todo
    _shaderIdLogo = new ShaderProg(IDS_SHADER_LOGO_VERT, IDS_SHADER_LOGO_FRAG, "Logo");
    _shaderIdGreets = new ShaderProg(IDS_SHADER_GREETS_VERT, IDS_SHADER_GREETS_FRAG, "Greets");
#endif

	fbo512_2 = globalFboManager->fbos[5];
    fbo1024  = globalFboManager->fbos[0];
	fboFullscreen = globalFboManager->fbos[2];
	fboFullscreen2 = globalFboManager->fbos[3];
    fboFullscreen3 = globalFboManager->fbos[4];
	makeGreets(pixels);
	//free(pixels);
}

void logo::makeGreets(GLbyte* pixels)
{
	int numNames = 50;
	groupNames = new char*[numNames];
	for (int i=0; i<numNames; i++)
	{
		groupNames[i] = new char[100];
	}

	int idx = 0;

	sprintf(groupNames[idx++], "^ 3LN ^");	
	sprintf(groupNames[idx++], "^ Alcatraz ^");	
	sprintf(groupNames[idx++], "^ Approximate ^");		
	sprintf(groupNames[idx++], "^ ASD ^");	
	sprintf(groupNames[idx++], "^ BDSE ^");	
	sprintf(groupNames[idx++], "^ Brainstorm ^");	
	sprintf(groupNames[idx++], "^ Bilotrip ^");			
	sprintf(groupNames[idx++], "^ Codigos ^");	
	sprintf(groupNames[idx++], "^ Conspiracy ^");	
	sprintf(groupNames[idx++], "^ DBF ^");
	sprintf(groupNames[idx++], "^ Desire ^");	 
    sprintf(groupNames[idx++], "^ DSS ^");
    sprintf(groupNames[idx++], "^ DCS ^");
    sprintf(groupNames[idx++], "^ Exceed ^");
    sprintf(groupNames[idx++], "^ Fairlight ^");
    sprintf(groupNames[idx++], "^ Farbrausch ^");
	sprintf(groupNames[idx++], "^ Focus Design ^");
	sprintf(groupNames[idx++], "^ Gravity ^");
	sprintf(groupNames[idx++], "^ Haujobb ^");
	sprintf(groupNames[idx++], "^ Loonies ^");
	sprintf(groupNames[idx++], "^ Mercury ^");
    sprintf(groupNames[idx++], "^ Mudlord ^");
	sprintf(groupNames[idx++], "^ Paradox ^");
    sprintf(groupNames[idx++], "^ Quite ^");
    sprintf(groupNames[idx++], "^ Razor 1911 ^");
    sprintf(groupNames[idx++], "^ Rebels ^");
    sprintf(groupNames[idx++], "^ Rift ^");
    sprintf(groupNames[idx++], "^ SceneSat ^");	
    sprintf(groupNames[idx++], "^ Still ^");
	sprintf(groupNames[idx++], "^ TBC ^");
	sprintf(groupNames[idx++], "^ TRSI ^");
	sprintf(groupNames[idx++], "^ Titan ^");
	sprintf(groupNames[idx++], "^ Youth Uprising ^");
    
	numGroups = idx; // 33

    gXpos = new float[numGroups];
    gYpos = new float[numGroups];
    gZpos = new float[numGroups];

    for(int i=0; i<numGroups; i++)
    {
        gZpos[i] =  0;//-10.0f * msys_frand(&seed);
        gXpos[i] = 0;
        gYpos[i] = 0;
    }
    
    // 3LN
    gXpos[0] = -0.3f;
    gYpos[0] = -1.0f;
    gZpos[0] = -0.4f;

    // Alcatraz
    gXpos[1] = -0.8f;
    gYpos[1] = -0.88f;
    gZpos[1] = -0.5f;

    // Approximate
    gXpos[2] = -0.66f;
    gYpos[2] = -1.1f;
    gZpos[2] = -1.2f;

    // ASD
    gXpos[3] = -0.4f;
    gYpos[3] = -0.9f;
    gZpos[3] = 0.4f;

    // BDSE
    gXpos[4] = -0.4f;
    gYpos[4] = -1.0f;
    gZpos[4] = 0.4f;

    // Brainstorm
    gXpos[5] = -0.9f;
    gYpos[5] = -0.93f;
    gZpos[5] = -0.5f;

    // Bilotrip
    gXpos[6] = -0.5f;
    gYpos[6] = -1.2f;
    gZpos[6] = 0.4f;

    // Codigos
    gXpos[7] = -0.5f;
    gYpos[7] = -0.9f;
    gZpos[7] = 0.4f;

    // Conspiracy
    gXpos[8] = -1.2f;
    gYpos[8] = -0.95f;
    gZpos[8] = 0.2f;

    // DBF
    gXpos[9] = -1.0f;
    gYpos[9] = -1.12f;
    gZpos[9] = 0.3f;

    // Desire
    gXpos[10] = -0.4f;
    gYpos[10] = -0.9f;
    gZpos[10] = -0.21f;

    // DSS
    gXpos[11] = -0.4f;
    gYpos[11] = -1.1f;
    gZpos[11] = 0.4f;
    
    // Dual Crew / Shining
    gXpos[12] = -0.2f;
    gYpos[12] = -0.8f;
    gZpos[12] = -0.2f;

    // Exceed
    gXpos[13] = -0.4f;
    gYpos[13] = -1.1f;
    gZpos[13] = 0.4f;
     
    // Fairlight
    gXpos[14] = -1.2f;
    gYpos[14] = -0.8f;
    gZpos[14] = 0.4f;

    // Farbrausch
    gXpos[15] = -1.2f;
    gYpos[15] = -1.1f;
    gZpos[15] = 0.4f;
    
    // Focus Design
    gXpos[16] = -1.2f;
    gYpos[16] = -1.1f;
    gZpos[16] = 0.4f;

    // Gravity
    gXpos[17] = -0.1f;
    gYpos[17] = -0.8f;
    gZpos[17] = 0.1f;

    // Haujobb
    gXpos[18] = -1.4f;
    gYpos[18] = -1.1f;
    gZpos[18] = 0.1f;

    // Loonies
    gXpos[19] = -0.5f;
    gYpos[19] = -0.6f;
    gZpos[19] = 0.1f;

    // Mercury
    gXpos[20] = -1.6f;
    gYpos[20] = -0.8f;
    gZpos[20] = 0.1f;

    // Mudlord
    gXpos[21] = -0.6f;
    gYpos[21] = -1.5f;
    gZpos[21] = 0.4f;

    // Paradox
    gXpos[22] = -0.0f;
    gYpos[22] = -1.1f;
    gZpos[22] = 0.2f;

    // Quite
    gXpos[23] = -0.0f;
    gYpos[23] = -1.1f;
    gZpos[23] = 0.2f;

    // Razor 1911
    gXpos[24] = -1.4f;
    gYpos[24] = -1.1f;
    gZpos[24] = 0.1f;

    // Rebels
    gXpos[25] = -0.4f;
    gYpos[25] = -0.8f;
    gZpos[25] = 0.1f;

    // Rift
    gXpos[26] = -0.8f;
    gYpos[26] = -1.4f;
    gZpos[26] = 0.3f;

    // SceneSat
    gXpos[27] = -0.3f;
    gYpos[27] = -0.8f;
    gZpos[27] = 0.1f;

    // Still
    gXpos[28] = -1.4f;
    gYpos[28] = -1.1f;
    gZpos[28] = 0.1f;

    // TBC
    gXpos[29] = -1.4f;
    gYpos[29] = -0.6f;
    gZpos[29] = 0.1f;

    // TRSI
    gXpos[30] = -1.4f;
    gYpos[30] = -1.1f;
    gZpos[30] = 0.1f;

    // Titan
    gXpos[31] = 0.75f;
    gYpos[31] = -1.1f;
    gZpos[31] = 0.1f;

    // Youth Uprising
    gXpos[32] = -0.7f;
    gYpos[32] = -0.9f;
    gZpos[32] = 0.3f;
   
    for(int i=0; i<numGroups; i++)
    {
        gZpos[i] -= i * 0.220f;
    }
    
    gverts4 = new vertex4*[numGroups];
    gtc4_1 = new vertex4*[numGroups];
    gtc4_2 = new vertex4*[numGroups];
    gtc4_3 = new vertex4*[numGroups];
    gverts4 = new vertex4*[numGroups];
    gbn = new vertex*[numGroups];
    gvertCols = new color4f*[numGroups];

	vboGVerts3 = new GLuint[numGroups];
	vboGNormals = new GLuint[numGroups];
	vboGColors = new GLuint[numGroups];
	vboGTexCoords1 = new GLuint[numGroups];
	vboGTexCoords2 = new GLuint[numGroups];
	vboGTexCoords3 = new GLuint[numGroups];
	gnumblocks = new int[numGroups];

	oglGenBuffersARB(numGroups, vboGVerts3);
	oglGenBuffersARB(numGroups, vboGNormals);
	oglGenBuffersARB(numGroups, vboGColors);
	oglGenBuffersARB(numGroups, vboGTexCoords1);
	oglGenBuffersARB(numGroups, vboGTexCoords2);
	oglGenBuffersARB(numGroups, vboGTexCoords3);

	for(int i=0; i<numGroups; i++)
	{
        makeGroupGreet(i, pixels);		
	}	
}

void logo::makeGroupGreet(int groupNum, GLbyte* pixels)
{
	float offsetY = 0;
	float startY = 0;
	float offsetX = 0;
	cubeSize = 2.0f / 256.0f;
	objectStack *s = new objectStack(256*50);

	int offset = 0;

	int charSize = 9;
	int charsPerLine = 16;

	int charOffx = 0;
    
	for(int i=0; i<(int)strlen(groupNames[groupNum]); i++)
	{
		char ch = groupNames[groupNum][i];
		int line =(ch-32)/charsPerLine;
		int lineChar = (ch-32)%charsPerLine;

		int offPix = 0;

		if (ch==' ')
		{
			charOffx+=charSize;
			continue;
		}

		for(int x = 0; x<charSize; x++) // each col
		{
			bool hitPixel = false;

			for(int y=0; y<charSize; y++) // each row
			{			
				int yp = (line*charSize)+line+y+1;
				int xp = (lineChar*charSize)+lineChar+x;
				color4f color = getBlockColor(pixels, xp, yp);
				if (color.a > 0)
				{
                    for(int level = 0; level<1; level++)
                    {
                        logoBlock* b = new logoBlock();
                        b->color = color;
                        if (ch == '^')
                        {
                            color.g = 0.5f;
                            color.b = 0.5f;
                        }
                        b->height = 1;
                        b->currentPos.x = (float)0;
                        b->currentPos.y = (float)0;
                        b->currentPos.z = 0;
                        b->color = color;
                        b->percent = 0;
                        b->percent2 = 0;

                        b->scalex = 1;
                        b->scaley = 1;
                        b->scalez = 1;
                        b->startTime = 0;
                        s->push((void*)b);

                        b->targetPos.x = ((float)(offPix+charOffx+offsetX)*cubeSize*2);
                        b->targetPos.y = 1.0f-(((float)y-startY+offsetY)*cubeSize*2);
                        b->targetPos.z = cubeSize*level;
                        hitPixel = true;
                    }
				}
			}

			if (hitPixel)
			{
				offPix++;
			}
		}
		charOffx+=offPix+1;
	}

	gnumblocks[groupNum] = s->count;

	for (int i=0; i<numGroups; i++)
	{
		gverts4[i] = new vertex4[gnumblocks[groupNum]*24];
		gtc4_1[i] = new vertex4[gnumblocks[groupNum]*24];
		gtc4_2[i] = new vertex4[gnumblocks[groupNum]*24];
		gtc4_3[i] = new vertex4[gnumblocks[groupNum]*24];
		gverts4[i] = new vertex4[gnumblocks[groupNum]*24];
		gbn[i] = new vertex[gnumblocks[groupNum]*24];
		gvertCols[i] = new color4f[gnumblocks[groupNum]*24];
	}
	
	int idx = 0;

	for(int i=0; i<gnumblocks[groupNum]; i++)
	{
		logoBlock *b = (logoBlock*)s->items[i];
		idx = createCube(idx, i, b->color, gvertCols[groupNum], gverts4[groupNum], gbn[groupNum], 6.0f);
	}
    
	int tcidx = 0;

	// set source positions
	for(int i=0; i<gnumblocks[groupNum]; i++)
	{
		logoBlock *b = (logoBlock*)s->items[i];

		float amt = 1.0f;

		float vx = rng->rFloat()*amt;
		float vy = rng->rFloat()*amt;
		float vz = rng->rFloat()*amt;

		float vxp = rng->rFloat()*amt;
		float vyp = rng->rFloat()*amt;
		float vzp = rng->rFloat()*amt;


		if (vx >= 0.5f)
		{
			b->currentPos.x =  -vxp;
		}else{
			b->currentPos.x =  +vxp;
		}

		if (vy >= 0.5f)
		{
			b->currentPos.y = -vyp;
		}else{
			b->currentPos.y = vyp;
		}

		if (vz >= 0.5f)
		{
			b->currentPos.z = -vzp;
		}else{
			b->currentPos.z = vzp;
		}

		b->currentPos.z += 1.0f;

		float speed = rng->rFloat()*0.3f;
		for(int i=0; i<24; i++)
		{
			// target
			gtc4_1[groupNum][tcidx].x = b->targetPos.x;
			gtc4_1[groupNum][tcidx].y = b->targetPos.y;
			gtc4_1[groupNum][tcidx].z = b->targetPos.z;
			gtc4_1[groupNum][tcidx].w = b->startTime;

			// source
			gtc4_2[groupNum][tcidx].x = b->currentPos.x;
			gtc4_2[groupNum][tcidx].y = b->currentPos.y;
			gtc4_2[groupNum][tcidx].z = b->currentPos.z;
			gtc4_2[groupNum][tcidx].w = b->color.r > 0.0f && b->color.g > 0.0f && b->color.b > 0.0f && b->color.a > 0.0f
				&& b->colorEnd.r > 0.0f && b->colorEnd.g > 0.0f && b->colorEnd.b > 0.0f && b->colorEnd.a > 0.0f ? 1.0f : 0.0f;

			vertex v;
			CrossProduct(&v, &b->targetPos, &b->currentPos);

			// cross product
			gtc4_3[groupNum][tcidx].x = v.x;
			gtc4_3[groupNum][tcidx].y = v.y;
			gtc4_3[groupNum][tcidx].z = v.z;
			gtc4_3[groupNum][tcidx].w = 1.0f + (speed);

			tcidx++;
		}

	}
	CheckGlError(true);


	// upload vertex data to vbo
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, vboGVerts3[groupNum]);
	oglBufferDataARB(GL_ARRAY_BUFFER_ARB, gnumblocks[groupNum]*24*sizeof(vertex4), &gverts4[groupNum][0], GL_STATIC_DRAW_ARB);
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	CheckGlError(true);

	// normals
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, vboGNormals[groupNum]);
	oglBufferDataARB(GL_ARRAY_BUFFER_ARB, gnumblocks[groupNum]*24*sizeof(vertex), &gbn[groupNum][0], GL_STATIC_DRAW_ARB);
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	CheckGlError(true);

	// colors
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, vboGColors[groupNum]);
	oglBufferDataARB(GL_ARRAY_BUFFER_ARB, gnumblocks[groupNum]*24*sizeof(color4f), &gvertCols[groupNum][0], GL_STATIC_DRAW_ARB);
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	CheckGlError(true);

	// target positions into texcoord 1
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, vboGTexCoords1[groupNum]);
	oglBufferDataARB(GL_ARRAY_BUFFER_ARB, gnumblocks[groupNum]*24*sizeof(vertex4), &gtc4_1[groupNum][0], GL_STATIC_DRAW_ARB);
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	CheckGlError(true);

	// current positions into texcoord 2
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, vboGTexCoords2[groupNum]);
	oglBufferDataARB(GL_ARRAY_BUFFER_ARB, gnumblocks[groupNum]*24*sizeof(vertex4), &gtc4_2[groupNum][0], GL_STATIC_DRAW_ARB);
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	CheckGlError(true);

	// mid positions into texcoord 2
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, vboGTexCoords3[groupNum]);
	oglBufferDataARB(GL_ARRAY_BUFFER_ARB, gnumblocks[groupNum]*24*sizeof(vertex4), &gtc4_3[groupNum][0], GL_STATIC_DRAW_ARB);
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	CheckGlError(true);


    delete(s);
}


void logo::getBlocks(GLbyte* pixels, GLbyte* destPixels)
{
	cubeSize = 2.0f / 256.f;
	numblocks = 0;
	objectStack *s = new objectStack(256*256);

	int startY = 100;

	float halfSize = 108.0f;

	float st = -2.0f;
	int offsetY = 35;
	int offsetX = 0;
	CheckGlError(false);

	for(int x=0; x<256; x++)
	{
		for(int y=70; y<160; y++)
		{			
			color4f color = getBlockColor(pixels, x, y);
			color4f colorEnd = getBlockColor(destPixels, x, y);

			if ((color.r  > 0.0f || color.g > 0.0f || color.b > 0) && color.a > 0.0f)
			{
				logoBlock *b = new logoBlock();
				b->targetPos.x = ((float)(x+offsetX)*cubeSize*2)-(108*cubeSize);
				b->targetPos.y = (((float)y-startY+offsetY)*cubeSize*2)-(16*cubeSize);
				b->targetPos.z = 0;

				b->startTime = 0;
				b->percent = b->percent2 = 0;

				b->height = 1;

				if (color.r == 1.0f)
				{
					b->height = 4;
				}else if (color.r > 0.75f)
				{
					b->height = 3;
				}else{
					b->height = 2;
				}

				b->currentPos.x = (float)0;
				b->currentPos.y = (float)0;
				b->currentPos.z = 0;
				b->color = color;

				b->scalex = 1;
				b->scaley = 2;
				b->scalez = 1;

				b->startTime = st;
				st+=0.00012f;

				s->push((void*)b);
				numblocks++;

				if (color.r!=0.0f)
				{
					int layers = 2;
					for(int i=0; i<layers; i++)
					{				
						for (int j=0; j<2; j++)
						{
							float z = cubeSize*(i+1);
							if (j==0)
							{
								z = -z;
							}
							logoBlock *b = new logoBlock();
							b->targetPos.x = ((float)(x+offsetX)*cubeSize*2)-(108*cubeSize);
							b->targetPos.y = (((float)y-startY+offsetY)*cubeSize*2)-(16*cubeSize);
							b->targetPos.z = z;

							b->startTime = 0;
							b->percent = b->percent2 = 0;

							b->currentPos.x = (float)0;
							b->currentPos.y = (float)0;
							b->currentPos.z = 0;
							b->color = color;
							b->colorEnd = colorEnd;

							b->scalex = 1;
							b->scaley = 1;//((float)ca)/255.0f;
							b->scalez = 1;

							b->startTime = st*1.1f;

							s->push((void*)b);
							numblocks++;
						}
					}
				}

			}
		}
	}	
	CheckGlError(false);
	
	int idx = 0;

	verts4 = new vertex4[numblocks*24];
	tc4_1 = new vertex4[numblocks*24];
	tc4_2 = new vertex4[numblocks*24];
	tc4_3 = new vertex4[numblocks*24];
	verts4 = new vertex4[numblocks*24];
	bn = new vertex[numblocks*24];
	vertCols = new color4f[numblocks*24];

	for(int i=0; i<numblocks; i++)
	{
		logoBlock *b = (logoBlock*)s->items[i];
		idx = createCube(idx, i, b->color, vertCols, verts4, bn, 1.0f);
	}

	CheckGlError(false);
	rng = new RNG();

	indices = new int[numblocks*24];

	int tcidx = 0;

	// set source positions
	for(int i=0; i<numblocks; i++)
	{
		logoBlock *b = (logoBlock*)s->items[i];

		float amt = 1.0f;

		float vx = rng->rFloat()*amt;
		float vy = rng->rFloat()*amt;
		float vz = rng->rFloat()*amt;

		float vxp = rng->rFloat()*amt;
		float vyp = rng->rFloat()*amt;
		float vzp = rng->rFloat()*amt;


		if (vx >= 0.5f)
		{
			b->currentPos.x =  -vxp;
		}else{
			b->currentPos.x =  +vxp;
		}

		if (vy >= 0.5f)
		{
			b->currentPos.y = -vyp;
		}else{
			b->currentPos.y = vyp;
		}

		if (vz >= 0.5f)
		{
			b->currentPos.z = -vzp;
		}else{
			b->currentPos.z = vzp;
		}

		b->currentPos.z += 1.0f;

		float speed = rng->rFloat()*0.3f;
		for(int i=0; i<24; i++)
		{
			// target
			tc4_1[tcidx].x = b->targetPos.x;
			tc4_1[tcidx].y = b->targetPos.y;
			tc4_1[tcidx].z = b->targetPos.z;
			tc4_1[tcidx].w = b->startTime;

			// source
			tc4_2[tcidx].x = b->currentPos.x;
			tc4_2[tcidx].y = b->currentPos.y;
			tc4_2[tcidx].z = b->currentPos.z;
			tc4_2[tcidx].w = b->color.r > 0.0f && b->color.g > 0.0f && b->color.b > 0.0f && b->color.a > 0.0f
				&& b->colorEnd.r > 0.0f && b->colorEnd.g > 0.0f && b->colorEnd.b > 0.0f && b->colorEnd.a > 0.0f ? 1.0f : 0.0f;

			vertex v;
			CrossProduct(&v, &b->targetPos, &b->currentPos);

			// cross product
			tc4_3[tcidx].x = v.x;
			tc4_3[tcidx].y = v.y;
			tc4_3[tcidx].z = v.z;
			tc4_3[tcidx].w = 1.0f + (speed);

			tcidx++;
		}

	}
	CheckGlError(true);

	// upload vertex data to vbo
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, vboVerts3);
	oglBufferDataARB(GL_ARRAY_BUFFER_ARB, numblocks*24*sizeof(vertex4), &verts4[0], GL_STATIC_DRAW_ARB);
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	CheckGlError(true);

	// normals
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, vboNormals);
	oglBufferDataARB(GL_ARRAY_BUFFER_ARB, numblocks*24*sizeof(vertex), &bn[0], GL_STATIC_DRAW_ARB);
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	CheckGlError(true);

	// colors
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, vboColors);
	oglBufferDataARB(GL_ARRAY_BUFFER_ARB, numblocks*24*sizeof(color4f), &vertCols[0], GL_STATIC_DRAW_ARB);
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	CheckGlError(true);

	// target positions into texcoord 1
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, vboTexCoords1);
	oglBufferDataARB(GL_ARRAY_BUFFER_ARB, numblocks*24*sizeof(vertex4), &tc4_1[0], GL_STATIC_DRAW_ARB);
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	CheckGlError(true);

	// current positions into texcoord 2
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, vboTexCoords2);
	oglBufferDataARB(GL_ARRAY_BUFFER_ARB, numblocks*24*sizeof(vertex4), &tc4_2[0], GL_STATIC_DRAW_ARB);
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	CheckGlError(true);

	// mid positions into texcoord 2
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, vboTexCoords3);
	oglBufferDataARB(GL_ARRAY_BUFFER_ARB, numblocks*24*sizeof(vertex4), &tc4_3[0], GL_STATIC_DRAW_ARB);
	oglBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	CheckGlError(true);

	for(int i=0; i<24*numblocks; i++)
	{		
		indices[i] = i;
	}
}

// create a cube (for adding to a vbo)
int logo::createCube(int idx, int cubeNumVal, color4f color, color4f* vCols, vertex4* v4, vertex* ns, float depth)
{
	int index = idx;
	float cubeNum = (float)cubeNumVal;
	
	// target vertex pos
	// front 
	populateVertex4(&v4[index++], -cubeSize, -cubeSize, +cubeSize, cubeNum);
	populateVertex4(&v4[index++], -cubeSize, +cubeSize, +cubeSize, cubeNum);
	populateVertex4(&v4[index++], +cubeSize, +cubeSize, +cubeSize, cubeNum);
	populateVertex4(&v4[index++], +cubeSize, -cubeSize, +cubeSize, cubeNum);

	// back 
	populateVertex4(&v4[index++], -cubeSize, -cubeSize, -cubeSize*depth, cubeNum);
	populateVertex4(&v4[index++], -cubeSize, +cubeSize, -cubeSize*depth, cubeNum);
	populateVertex4(&v4[index++], +cubeSize, +cubeSize, -cubeSize*depth, cubeNum);
	populateVertex4(&v4[index++], +cubeSize, -cubeSize, -cubeSize*depth, cubeNum);

	// top
	populateVertex4(&v4[index++], -cubeSize, -cubeSize, +cubeSize, cubeNum);
	populateVertex4(&v4[index++], -cubeSize, -cubeSize, -cubeSize*depth, cubeNum);
	populateVertex4(&v4[index++], +cubeSize, -cubeSize, -cubeSize*depth, cubeNum);
	populateVertex4(&v4[index++], +cubeSize, -cubeSize, +cubeSize, cubeNum);

	// bottom
	populateVertex4(&v4[index++], -cubeSize, +cubeSize, +cubeSize, cubeNum);
	populateVertex4(&v4[index++], -cubeSize, +cubeSize, -cubeSize*depth, cubeNum);
	populateVertex4(&v4[index++], +cubeSize, +cubeSize, -cubeSize*depth, cubeNum);
	populateVertex4(&v4[index++], +cubeSize, +cubeSize, +cubeSize, cubeNum);

	// left
	populateVertex4(&v4[index++], -cubeSize, -cubeSize, -cubeSize*depth, cubeNum);
	populateVertex4(&v4[index++], -cubeSize, +cubeSize, -cubeSize*depth, cubeNum);
	populateVertex4(&v4[index++], -cubeSize, +cubeSize, +cubeSize, cubeNum);
	populateVertex4(&v4[index++], -cubeSize, -cubeSize, +cubeSize, cubeNum);

	// right
	populateVertex4(&v4[index++], +cubeSize, -cubeSize, -cubeSize*depth, cubeNum);
	populateVertex4(&v4[index++], +cubeSize, +cubeSize, -cubeSize*depth, cubeNum);
	populateVertex4(&v4[index++], +cubeSize, +cubeSize, +cubeSize, cubeNum);
	populateVertex4(&v4[index++], +cubeSize, -cubeSize, +cubeSize, cubeNum);

	// colors
	index = idx;
	for(int i=0; i<24; i++)
	{
		populateColor4f(&vCols[index++], color.r, color.g, color.b, color.a);
	}
		
	// normals
	index = idx;

	// front
	populateVertex(&ns[index++], 0.0f, 0.0f, 1.0f);
	populateVertex(&ns[index++], 0.0f, 0.0f, 1.0f);
	populateVertex(&ns[index++], 0.0f, 0.0f, 1.0f);
	populateVertex(&ns[index++], 0.0f, 0.0f, 1.0f);

	// back
	populateVertex(&ns[index++], 0.0f, 0.0f, -1.0f);
	populateVertex(&ns[index++], 0.0f, 0.0f, -1.0f);
	populateVertex(&ns[index++], 0.0f, 0.0f, -1.0f);
	populateVertex(&ns[index++], 0.0f, 0.0f, -1.0f);

	// top
	populateVertex(&ns[index++], 0.0f, -1.0f, 0.0f);
	populateVertex(&ns[index++], 0.0f, -1.0f, 0.0f);
	populateVertex(&ns[index++], 0.0f, -1.0f, 0.0f);
	populateVertex(&ns[index++], 0.0f, -1.0f, 0.0f);

	// bottom
	populateVertex(&ns[index++], 0.0f, 1.0f, 0.0f);
	populateVertex(&ns[index++], 0.0f, 1.0f, 0.0f);
	populateVertex(&ns[index++], 0.0f, 1.0f, 0.0f);
	populateVertex(&ns[index++], 0.0f, 1.0f, 0.0f);


	// left
	populateVertex(&ns[index++], -1.0f, 0.0f, 0.0f);
	populateVertex(&ns[index++], -1.0f, 0.0f, 0.0f);
	populateVertex(&ns[index++], -1.0f, 0.0f, 0.0f);
	populateVertex(&ns[index++], -1.0f, 0.0f, 0.0f);

	// right
	populateVertex(&ns[index++], 1.0f, 0.0f, 0.0f);
	populateVertex(&ns[index++], 1.0f, 0.0f, 0.0f);
	populateVertex(&ns[index++], 1.0f, 0.0f, 0.0f);
	populateVertex(&ns[index++], 1.0f, 0.0f, 0.0f);
	
	return index;
}

// get color of pixel
color4f logo::getBlockColor(GLbyte* pixels, int x, int y)
{
	color4f col;

	GLubyte cr = pixels[(x*4)+(y*256*4)];
	GLubyte cg = pixels[(x*4)+(y*256*4)+1];
	GLubyte cb = pixels[(x*4)+(y*256*4)+2];
	GLubyte ca = pixels[(x*4)+(y*256*4)+3];

	col.r = cr/255.0f;
	col.g = cg/255.0f;
	col.b = cb/255.0f;
	col.a = ca/255.0f;


	return col;
}

#define lerpy(t, a, b) ( a + t * (b - a) )

void logo::refreshShaders()
{
    _shaderIdGreets->Refresh();
    _shaderIdLogo->Refresh();
}

void logo::draw(double timeIn)
{	
	float time = (float)timeIn;
	time+=5;
	glEnable(GL_TEXTURE_3D);
	glEnable(GL_BLEND);
		
	glPushMatrix();
	{
		glLoadIdentity();	

		float zv = -4.0f;

		if (time < 5)
		{
			zv -= time*1.5f;

		}

		glTranslatef(-1.2f,0.4f, zv);
        
		DemoLight(time);
		DemoLight2(time);
		DemoLight3(time);
		glEnable(GL_COLOR_MATERIAL);

		CheckGlError(false);
			
		glEnableClientState(GL_VERTEX_ARRAY);	
		oglBindBufferARB( GL_ARRAY_BUFFER, vboVerts3 );
		glVertexPointer(4, GL_FLOAT, 0, 0);
		CheckGlError(true);

		glEnableClientState(GL_NORMAL_ARRAY);	
		oglBindBufferARB( GL_ARRAY_BUFFER, vboNormals );
		glNormalPointer(GL_FLOAT, 0, 0);
		CheckGlError(true);

		glEnableClientState(GL_COLOR_ARRAY);	
		oglBindBufferARB( GL_ARRAY_BUFFER, vboColors );
		glColorPointer(4, GL_FLOAT, 0, 0);
		CheckGlError(true);

		oglClientActiveTextureARB(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
		oglBindBufferARB( GL_ARRAY_BUFFER, vboTexCoords1 );
		glTexCoordPointer(4, GL_FLOAT, 0, 0);
		CheckGlError(true);

		oglClientActiveTextureARB(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
		oglBindBufferARB( GL_ARRAY_BUFFER, vboTexCoords2 );
		glTexCoordPointer(4, GL_FLOAT, 0, 0);
		CheckGlError(true);

		oglClientActiveTextureARB(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
		oglBindBufferARB( GL_ARRAY_BUFFER, vboTexCoords3 );
		glTexCoordPointer(4, GL_FLOAT, 0, 0);
		CheckGlError(true);

		float rewind = 14.0f;

		if (time > rewind)
		{
			//time = rewind - (time-rewind);
		}
		
		// zoom scene
		float zm = 0.03f;
		float zval = time < 10 ? time*zm : 10*zm;
		glTranslatef(0,zval,-1.5);

		// rotate scene
		float rotSpeed = time <= 10 ? 10-time : 0;
		float rval = time <= 10 ? time * 36 : 0;
		glRotatef(rval, 0, rotSpeed, 0);	
		glEnable(GL_DEPTH_TEST);

		glColor4f(1,0.1f,0.2f,1);
				
		float rv = -45+180;
		CheckGlError(false);
		CheckGlError(true);
		oglUseProgram(_shaderIdLogo->progId);		

		setShaderParam1f(_shaderIdLogo->progId, "time", time);

		glRotatef(rv, 1, 0, 0);

		int i =0;
		
		vertex v;
		CheckGlError(true);

		CheckGlError(true);
		// draw cubes
		setShaderParam1i(_shaderIdLogo->progId, "black", 0);
		glClearColor(0,0,0,0); // red
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glPushMatrix();
		{
			glTranslatef(0,0.16f, 0);
			glDrawArrays(GL_QUADS, 0, 24*numblocks);
		}
		glPopMatrix();
		CheckGlError(false);
        
		oglUseProgram(0);
		glBindTexture(GL_TEXTURE_3D, 0);
		CheckGlError(false);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		
		oglClientActiveTextureARB(GL_TEXTURE2);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		
		oglClientActiveTextureARB(GL_TEXTURE1);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		oglClientActiveTextureARB(GL_TEXTURE0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		CheckGlError(false);
	}
	glPopMatrix();
	
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_3D);
}

float lightPositionOnScreenX = 0;
float lightPositionOnScreenY = 0;
float exposure = 0.0014f;
float decay = 1.0f;
float density = 0.84f;
float weight = 5.65f;

void logo::drawGroupGreet(int groupNum, float time)
{
    if (time < 0.0) return;

	glEnableClientState(GL_VERTEX_ARRAY);	
	oglBindBufferARB( GL_ARRAY_BUFFER, vboGVerts3[groupNum] );
	glVertexPointer(4, GL_FLOAT, 0, 0);
	CheckGlError(true);

	glEnableClientState(GL_NORMAL_ARRAY);	
	oglBindBufferARB( GL_ARRAY_BUFFER, vboGNormals[groupNum] );
	glNormalPointer(GL_FLOAT, 0, 0);
	CheckGlError(true);

	glEnableClientState(GL_COLOR_ARRAY);	
	oglBindBufferARB( GL_ARRAY_BUFFER, vboGColors[groupNum] );
	glColorPointer(4, GL_FLOAT, 0, 0);
	CheckGlError(true);

	oglClientActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
	oglBindBufferARB( GL_ARRAY_BUFFER, vboGTexCoords1[groupNum] );
	glTexCoordPointer(4, GL_FLOAT, 0, 0);
	CheckGlError(true);

	oglClientActiveTextureARB(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
	oglBindBufferARB( GL_ARRAY_BUFFER, vboGTexCoords2[groupNum] );
	glTexCoordPointer(4, GL_FLOAT, 0, 0);
	CheckGlError(true);

	oglClientActiveTextureARB(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
	oglBindBufferARB( GL_ARRAY_BUFFER, vboGTexCoords3[groupNum] );
	glTexCoordPointer(4, GL_FLOAT, 0, 0);
	CheckGlError(true);

	oglUseProgram(_shaderIdGreets->progId);		

	setShaderParam1f(_shaderIdGreets->progId, "time", time);
    setShaderParam1f(_shaderIdGreets->progId, "cubeSize", cubeSize);
	//setShaderParam1i(_shaderIdLogo, "NoiseTex", 3);

	// draw cubes
	setShaderParam1i(_shaderIdGreets->progId, "black", 0);

	//fboFullscreen2->BindFbo(true);
	glDrawArrays(GL_QUADS, 0, 24*gnumblocks[groupNum]);
	//fboFullscreen2->BindFbo(false);

	//fbo512_2->BindTexture(0);

	CheckGlError(false);

	oglUseProgram(0);

}

void logo::waterGreets(float time)
{   
    return;
}


/************************************************************************/
/*  Create plasma texture from perlin noise in fbo 512x512 (#1)                          
/************************************************************************/
void logo::GDrawFboPlasma(float time, bool* hitBeatInfo)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setShaderParam1f(_julia->_shaderIdPlasma->progId, "mode", 1);
    _julia->DrawFboPlasma(time*3);
    _julia->fbo512->BindTexture(1);        
    //setShaderParam1f(_julia->_shaderIdTunnel->progId, "mode", 1);
    _julia->DrawTunnelToFbo(time*2, hitBeatInfo);
    //_julia->DrawJuliaToFbo(time, hitBeatInfo);
    fboFullscreen2->BindFbo(true);
    //_julia->DrawToScreen(time, hitBeatInfo);

    glPushMatrix();
    {
        fbo1024->BindTexture(1);
        glEnable(GL_TEXTURE_2D);
        ViewOrtho(fboFullscreen->info.width, fboFullscreen->info.height);		

        glViewport(0, 0, fboFullscreen->info.width, fboFullscreen->info.height); // change viewport to match screen size

        glBegin(GL_QUADS);
        {
            glTexCoord2f(0,0);
            glVertex2i(0,0);

            glTexCoord2f(0,1);
            glVertex2i(0,fboFullscreen->info.height);

            glTexCoord2f(1,1);
            glVertex2i(fboFullscreen->info.width, fboFullscreen->info.height);

            glTexCoord2f(1,0);
            glVertex2i(fboFullscreen->info.width, 0);
        }		
        glEnd();
        ViewPerspective();
        glViewport(0, 0, fboFullscreen2->info.width, fboFullscreen2->info.height); // change viewport to match screen size
        glDisable(GL_TEXTURE_2D);
    }
}

void logo::drawGreets(float time, bool* hitBeatInfo)
{    
    time += 2;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
        GDrawFboPlasma(time, hitBeatInfo);
    glPopMatrix();
    fboFullscreen2->BindFbo(true);

    GDemoLight(time);
   // Material(time);
    glEnable(GL_COLOR_MATERIAL);
    GDemoLight2(time);
    //GDemoLight3(time);
    //glColor4f(0.5f, 0.1f, 0.1f, 0.1f);
	CheckGlError(false);
    
	glPushMatrix();
	{
		glLoadIdentity();	
        
        //glClearColor(0,0,0,0); // red
        //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glClear( GL_DEPTH_BUFFER_BIT );

       // glEnable(GL_TEXTURE_3D);
        glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glColor4f(1,1,1,1);
		
		for(int i=0; i<numGroups; i++)
		{
			glPushMatrix();
			{
                float zv = -4.0f;

                if (time < 5)
                {
                    //zv -= time*1.5f;

                }
                //glRotatef(180, 1, 0, 0);
                glTranslatef(gXpos[i], gYpos[i], zv+gZpos[i]);

				float groupTime = time-(0.8f*i);
				// zoom scene
				float zm = 0.03f;
				float zval = groupTime < 10 ? groupTime*zm : 10*zm;
				//glTranslatef(0,zval,-1.5);

				// rotate scene
				float rotSpeed = groupTime <= 10 ? 10-groupTime : 0;
				float rval = groupTime <= 10 ? groupTime * 36 : 0;
				//glRotatef(rval, 0, rotSpeed, 0);	
				glEnable(GL_DEPTH_TEST);

				//glColor4f(1,1,1.0f,1);

				float rv = -45+180;
				CheckGlError(false);
				//glClientActiveTexture(GL_TEXTURE3);
				//glBindTexture(GL_TEXTURE_3D, Noise3D::GetHandle());
				CheckGlError(true);


				//glRotatef(rv, 1, 0, 0);

				// text
                glTranslatef(0,0,groupTime*0.8f);
               // glRotatef(0,0.0f,0,0.0f);
				drawGroupGreet(i, groupTime);
			}
			glPopMatrix();
		}

		glBindTexture(GL_TEXTURE_3D, 0);
		CheckGlError(false);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		
		oglClientActiveTextureARB(GL_TEXTURE2);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		
		oglClientActiveTextureARB(GL_TEXTURE1);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		oglClientActiveTextureARB(GL_TEXTURE0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		CheckGlError(false);
	}
	glPopMatrix();

   // fboFullscreen->BindFbo(true);
	
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_3D);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//GodRays();
}

void logo::DemoLight(float time)
{
	/*
	lookAt(0.0f,0.0f,4.0f, 
		0.0,0.0,-1.0,
		0.0f,1.0f,0.0f);*/

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	// Light model parameters:
	// -------------------------------------------

	GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// -------------------------------------------
	// Spotlight Attenuation

	GLfloat spot_direction[] = {-1.0, -1.0, -1.0 };
	GLint spot_exponent = 30;
	GLint spot_cutoff = 180;

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
	glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);

	GLfloat Kc = 1.0;
	GLfloat Kl = 0.0;
	GLfloat Kq = 0.0;

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);


	// ------------------------------------------- 
	// Lighting parameters:

	GLfloat light_pos[] = {5.0f, 5.0f, 0.0f, 1.0f};
	GLfloat light_Ka[]  = {0.2f, 0.4f, 0.0f, 1.0f};
	GLfloat light_Kd[]  = {0.2f, 0.8f, 0.2f, 1.0f};
	GLfloat light_Ks[]  = {0.0f, 1.0f, 0.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);
}

void logo::DemoLight2(float time)
{
	/*
	lookAt(0.0f,0.0f,4.0f, 
		0.0,0.0,-1.0,
		0.0f,1.0f,0.0f);*/

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);

	// Light model parameters:
	// -------------------------------------------

	GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// -------------------------------------------
	// Spotlight Attenuation

	GLfloat spot_direction[] = {0.0, -2.0f, 0.0 };
	GLint spot_exponent = 30;
	GLint spot_cutoff = 180;

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLighti(GL_LIGHT1, GL_SPOT_EXPONENT, spot_exponent);
	glLighti(GL_LIGHT1, GL_SPOT_CUTOFF, spot_cutoff);

	GLfloat Kc = 1.0;
	GLfloat Kl = 0.0;
	GLfloat Kq = 0.0;

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION,Kc);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, Kl);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, Kq);


	// ------------------------------------------- 
	// Lighting parameters:

	GLfloat light_pos[] = {0.0f, 5.0f, 0.0f, 1.0f};
	GLfloat light_Ka[]  = {0.0f, 0.5f, 0.0f, 1.0f};
	GLfloat light_Kd[]  = {0.0f, 0.8f, 0.2f, 1.0f};
	GLfloat light_Ks[]  = {0.0f, 1.0f, 0.0f, 1.0f};

	glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_Ks);
}

void logo::DemoLight3(float time)
{
	/*
	lookAt(0.0f,0.0f,4.0f, 
		0.0,0.0,-1.0,
		0.0f,1.0f,0.0f);*/

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT3);
	glEnable(GL_NORMALIZE);

	// Light model parameters:
	// -------------------------------------------

	GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// -------------------------------------------
	// Spotlight Attenuation

	GLfloat spot_direction[] = {2.0, 2.0, -10.0 };
	GLint spot_exponent = 30;
	GLint spot_cutoff = 180;

	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
	glLighti(GL_LIGHT3, GL_SPOT_EXPONENT, spot_exponent);
	glLighti(GL_LIGHT3, GL_SPOT_CUTOFF, spot_cutoff);

	GLfloat Kc = 1.0;
	GLfloat Kl = 0.0;
	GLfloat Kq = 0.0;

	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION,Kc);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, Kl);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, Kq);


	// ------------------------------------------- 
	// Lighting parameters:

	GLfloat light_pos[] = {0.0f, -5.0f, 0.0f, 1.0f};
	GLfloat light_Ka[]  = {0.1f, 0.8f, 0.0f, 1.0f};
	GLfloat light_Kd[]  = {0.1f, 1.0f, 0.0f, 1.0f};
	GLfloat light_Ks[]  = {0.2f, 1.0f, 0.0f, 1.0f};

	glLightfv(GL_LIGHT3, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_Ks);
}







void logo::GDemoLight(float time)
{
	/*
	lookAt(0.0f,0.0f,4.0f, 
		0.0,0.0,-1.0,
		0.0f,1.0f,0.0f);*/

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	// Light model parameters:
	// -------------------------------------------

	GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// -------------------------------------------
	// Spotlight Attenuation

	GLfloat spot_direction[] = {-1.0, -1.0, 1.0 };
	GLint spot_exponent = 30;
	GLint spot_cutoff = 180;

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
	glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);

	GLfloat Kc = 1.0;
	GLfloat Kl = 0.0;
	GLfloat Kq = 0.0;

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);


	// ------------------------------------------- 
	// Lighting parameters:

	GLfloat light_pos[] = {5.0f, 5.0f, 5.0f, 1.0f};
	GLfloat light_Ka[]  = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat light_Kd[]  = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);
}

void logo::GDemoLight2(float time)
{
	/*
	lookAt(0.0f,0.0f,4.0f, 
		0.0,0.0,-1.0,
		0.0f,1.0f,0.0f);*/

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);

	// Light model parameters:
	// -------------------------------------------

	GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// -------------------------------------------
	// Spotlight Attenuation

	GLfloat spot_direction[] = {0.0, -2.0f * time, 0.0 };
	GLint spot_exponent = 30;
	GLint spot_cutoff = 180;

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLighti(GL_LIGHT1, GL_SPOT_EXPONENT, spot_exponent);
	glLighti(GL_LIGHT1, GL_SPOT_CUTOFF, spot_cutoff);

	GLfloat Kc = 1.0;
	GLfloat Kl = 0.0;
	GLfloat Kq = 0.0;

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION,Kc);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, Kl);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, Kq);


	// ------------------------------------------- 
	// Lighting parameters:

	GLfloat light_pos[] = {0.0f, 5.0f, -10.0f, 1.0f};
	GLfloat light_Ka[]  = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat light_Kd[]  = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat light_Ks[]  = {1.0f, 1.0f, 0.0f, 1.0f};

	glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_Ks);
}

void logo::GDemoLight3(float time)
{
	/*
	lookAt(0.0f,0.0f,4.0f, 
		0.0,0.0,-1.0,
		0.0f,1.0f,0.0f);*/

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT3);
	glEnable(GL_NORMALIZE);

	// Light model parameters:
	// -------------------------------------------

	GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// -------------------------------------------
	// Spotlight Attenuation

	GLfloat spot_direction[] = {2.0, 2.0, 10.0 };
	GLint spot_exponent = 30;
	GLint spot_cutoff = 180;

	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
	glLighti(GL_LIGHT3, GL_SPOT_EXPONENT, spot_exponent);
	glLighti(GL_LIGHT3, GL_SPOT_CUTOFF, spot_cutoff);

	GLfloat Kc = 1.0;
	GLfloat Kl = 0.0;
	GLfloat Kq = 0.0;

	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION,Kc);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, Kl);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, Kq);


	// ------------------------------------------- 
	// Lighting parameters:

	GLfloat light_pos[] = {0.0f, -5.0f, 0.0f, 1.0f};
	GLfloat light_Ka[]  = {0.1f*time, 0.8f, 0.0f, 1.0f};
	GLfloat light_Kd[]  = {0.1f, 1.0f, 0.0f, 1.0f};
	GLfloat light_Ks[]  = {0.2f, 1.0f, 0.0f, 1.0f};

	glLightfv(GL_LIGHT3, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_Ks);
}

logo::~logo(void)
{
	/*
	oglDeleteBuffersARB(numGroups, &vboVerts3);
	oglDeleteBuffersARB(numGroups, &vboNormals);
	oglDeleteBuffersARB(numGroups, &vboColors);
	oglDeleteBuffersARB(numGroups, &vboTexCoords1);
	oglDeleteBuffersARB(numGroups, &vboTexCoords2);
	oglDeleteBuffersARB(numGroups, &vboTexCoords3);*/
}



