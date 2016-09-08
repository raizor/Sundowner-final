/* File generated with Shader Minifier 1.1.1
 * http://www.ctrl-alt-test.fr
 */
#ifndef SHADER_CODE_H_
# define SHADER_CODE_H_

const char fract_FS_glsl[] = ""
 "uniform float time,aspect,deform_value;"
 "float v=3.14159,s=v/deform_value,d=-1.,m=-1.;"
 "uniform int width,height,maxIterations,numSteps;"
 "uniform bool antialiasing;"
 "uniform float ambientLight,ambientOcclusion,shadows,epsilon,delta,escapeThreshold,boundingRadius,intersectMultZ,colorSpread,specularity,specularExponent;"
 "uniform vec4 backgroundColor,surfaceColor,mu,camera;"
 "uniform vec3 light,light2;"
 "uniform vec4 lightColor,lightColor2;"
 "uniform sampler2D texture;"
 "varying vec3 Position;\n"
 "#define BOUNDING_RADIUS_2 2.5\n"
 "#define ESCAPE_THRESHOLD 10.0\n"
 "#define DEL 0.0001\n"
 "#define EPSILON 0.001\n"
 "vec2 a=vec2(float(width),float(height));"
 "float w=float(a.x)/float(a.y),x=cos(-camera.x),c=sin(-camera.x);"
 "mat3 t=mat3(x,0,c,0,1,0,-c,0,x);"
 "float i=cos(-camera.y),y=sin(-camera.y);"
 "mat3 l=mat3(i,-y,0,y,i,0,0,0,1);"
 "float e=cos(-camera.z),o=sin(-camera.z);"
 "mat3 n=mat3(1,0,0,0,e,-o,0,o,e),r=n*t*l;"
 "vec3 f=vec3(0,0,camera.w)*r,h=light*r;"
 "float D(vec2 v)"
 "{"
   "return fract(sin(dot(v.xy,vec2(12.9898,78.233)))*43758.5);"
 "}"
 "vec4 D(vec4 v,vec4 a)"
 "{"
   "vec4 f;"
   "f.x=v.x*a.x-dot(v.yzw,a.yzw);"
   "f.yzw=v.x*a.yzw+a.x*v.yzw+cross(v.yzw,a.yzw);"
   "return f;"
 "}"
 "vec4 b(vec4 v)"
 "{"
   "vec4 f;"
   "f.x=v.x*v.x-dot(v.yzw,v.yzw);"
   "f.yzw=2.*v.x*v.yzw;"
   "return f;"
 "}"
 "vec3 b(vec3 v,vec4 a)"
 "{"
   "vec3 f;"
   "vec4 s=vec4(v.x,v.y,v.z,0.);"
   "float i,y,x;"
   "vec4 w=s-vec4(delta,0.,0.,0.),d=s+vec4(delta,0.,0.,0.),m=s-vec4(0.,delta,0.,0.),c=s+vec4(0.,delta,0.,0.),t=s-vec4(0.,0.,delta,0.),h=s+vec4(0.,0.,delta,0.);"
   "for(int r=0;r<maxIterations;r++)"
     "w=b(w)+a,d=b(d)+a,m=b(m)+a,c=b(c)+a,t=b(t)+a,h=b(h)+a;"
   "i=length(d)-length(w);"
   "y=length(c)-length(m);"
   "x=length(h)-length(t);"
   "f=normalize(vec3(i,y,x));"
   "return f;"
 "}"
 "void D(vec3 v,vec3 a,vec3 y,vec3 t,float f,out vec4 i,out vec4 w)"
 "{"
   "vec4 s;"
   "vec3 x=surfaceColor.xyz,m=normalize(v-y),d=normalize(a-y);"
   "float c=dot(t,m);"
   "vec3 h=m-2.*c*t,r=normalize(h);"
   "r.y*=.6;"
   "r.y+=.22;"
   "if(r.z>0.)"
     "{"
       "vec2 e;"
       "e=r.xy/(2.*(1.+r.z))+.5;"
       "s=texture2D(texture,e);"
     "}"
   "else"
     "{"
       "vec2 e;"
       "e=r.xy/(2.*(1.-r.z))+.5;"
       "s=texture2D(texture,e);"
     "}"
   "x+=abs(h)*colorSpread;"
   "x+=abs(t)*colorSpread;"
   "x=(x*max(c,ambientLight)+specularity*pow(max(dot(d,h),0.),specularExponent))*f;"
   "i=vec4(x,1.);"
   "w=s;"
 "}"
 "vec3 p(vec3 v,vec3 s)"
 "{"
   "float f,x,y,i,w,m;"
   "f=2.*dot(v,s);"
   "x=dot(v,v)-boundingRadius;"
   "y=sqrt(f*f-4.*x);"
   "i=(-f+y)*.5;"
   "w=(-f-y)*.5;"
   "m=min(i,w);"
   "v+=m*s;"
   "return v;"
 "}"
 "vec3 p(vec2 v)"
 "{"
   "vec3 x=vec3(2.*aspect*v.x/float(width)-aspect,-2.*v.y/float(height)+1.,-2.);"
   "return x*r;"
 "}"
 "void D(inout vec4 v,inout vec4 x,vec4 a,int f)"
 "{"
   "for(int w=0;w<f;w++)"
     "{"
       "x=intersectMultZ*D(v,x);"
       "v=b(v)+a;"
       "if(dot(v,v)>escapeThreshold)"
         "break;"
     "}"
 "}"
 "vec2 D(inout vec3 v,inout vec3 a,vec4 x)"
 "{"
   "vec2 f=vec2(0.,0.);"
   "int w=0;"
   "while(w<numSteps)"
     "{"
       "vec4 t=vec4(v.x,v.y,v.z,0.),y=vec4(1.,0.,0.,0.);"
       "D(t,y,x,maxIterations);"
       "float s=length(t);"
       "f.x=.5*s*log(s)/length(y);"
       "v+=a*f.x;"
       "if(f.x<epsilon||dot(v,v)>boundingRadius)"
         "break;"
       "w+=1;"
     "}"
   "f.y=1.-clamp(float(w)/float(numSteps)*ambientOcclusion,0.,1.);"
   "return f;"
 "}"
 "void b(vec2 v,out vec4 a,out vec4 x)"
 "{"
   "a=backgroundColor;"
   "x=backgroundColor;"
   "vec3 w=f,y=p(v);"
   "y=normalize(y);"
   "w=p(w,y);"
   "if(dot(w,w)<boundingRadius+.01)"
     "{"
       "vec2 d=D(w,y,mu);"
       "if(d.x<epsilon)"
         "{"
           "vec3 s=b(w,mu);"
           "D(h,y,w,s,d.y,a,x);"
           "a.w=1.;"
           "x.w=1.;"
         "}"
       "else"
         " a.w=0.,x.w=0.;"
     "}"
 "}"
 "vec3 E(in vec2 v)"
 "{"
   "vec2 a,d=vec2(mod(camera.x,360.),mod(camera.y,360.)),y=vec2(sin(1.1*d.x+v.x),sin(1.2*d.y+v.y));"
   "float w=atan(y.y,y.x),x=sqrt(dot(y,y));"
   "a.x=sin(d.x)+v.x*sqrt(x*x+1.);"
   "a.y=sin(.6+1.1*d.x)+v.y*sqrt(x*x+1.);"
   "return texture2D(texture,a*.5).xyz;"
 "}"
 "vec4 D()"
 "{"
   "vec2 v=-1.+2.*gl_FragCoord.xy/a.xy,x=v;"
   "vec3 f=vec3(0.);"
   "vec2 s=(vec2(0.,0.)-v)/40.;"
   "float w=1.;"
   "for(int i=0;i<40;i++)"
     "{"
       "vec3 y=E(x);"
       "y=smoothstep(.1,1.,y*y);"
       "f+=w*y;"
       "w*=.99;"
       "x+=s;"
     "}"
   "f/=40.;"
   "float y=1.5/(1.+dot(v,v));"
   "return vec4(f*y,1.);"
 "}"
 "void main()"
 "{"
   "vec4 v=vec4(0.,0.,0.,0.),s=vec4(0.,0.,0.,0.),d=vec4(0.,0.,0.,1.);"
   "vec2 w=vec2(Position)*a;"
   "if(Position.y<.22||Position.y>.88)"
     "discard;"
   "b(w,v,s);"
   "if(d.w<=0.)"
     "discard;"
   "gl_FragData[0]=v;"
   "gl_FragData[1]=s;"
 "}";

const char fract_VS_glsl[] = ""
 "varying vec3 Position;"
 "void main()"
 "{"
   "gl_Position=ftransform(),gl_TexCoord[0]=gl_MultiTexCoord0,Position=vec3(gl_MultiTexCoord0);"
 "}";

const char greets_FS_glsl[] = ""
 "uniform int black;"
 "uniform sampler2D tex;"
 "varying vec3 N,v;"
 "varying float vpct,timev,explode,t;\n"
 "#define MAX_LIGHTS 3\n"
 "float f;"
 "void main()"
 "{"
   "f=.1;"
   "vec4 w,x=vec4(0.,0.,0.,0.);"
   "for(int i=0;i<MAX_LIGHTS;i++)"
     "{"
       "vec3 y=normalize(gl_LightSource[i].position.xyz-v),s=normalize(-v),d=normalize(-reflect(y,N)),a=normalize(d);"
       "a.y*=.6;"
       "a.y+=.22;"
       "float m=1.+.01*timev;"
       "if(a.z>0.)"
         "{"
           "vec2 r;"
           "r=a.xy/(2.*(1.+a.z))+.5;"
           "w=texture2D(tex,r*m);"
         "}"
       "else"
         "{"
           "vec2 r;"
           "r=a.xy/(2.*(1.-a.z))+.5;"
           "w=texture2D(tex,r*m);"
         "}"
       "vec4 n=gl_FrontLightProduct[i].ambient,r=(gl_Color*.5+gl_FrontLightProduct[i].diffuse*.5)*max(dot(N,y),0.);"
       "r=clamp(r,0.,1.);"
       "vec4 e=gl_FrontLightProduct[i].specular*pow(max(dot(d,s),0.),.3*gl_FrontMaterial.shininess);"
       "e=clamp(e,0.,1.);"
       "x+=n+r+e;"
       "vec4 c=x*.4+w*.6;"
       "x=x*(1.-vpct)+c*vpct;"
       "f=t/4.;"
       "float l=t>.99?(t-.99)*5.:0.;"
       "x.w=f+l;"
     "}"
   "gl_FragColor=black==0?x:vec4(0,0,0,1);"
 "}";

const char greets_VS_glsl[] = ""
 "uniform float tpx,tpy,tpz,cubeSize,height,pct,time;"
 "varying vec3 N,v;"
 "varying float explode,vpct,timev,t;"
 "mat4 s(float w)"
 "{"
   "return mat4(vec4(1.,0.,0.,0.),vec4(0.,cos(w),sin(w),0.),vec4(0.,-sin(w),cos(w),0.),vec4(0.,0.,0.,1.));"
 "}"
 "mat4 f(float w)"
 "{"
   "return mat4(vec4(cos(w),0.,-sin(w),0.),vec4(0.,1.,0.,0.),vec4(sin(w),0.,cos(w),0.),vec4(0.,0.,0.,1.));"
 "}"
 "void main()"
 "{"
   "timev=time;"
   "float w=gl_Vertex.w;"
   "vec4 y=gl_MultiTexCoord0,a=gl_MultiTexCoord1,x=gl_MultiTexCoord2;"
   "float e=x.w;"
   "x.w=1.;"
   "float c=w*.005,d=a.w;"
   "t=clamp((time-c)*.4*e,0.,1.);"
   "explode=0.;"
   "if(time-c>3.)"
     "t-=time-c-3.,explode=1.;"
   "vpct=0.;"
   "float m=1.-t,r=m*2.,h=m*(8.+w*.1);"
   "mat4 n=s(h),i=f(h);"
   "vec4 l=vec4(gl_Normal,1.)*n*i,g=mix(a,y,t);"
   "float z=t<.5?t*2.:.5-(t-.5);"
   "vec4 b=mix(g,x,z),u=vec4(gl_Vertex.xyz,1.);"
   "if(u.z<=-cubeSize*6.)"
     "{"
       "u.z=-cubeSize;"
       "float k=t>.98?(t-.98)*1200.:1.;"
       "u.z=-cubeSize*k;"
     "}"
   "u*=n*i;"
   "if(explode>0.)"
     "g.y-=0.;"
   "else"
     " g.y-=0.,u.z-=(1.-t)*40.;"
   "u.xyz+=g.xyz;"
   "u.xyz*=1.+r;"
   "mat4 P=gl_ModelViewMatrix;"
   "float E=tpx;"
   "mat4 k=mat4(vec4(tpx,0.,0.,0.),vec4(0.,tpy,0.,0.),vec4(0.,0.,tpz,0.),vec4(0.,0.,0.,1.));"
   "v=vec3(P*u);"
   "N=normalize(gl_NormalMatrix*l.xyz);"
   "vec3 o=vec3(P*u);"
   "gl_Position=gl_ProjectionMatrix*vec4(o,1.);"
   "gl_FrontColor=gl_Color+tpx-tpy;"
 "}";

const char logo_FS_glsl[] = ""
 "varying vec3 N,v;"
 "varying float vpct,timev;"
 "uniform int black;"
 "uniform sampler2D tex;\n"
 "#define MAX_LIGHTS 3\n"
 "void main()"
 "{"
   "vec4 f,x=vec4(0.,0.,0.,0.);"
   "for(int w=0;w<MAX_LIGHTS;w++)"
     "{"
       "vec3 y=normalize(gl_LightSource[w].position.xyz-v),s=normalize(-v),t=normalize(-reflect(y,N)),a=normalize(t);"
       "a.y*=.6;"
       "a.y+=.22;"
       "float m=1.+.01*timev;"
       "if(a.z>0.)"
         "{"
           "vec2 r;"
           "r=a.xy/(2.*(1.+a.z))+.5;"
           "f=texture2D(tex,r*m);"
         "}"
       "else"
         "{"
           "vec2 r;"
           "r=a.xy/(2.*(1.-a.z))+.5;"
           "f=texture2D(tex,r*m);"
         "}"
       "vec4 n=gl_FrontLightProduct[w].ambient,i=(gl_Color*.5+gl_FrontLightProduct[w].diffuse*.5)*max(dot(N,y),0.);"
       "i=clamp(i,0.,1.);"
       "vec4 r=gl_FrontLightProduct[w].specular*pow(max(dot(t,s),0.),.3*gl_FrontMaterial.shininess);"
       "r=clamp(r,0.,1.);"
       "x+=n+i+r;"
     "}"
   "gl_FragColor=black==0?x:vec4(0,0,0,1);"
 "}";

const char logo_VS_glsl[] = ""
 "uniform float cubeSize,height,pct,time;"
 "uniform sampler3D NoiseTex;"
 "varying vec3 N,v;"
 "varying float vpct,timev;"
 "mat4 t(float w)"
 "{"
   "return mat4(vec4(1.,0.,0.,0.),vec4(0.,cos(w),sin(w),0.),vec4(0.,-sin(w),cos(w),0.),vec4(0.,0.,0.,1.));"
 "}"
 "mat4 n(float w)"
 "{"
   "return mat4(vec4(cos(w),0.,-sin(w),0.),vec4(0.,1.,0.,0.),vec4(sin(w),0.,cos(w),0.),vec4(0.,0.,0.,1.));"
 "}"
 "void main()"
 "{"
   "timev=time;"
   "float w=gl_Vertex.w;"
   "vec4 f=vec4(gl_Vertex.xyz,1.),a=gl_MultiTexCoord0,x=gl_MultiTexCoord1,y=gl_MultiTexCoord2;"
   "float r=y.w;"
   "y.w=1.;"
   "float s=a.w*.5,d=x.w,c=clamp((time-s)*.1*r,0.,1.);"
   "if(time-s>12.)"
     "c-=time-s-12.;"
   "vpct=0.;"
   "float m=1.-c,e=m*2.,h=m*(w*.002);"
   "mat4 g=t(h),i=n(h);"
   "f*=g*i;"
   "vec4 l=vec4(gl_Normal,1.)*g*i,b=mix(x,a,c);"
   "float z=c<.5?c*2.:.5-(c-.5);"
   "vec4 k=mix(b,y,z);"
   "f.xyz+=b.xyz;"
   "f.xyz*=1.+e;"
   "v=vec3(gl_ModelViewMatrix*f);"
   "N=normalize(gl_NormalMatrix*l.xyz);"
   "vec3 P=vec3(gl_ModelViewMatrix*f);"
   "gl_Position=gl_ProjectionMatrix*vec4(P,1.);"
   "gl_FrontColor=gl_Color;"
 "}";

const char menger_FS_glsl[] = ""
 "#define OCCLUSION\n"
 "#define SHADOW\n"
 "#define REFLECTION\n"
 "#define ANTIALIAS\n"
 "#define MAX_STEPS 255\n"
 "#define MAX_OCCLUSION 10\n"
 "#define MAX_ALIAS 3\n"
 "const float v=3.14159,s=6e-07,y=4.;"
 "uniform vec2 resolution;"
 "uniform float displace1;"
 "uniform vec2 mouse;"
 "uniform float time;"
 "uniform int boxIterations,spongeIterations;"
 "uniform float twistX,twistY,twistZ;"
 "vec3 f=vec3(.0001),x=vec3(0.),m=vec3(-125.);"
 "mat3 w=mat3(1.,0.,0.,0.,1.,0.,0.,0.,1.),r=mat3(1.,0.,0.,0.,1.,0.,0.,0.,1.),t=mat3(1.,0.,0.,0.,1.,0.,0.,0.,1.);"
 "float i=.2;"
 "vec3 a=vec3(0.),e=vec3(0.),c=vec3(0.);"
 "float u=120.,d=.3,k=.5,g=.5;"
 "int n=0;"
 "vec3 b=vec3(sin(time*.1)*.5+.5,cos(time*.2)*.5+.5,sin(time*.3)*.5+.5),h=vec3(1.,.6,.7),z=vec3(.8,.4,.1);"
 "const vec3 o=vec3(.5,.2,.2),N=vec3(1.,1.,1.),l=vec3(1.,1.,1.),P=vec3(1.,.9,.2),X=vec3(0.,0.,0.),S=vec3(0.,0.,0.);"
 "const float M=.25,p=1.,Y=.5,E=.5,A=0.,Z=0.,I=.3,C=.5,O=0.;"
 "const int R=1;"
 "const float D=3.,L=0.,T=1.5,F=4.,U=0.,W=10.,H=0.,q=0.,V=0.,Q=1.,K=2.;"
 "const bool B=false;"
 "float J=resolution.x,G=resolution.y,j=J/G,ab=1./min(J,G),ac=100000.,ad=0.;"
 "float ae(const vec2 v)"
 "{"
   "const vec2 w=vec2(12.9898,78.233);"
   "const float f=43758.5;"
   "return fract(sin(dot(v,w))*f);"
 "}"
 "vec3 af(const vec2 v)"
 "{"
   "const vec2 w=vec2(12.9898,78.233),y=vec2(4.898,7.23),x=vec2(.23,1.111);"
   "const float f=43758.5,s=23421.6,t=392820.;"
   "return vec3(fract(sin(dot(v,w))*f),fract(sin(dot(v,y))*s),fract(sin(dot(v,x))*t));"
 "}"
 "float ae(float v,float f)"
 "{"
   "return min(v,f);"
 "}"
 "float af(float v,float f)"
 "{"
   "return max(-v,f);"
 "}"
 "float ag(float v,float f)"
 "{"
   "return max(v,f);"
 "}"
 "vec3 ae()"
 "{"
   "vec2 v=-1.+2.*gl_FragCoord.xy/resolution;"
   "v.x*=resolution.x/resolution.y;"
   "float f=length(v)*1.5,w=atan(v.y,v.x)+time*1.9,x=time-10./f;"
   "vec3 a=vec3(abs(sin(x+w)))*f*.15;"
   "a+=vec3(abs(sin(-x*1113.14*(w*3.14159))))*f*.55;"
   "a.x*=.48;"
   "a.y*=.36*sin(time*.22);"
   "a.z=.11*cos(time*.22);"
   "gl_FragColor=vec4(a,1.);"
   "return a;"
 "}"
 "float ag(vec3 v)"
 "{"
   "int w=spongeIterations;"
   "vec3 f=v*.5+.5,a=abs(f-.5)-.5;"
   "float x=c.x,t=max(a.x,max(a.y,a.z));"
   "if(t<ac)"
     "ac=t;"
   "for(int i=1;i<6;i++)"
     "{"
       "if(i>w)"
         "break;"
       "x*=c.y;"
       "a=.5-abs(mod(f*x,c.z)-1.5)+e;"
       "t=max(t,min(max(a.x,a.z),min(max(a.x,a.y),max(a.y,a.z)))/x);"
       "if(t<ac)"
         "ac=t;"
       "if(i==R)"
         "ad=dot(a,a);"
     "}"
   "return t*2.;"
 "}"
 "vec3 ah(vec3 v,float f)"
 "{"
   "float w=cos(v.y),y=sin(v.y);"
   "mat2 a=mat2(w,-y,y,w);"
   "vec3 t=vec3(a*v.xy,v.z);"
   "return t;"
 "}"
 "float ai(float v,float f)"
 "{"
   "return v+f;"
 "}"
 "vec3 aj(vec3 v,vec2 a)"
 "{"
   "float f,y;"
   "mat2 w;"
   "vec3 i;"
   "f=cos(a.y*v.y);"
   "y=sin(a.y*v.y);"
   "w=mat2(f,-y,y,f);"
   "i=vec3(w*v.xz,v.y);"
   "f=cos(a.x*i.x);"
   "y=sin(a.x*i.x);"
   "w=mat2(f,-y,y,f);"
   "i=vec3(w*v.yz,v.x);"
   "return i;"
 "}"
 "float ak(vec3 v,float w)"
 "{"
   "return sin(w*v.x)*sin(w*v.y)*sin(w*v.z);"
 "}"
 "float ae(vec3 v,vec3 f,float w)"
 "{"
   "return length(max(abs(v)-f,0.))-w;"
 "}"
 "float al(vec3 v,float f)"
 "{"
   "vec3 a=abs(v)-vec3(f,f,f);"
   "a=max(a,0.);"
   "return max(max(a.x,a.y),a.z);"
 "}"
 "float am(vec3 v,float f)"
 "{"
   "return length(v)-f;"
 "}"
 "float ah(vec3 v)"
 "{"
   "float f=0.;"
   "v*=r;"
   "f=ag(aj(v,vec2(-twistX,-twistY)));"
   "float w=al(aj(v,vec2(twistX,twistY)),.8-time*.003);"
   "v=aj(v,vec2(-twistX,-twistY));"
   "float a=am(v-vec3(-1.1,1.1,1.1),time*.005);"
   "f=ae(f,w);"
   "f=af(a,f);"
   "f=ai(f,ak(v,displace1));"
   "return f;"
 "}"
 "bool ae(vec3 v,vec3 f,out float w,out float i)"
 "{"
   "float a=dot(v,f),y=dot(v,v)-u,t=a*a-y;"
   "if(t<=0.)"
     "return false;"
   "t=sqrt(t);"
   "a=-a;"
   "w=max(0.,a-t);"
   "i=a+t;"
   "return true;"
 "}"
 "vec3 an(vec3 v,float w)"
 "{"
   "vec2 a=vec2(w,0.);"
   "return normalize(vec3(ah(v+a.xyy)-ah(v-a.xyy),ah(v+a.yxy)-ah(v-a.yxy),ah(v+a.yyx)-ah(v-a.yyx)));"
 "}"
 "float af(vec3 v,vec3 f,float w)"
 "{"
   "float a=1.;"
   "\n#ifdef OCCLUSION\n"
   "float i=w*K,y=1./D;"
   "for(int t=0;t<MAX_OCCLUSION;t++)"
     "{"
       "if(t>=int(D))"
         "break;"
       "float x=ah(v+f*i);"
       "a*=1.-max(0.,(i-x)*y/i);"
       "i+=w*L;"
     "}"
   "\n#endif\n"
   "return 1.-a;"
 "}"
 "float ae(vec3 v,vec3 f,vec3 a,float w,float m)"
 "{"
   "float t=1.;"
   "\n#ifdef SHADOW\n"
   "float i=m*K;"
   "for(int r=0;r<MAX_STEPS;r++)"
     "{"
       "if(i>=w)"
         "break;"
       "float s=ah(v+a*i);"
       "if(s<m)"
         "return 1.;"
       "if(W!=0.)"
         "t=min(t,W*(s/i));"
       "i+=s;"
     "}"
   "\n#endif\n"
   "return 1.-t;"
 "}"
 "vec3 ao(const vec3 v,const vec3 f)"
 "{"
   "if(I>0.)"
     "return P*(1.-clamp(abs(acos(dot(v,f)/length(f)))/I,0.,1.));"
   "return vec3(0.);"
 "}"
 "vec3 ai(const vec3 v)"
 "{"
   "if(O>0.)"
     "return mix(vec3(0.),S,clamp(abs(v.y)*O,0.,1.));"
   "return S;"
 "}"
 "vec3 ae(vec3 v,vec3 f,vec3 w,vec3 a,vec3 t,float x,int y,float m,float s)"
 "{"
   "w-=f*(m*K);"
   "if(p<0.)"
     "return vec3(.5)+normalize(w)*abs(p);"
   "vec3 i=h*p;"
   "if(R>0)"
     "i=mix(i,mix(z,o,ac*Y),ad*E);"
   "i+=b*M;"
   "vec3 d=t-w,r=normalize(d);"
   "float e=length(d),c=dot(a,r),n=0.;"
   "if(C!=0.)"
     "{"
       "if(c>0.)"
         "{"
           "n=ae(w,f,r,e,m);"
           "if(W==0.&&n>0.)"
             "n=1.;"
         "}"
       "else"
         " n=1.;"
     "}"
   "if(F!=0.&&T!=0.&&c>0.)"
     "i+=vec3(pow(max(c,0.),F)*T)*(1.-n);"
   "if(H!=0.&&c<=0.)"
     "i*=abs(c)*H;"
   "if(n!=0.)"
     "i=mix(i,X,n*C);"
   "if(D!=0.)"
     "{"
       "float g=L==0.?float(y)/float(MAX_STEPS)*D:af(w,a,L);"
       "i=mix(i,vec3(0.),clamp(g,0.,1.));"
     "}"
   "if(A!=0.)"
     "i=mix(i,N,float(y)/float(MAX_STEPS)*A);"
   "if(q!=0.)"
     "i=mix(S,i,exp(-pow(x*exp(V),2.))*q);"
   "return i;"
 "}"
 "vec3 af(vec3 v,vec3 a,out vec3 w,out vec3 y)"
 "{"
   "ac=10000.;"
   "ad=0.;"
   "float x=0.,r=0.,i=0.,c=0.,e=s;"
   "bool h=false,u=false;"
   "int z=0;"
   "if(ae(v,a,x,r))"
     "{"
       "float n=ab*d;"
       "u=true;"
       "c=x;"
       "if(g!=0.)"
         "c+=g*ae(a.xy);"
       "for(int b=0;b<MAX_STEPS;b++)"
         "{"
           "z=b;"
           "w=f+a*c;"
           "i=ah(w)*k;"
           "c+=i;"
           "if(c>r||c<x)"
             "break;"
           "if(i<e)"
             "{"
               "h=true;"
               "break;"
             "}"
           "e=c*n;"
         "}"
     "}"
   "vec3 n=m*t,b;"
   "if(h)"
     "{"
       "y=an(w-Q*e*a,e);"
       "if(M<0.)"
         "b=vec3(.5)+y*abs(M);"
       "else"
         " b=aj(v,a,w,y,n,c,z,e,i);"
     "}"
   "else"
     "{"
       "b=ae()+ao(a,n);"
       "if(Z>0.)"
         "{"
           "float o=clamp(float(z)/float(MAX_STEPS)*Z,0.,1.);"
           "b=mix(b,l,o);"
         "}"
       "w=vec3(0.);"
     "}"
   "if(B&&!u)"
     "b.x+=6.;"
   "return b;"
 "}"
 "vec3 aj(const vec2 v)"
 "{"
   "vec3 a=normalize(vec3(v,1.)*w),y=vec3(0.),t=vec3(0.),i=af(f,a,y,t);"
   "\n#ifdef REFLECTION\n"
   "if(U>0.&&y!=vec3(0.))"
     "{"
       "vec3 x=normalize(a-2.*dot(t,a)*t);"
       "i+=af(f,x,y,t)*U;"
     "}"
   "\n#endif\n"
   "return clamp(i,0.,1.);"
 "}"
 "vec3 ak(const vec2 v)"
 "{"
   "\n#ifdef ANTIALIAS\n"
   "vec2 f=v;"
   "float w=float(n+1),a=ab/w;"
   "vec3 i=vec3(0.);"
   "for(int t=0;t<MAX_ALIAS;t++)"
     "{"
       "if(t>n)"
         "break;"
       "for(int r=0;r<MAX_ALIAS;r++)"
         "{"
           "if(r>n)"
             "break;"
           "i+=aj(f);"
           "f.x+=a;"
         "}"
       "f.y+=a;"
     "}"
   "return i/pow(w,2.);"
   "\n#else\n"
   "return aj(v);"
   "\n#endif\n"
 "}"
 "mat3 af(float v,float a,float f,float t,mat3 w)"
 "{"
   "float x=w[0].x,y=w[0].y,r=w[0].z,e=w[1].x,i=w[1].y,c=w[1].z,m=w[2].x,s=w[2].y,d=w[2].z,h=radians(v),n=sin(h),z=cos(h),o=1.-z,l=a*a*o+z,g=f*a*o+t*n,N=t*a*o-f*n,b=a*f*o-t*n,u=f*f*o+z,k=t*f*o+a*n,p=a*t*o+f*n,P=f*t*o-a*n,X=t*t*o+z;"
   "return mat3(x*l+e*g+m*N,y*l+i*g+s*N,r*l+c*g+d*N,x*b+e*u+m*k,y*b+i*u+s*k,r*b+c*u+d*k,x*p+e*P+m*X,y*p+i*P+s*X,r*p+c*P+d*X);"
 "}"
 "void main()"
 "{"
   "i=3.;"
   "c=vec3(1.,3.,3.);"
   "u=5.;"
   "f.z-=5.2;"
   "f.y+=.3;"
   "r=af(time*.13*-360.,1.,0.,0.,r);"
   "r=af(time*.1*-360.,0.,1.,0.,r);"
   "float v=gl_FragCoord.x/J,y=gl_FragCoord.y/G;"
   "vec2 a=vec2((v-.5)*j,y-.5);"
   "if(a.y<-.37||a.y>.25)"
     "discard;"
   "vec3 w=n>0?ak(a):aj(a);"
   "gl_FragColor=vec4(w,1.);"
 "}";

const char menger_VS_glsl[] = ""
 "uniform vec4 fpar[4];"
 "void main()"
 "{"
   "gl_Position=gl_Vertex;"
   "vec3 v=normalize(fpar[1].xyz-fpar[0].xyz),a=normalize(cross(v,vec3(0.,1.,0.))),y=cross(a,v),f=vec3(gl_Vertex.x*1.333,gl_Vertex.y,.75);"
   "gl_TexCoord[0].xyz=mat3(a,y,v)*f;"
   "gl_TexCoord[1]=vec4(.5)+gl_Vertex*.5;"
 "}";

const char mixer_FS_glsl[] = ""
 "uniform vec2 sizeBg,sizeFract;"
 "uniform float time,aspect,value;"
 "uniform sampler2D textureFractalPhong,textureFractalEnv,textureTunnel;"
 "void main()"
 "{"
   "vec2 v=gl_TexCoord[0].xy,a=v;"
   "if(v.y<.22||v.y>.88)"
     "discard;"
   "vec4 f=vec4(texture2D(textureFractalPhong,v)),y=vec4(texture2D(textureFractalEnv,v)),c;"
   "if(f.w>0.)"
     "c=f*.5+y*.5*(f.x+f.y+f.z);"
   "else"
     " c=vec4(texture2D(textureTunnel,v));"
   "c*=a.x*(6.-a.x*6.)*a.y*(6.-a.y*6.);"
   "gl_FragColor=c;"
   "gl_FragDepth=.2;"
 "}";

const char phong_FS_glsl[] = ""
 "varying vec3 N,v;\n"
 "#define MAX_LIGHTS 3\n"
 "void main()"
 "{"
   "vec4 f=vec4(0.,0.,0.,0.);"
   "for(int i=0;i<MAX_LIGHTS;i++)"
     "{"
       "vec3 w=normalize(gl_LightSource[i].position.xyz-v),y=normalize(-v),a=normalize(-reflect(w,N));"
       "vec4 m=gl_FrontLightProduct[i].ambient,t=gl_FrontLightProduct[i].diffuse*max(dot(N,w),0.);"
       "t=clamp(t,0.,1.);"
       "vec4 r=gl_FrontLightProduct[i].specular*pow(max(dot(a,y),0.),.3*gl_FrontMaterial.shininess);"
       "r=clamp(r,0.,1.);"
       "f+=m+t+r;"
     "}"
   "gl_FragColor=gl_FrontLightModelProduct.sceneColor+f;"
 "}";

const char phong_VS_glsl[] = ""
 "varying vec3 N,v;"
 "float f=6.28319,y=50.,a=40.;"
 "void main()"
 "{"
   "float f=0.,y=0.,a=1.;"
   "vec4 w=gl_Vertex;"
   "float i=gl_Vertex.w;"
   "w.w=1.;"
   "float t=.2;"
   "v=vec3(gl_ModelViewMatrix*w);"
   "N=normalize(gl_NormalMatrix*gl_Normal);"
   "gl_Position=gl_ModelViewProjectionMatrix*w;"
 "}";

const char plasma_FS_glsl[] = ""
 "uniform sampler2D texture;"
 "uniform vec2 resolution;"
 "uniform float time;"
 "uniform int value;"
 "uniform float mode,deform_value;"
 "float v=.11,w=.05,a=.09,y=.13,t=3.14159,x=t/deform_value,f=1.;"
 "vec2 n(in vec2 v)"
 "{"
   "float a=.2,y=a*cos(v.x*t/x*5.)+a*cos(v.y*t/x*10.);"
   "return vec2(v.x*y-.5,v.y*y-.5);"
 "}"
 "void main()"
 "{"
   "vec2 i=-1.+2.*gl_TexCoord[0].xy;"
   "i=n(i);"
   "vec3 c;"
   "float x,r,e,m=time,d=i.x*cos(i.x*v+m*.0625),s=i.x*sin(i.x*w+m*.025),g=i.y*cos(i.y*a+m*.0375),h=i.y*sin(i.y*y+m*.05),o=i.x*cos(d+s+m*(v-w))*.875,z=i.y*sin(s+g+m*(a-y))*.75,u=i.x*cos(g+h+m*(v-a))*1.5,l=i.y*sin(h+d+m*(w-y))*.625,N=(i.x*v*o+f)*cos(i.x*w*z),b=1.,p=.25*cos(o-u),P=.25*sin(z-l),k=.25*cos(N),S=.25*sin(b);"
   "x=p+P-k+S;"
   "r=-p-P+k;"
   "if(value==0)"
     "c=texture2D(texture,vec2(x,r)*2.).xyz;"
   "else"
     " c=texture2D(texture,vec2(x*cos(t*i.x),r*cos(t*i.y))).xyz;"
   "c=c*c;"
   "e=(c.x+c.y+c.z)*2.;"
   "c=vec3(e+x*.1,e-(x+r)*.5,e+r*.9);"
   "gl_FragColor=vec4(c,1.);"
 "}";

const char plasmaEffect_FS_glsl[] = ""
 "uniform sampler2D texture;"
 "varying vec3 Position;"
 "vec3 f(in sampler2D v,in vec2 a)"
 "{"
   "float f=.25,y=f*cos(a.x*PI/lens)+f*cos(a.y*PI/lens);"
   "vec2 w=vec2(a.x*y-.5,a.y*y-.5);"
   "return texture2D(v,w).xyz;"
 "}"
 "vec4 f()"
 "{"
   "vec2 v=-1.+2.*gl_FragCoord.xy/size.xy,f;"
   "float a=sqrt(dot(v,v)),y=atan(v.y,v.x)+.5*sin(1.5*a-2.16*time),i=.5+.5*cos(3.*y);"
   "i=smoothstep(0.,1.,i);"
   "i=smoothstep(0.,1.,i);"
   "i=smoothstep(0.,1.,i);"
   "i=smoothstep(0.,1.,i);"
   "f.x=time+1./(a+.2*i);"
   "f.y=3.*y/3.1416;"
   "float w=(.5+.5*i)*a*a;"
   "vec3 t=texture2D(texture,f).xyz;"
   "float x=.5+.5*cos(7.*y);"
   "x=smoothstep(0.,.4,x)-smoothstep(.4,.7,x);"
   "x=1.-.5*x*a;"
   "return vec4(t*w*x,1.);"
 "}"
 "void main()"
 "{"
   "c=f();"
 "}";

const char plasmaEffect_VS_glsl[] = ""
 "varying vec3 Position;"
 "void main()"
 "{"
   "gl_Position=ftransform(),gl_TexCoord[0]=gl_MultiTexCoord0,Position=vec3(gl_MultiTexCoord0);"
 "}";

const char rtunnel_FS_glsl[] = ""
 "uniform vec2 size;"
 "uniform float time,aspect;"
 "uniform sampler2D texture,texture1f,texture2f;"
 "uniform float mixer;"
 "void main()"
 "{"
   "vec2 v=-1.+2.*gl_FragCoord.xy/size.xy,f;"
   "if(gl_TexCoord[0].xy.y<.22||v.y>.88)"
     "discard;"
   "float a=sqrt(dot(v,v))*(1.+mixer*.22),y=atan(v.y,v.x*aspect)+.5*sin(1.5*a-2.16*time),i=.5+.5*cos(3.*y);"
   "i=smoothstep(0.,1.,i);"
   "f.x=time+1./(a+.2*i);"
   "f.y=2.*y/3.1416;"
   "float w=(.5+.5*i)*a*a;"
   "vec3 t=texture2D(texture,f).xyz;"
   "vec4 x=texture2D(texture1f,f*8.).xyzw,r=texture2D(texture2f,f*8.).xyzw;"
   "float m=.5+.5*cos(7.*y);"
   "m=smoothstep(0.,.4,m)-smoothstep(.4,.7,m);"
   "m=1.-.5*m*a;"
   "gl_FragColor=vec4(t*w*m,1.);"
   "gl_FragDepth=.1;"
 "}";

const char tv_FS_glsl[] = ""
 "#define NOISE_AND_SCANLINES\n"
 "uniform sampler2D texture;"
 "uniform int screen_w,screen_h;"
 "uniform float time,value1,value2,deform_value,distort1,distort2,distort3;"
 "const float v=3.14159;"
 "float f=v/deform_value;"
 "vec2 t(in vec2 a,in float w)"
 "{"
   "float i=1.5-(w*cos(a.x*v/f)+w*cos(a.y*v/f));"
   "return vec2(a.x*i-.5,a.y*i-.5);"
 "}"
 "vec3 s(in sampler2D v,in vec2 f)"
 "{"
   "return f.x<-1.||f.x>0.||f.y<-1.||f.y>0.?vec3(0.,0.,0.):texture2D(v,f).xyz;"
 "}"
 "float s(in vec2 v)"
 "{"
   "return fract(sin(dot(v.xy,vec2(12.9898,78.233)))*43758.5);"
 "}"
 "void main()"
 "{"
   "vec2 a=vec2(0.,0.);"
   "a.x=1.-gl_TexCoord[0].x*2.;"
   "a.y=1.-gl_TexCoord[0].y*2.+.1;"
   "vec2 w=gl_TexCoord[0].xy,y=t(a,distort1),x=t(a,distort2),i=t(a,distort3);"
   "float r=(2.-cos(v/f/2.+y.x*v/f)-cos(v/f/2.+y.y*v/f))*32.,m=s(vec2(a.x+time,a.y-time)),d=s(vec2(a.x-time,a.y+time)),c=m*2./float(screen_w);"
   "vec3 n=s(texture,y),e=s(texture,x),h=s(texture,i);"
   "float z=value2*r;"
   "vec3 u;"
   "u.x=s(texture,vec2(y.x-c-z,y.y-c)).x;"
   "u.y=s(texture,vec2(y.x,y.y)).y;"
   "u.z=s(texture,vec2(y.x-c+z,y.y-c)).z;"
   "vec3 g=n+e*e+h*.1+u*.15;"
   "g+=value1;"
   "\n#ifdef NOISE_AND_SCANLINES\n"
   "g-=(vec3(m,m,m)-vec3(d,d,d))*.125;"
   "g*=.75+.25*sin(y.x*float(screen_w)*10.2);"
   "g*=.9+.1*cos(y.y*float(screen_h)*10.2)*sin(.5+y.x*float(screen_w)*10.2);"
   "\n#endif\n"
   "g*=w.x*(6.-w.x*6.)*w.y*(6.-w.y*6.);"
   "\n#ifdef NOISE_AND_SCANLINES\n"
   "g*=.975+.025*sin(time*77.7*cos(time*191.1));"
   "\n#endif\n"
   "gl_FragColor=vec4(g,1.);"
 "}";

const char tv_VS_glsl[] = ""
 "varying vec3 Position;"
 "void main()"
 "{"
   "gl_Position=ftransform(),gl_TexCoord[0]=gl_MultiTexCoord0,Position=vec3(gl_MultiTexCoord0);"
 "}";

const char tvNN_FS_glsl[] = ""
 "uniform sampler2D texture;"
 "uniform int screen_w,screen_h;"
 "uniform float time,value1,value2,deform_value,distort1,distort2,distort3;"
 "const float v=3.14159;"
 "float f=v/deform_value;"
 "vec2 t(in vec2 a,in float w)"
 "{"
   "float i=1.5-(w*cos(a.x*v/f)+w*cos(a.y*v/f));"
   "return vec2(a.x*i-.5,a.y*i-.5);"
 "}"
 "vec3 s(in sampler2D v,in vec2 f)"
 "{"
   "return f.x<-1.||f.x>0.||f.y<-1.||f.y>0.?vec3(0.,0.,0.):texture2D(v,f).xyz;"
 "}"
 "float s(in vec2 v)"
 "{"
   "return fract(sin(dot(v.xy,vec2(12.9898,78.233)))*43758.5);"
 "}"
 "void main()"
 "{"
   "vec2 a=vec2(0.,0.);"
   "a.x=1.-gl_TexCoord[0].x*2.;"
   "a.y=1.-gl_TexCoord[0].y*2.+.1;"
   "vec2 w=gl_TexCoord[0].xy,y=t(a,distort1),x=t(a,distort2),i=t(a,distort3);"
   "float r=(2.-cos(v/f/2.+y.x*v/f)-cos(v/f/2.+y.y*v/f))*32.,m=s(vec2(a.x+time,a.y-time)),d=s(vec2(a.x-time,a.y+time)),c=m*2./float(screen_w);"
   "vec3 n=s(texture,y),e=s(texture,x),h=s(texture,i);"
   "float z=value2*r;"
   "vec3 u;"
   "u.x=s(texture,vec2(y.x-c-z,y.y-c)).x;"
   "u.y=s(texture,vec2(y.x,y.y)).y;"
   "u.z=s(texture,vec2(y.x-c+z,y.y-c)).z;"
   "vec3 g=n+e*e+h*.1+u*.15;"
   "g+=value1;"
   "\n#ifdef NOISE_AND_SCANLINES\n"
   "g-=(vec3(m,m,m)-vec3(d,d,d))*.125;"
   "g*=.75+.25*sin(y.x*float(screen_w)*10.2);"
   "g*=.9+.1*cos(y.y*float(screen_h)*10.2)*sin(.5+y.x*float(screen_w)*10.2);"
   "\n#endif\n"
   "g*=w.x*(6.-w.x*6.)*w.y*(6.-w.y*6.);"
   "\n#ifdef NOISE_AND_SCANLINES\n"
   "g*=.975+.025*sin(time*77.7*cos(time*191.1));"
   "\n#endif\n"
   "gl_FragColor=vec4(g,1.);"
 "}";

const char water_FS_glsl[] = ""
 "uniform float time;"
 "uniform vec2 resolution;"
 "uniform sampler2D tex0;"
 "void main()"
 "{"
   "vec2 v=resolution.xy/2.,a=gl_FragCoord.xy;"
   "a.x-=.5*v.x*sin(time/2.)+.3*v.x*cos(time)+v.x;"
   "a.y-=.4*v.y*sin(time/5.)+.3*v.y*cos(time)+v.y;"
   "float f=length(a);"
   "vec2 w=gl_FragCoord.xy/resolution.xy+a/f*sin(f/30.-time*10.)/25.;"
   "vec3 t=texture2D(tex0,w).xyz*50./f;"
   "gl_FragColor=vec4(t,1.);"
 "}";

const char water_VS_glsl[] = ""
 "varying vec3 Position;"
 "void main()"
 "{"
   "gl_Position=ftransform(),gl_TexCoord[0]=gl_MultiTexCoord0,Position=vec3(gl_MultiTexCoord0);"
 "}";

#endif // SHADER_CODE_H_
