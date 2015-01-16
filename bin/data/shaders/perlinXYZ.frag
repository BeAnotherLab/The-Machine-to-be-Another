
uniform sampler2DRect tex;
varying vec2 texcoord0;
uniform float changingL;
uniform float changingA;
uniform float changingB;
uniform float time;

//perlin noise calculations
vec3 mod289(vec3 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x)
{
    return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
    return 1.79284291400159 - 0.85373472095314 * r;
}

vec3 fade(vec3 t) {
    return t*t*t*(t*(t*6.0-15.0)+10.0);
}

// Classic Perlin noise
float cnoise(vec3 P)
{
    vec3 Pi0 = floor(P); // Integer part for indexing
    vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
    Pi0 = mod289(Pi0);
    Pi1 = mod289(Pi1);
    vec3 Pf0 = fract(P); // Fractional part for interpolation
    vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
    vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
    vec4 iy = vec4(Pi0.yy, Pi1.yy);
    vec4 iz0 = Pi0.zzzz;
    vec4 iz1 = Pi1.zzzz;
    
    vec4 ixy = permute(permute(ix) + iy);
    vec4 ixy0 = permute(ixy + iz0);
    vec4 ixy1 = permute(ixy + iz1);
    
    vec4 gx0 = ixy0 * (1.0 / 7.0);
    vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
    gx0 = fract(gx0);
    vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
    vec4 sz0 = step(gz0, vec4(0.0));
    gx0 -= sz0 * (step(0.0, gx0) - 0.5);
    gy0 -= sz0 * (step(0.0, gy0) - 0.5);
    
    vec4 gx1 = ixy1 * (1.0 / 7.0);
    vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
    gx1 = fract(gx1);
    vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
    vec4 sz1 = step(gz1, vec4(0.0));
    gx1 -= sz1 * (step(0.0, gx1) - 0.5);
    gy1 -= sz1 * (step(0.0, gy1) - 0.5);
    
    vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
    vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
    vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
    vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
    vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
    vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
    vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
    vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);
    
    vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
    g000 *= norm0.x;
    g010 *= norm0.y;
    g100 *= norm0.z;
    g110 *= norm0.w;
    vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
    g001 *= norm1.x;
    g011 *= norm1.y;
    g101 *= norm1.z;
    g111 *= norm1.w;
    
    float n000 = dot(g000, Pf0);
    float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
    float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
    float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
    float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
    float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
    float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
    float n111 = dot(g111, Pf1);
    
    vec3 fade_xyz = fade(Pf0);
    vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
    vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
    float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
    return 2.2 * n_xyz;
}

// Classic Perlin noise, periodic variant
float pnoise(vec3 P, vec3 rep)
{
    vec3 Pi0 = mod(floor(P), rep); // Integer part, modulo period
    vec3 Pi1 = mod(Pi0 + vec3(1.0), rep); // Integer part + 1, mod period
    Pi0 = mod289(Pi0);
    Pi1 = mod289(Pi1);
    vec3 Pf0 = fract(P); // Fractional part for interpolation
    vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
    vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
    vec4 iy = vec4(Pi0.yy, Pi1.yy);
    vec4 iz0 = Pi0.zzzz;
    vec4 iz1 = Pi1.zzzz;
    
    vec4 ixy = permute(permute(ix) + iy);
    vec4 ixy0 = permute(ixy + iz0);
    vec4 ixy1 = permute(ixy + iz1);
    
    vec4 gx0 = ixy0 * (1.0 / 7.0);
    vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
    gx0 = fract(gx0);
    vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
    vec4 sz0 = step(gz0, vec4(0.0));
    gx0 -= sz0 * (step(0.0, gx0) - 0.5);
    gy0 -= sz0 * (step(0.0, gy0) - 0.5);
    
    vec4 gx1 = ixy1 * (1.0 / 7.0);
    vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
    gx1 = fract(gx1);
    vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
    vec4 sz1 = step(gz1, vec4(0.0));
    gx1 -= sz1 * (step(0.0, gx1) - 0.5);
    gy1 -= sz1 * (step(0.0, gy1) - 0.5);
    
    vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
    vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
    vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
    vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
    vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
    vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
    vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
    vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);
    
    vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
    g000 *= norm0.x;
    g010 *= norm0.y;
    g100 *= norm0.z;
    g110 *= norm0.w;
    vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
    g001 *= norm1.x;
    g011 *= norm1.y;
    g101 *= norm1.z;
    g111 *= norm1.w;
    
    float n000 = dot(g000, Pf0);
    float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
    float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
    float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
    float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
    float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
    float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
    float n111 = dot(g111, Pf1);
    
    vec3 fade_xyz = fade(Pf0);
    vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
    vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
    float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
    return 2.2 * n_xyz;
}
vec4 RGBtoXYZ(vec4 colRGB)
{
    float var_R = (colRGB.x/255.0);
    float var_G = (colRGB.y/255.0);
    float var_B = (colRGB.z/255.0);
    
    if( var_R > 0.04045)
    {
        var_R = pow(( (var_R + 0.055) / 1.055 ) , 2.2);
    }
    else
    {
        var_R = (var_R / 12.92);
    }
    if (var_G > 0.04045)
    {
        var_G = pow(( (var_G + 0.055) / 1.055 ) , 2.2);
        
    }
    else
    {
        var_G = (var_G / 12.92);
    }
    if (var_B > 0.04045)
    {
        var_B = pow(( (var_B + 0.055) / 1.055 ) , 2.2);
    }
    else
    {
        var_B = (var_B / 12.92);
    }
    
    var_R = var_R * 100.0;
    var_G = var_G * 100.0;
    var_B = var_B * 100.0;
    
    float var_X = (var_R * 0.4124) + (var_G * 0.3576) + (var_B * 0.1805);
    float var_Y = (var_R * 0.2126) + (var_G * 0.7152) + (var_B * 0.0722);
    float var_Z = (var_R * 0.0193) + (var_G * 0.1192) + (var_B * 0.9505);
    
    return vec4(var_X,var_Y,var_Z,1.0);
}

vec4 XYZtoRGB(vec4 colXYZ)
{
    float var_X = (colXYZ.x/100.0);
    float var_Y = (colXYZ.y/100.0);
    float var_Z = (colXYZ.z/100.0);
    
    float var_R = (var_X * 3.2410) + (var_Y * -1.5374) + (var_Z * -0.4986);
    float var_G = (var_X * -0.9692) + (var_Y * 1.8760) + (var_Z * 0.0416);
    float var_B = (var_X * 0.0556) + (var_Y * -0.2040) + (var_Z * 1.0570);
    
    if( var_R > 0.00304)
    {
        var_R = (1.055 * pow(var_R, (1.0/2.2))) - 0.055;
    }
    else
    {
        var_R = var_R * 12.92;
    }
    if( var_G > 0.00304)
    {
        var_G = (1.055 * pow(var_G, (1.0/2.2))) - 0.055;
    }
    else
    {
        var_G = var_G * 12.92;
    }
    if( var_B > 0.00304)
    {
        var_B = (1.055 * pow(var_B, (1.0/2.2))) - 0.055;
    }
    else
    {
        var_B = var_B * 12.92;
    }
    
    var_R = var_R * 255.0;
    var_G = var_G * 255.0;
    var_B = var_B * 255.0;
    
    return vec4(var_R,var_G,var_B,1.0);
}

vec4 XYZtoLAB(vec4 colXYZ)
{
    float var_X = colXYZ.x / 96.42;
    float var_Y = colXYZ.y / 100.0;
    float var_Z = colXYZ.z / 82.52;
    
    float oneThird = (1.0/3.0);
    if( var_X > 0.008856)
    {
        var_X = pow(var_X, oneThird);
    }
    else
    {
        var_X = (7.787 * var_X) + (16.0/116.0);
    }
    if( var_Y > 0.008856)
    {
        var_Y = pow(var_Y, oneThird);
    }
    else
    {
        var_Y = (7.787 * var_Y) + (16.0/116.0);
    }
    if( var_Z > 0.008856)
    {
        var_Z = pow(var_Z, oneThird);
    }
    else
    {
        var_Z = (7.787 * var_Z) + (16.0/116.0);
    }
    
    float var_L = (116.0*var_Y) - 16.0;
    float var_A = 500.0 * (var_X - var_Y);
    float var_B = 200.0 * (var_Y - var_Z);
    
    return vec4(var_L,var_A,var_B,1.0);
}

vec4 LABtoXYZ( vec4 colLAB)
{
    float var_Y = ((colLAB.x + 16.0) / 116.0);
    float var_X = (colLAB.y / 500.0) + var_Y;
    float var_Z = var_Y - (colLAB.z/ 200.0);
    
    var_X = var_X * 96.42;
    var_Y = var_Y * 100.0;
    var_Z = var_Z * 82.52;
    
    
    if (var_Y > (6.0/29.0))
    {
        var_Y = pow(var_Y,3.0);
    }
    else
    {
        var_Y = ((var_Y - 16.0)/116.0)/7.787;
    }
    if (var_X > (6.0/29.0))
    {
        var_X = pow(var_X,3.0);
    }
    else
    {
        var_X = ((var_X - 16.0)/116.0)/7.787;
    }
    if (var_Z > (6.0/29.0))
    {
        var_Z = pow(var_Z,3.0);
    }
    else
    {
        var_Z = ((var_Z - 16.0)/116.0)/7.787;
    }
    
    
    return vec4(var_X,var_Y,var_Z,1.0);
}

float F(const float p)
{
	if (p<0.008856)
		return p*(841.0/108.0) + (4.0/29.0);
	return pow(p,1.0/3.0);
}

vec4 XYZToLab(const vec4 vector)
{
	float fX = vector.x/0.950456;
	float fY = vector.y/1.0;
	float fZ = vector.z/1.088754;
	fX = F(fX);
	fY = F(fY);
	fZ = F(fZ);
    //	float X = vector.x, Y = vector.y, Z = vector.z;
    //	float XXn = X / Xn, YYn = Y / Yn, ZZn = Z / Zn;
    //	float fX = LabF(XXn), fY = LabF(YYn), fZ = LabF(ZZn);
	return vec4(116.0 * fY - 16.0,
				500.0 * (fX - fY),
				200.0 * (fY - fZ),
                1.0);
}

float invF(const float p)
{
	float r = p*p*p;
	if (r < 0.008856)
		return (p-4.0/29.0)*(108.0/841.0);
	else
		return r;
}

// Convert input Lab to XYZ color space
vec4 LabToXYZ(const vec4 vector)
{
	float Y = (vector.x + 16.0)/116.0;
	float X = Y + vector.y/500.0;
	float Z = Y - vector.z/200.0;
	X = 0.950456 * invF(X);
	Y = 1.0 * invF(Y);
	Z = 1.088754 * invF(Z);
    //    float L = vector.x, a = vector.y, b = vector.z;
    //    float cY = (L + 16.0) / 116.0, Y = Yn * LabFi(cY), pY = pow(Y / Yn, 1.0 / 3.0);
    //    float cX = a / 500.0 + pY, X = Xn * cX * cX * cX;
    //    float cZ = pY - b / 200.0, Z = Zn * cZ * cZ * cZ;
    return vec4(X, Y, Z,1.0);
}


void main()
{
    vec2 pos = texcoord0;
    
    vec2 shiftL;
    vec2 shiftA;
    vec2 shiftB;
    
    shiftL.x = cnoise(vec3(pos*0.01, time *0.5 + 12.0))*changingL*350.0;
    shiftL.y = cnoise(vec3(pos*0.01, time *0.5 + 17.0))*changingL*350.0;
    
    shiftA.x = cnoise(vec3(pos*0.01, time *0.5 + 12.0))*changingA*350.0;
    shiftA.y = cnoise(vec3(pos*0.01, time *0.5 + 17.0))*changingA*350.0;
    
    shiftB.x = cnoise(vec3(pos*0.01, time *0.5 + 12.0))*changingB*350.0;
    shiftB.y = cnoise(vec3(pos*0.01, time *0.5 + 17.0))*changingB*350.0;
    
    /*
    vec4  colL = XYZToLab(RGBtoXYZ(texture2DRect(tex,pos + shiftL)));
    vec4  colA = XYZToLab(RGBtoXYZ(texture2DRect(tex,pos + shiftA)));
    vec4  colB = XYZToLab(RGBtoXYZ(texture2DRect(tex,pos + shiftB)));
     
    float lc = colL.x;
    float ac = colA.y;
    float bc = colB.z;
     
    vec4 colLAB = vec4(lc,ac,bc,1.0);
     
    vec4 newXYZ = LabToXYZ(colLAB);
    vec4 newRGB = XYZtoRGB(newXYZ);
     */
    
    
    vec4  colL = RGBtoXYZ(texture2DRect(tex,pos + shiftL));
    vec4  colA = RGBtoXYZ(texture2DRect(tex,pos + shiftA));
    vec4  colB = RGBtoXYZ(texture2DRect(tex,pos + shiftB));
    
    
    float lc = colL.x;
    float ac = colA.y;
    float bc = colB.z;
    
    vec4 colLAB = vec4(lc,ac,bc,1.0);
    
    vec4 newXYZ = LABtoXYZ(colLAB);
    vec4 newRGB = XYZtoRGB(colLAB);
    
    
    
    gl_FragColor = newRGB;
}
