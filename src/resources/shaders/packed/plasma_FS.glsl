uniform sampler2D texture; 
uniform vec2 resolution;
uniform float time; 
uniform int value; 
uniform float mode;
uniform float deform_value; 

float ang1=0.11; 
float ang2=0.05; 
float ang3=0.09; 
float ang4=0.13;

float PI=3.14159265358979323846; 
float lens=PI/deform_value; 
  
float v=1.0; 
 
vec2 deform(in vec2 p) 
{ 
    	float radius=0.2; 
	    float zoom=radius*cos(p.x*PI/lens*5.0)+radius*cos(p.y*PI/lens*10.0); 
	    return vec2(p.x*zoom-0.5,p.y*zoom-0.5); 
} 
 
void main(void) 
{ 
    //if (mode<1.0)
   // greets page
	    vec2 p=-1.0+2.0*gl_TexCoord[0].xy; 
 
	    p=deform(p);  
 
	    vec3 color; 
	    float r,b,c; 

	    float timeVal = time;
 
	    float p_a=p.x*cos(p.x*ang1+timeVal*0.0625); 
	    float p_b=p.x*sin(p.x*ang2+timeVal*0.0250); 
	    float p_c=p.y*cos(p.y*ang3+timeVal*0.0375); 
	    float p_d=p.y*sin(p.y*ang4+timeVal*0.0500); 
 
	    float a_a=p.x*cos(p_a+p_b+timeVal*(ang1-ang2))*0.875; 
	    float a_b=p.y*sin(p_b+p_c+timeVal*(ang3-ang4))*0.750; 
	    float a_c=p.x*cos(p_c+p_d+timeVal*(ang1-ang3))*1.500; 
	    float a_d=p.y*sin(p_d+p_a+timeVal*(ang2-ang4))*0.625; 
	    float a_e=(p.x*ang1*a_a+v)*cos(p.x*ang2*a_b); 
	    float a_f=1.0;//(p.y*ang3*a_c+v)*sin(p.y*ang4*a_d); 
	    float a1=0.25*cos(a_a-a_c); 
	    float a2=0.25*sin(a_b-a_d); 
	    float a3=0.25*cos(a_e); 
	    float a4=0.25*sin(a_f); 
 
	    r=a1+a2-a3+a4; 
	    b=-a1-a2+a3; 
 
	    if(value==0) 
	    { 
		    color=texture2D(texture,vec2(r,b)*2.0).xyz; 
	    }else{ 
		    color=texture2D(texture,vec2(r*cos(PI*p.x),b*cos(PI*p.y))).xyz; 
	    } 
 
	    color=color*color; 
	    c=(color.r+color.g+color.b)*2.0; 
	    color=vec3(c+r*0.1,c-(r+b)*0.5,c+b*0.9); 
 
	    gl_FragColor=vec4(color,1.0); 
} 
