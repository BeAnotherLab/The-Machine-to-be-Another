// Constants
const float C_PI    = 3.1415;
const float C_2PI   = 2.0 * C_PI;
const float C_2PI_I = 1.0 / (2.0 * C_PI);
const float C_PI_2  = C_PI / 2.0;

varying float lightintensity;

uniform float radius;
uniform vec2  freqR;
uniform vec2  freqG;
uniform vec2  freqB;
uniform vec2  amp;

varying float red;
varying float green;
varying float blue;

uniform sampler2DRect image;

float pertubation(int color, vec2 freq)
{
    vec2  perturb;
    float rad;
    
    // Compute a perturbation factor for the x-direction
    rad = (gl_TexCoord[0].s + gl_TexCoord[0].t - 1.0 + radius) * freq.x;
    
    // Wrap to -2.0*PI, 2*PI
    rad = rad * C_2PI_I;
    rad = fract(rad);
    rad = rad * C_2PI;
    
    // Center in -PI, PI
    if (rad >  C_PI) rad = rad - C_2PI;
    if (rad < -C_PI) rad = rad + C_2PI;
    
    // Center in -PI/2, PI/2
    if (rad >  C_PI_2) rad =  C_PI - rad;
    if (rad < -C_PI_2) rad = -C_PI - rad;
    
    perturb.x  = (rad - (rad * rad * rad / 6.0)) * amp.x;
    
    // Now compute a perturbation factor for the y-direction
    rad = (gl_TexCoord[0].s - gl_TexCoord[0].t + radius) * freq.y;
    
    // Wrap to -2*PI, 2*PI
    rad = rad * C_2PI_I;
    rad = fract(rad);
    rad = rad * C_2PI;
    
    // Center in -PI, PI
    if (rad >  C_PI) rad = rad - C_2PI;
    if (rad < -C_PI) rad = rad + C_2PI;
    
    // Center in -PI/2, PI/2
    if (rad >  C_PI_2) rad =  C_PI - rad;
    if (rad < -C_PI_2) rad = -C_PI - rad;
    
    perturb.y  = (rad - (rad * rad * rad / 6.0)) * amp.y;
    
    if(color == 0)
    {
       return texture2DRect(image, perturb + gl_TexCoord[0].st).r;
    }
    if(color == 1)
    {
        return texture2DRect(image, perturb + gl_TexCoord[0].st).g;
    }
    if(color == 2)
    {
        return texture2DRect(image, perturb + gl_TexCoord[0].st).b;
    }

}

void main (void)
{
    /*
    red = pertubation(0,freqR);
    green = pertubation(1,freqG);
    blue = pertubation(2,freqB);
     */
    
    gl_FragColor = vec4(pertubation(0,freqR),pertubation(1,freqG),pertubation(2,freqB), 1.0);
}
