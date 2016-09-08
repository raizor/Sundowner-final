// GREETS

uniform int black;
uniform sampler2D tex;

varying vec3 N;
varying vec3 v;    
varying float vpct;
varying float timev;


varying float explode;
varying float t;

#define MAX_LIGHTS 3

float alpha;

void main (void)  
{  
   alpha = 0.1;
   vec4 envOutput;
   vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
   for (int i=0;i<MAX_LIGHTS;i++)
   {
	   vec3 L = normalize(gl_LightSource[i].position.xyz - v);   
	   vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	   vec3 R = normalize(-reflect(L,N));  

	   	vec3 vR = normalize(R);
	
		vR.y *= 0.6;
		vR.y += 0.22;
		float mu = 1.0 + (0.01 * timev);
		if(vR.z>0.0)
		{
			vec2 frontUV;
			frontUV = (vR.xy / (2.0*(1.0 + vR.z))) + 0.5;
			envOutput = texture2D( tex, frontUV*mu );
		}
		else
		{
			vec2 backUV;
			backUV = (vR.xy / (2.0*(1.0 - vR.z))) + 0.5;
			envOutput = texture2D( tex, backUV*mu );
		}
	 
	   vec4 Iamb = gl_FrontLightProduct[i].ambient;    

	   vec4 Idiff = ((gl_Color*0.5) + (gl_FrontLightProduct[i].diffuse*0.5)) * max(dot(N,L), 0.0);
	   

	   Idiff = clamp(Idiff, 0.0, 1.0);     
	   
	   vec4 Ispec = gl_FrontLightProduct[i].specular 
					* pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
	   Ispec = clamp(Ispec, 0.0, 1.0); 

	   finalColor += Iamb + Idiff + Ispec;  

	   vec4 mixv = finalColor * 0.4 + envOutput * 0.6;
	   finalColor = (finalColor * (1.0-vpct)) + (mixv * (vpct));
       alpha = t/4.0;
       float boost = t > 0.99 ? (t-0.99) * 5.0 : 0.0;
       finalColor.a = alpha + boost;

   }
    gl_FragColor = black == 0 ? finalColor : vec4(0,0,0,1); 
}