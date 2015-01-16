
uniform sampler2DRect tex;

varying vec2 texcoord0;

uniform float threshR;
uniform float threshG;
uniform float threshB;

void main()
{

    float rChannel		= texture2DRect( tex, texcoord0 ).r;
	float gChannel		= texture2DRect( tex, texcoord0 ).g;
	float bChannel		= texture2DRect( tex, texcoord0 ).b;
    
    float red = 255.0;
    float green = 255.0;
    float blue = 255.0;
    
    
    if(rChannel < threshR)
    {
        red = 0.0;
    }
    else
    {
        red = 255.0;
    }
    if(gChannel < threshG)
    {
        green = 0.0;
    }
    else
    {
        green = 255.0;
    }
    if(bChannel < threshB)
    {
        blue = 0.0;
    }
    else
    {
        blue = 255.0;
    }
    
    gl_FragColor = vec4(red,green,blue,1.0);

}