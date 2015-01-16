
uniform sampler2DRect tex;

varying vec2 texcoord0;

uniform float brightness;

void main()
{
    

    float rChannel		= texture2DRect( tex, texcoord0 ).r * brightness;
	float gChannel		= texture2DRect( tex, texcoord0 ).g* brightness;
	float bChannel		= texture2DRect( tex, texcoord0 ).b* brightness;
    

    gl_FragColor = vec4(rChannel,gChannel,bChannel,1.0);

}