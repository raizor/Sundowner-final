uniform sampler2D texture;
varying vec3 Position;

// background stuff
vec3 deform(in sampler2D tex,in vec2 p) 
{ 	
	float radius=0.25; 
	float zoom=radius*cos(p.x*PI/lens)+radius*cos(p.y*PI/lens); 
	vec2 uv = vec2(p.x*zoom-0.5, p.y*zoom-0.5); 
	return texture2D(tex,uv).xyz; 
} 


vec4 bg() // relief tunnel
{
    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / size.xy; 
    vec2 uv;

    float r = sqrt( dot(p,p) );
    float a = atan(p.y,p.x) + 0.5*sin(1.5*r-2.16*time);

    float s = 0.5 + 0.5*cos(3.0*a);
    s = smoothstep(0.0,1.0,s);
    s = smoothstep(0.0,1.0,s);
    s = smoothstep(0.0,1.0,s);
    s = smoothstep(0.0,1.0,s);

    uv.x = time + 1.0/( r + .2*s);
    uv.y = 3.0*a/3.1416;

    float w = (0.5 + 0.5*s)*r*r;

    vec3 col = texture2D(texture,uv).xyz;

    float ao = 0.5 + 0.5*cos(7.0*a);
    ao = smoothstep(0.0,0.4,ao)-smoothstep(0.4,0.7,ao);
    ao = 1.0-0.5*ao*r;

    return vec4(col*w*ao,1.0);
}

// The main loop
void main()
{
	c = bg();
}