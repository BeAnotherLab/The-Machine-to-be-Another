
uniform sampler2DRect tex;
uniform vec2 LensCenter; 
uniform vec2 ScreenCenter; 
uniform vec2 Scale; 
uniform vec2 ScaleIn; 
uniform vec4 HmdWarpParam;

varying vec2 texcoord0;


vec2 HmdWarpExp(vec2 texIn)
{
    float _x = texIn.x / 640.0;
    float _y = texIn.y / 800.0;
    vec2 aye = vec2(_x, _y);
    
    vec2 center = vec2(0.5,0.5);
    
    vec2 theta = (aye - center)*ScaleIn;
    
    float  rSq= theta.x * theta.x + theta.y * theta.y;
    
    vec2 theta1 = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq +
    HmdWarpParam.z * rSq * rSq + HmdWarpParam.w * rSq * rSq * rSq);
    
    return center + Scale * theta1;
}

void main()
{
    vec2 tc = HmdWarpExp(texcoord0);
    
    float _x = tc.x * 640.0;
    float _y = tc.y * 800.0;
    vec2 tcAye = vec2(_x, _y);
    
    vec2 tc2 = HmdWarpExp(tcAye);
    
    float _x2 = tc2.x * 640.0;
    float _y2 = tc2.y * 800.0;
    
    vec2 tcAye2 = vec2(_x2, _y2);
    

    float rChannel		= texture2DRect( tex, tcAye ).r;
	float gChannel		= texture2DRect( tex, tcAye ).g;
	float bChannel		= texture2DRect( tex, tcAye ).b;
    
    float gr = (rChannel + gChannel + bChannel)/3.0;
    
    if(gr < 0.25){
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else{
         gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
	

}