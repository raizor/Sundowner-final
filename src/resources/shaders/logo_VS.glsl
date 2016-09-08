// LOGO
// pseudo instanced cube logo shitz0r

uniform float cubeSize;
uniform float height;
uniform float pct;
uniform float time;
uniform sampler3D NoiseTex;

varying vec3 N;
varying vec3 v;
varying float vpct;
varying float timev;


mat4 rotate_x(float theta)
{
    return mat4(
        vec4(1.0,         0.0,         0.0, 0.0),
        vec4(0.0,  cos(theta),  sin(theta), 0.0),
        vec4(0.0, -sin(theta),  cos(theta), 0.0),
        vec4(0.0,         0.0,         0.0, 1.0)
    );
}

mat4 rotate_y(float theta)
{
    return mat4(
        vec4(cos(theta),   0.0,  -sin(theta), 0.0),
        vec4(0.0,          1.0,  0.0,        0.0),
        vec4(sin(theta),  0.0,  cos(theta), 0.0),
        vec4(0.0,          0.0,  0.0,        1.0)
    );
}

 void main()
 {
   timev = time;
   float cubeNum = gl_Vertex.w;
   vec4 vert = vec4(gl_Vertex.xyz, 1.0);  
   vec4 tc1 = gl_MultiTexCoord0; // start pos
   vec4 tc2 = gl_MultiTexCoord1; // end pos
   
   // speed - not used
   vec4 tcCross = gl_MultiTexCoord2; 
   float speed = tcCross.w;
   tcCross.w = 1.0;
    
   float startTime = tc1.w * 0.5;
   float height = tc2.w;
   float t = clamp((time - startTime) * 0.1 * speed, 0.0, 1.0);
   
   if (time - startTime > 12.0)
   {
	t-=  time - startTime - 12.0; 
   }

   vpct = 0.0;//t > 0.8 ? (t-0.8) * 5.0 : 0.0;
   float invT = 1.0-t;
   float xs = (invT*2.00);

   float rotCubeVal = invT*(cubeNum*0.002);
   
   mat4 rx = rotate_x(rotCubeVal);
   mat4 ry = rotate_y(rotCubeVal);
   vert*=rx * ry;
   vec4 norm = vec4(gl_Normal, 1.0) * rx * ry;
   
   vec4 tc3 = mix(tc2, tc1, t);
   float amt = t < 0.5 ? t * 2.0 : 0.5 - (t-0.5);
   
   vec4 tc4 = mix(tc3, tcCross, amt);

   vert.xyz += tc3.xyz;      
   vert.xyz*=(1.0+xs);

   

   v = vec3(gl_ModelViewMatrix * vert);       
   // normal
   N = normalize(gl_NormalMatrix * norm.xyz);

   vec3 v = vec3(gl_ModelViewMatrix * vert);   

   gl_Position = gl_ProjectionMatrix * vec4(v, 1.0);
   gl_FrontColor = gl_Color;
 }