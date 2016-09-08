
uniform vec2 sizeBg;
uniform vec2 sizeFract;
uniform float time;
uniform float aspect;
uniform float value;
uniform sampler2D textureFractalPhong;
uniform sampler2D textureFractalEnv;
uniform sampler2D textureTunnel;


void main(void)
{
    vec2 p=gl_TexCoord[0].xy; 
	vec2 q = p;	
	if (p.y < 0.22 || p.y > 0.88 ) discard; 

	vec4 phongCol = vec4(texture2D(textureFractalPhong, p));
	vec4 envCol = vec4(texture2D(textureFractalEnv, p));
	vec4 color;
	if (phongCol.a > 0.0)
	{
		color = (phongCol*0.5)+(envCol*0.5*(phongCol.r+phongCol.g+phongCol.b));
	}else{
		color = vec4(texture2D(textureTunnel, p));
	}	
	color*=q.x*(6.0-q.x*6.0)*q.y*(6.0-q.y*6.0);	
	gl_FragColor = color;
	gl_FragDepth = 0.2;
}
