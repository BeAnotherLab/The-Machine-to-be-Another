
uniform sampler2DRect tex;
varying vec2 texcoord0;
uniform float changingL;
uniform float changingA;
uniform float changingB;
uniform float time;

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
    float var_X = colXYZ.x / 9.642;
    float var_Y = colXYZ.y / 1.000;
    float var_Z = colXYZ.z / 8.252;
    
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
    
    var_X = var_X * 9.642;
    var_Y = var_Y * 1.000;
    var_Z = var_Z * 8.252;
    
    
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

    
    vec4  col = XYZToLab(RGBtoXYZ(texture2DRect(tex,pos)));
    
    col.x = col.x * changingL;
    col.y = col.y * changingA;
    col.z = col.z * changingB;
     
    vec4 newXYZ = LabToXYZ(col);
    vec4 newRGB = XYZtoRGB(newXYZ);
    
    gl_FragColor = newRGB;
}
