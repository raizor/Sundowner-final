
#define OCCLUSION
#define SHADOW
#define REFLECTION
#define ANTIALIAS

#define MAX_STEPS	255
#define MAX_OCCLUSION	10
#define MAX_ALIAS	3

const float PI=3.14159;
const float EPSILON=6e-7;
const float BAILOUT=4.0;

uniform vec2 resolution;
uniform float displace1;
uniform vec2 mouse;
uniform float time;

uniform int boxIterations;
uniform int spongeIterations;

uniform float twistX;
uniform float twistY;
uniform float twistZ;

// Would-be uniforms

vec3 u_vCamera=vec3(0.0001);
vec3 u_vObject=vec3(0.0);
vec3 u_vLight=vec3(-125.0);

mat3 u_mCamera=mat3(
	1.0,0.0,0.0,
	0.0,1.0,0.0,
	0.0,0.0,1.0);

mat3 u_mObject=mat3(
	1.0,0.0,0.0,
	0.0,1.0,0.0,
	0.0,0.0,1.0);

mat3 u_mLight=mat3(
	1.0,0.0,0.0,
	0.0,1.0,0.0,
	0.0,0.0,1.0);

float u_fPower=0.2;
vec3 u_vJulia=vec3(0.0);
vec3 u_vOffset=vec3(0.0);
vec3 u_vClamp=vec3(0.0);
float u_fBounds=120.0;

float u_fDetail=0.3;
float u_fSmooth=0.5;
float u_fDither=0.5;
int u_nAlias=0;

//vec3 u_cAmbient=vec3(1.0,1.0,1.0);
vec3 u_cAmbient=vec3((sin(time*0.1)*0.5)+0.5,(cos(time*0.2)*0.5)+0.5,(sin(time*0.3)*0.5)+0.5);
vec3 u_cDiffuse=vec3(1.0,0.6,0.7);
vec3 u_cColour1=vec3(0.8,0.4,0.1);
const vec3 u_cColour2=vec3(0.5,0.2,0.2);
const vec3 u_cInnerGlow=vec3(1.0,1.0,1.0);
const vec3 u_cOuterGlow=vec3(1.0,1.0,1.0);
const vec3 u_cLight=vec3(1.0,0.9,0.2);
const vec3 u_cShadow=vec3(0.0,0.0,0.0);
const vec3 u_cBackground=vec3(0.0,0.0,0.0);

const float u_fAmbient=0.25;
const float u_fDiffuse=1.0;
const float u_fColour1=0.5;
const float u_fColour2=0.5;
const float u_fInnerGlow=0.0;
const float u_fOuterGlow=0.0;
const float u_fLight=0.3;
const float u_fShadow=0.5;
const float u_fBackground=0.0;

const int u_nColouring=1;

const float u_fOcclusion=3.0;
const float u_fEnhance=0.0;
const float u_fShininess=1.5;
const float u_fHardness=4.0;
const float u_fReflection=0.0;
const float u_fSoftShadow=10.0;
const float u_fBacklight=0.0;
const float u_fFog=0.0;
const float u_fFalloff=0.0;

const float fNormalDetail=1.0;
const float fShadowDetail=2.0;

const bool u_bBounds=false;

float u_fWidth=resolution.x;
float u_fHeight=resolution.y;
float u_fRatio=u_fWidth/u_fHeight;
float u_fScale=1.0/min(u_fWidth,u_fHeight);

float fMinDist=100000.0;
float fOrbitTrap=0.0;

float rand(const vec2 vSeed)
{
	const vec2 vR1=vec2(12.9898,78.233);
	const float fR1=43758.5453;

	return fract(sin(dot(vSeed,vR1))*fR1);
}

vec3 rand3(const vec2 vSeed)
{
	const vec2 vR1=vec2(12.9898,78.233);
	const vec2 vR2=vec2(4.898,7.23);
	const vec2 vR3=vec2(0.23,1.111);

	const float fR1=43758.5453;
	const float fR2=23421.631;
	const float fR3=392820.23;

	return vec3(
		fract(sin(dot(vSeed,vR1))*fR1),
		fract(sin(dot(vSeed,vR2))*fR2),
		fract(sin(dot(vSeed,vR3))*fR3));
}

float opUnion( float d1, float d2 )
{
    return min(d1,d2);
}

float opSubtract( float d1, float d2 )
{
    return max(-d1,d2);
}

float opIntersect( float d1, float d2 )
{
    return max(d1,d2);
}

vec3 GetBg()
{
	vec2 pos = -1.0 + 2.0 * gl_FragCoord.xy / resolution;
	pos.x *= (resolution.x / resolution.y);
	
	float u = length(pos)*1.5;
	float v = atan(pos.y, pos.x)+(time*1.9);
	float t = time - 10.0 / u;
	
	vec3 color = vec3(abs(sin(t * 1.0 + v))) * u * 0.15;
	color += vec3(abs(sin(-t*1113.14159265* (v*3.14159265)))) * u * 0.55;
	
	color.x *= 0.48;
	color.y *= 0.36*sin(time*0.22);
	color.z=0.11*cos(time*0.22);
	
	gl_FragColor = vec4(color, 1.0);
	return color;
	}

float deSponge(vec3 vRay)
{
	int nSteps=spongeIterations;

	vec3 c=(vRay*0.5)+0.5;
	vec3 v=abs(c-0.5)-0.5;

	float m=u_vClamp.x;
	float r=max(v.x,max(v.y,v.z));

	if(r<fMinDist) fMinDist=r;

	for(int n=1;n<6;n++)
	{
		if(n>nSteps) break;

		m*=u_vClamp.y;
		v=(0.5-abs(mod(c*m,u_vClamp.z)-1.5))+u_vOffset;
		r=max(r,min(max(v.x,v.z),min(max(v.x,v.y),max(v.y,v.z)))/m);

		if(r<fMinDist) fMinDist=r;
		if(n==u_nColouring) 
		{
			fOrbitTrap=dot(v,v);
		}
	}
	return r*2.0;
}

vec3 opBend(vec3 p, float amount)
{	
	float c = cos(1.0*p.y);
    float s = sin(1.0*p.y);
    mat2  m = mat2(c,-s,s,c);
    vec3  q = vec3(m*p.xy,p.z);
    return q;
}

float opDisplace( float primitive, float displacement )
{
    return primitive+displacement;
}

vec3 opTwist( vec3 p, vec2 amount )
{
	float c;
	float s;
	mat2 m;
	vec3 q;

	// y
    c = cos(amount.y*p.y);
    s = sin(amount.y*p.y);
    m = mat2(c,-s,s,c);
    q = vec3(m*p.xz,p.y);

	c = cos(amount.x*q.x);
    s = sin(amount.x*q.x);
    m = mat2(c,-s,s,c);
    q = vec3(m*p.yz,p.x);

	return q;
}


float displace(vec3 p, float amt)
{
	return sin(amt*p.x)*sin(amt*p.y)*sin(amt*p.z);
}

float udRoundBox( vec3 p, vec3 b, float r )
{
  return length(max(abs(p)-b,0.0))-r;
}

float minimumDistanceToCube( vec3 pos, float scale )
{
  vec3 dist= abs( pos ) - vec3(scale, scale, scale);

  dist= max( dist, 0.0 );

  return max( max(dist.x, dist.y), dist.z );
}

float sdSphere( vec3 p, float s )
{
  return length(p)-s;
}

float rayDistance(vec3 vRay)
{
	float val = 0.0;
	vRay*=u_mObject;

	val =  deSponge(opTwist(vRay,  vec2(-twistX, -twistY)));
	float cube = minimumDistanceToCube(opTwist(vRay,  vec2(twistX, twistY)), 0.8-(time*0.003));

	vRay = opTwist(vRay,  vec2(-twistX, -twistY));

	float sphere = sdSphere(vRay-vec3(-1.1,1.1,1.1), time*0.005);	
	val = opUnion(val, cube);
	val = opSubtract(sphere, val);

	val =  opDisplace(val, displace(vRay, displace1));
	return val;
}


bool rayIntersec(vec3 vRay,vec3 vDir,out float fMin,out float fMax)
{
	float fRdt=dot(vRay,vDir);
	float fRdr=dot(vRay,vRay)-u_fBounds;

	float fDisc=(fRdt*fRdt)-fRdr;

	if(fDisc<=0.0) return false;

	fDisc=sqrt(fDisc);
	fRdt=-fRdt;

	fMin=max(0.0,fRdt-fDisc);
	fMax=fRdt+fDisc;

	return true;
}

vec3 rayNormal(vec3 vRay,float fEps)
{
	vec2 e=vec2(fEps,0.0);

	return normalize(vec3(
		rayDistance(vRay+e.xyy)-rayDistance(vRay-e.xyy),
		rayDistance(vRay+e.yxy)-rayDistance(vRay-e.yxy),
		rayDistance(vRay+e.yyx)-rayDistance(vRay-e.yyx)));
}

float rayOcclusion(vec3 vRay,vec3 vDir,float fEps)
{
	float fOcclusion=1.0;

	#ifdef OCCLUSION
	float fLen=fEps*fShadowDetail;
	float fStep=1.0/u_fOcclusion;

	for(int n=0;n<MAX_OCCLUSION;n++)
	{
		if(n>=int(u_fOcclusion)) break;

		float fDist=rayDistance(vRay+vDir*fLen);
		fOcclusion*=1.0-max(0.0,(fLen-fDist)*fStep/fLen);

		fLen+=fEps*u_fEnhance;
	}
	#endif 

	return 1.0-fOcclusion;
}

float rayShadow(vec3 vRay,vec3 vDir,vec3 vLight,float fLight,float fEps)
{
	float fShadow=1.0;

	#ifdef SHADOW
	float fLen=fEps*fShadowDetail;

	for(int n=0;n<MAX_STEPS;n++)
	{
		if(fLen>=fLight) break;

		float fDist=rayDistance(vRay+(vLight*fLen));
		if(fDist<fEps) return 1.0;

		if(u_fSoftShadow!=0.0)
			fShadow=min(fShadow,u_fSoftShadow*(fDist/fLen));

		fLen+=fDist;
	}
	#endif

	return 1.0-fShadow;
}


vec3 raySun(const vec3 vDir,const vec3 vLight)
{
	if(u_fLight>0.0)
		return u_cLight*(1.0-clamp(abs(acos(dot(vDir,vLight)/length(vLight)))/u_fLight,0.0,1.0));

	return vec3(0.0);
}

vec3 rayBackground(const vec3 vDir)
{
	if(u_fBackground>0.0)
		return mix(vec3(0.0),u_cBackground,clamp(abs(vDir.y)*u_fBackground,0.0,1.0));

	return u_cBackground;
}

vec3 rayColour(vec3 vRay,vec3 vDir,vec3 vHit,vec3 vNormal,vec3 vLight,float fLen,int nStep,float fEps,float fDist)
{
	vHit-=vDir*(fEps*fShadowDetail);

	if(u_fDiffuse<0.0)
		return vec3(0.5)+(normalize(vHit)*abs(u_fDiffuse));

	vec3 colour=(u_cDiffuse*1.0)*(u_fDiffuse*1.0);

	if(u_nColouring>0)
		colour=mix(colour,mix(u_cColour1,u_cColour2,fMinDist*u_fColour1),fOrbitTrap*u_fColour2);

	colour+=u_cAmbient*u_fAmbient;

	// get light info
	vec3 vToLight=vLight-vHit;
	vec3 vHalf=normalize(vToLight);

	float fLight=length(vToLight);
	float fProd=dot(vNormal,vHalf);

	float fShadow=0.0;

	if(u_fShadow!=0.0)
	{
		if(fProd>0.0)
		{
			fShadow=rayShadow(vHit,vDir,vHalf,fLight,fEps);
			if(u_fSoftShadow==0.0 && fShadow>0.0) fShadow=1.0;
		}
		else fShadow=1.0;
	}


	if(u_fHardness!=0.0 && u_fShininess!=0.0 && fProd>0.0)
		colour+=vec3(pow(max(fProd,0.0),u_fHardness)*u_fShininess)*(1.0-fShadow);

	if(u_fBacklight!=0.0 && fProd<=0.0)
		colour*=abs(fProd)*u_fBacklight;

	if(fShadow!=0.0)
		colour=mix(colour,u_cShadow,fShadow*u_fShadow);

	if(u_fOcclusion!=0.0)
	{
		float fOcclusion=(u_fEnhance==0.0)?
			(float(nStep)/float(MAX_STEPS))*u_fOcclusion:
			rayOcclusion(vHit,vNormal,u_fEnhance);

		colour=mix(colour,vec3(0.0),clamp(fOcclusion,0.0,1.0));
	}

	if(u_fInnerGlow!=0.0)
		colour=mix(colour,u_cInnerGlow,(float(nStep)/float(MAX_STEPS))*u_fInnerGlow);

	if(u_fFog!=0.0)
		colour=mix(u_cBackground,colour,exp(-pow(fLen*exp(u_fFalloff),2.0))*u_fFog);

	return colour;
}

vec3 rayMarch(vec3 vRay,vec3 vDir,out vec3 vHit,out vec3 vNormal)
{
	fMinDist=10000.0;
	fOrbitTrap=0.0;

	float fMin = 0.0;
	float fMax = 0.0;

	float fDist=0.0;
	float fLen=0.0;
	float fEps=EPSILON;

	bool bHit=false;
	bool bInside=false;

	int nStep=0;

	if(rayIntersec(vRay,vDir,fMin,fMax))
	{
		float fFactor=u_fScale*u_fDetail;

		bInside=true;

		fLen=fMin;

		if(u_fDither!=0.0)
			fLen+=u_fDither*rand(vDir.xy);

		for(int n=0;n<MAX_STEPS;n++)
		{
			nStep=n;

			vHit=u_vCamera+(vDir*fLen);
			fDist=rayDistance(vHit)*u_fSmooth;

			fLen+=fDist;

			if(fLen>fMax || fLen<fMin)
				break;

			if(fDist<fEps)
			{
				bHit=true;
				break;
			}

			fEps=fLen*fFactor;
		}
	}

	vec3 vLight=u_vLight*u_mLight;
	vec3 cColour;

	if(bHit)
	{
		vNormal=rayNormal(vHit-((fNormalDetail*fEps)*vDir),fEps);

		if(u_fAmbient<0.0) cColour=vec3(0.5)+(vNormal*abs(u_fAmbient));
		else cColour=rayColour(vRay,vDir,vHit,vNormal,vLight,fLen,nStep,fEps,fDist);
	}
	else
	{
		cColour=GetBg()+raySun(vDir,vLight);

		if(u_fOuterGlow>0.0)
		{
			float fGlow=clamp((float(nStep)/float(MAX_STEPS))*u_fOuterGlow,0.0,1.0);
			cColour=mix(cColour,u_cOuterGlow,fGlow);
		}

		vHit=vec3(0.0);
	}

	if(u_bBounds && !bInside) cColour.r+=6.0;

	return cColour;
}

vec3 rayColour(const vec2 vPoint)
{
	vec3 vDir=normalize(vec3(vPoint,1.0)*u_mCamera);

	vec3 vHit = vec3(0.0);
	vec3 vNormal= vec3(0.0);

	vec3 cColour=rayMarch(u_vCamera,vDir,vHit,vNormal);

	#ifdef REFLECTION
	if(u_fReflection>0.0 && vHit!=vec3(0.0))
	{
		// march to reflection
		vec3 vReflect=normalize(vDir-2.0*dot(vNormal,vDir)*vNormal);
		cColour+=rayMarch(u_vCamera,vReflect,vHit,vNormal)*u_fReflection;
	}
	#endif

	return clamp(cColour,0.0,1.0);
}

vec3 rayAntiAlias(const vec2 vPoint)
{
	#ifdef ANTIALIAS
	vec2 v=vPoint;

	float fScale=float(u_nAlias+1);
	float fStep=u_fScale/fScale;

	vec3 cColour=vec3(0.0);

	for(int y=0;y<MAX_ALIAS;y++)
	{
		if(y>u_nAlias) break;

		for(int x=0;x<MAX_ALIAS;x++)
		{
			if(x>u_nAlias) break;

			cColour+=rayColour(v);
			v.x+=fStep;
		}
		v.y+=fStep;
	}
	return cColour/pow(fScale,2.0);
	#else  // ANTIALIAS
	return rayColour(vPoint);
	#endif // ANTIALIAS
}

mat3 rotate(float fAngle,float x,float y,float z,mat3 m)
{
	float a00=m[0].x,a01=m[0].y,a02=m[0].z,
		a10=m[1].x,a11=m[1].y,a12=m[1].z,
		a20=m[2].x,a21=m[2].y,a22=m[2].z;

	float fTheta=radians(fAngle);

	float s=sin(fTheta);
	float c=cos(fTheta);

	float t=1.0-c;

	float b00=x*x*t+c,b01=y*x*t+z*s,b02=z*x*t-y*s,
		b10=x*y*t-z*s,b11=y*y*t+c,b12=z*y*t+x*s,
		b20=x*z*t+y*s,b21=y*z*t-x*s,b22=z*z*t+c;

	return mat3(
		a00*b00+a10*b01+a20*b02,a01*b00+a11*b01+a21*b02,a02*b00+a12*b01+a22*b02,
		a00*b10+a10*b11+a20*b12,a01*b10+a11*b11+a21*b12,a02*b10+a12*b11+a22*b12,
		a00*b20+a10*b21+a20*b22,a01*b20+a11*b21+a21*b22,a02*b20+a12*b21+a22*b22);
}

void main(void)
{	
	u_fPower=3.0;
	u_vClamp=vec3(1.0,3.0,3.0);
	u_fBounds=5.0;
	u_vCamera.z-=5.2;
	u_vCamera.y+=0.3;
		
	u_mObject=rotate(time*0.13*-360.0,1.0,0.0,0.0,u_mObject);
	u_mObject=rotate(time*0.1*-360.0,0.0,1.0,0.0,u_mObject);

	float wr = gl_FragCoord.x/u_fWidth;
	float hr = gl_FragCoord.y/u_fHeight;



	// get ray point
	vec2 vPoint=vec2(
		((wr)-0.5)*u_fRatio,
		(hr)-0.5);

	if (vPoint.y  < -0.37 || vPoint.y  > 0.25) discard; 

	vec3 cColour=(u_nAlias>0)?
		rayAntiAlias(vPoint):
		rayColour(vPoint);

	gl_FragColor=vec4(cColour,1.0);
}
