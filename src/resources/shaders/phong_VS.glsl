varying vec3 N;
varying vec3 v;
float twoPo = 2.0 * 3.1415926;
float num_segments = 50.0;
float levels = 40.0;

void main(void)  
{
	float cx = 0.0;
	float cy = 0.0;
	float r = 1.0;
	vec4 vv = gl_Vertex;
	float cubeNum = gl_Vertex.w;
	vv.w = 1.0;

	float zdepth = 0.2;
	//vv.z -= zdepth * cubeNum;

	/*
	// translate z
	vv.z-=cubeNum*0.1;
	
	float theta = twoPo * cubeNum / num_segments;
	float theta2 = twoPo * (cubeNum+1.0) / num_segments; 
	
	float cost = cos(theta);
	float sint = cos(theta);

	float cost2 = cos(theta2);
	float sint2 = cos(theta2);

	// x pos
	if (vv.x > 0.0)
	{
		vv.x = r * cost; // left
	}else{
		vv.x = r * cost2; // right
	}

	if (vv.y > 0.0) // outer
	{
		vv.x *= 1.1;
	}

	// y pos
	if (vv.y > 0.0)
	{
		vv.y = r * sint; // outer
	}else{
		vv.y = r * sint2; // inner
	}

	if (vv.y > 0.0) // outer
	{
		vv.y *= 1.1;
	}


	  
  	float outerX1 = r * 1.1 * cos(theta);
	float outerY1 = r * 1.1 * sin(theta);
		
	float outerX2 = r * 1.1 * cos(theta2);
	float outerY2 = r * 1.1 * sin(theta2);
	
	float innerX1 = r * 1.0 * cos(theta);
	float innerY1 = r * 1.0 * sin(theta);

	float innerX2 = r * 1.0 * cos(theta2);
	float innerY2 = r * 1.0 * sin(theta2);
	*/


	
	v = vec3(gl_ModelViewMatrix * vv);       
	N = normalize(gl_NormalMatrix * gl_Normal);

	gl_Position = gl_ModelViewProjectionMatrix * vv;  
}
          