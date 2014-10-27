//uniform sampler2D tex;

uniform sampler2DRect tex;
uniform vec2 LensCenter; 
uniform vec2 ScreenCenter; 
uniform vec2 Scale; 
uniform vec2 ScaleIn; 
uniform vec4 HmdWarpParam;
uniform float thresh;

varying vec2 texcoord0;


vec2 Warp(vec2 texc)
{
    vec2 center = vec2(0.5,0.5);
    
    vec2 ff = vec2(640.0, 800.0);
    
    vec2 normTexCoord = texc / ff;
    
    vec2 xy = (normTexCoord - center * ScaleIn);
    
    xy = (texc - (vec2(320.0,400.0) / Scale ))/ff;
    
    float r = sqrt(dot(xy,xy));
    
    float r2 = r * r;
    
    float r4 = r2 * r2;
    
    float coeff = ((HmdWarpParam.x * r2) + (HmdWarpParam.y * r4));
    
    float dx = HmdWarpParam.z * (2.0 * xy.x * xy.y) + HmdWarpParam.w * (r2 + 2.0 * xy.x * xy.x);
    
    float dy = HmdWarpParam.z * (r2 + 2.0 * xy.y * xy.y ) + HmdWarpParam.w * ( 2.0 * xy.x * xy.y);
    
    if(LensCenter.y == 0.0)
    {
        xy = ( (xy + (xy * coeff) + vec2(dx,dy) ) * ff * Scale)  + vec2(320.0,400.0) + vec2(LensCenter.x,0.0);
    }
    else
    {
        xy = ( (xy + (xy * coeff) + vec2(dx,dy) ) * ff * Scale)  + vec2(320.0,400.0) - vec2(LensCenter.x,0.0);
    }
    
    return xy;
}

void main()
{
    vec2 coord = Warp(texcoord0);
    
    vec4 col = texture2DRect(tex, coord);

    float r = col.x;
    float g = col.y;
    float b = col.z;
    float br =  (r + g + b )/3.0;
    
    /*
    if(coord.x < 0.0 || coord.x > 640.0 || coord.y < 0.0 || coord.y > 640.0  || br < thresh)
    {
       gl_FragColor = vec4(0.0,0.0,0.0,1.0);
       // gl_FragColor = texture2DRect(tex, coord);
    }
    */
    
    
    
    if(coord.x < 0.0 || coord.x > 640.0 || coord.y < 0.0 || coord.y > 640.0 )
    {
        gl_FragColor = vec4(0.0,0.0,0.0,1.0);
        //gl_FragColor = texture2DRect(tex, coord);
    }
    
    
    else
    {
      //  gl_FragColor = vec4(1.0,1.0,1.0,1.0);
        gl_FragColor = texture2DRect(tex, coord);
    }
}


