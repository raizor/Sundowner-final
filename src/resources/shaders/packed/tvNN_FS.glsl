uniform sampler2D texture; 
uniform int screen_w; 
uniform int screen_h; 
uniform float time; 
uniform float value1; 
uniform float value2; 
uniform float deform_value; 
uniform float distort1;
uniform float distort2;
uniform float distort3;
 
const float PI=3.14159265358979323846; 
float lens=PI/deform_value; 
 
vec2 zp(in vec2 p,in float radius) 
	{ 
	float zp=1.5-(radius*cos(p.x*PI/lens)+radius*cos(p.y*PI/lens)); 
	return vec2(p.x*zp-0.5,p.y*zp-0.5); 
	} 
 
vec3 deform(in sampler2D tex,in vec2 p) 
	{ 
	return (p.x<-1.0||p.x>0.0||p.y<-1.0||p.y>0.0)?vec3(0.0,0.0,0.0):texture2D(tex,p).xyz; 
	} 
 
float rand(in vec2 p) 
	{ 
	return fract(sin(dot(p.xy,vec2(12.9898,78.233)))*43758.5453); 
	} 
 
void main(void) 
	{ 
	vec2 p = vec2(0.0, 0.0);
	p.x = 1.0-(gl_TexCoord[0].x*2.0);
	p.y = (1.0-(gl_TexCoord[0].y*2.0))+0.1;
	vec2 q=gl_TexCoord[0].xy; 
	vec2 z =zp(p,distort1); 
	vec2 z1=zp(p,distort2); 
	vec2 z2=zp(p,distort3); 
	float g=(2.0-cos(PI/lens/2.0+z.x*PI/lens)-cos(PI/lens/2.0+z.y*PI/lens))*32.0; 
 
	float rnd1=rand(vec2(p.x+time,p.y-time)); 
	float rnd2=rand(vec2(p.x-time,p.y+time)); 
	float d=rnd1*2.0/float(screen_w); 
 
	vec3 source=deform(texture,z); 
	vec3 glass1=deform(texture,z1); 
	vec3 glass2=deform(texture,z2); 
 
	float v=value2*g; 
 
	vec3 noise; 
	noise.x=deform(texture,vec2(z.x-d-v,z.y-d)).x; 
	noise.y=deform(texture,vec2(z.x    ,z.y  )).y; 
	noise.z=deform(texture,vec2(z.x-d+v,z.y-d)).z; 
 
	//vec3 color=source+(glass1*glass1)+(glass2*0.1)+noise; 
	vec3 color=source+(glass1*glass1)+(glass2*0.1)+(noise*0.15); 
 
	color+=value1;											// flash 
#ifdef NOISE_AND_SCANLINES
	color-=(vec3(rnd1,rnd1,rnd1)-vec3(rnd2,rnd2,rnd2))*0.125;// noise 
	color*=0.75+0.25*sin(z.x*float(screen_w)*10.2);			// scanline w 
	color*=0.90+0.10*cos(z.y*float(screen_h)*10.2)*sin(0.5+z.x*float(screen_w)*10.2);	// scanline h 
#endif
	color*=q.x*(6.0-q.x*6.0)*q.y*(6.0-q.y*6.0);				// vignetting 
#ifdef NOISE_AND_SCANLINES
	color*=0.975+0.025*sin(time*77.7*cos(time*191.1));	// flicking 
#endif	
	gl_FragColor=vec4(color,1.0); 
	} 
