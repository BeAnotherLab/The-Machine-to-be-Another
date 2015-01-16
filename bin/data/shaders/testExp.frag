
uniform sampler2DRect tex;

varying vec2 texcoord0;

uniform float thresh;

void main()
{

    float rChannel		= texture2DRect( tex, texcoord0 ).r;
	float gChannel		= texture2DRect( tex, texcoord0 ).g;
	float bChannel		= texture2DRect( tex, texcoord0 ).b;
    
    float gr = (rChannel + gChannel + bChannel)/3.0;
    
    if(gr < thresh){
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else{
         gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    

}