varying vec3 Position;

void main() 
{
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
	Position = vec3(gl_MultiTexCoord0);
}
