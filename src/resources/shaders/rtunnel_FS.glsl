// Relief tunnel effect

uniform vec2 size;
uniform float time;
uniform float aspect;
uniform sampler2D texture;
uniform sampler2D texture1f;
uniform sampler2D texture2f;
uniform float mixer;

void main(void)
{
    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / size.xy; 
    vec2 uv;

	if (gl_TexCoord[0].xy.y < 0.22 || p.y > 0.88 ) discard; 

    float r = sqrt( dot(p,p) )*(1.0+(mixer*0.22));
    float a = atan(p.y,p.x*aspect) + 0.5*sin(1.5*r-2.16*time);

    float s = 0.5 + 0.5*cos(3.0*a);
    s = smoothstep(0.0,1.0,s);
    //s = smoothstep(0.0,1.0,s);
    //s = smoothstep(0.0,1.0,s);
    //s = smoothstep(0.0,1.0,s);

    uv.x = time + 1.0/( r + .2*s);
    uv.y = 2.0*a/3.1416;

    float w = (0.5 + 0.5*s)*r*r;

    vec3 col = texture2D(texture,uv).xyz;

	vec4 phong = texture2D(texture1f,uv*8.0).xyzw;
	vec4 env = texture2D(texture2f,uv*8.0).xyzw;
	
	float ao = 0.5 + 0.5*cos(7.0*a);
    ao = smoothstep(0.0,0.4,ao)-smoothstep(0.4,0.7,ao);
    ao = 1.0-0.5*ao*r;

	/*
	if (env.r > 0.0)
	{
		//col.xyz = env.xyz;
	}*/

	//env.a = 1.0;


	
	//if (phong.r > 0.0)
	//{
	//	vec4 nc = vec4(((env.xyz+phong.xyz+col)*w*ao),1.0);
	//	gl_FragColor = nc;
	//}else{
		gl_FragColor = vec4((col*w*ao),1.0);
	//}
	
	gl_FragDepth = 0.1;
}
