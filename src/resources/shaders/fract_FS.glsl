uniform float time;
uniform float aspect;
uniform float deform_value; 
float PI=3.14159265358979323846; 
float lens=PI/deform_value; 
float x=-1.0; 
float y=-1.0; 

uniform int width;
uniform int height;
uniform int maxIterations;
uniform int numSteps;

uniform bool antialiasing;

uniform float ambientLight;
uniform float ambientOcclusion;
uniform float shadows;
uniform float epsilon;
uniform float delta;
uniform float escapeThreshold;
uniform float boundingRadius;
uniform float intersectMultZ;
uniform float colorSpread;
uniform float specularity;
uniform float specularExponent;

uniform vec4 backgroundColor;
uniform vec4 surfaceColor;

uniform vec4 mu;
uniform vec4 camera;

uniform vec3 light;
uniform vec3 light2;

uniform vec4 lightColor;
uniform vec4 lightColor2;

uniform sampler2D texture;

varying vec3 Position;

#define BOUNDING_RADIUS_2 2.5
#define ESCAPE_THRESHOLD 10.0
#define DEL 0.0001
#define EPSILON 0.001

vec2 size = vec2(float(width), float(height));
float aspectRatio = float(size.x) / float(size.y);

float c1 = cos(-camera.x);
float s1 = sin(-camera.x);
mat3 viewRotationY = mat3( c1, 0, s1, 
						   0, 1, 0, 
						 -s1, 0, c1);

float c2 = cos(-camera.y);
float s2 = sin(-camera.y);
    
mat3 viewRotationZ = mat3( c2, -s2, 0,
                              s2, c2, 0, 
                              0, 0, 1);
    
float c3 = cos(-camera.z);
float s3 = sin(-camera.z);
    
mat3 viewRotationX = mat3( 1, 0, 0,
                              0, c3, -s3, 
                              0, s3, c3);
    
    
mat3 viewRotation = viewRotationX * viewRotationY * viewRotationZ;
    
vec3 eye = vec3(0, 0, camera.w) * viewRotation;
vec3 lightSource = light * viewRotation;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec4 quatMult(vec4 q1, vec4 q2)
{
	vec4 r;

	r.x   = q1.x * q2.x - dot(q1.yzw, q2.yzw);
	r.yzw = q1.x * q2.yzw + q2.x * q1.yzw + cross(q1.yzw, q2.yzw);

	return r;
}

vec4 quatSq(vec4 q)
{
	vec4 r;

	r.x   = q.x * q.x - dot(q.yzw, q.yzw);
	r.yzw = 2.0 * q.x * q.yzw;

	return r;
}
    
vec3 normEstimate(vec3 p, vec4 c)
{
	vec3 N;
	vec4 qP = vec4(p.x, p.y, p.z, 0.0);
	float gradX, gradY, gradZ;

	vec4 gx1 = qP - vec4(delta, 0.0, 0.0, 0.0);
	vec4 gx2 = qP + vec4(delta, 0.0, 0.0, 0.0);
	vec4 gy1 = qP - vec4(0.0, delta, 0.0, 0.0);
	vec4 gy2 = qP + vec4(0.0, delta, 0.0, 0.0);
	vec4 gz1 = qP - vec4(0.0, 0.0, delta, 0.0);
	vec4 gz2 = qP + vec4(0.0, 0.0, delta, 0.0);

	for (int i = 0; i < maxIterations; i++) {
		gx1 = quatSq(gx1) + c;
		gx2 = quatSq(gx2) + c;
		gy1 = quatSq(gy1) + c;
		gy2 = quatSq(gy2) + c;
		gz1 = quatSq(gz1) + c;
		gz2 = quatSq(gz2) + c;
	}

	gradX = length(gx2) - length(gx1);
	gradY = length(gy2) - length(gy1);
	gradZ = length(gz2) - length(gz1);

	N = normalize(vec3(gradX, gradY, gradZ));

	return N;
}

void ColorMap(vec3 light, vec3 eye, vec3 pt, vec3 N, float ao, out vec4 phongCol, out vec4 envCol)
{
	vec4 envOutput;
	vec3 phongDiffuse = surfaceColor.rgb;	
	vec3 L = normalize(light - pt);  
	vec3 E = normalize(eye - pt);  	
	float  NdotL = dot(N, L);		
	vec3 R = L - 2.0 * NdotL * N;	  	
	
	vec3 vR = normalize(R);
	
	vR.y *= 0.6;
	vR.y += 0.22;
	if(vR.z>0.0)
	{
		vec2 frontUV;
		frontUV = (vR.xy / (2.0*(1.0 + vR.z))) + 0.5;
		envOutput = texture2D( texture, frontUV );
	}
	else
	{
		vec2 backUV;
		backUV = (vR.xy / (2.0*(1.0 - vR.z))) + 0.5;
		envOutput = texture2D( texture, backUV );
	}
	
	phongDiffuse += abs(R) * colorSpread;
	phongDiffuse += abs(N) * colorSpread;

        
	phongDiffuse = (phongDiffuse * max(NdotL, ambientLight) + specularity * pow(max(dot(E,R), 0.0), specularExponent)) * ao;

	phongCol = vec4(phongDiffuse, 1.0);
	envCol = envOutput;
}

vec3 intersectSphere(vec3 rO, vec3 rD)
{
	float B, C, d, t0, t1, t;
	
	B = 2.0 * dot(rO, rD);
	C = dot(rO, rO) - boundingRadius;
	d = sqrt(B*B - 4.0 * C) ;
	t0 = (-B + d) * 0.5;
	t1 = (-B - d) * 0.5;
	t = min(t0, t1);
	rO += t * rD;
	
	return rO;
}


vec3 rayDirection(vec2 p)
{
	vec3 direction = vec3( 2.0 * aspect * p.x / float(width) - aspect, 
					-2.0 * p.y / float(height) + 1.0, 
						-2.0);
	return direction * viewRotation;
}

void iterateIntersect(inout vec4 q, inout vec4 qp, vec4 c, int maxIterations)
{
	for (int i = 0; i < maxIterations; i++) {
		qp = intersectMultZ * quatMult(q, qp);
		q = quatSq(q) + c;

		if (dot(q, q) > escapeThreshold) break;
	}
}

vec2 intersectQJulia(inout vec3 rO, inout vec3 rD, vec4 c)
{
	vec2 dist = vec2(0., 0.);
	int n = 0;
        
	while (n < numSteps) {
		vec4 z = vec4(rO.x, rO.y, rO.z, 0.0);

		vec4 zp = vec4(1.0, 0.0, 0.0, 0.0);   
		
		iterateIntersect(z, zp, c, maxIterations);
		
		float normZ = length(z);
		dist.s = 0.5 * normZ * log(normZ) / length(zp); 
		
		rO += rD * dist.s; // (step)
		
		if (dist.s < epsilon || dot(rO, rO) > boundingRadius) break;
            
		n += 1;
	}
        
    dist.t = 1.0 - clamp( (float(n) / float(numSteps)) * ambientOcclusion, 0.0, 1.0 );
	return dist;
}

void renderPoint(vec2 p, out vec4 phongCol, out vec4 envCol)
{
	phongCol = backgroundColor;
	envCol = backgroundColor;
	vec3 rO = eye;
	vec3 rD = rayDirection(p);

	rD = normalize(rD);  
	rO = intersectSphere(rO, rD);
	

	if (dot(rO, rO) < boundingRadius + 0.01) {
		vec2 dist = intersectQJulia(rO, rD, mu);
		
		if (dist.s < epsilon) {
			vec3 N = normEstimate(rO, mu);
						
			ColorMap(lightSource, rD, rO, N, dist.t, phongCol, envCol);
			phongCol.a = 1.0;  
			envCol.a = 1.0;  				
		}else{
			phongCol.a = 0.0;
			envCol.a = 0.0; 
		}	
	}
}

vec3 deformRadial( in vec2 p )
{
    vec2 uv;

	vec2 cam = vec2(mod(camera.x, 360.0), mod(camera.y, 360.0));

    vec2 q = vec2( sin(1.1*cam.x+p.x),sin(1.2*cam.y+p.y) );

    float a = atan(q.y,q.x);
    float r = sqrt(dot(q,q));

    uv.x = sin(0.0+1.0*cam.x)+p.x*sqrt(r*r+1.0);
    uv.y = sin(0.6+1.1*cam.x)+p.y*sqrt(r*r+1.0);

    return texture2D(texture,uv*.5).xyz;
}	

vec4 radialBlur()
{
	vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / size.xy;
    vec2 s = p;

    vec3 total = vec3(0.0);
    vec2 d = (vec2(0.0,0.0)-p)/40.0;
    float w = 1.0;
    for( int i=0; i<40; i++ )
    {
        vec3 res = deformRadial(s);
        res = smoothstep(0.1,1.0,res*res);
        total += w*res;
        w *= .99;
        s += d;
    }
    total /= 40.0;
    float r = 1.5/(1.0+dot(p,p));
    return vec4( total*r,1.0);
}

void main()
{
	vec4 phongCol= vec4(0.0, 0.0, 0.0, 0.0);
	vec4 envCol = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 c = vec4(0.0, 0.0, 0.0, 1.0);
	vec2 p = vec2(Position) * size;
	
	if (Position.y < 0.22 || Position.y > 0.88 ) discard; 
	
	renderPoint(p, phongCol, envCol);	
			
	if (c.a <= 0.0) discard;

	gl_FragData[0] = phongCol;
	gl_FragData[1] = envCol;
}
