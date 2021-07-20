//shader version
#version 150 core

//mode of drawing
//if is true, then use Texture
//otherwise draw gradient
uniform int useTexture;

//texture object
uniform sampler2D mytexture;

uniform vec4 mycolor;

uniform vec3 light_pos;

uniform vec3 background_light_power;
uniform vec3 diffuse_light_power;
uniform vec3 reflect_light_power;

uniform vec3 background_light_koef;
uniform vec3 diffuse_light_koef;
uniform vec3 reflect_light_koef;
uniform float shine_koef;

uniform int light_source;

//retrieve this data form vertex shader
in VertexData
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
} VertexIn;

// helper function inside fragment shader
// converts color from hsv space to rgb space
// Hue is in range 0..360
// Saturation and Lightness in range 0..1
vec3 hsv2rgb(vec3 hsvColor)
{
	//return variable
	vec3 rgbColor;
	
	//index in HSV pyramide
	int hue_index = int(floor(hsvColor.x/60.0f));

	//some helpers
	float C = hsvColor.y*hsvColor.z;
	float X = C*(1 - abs(mod(hsvColor.x/60.0f,2) - 1));
	float m = hsvColor.z-C;

	//assign color
	if (hue_index==0)
		rgbColor = vec3(C,X,0);
	else
	if (hue_index==1)
		rgbColor = vec3(X,C,0);
	else
	if (hue_index==2)
		rgbColor = vec3(0,C,X);
	else
	if (hue_index==3)
		rgbColor = vec3(0,X,C);
	else
	if (hue_index==4)
		rgbColor = vec3(X,0,C);
	else //hue_index==5
		rgbColor = vec3(C,0,X);
	
		
	return rgbColor+vec3(m,m,m);
}

//main function
		
void main()
{
    vec4 tmp_color;
	if (useTexture>0)
    {
		//take color from texture using texture2D
		tmp_color = vec4(texture2D(mytexture,VertexIn.texcoord.xy).rgb, mycolor.w);
    }
	else
	{
		    //draw rainbow round
		//float dX = VertexIn.position.x + 0.5f;
		//float dY = VertexIn.position.y - 0.5f;
		    //radius to the center of square
		//float R = sqrt(dX*dX + dY*dY);

		    //convert from Hue-Saturation-Value to Red-Green-Blue
		//vec3 rgbColor = hsv2rgb(vec3(R*360.0f,1.0f,0.5f));

		    //draw pixel. 4-th component is alpha-channel. use it for transparency
		    //1 is non-transparent
		    //0 is transparent

		    //some compilers use optimization, so use must use all attribute variables you pass to shaders
		    //for example not using VertexIn.normal in this line can cause blue screen, because normals will not allocate
		    //
		//tmp_color = vec4(rgbColor.rgb, 1);//1
        tmp_color = vec4 (mycolor.rgb/255, mycolor.w);//1
	}
    
    //vec3 illumination = vec3 (0, 0, 0);

    vec3 illumination = vec3 ( background_light_power.x * background_light_koef.x,
                               background_light_power.y * background_light_koef.y,
                               background_light_power.z * background_light_koef.z);

    vec3 light = normalize (light_pos - VertexIn.position);
    float angle_normal_light = /*cos*/ VertexIn.normal.x * light.x + VertexIn.normal.y * light.y + VertexIn.normal.z * light.z; //dot (VertexIn.normal, light);
    float angle_normal_light_not_filtered = angle_normal_light;
    if (angle_normal_light < 0)
        angle_normal_light = 0;
    illumination = illumination + vec3 (diffuse_light_koef.x * diffuse_light_power.x,
                                        diffuse_light_koef.y * diffuse_light_power.y,
                                        diffuse_light_koef.z * diffuse_light_power.z) *
                                  angle_normal_light + vec3 (1, 1, 1) * light_source;
    
    //vec3 viewer = normalize (VertexIn.position * (-1));
    //float angle_normal_viewer = /*cos*/ dot (VertexIn.normal, viewer);
    //if (angle_normal_viewer < 0)
    //    angle_normal_viewer = 0;
    //float angle_light_viewer = /*cos*/ dot (light, viewer);
    //if (angle_light_viewer < 0)
    //    angle_light_viewer = 0;
    //illumination = illumination + vec3 ( reflect_light_koef.x * reflect_light_power.x,
    //                                     reflect_light_koef.y * reflect_light_power.y,
    //                                     reflect_light_koef.z * reflect_light_power.z) *
    //                              pow ( 2*angle_normal_light * angle_normal_viewer - angle_light_viewer, shine_koef );

    vec3 reflection = VertexIn.normal * 2 * angle_normal_light_not_filtered - light;
    vec3 viewer = normalize (VertexIn.position * (-1));
    float angle_reflection_viewer = /*cos*/ reflection.x*viewer.x + reflection.y*viewer.y + reflection.z*viewer.z; //dot (reflection, viewer);
    if (angle_reflection_viewer < 0)
        angle_reflection_viewer = 0;
    vec3 glare = vec3 ( reflect_light_koef.x * reflect_light_power.x,
                        reflect_light_koef.y * reflect_light_power.y,
                        reflect_light_koef.z * reflect_light_power.z) *
                 pow ( angle_reflection_viewer, shine_koef ) * (1 - light_source);
    //vec3 glare = vec3 (0, 0, 0);
    illumination = vec3 (illumination.x * tmp_color.x + glare.x * ( reflect_light_power.x * 255 ) ,
                         illumination.y * tmp_color.y + glare.y * ( reflect_light_power.y * 255 ) ,
                         illumination.z * tmp_color.z + glare.z * ( reflect_light_power.z * 255 ) );

    //if (illumination.x >= 255)
      //  illumination.x = 255;
    //if (illumination.y >= 255)
      //  illumination.y = 255;
    //if (illumination.z >= 255)
      //  illumination.z = 255;
    
    gl_FragColor = vec4 (illumination.x, illumination.y, illumination.z, tmp_color.w);
}
