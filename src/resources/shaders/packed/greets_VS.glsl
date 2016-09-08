// GREETS
// pseudo instanced cube logo shitz0r

uniform float tpx;
uniform float tpy;
uniform float tpz;
uniform float cubeSize;
uniform float height;
uniform float pct;
uniform float time;

varying vec3 N;
varying vec3 v;
varying float explode;
varying float vpct;
varying float timev;
varying float t;


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
   vec4 tc1 = gl_MultiTexCoord0; // start pos
   vec4 tc2 = gl_MultiTexCoord1; // end pos
   
   // speed - not used
   vec4 tcCross = gl_MultiTexCoord2; 
   float speed = tcCross.w;
   tcCross.w = 1.0;
    
   float startTime = cubeNum* 0.005;
   float height = tc2.w;
   t = clamp((time - startTime) * 0.4 * speed, 0.0, 1.0);
   
   explode = 0.0;

   if (time - startTime > 3.0)
   {
       t-=  time - startTime - 3.0; 
       explode = 1.0;
       //gl_ModelViewMatrix *= 2.0;
   }

   
   //vert.z = 0.0;

   vpct = 0.0;//t > 0.8 ? (t-0.8) * 5.0 : 0.0;
   float invT = 1.0-t;
   float xs = (invT*2.00);

   float rotCubeVal = invT*(8.0+(cubeNum*0.1));
   
   mat4 rx = rotate_x(rotCubeVal);
   mat4 ry = rotate_y(rotCubeVal);
   
   vec4 norm = vec4(gl_Normal, 1.0) * rx * ry;
   
   vec4 tc3 = mix(tc2, tc1, t);

   float amt = t < 0.5 ? t * 2.0 : 0.5 - (t-0.5);
   
   vec4 tc4 = mix(tc3, tcCross, amt);

   vec4 vert = vec4(gl_Vertex.xyz, 1.0);    
   
   if (vert.z <= -cubeSize*6.0)
   {
        vert.z = -(cubeSize*1.0);
        float boost = t > 0.98 ? (t-0.98) * 1200.0 : 1.0;
        vert.z = -cubeSize*boost;
   }

   vert*=rx * ry;

   if (explode > 0.0)
   {
    tc3.y -= (1.0-t)*0.0;
    //vert.z += (1.0-t)*40.0;
   }else{
    tc3.y -= (1.0-t)*0.0;
    vert.z -= (1.0-t)*40.0;
   }

   vert.xyz += tc3.xyz;      
   vert.xyz*=(1.0+xs);

   mat4 mvm = gl_ModelViewMatrix;
   //mat4 tm = vec4(0.0, 0.0, 0.0, 1.0);

   float ccc = tpx;

   mat4 tm =  mat4(
        vec4(tpx,   0.0,    0.0,    0.0),
        vec4(0.0,   tpy,   0.0,    0.0),
        vec4(0.0,   0.0,    tpz,    0.0),
        vec4(0.0,   0.0,    0.0,    1.0)
    );
      
   v = vec3(mvm * vert);       
   // normal
   N = normalize(gl_NormalMatrix * norm.xyz);

   /*
   vert.x += (time - startTime) * 0.1;
   vert.y += (time - startTime) * 0.05;
   vert.z += (time - startTime) * 0.2;*/

  /* vert.x *= tpx;
   vert.y *= tpy;
   vert.z *= tpz;*/

   vec3 v = vec3(mvm * vert);   

   //gl_Position = gl_ProjectionMatrix * vec4(v, 1.0) * vec4(translatePos, 1.0);
   gl_Position = gl_ProjectionMatrix * vec4(v, 1.0);
   gl_FrontColor = gl_Color + tpx - tpy;
 }