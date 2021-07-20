#include <glm/gtc/constants.hpp>

#include "serialization.h"
#include <math.h>

//=====================================================================================================================CONSTRUCTOR===================
Serializator :: Serializator():
windowWidth_ (1340),
windowHeight_ (800),
mouseX_ (0),
mouseY_ (0),
vision_step_ (glm::pi<float>()/45),
eye_ (vec3(0.0, 0.0, 11)),
cen_ (vec3(0.0, 0.0, eye_.z - 1)),
up_ (vec3(0.0, 1, 0)),
game_speed_(static_cast<float>(411949)),
light_spectrum_ (vec3(1, 1, 1)),
enable_schema_mode_ (true),
real_mode_scale_ (static_cast<float>(9.166368072918009E+06)),
schema_radius_scale_ (static_cast<float>(35570)),
schema_orbits_scale_ (static_cast<float>(21763054)),
real_mode_scale_step_ (static_cast<float>(0.5)),
schema_radius_scale_step_ (static_cast<float>(0.8)),
schema_orbits_scale_step_ (static_cast<float>(0.9)),
move_planets_(true),
elapsed_time_ (0),
last_time_measure_ (0)
{
    for (int i = 0; i < planet_num_ +1; ++i)
    {
        texture_[i] = NULL;
        use_texture_[i] = false;
    }

    for (int i = 0; i < planet_num_; i++)
        angle_[i] = 0;

    N_[0] = 0;
    N_[1] = static_cast<float>(48.33167);
    N_[2] = static_cast<float>(76.67069);
    N_[3] = static_cast<float>(348.73936);
    N_[4] = static_cast<float>(49.57854);
    N_[5] = static_cast<float>(100.55615);
    N_[6] = static_cast<float>(113.642);
    N_[7] = static_cast<float>(73.989821);
    N_[8] = static_cast<float>(131.79431);
    N_[9] = static_cast<float>(110.30347);
    N_[10] = 0;

    i_[0] = 0;
    i_[1] = static_cast<float>(7);
    i_[2] = static_cast<float>(3.39458);
    i_[3] = static_cast<float>(0);
    i_[4] = static_cast<float>(1.85061);
    i_[5] = static_cast<float>(1.03);
    i_[6] = static_cast<float>(2.485);
    i_[7] = static_cast<float>(0.773);
    i_[8] = static_cast<float>(1.767975);
    i_[9] = static_cast<float>(17.14175);
    i_[10] = 0;

    w_[0] = 0;
    w_[1] = static_cast<float>(29.124279);
    w_[2] = static_cast<float>(55.186);
    w_[3] = static_cast<float>(114.20783);
    w_[4] = static_cast<float>(286.537);
    w_[5] = static_cast<float>(275.066);
    w_[6] = static_cast<float>(336.013862);
    w_[7] = static_cast<float>(96.541318);
    w_[8] = static_cast<float>(265.646853);
    w_[9] = static_cast<float>(113.76349);
    w_[10] = 0;

    a_[0] = 0;
    a_[1] = static_cast<float>(57909068);
    a_[2] = static_cast<float>(108208930);
    a_[3] = static_cast<float>(149598261);
    a_[4] = static_cast<float>(227943820);
    a_[5] = static_cast<float>(778547200);
    a_[6] = static_cast<float>(1433449370);
    a_[7] = static_cast<float>(2876679082);
    a_[8] = static_cast<float>(4503443661);
    a_[9] = static_cast<float>(5906376272);
    a_[10] = 0;

    e_[0] = 0;
    e_[1] = static_cast<float>(0.20563069);
    e_[2] = static_cast<float>(0.0068);
    e_[3] = static_cast<float>(0.01671123);
    e_[4] = static_cast<float>(0.0933941);
    e_[5] = static_cast<float>(0.048775);
    e_[6] = static_cast<float>(0.055723);
    e_[7] = static_cast<float>(0.044406);
    e_[8] = static_cast<float>(0.011214);
    e_[9] = static_cast<float>(0.24880766);
    e_[10] = 0;

    M0_[0] = 0;
    M0_[1] = static_cast<float>(174.795884);
    M0_[2] = static_cast<float>(50.115);
    M0_[3] = static_cast<float>(357.51716);
    M0_[4] = static_cast<float>(19.3564);
    M0_[5] = static_cast<float>(18.818);
    M0_[6] = static_cast<float>(320.346750);
    M0_[7] = static_cast<float>(142.955717);
    M0_[8] = static_cast<float>(267.767281);
    M0_[9] = static_cast<float>(14.86012204);
    M0_[10] = 0;

    float change = glm::pi<float>() / 180;
    for (int i = 0; i < planet_num_; i++)
    {
        N_[i] *= change;
        i_[i] *= change;
        w_[i] *= change;
        M0_[i] *= change;
    }

    float fake = 0.0;
    axis_rotation_[0] = static_cast<float>(25.38);
    axis_rotation_[1] = static_cast<float>(58.646);
    axis_rotation_[2] = static_cast<float>(243.023);
    axis_rotation_[3] = static_cast<float>(0.99726968);
    axis_rotation_[4] = static_cast<float>(1.029);
    axis_rotation_[5] = static_cast<float>(0.4147);
    axis_rotation_[6] = static_cast<float>(0.4416);
    axis_rotation_[7] = static_cast<float>(0.71833);
    axis_rotation_[8] = static_cast<float>(0.6653);
    axis_rotation_[9] = static_cast<float>(6.38723);
    axis_rotation_[10] = static_cast<float>(1.0/fake);

    rotation_speed_[0] = static_cast<float>(1.0/fake);
    rotation_speed_[1] = static_cast<float>(87.969);
    rotation_speed_[2] = static_cast<float>(224.698);
    rotation_speed_[3] = static_cast<float>(365.256366);
    rotation_speed_[4] = static_cast<float>(686.98);
    rotation_speed_[5] = static_cast<float>(4332.589);
    rotation_speed_[6] = static_cast<float>(10759.22);
    rotation_speed_[7] = static_cast<float>(30685.4);
    rotation_speed_[8] = static_cast<float>(60190.03);
    rotation_speed_[9] = static_cast<float>(90613.3);
    rotation_speed_[10] = static_cast<float>(1.0/fake);

    radius_[0] = static_cast<float>(695990);
    radius_[1] = static_cast<float>(2439.7);
    radius_[2] = static_cast<float>(6051.8);
    radius_[3] = static_cast<float>(6371);
    radius_[4] = static_cast<float>(3389.5);
    radius_[5] = static_cast<float>(69911);
    radius_[6] = static_cast<float>(57316);
    radius_[7] = static_cast<float>(25266);
    radius_[8] = static_cast<float>(24552.5);
    radius_[9] = static_cast<float>(1195);
    radius_[10] = 0;

    //================================================================
    //================================================================
    //================================================================
    mass_[0] = static_cast<float>(1.99E+30);
    mass_[1] = static_cast<float>(3.33E+23);
    mass_[2] = static_cast<float>(4.87E+24);
    mass_[3] = static_cast<float>(5.97E+24);
    mass_[4] = static_cast<float>(6.42E+23);
    mass_[5] = static_cast<float>(1.90E+27);
    mass_[6] = static_cast<float>(5.68E+26);
    mass_[7] = static_cast<float>(8.68E+25);
    mass_[8] = static_cast<float>(1.02E+26);
    mass_[9] = static_cast<float>(1.30E+22);
    mass_[10] = 0;

    position_[0] = vec3 (9.166368072918009E+04, -3.587908596635106E+05, -1.263365847270218E+04);
    position_[1] = vec3 (3.981532574646779E+07, 2.693705871595619E+07, -1.427017526993466E+06);
    position_[2] = vec3 (1.083995110582697E+08, -7.516123504840386E+06, -6.361445678009944E+06);
    position_[3] = vec3 (1.162398804913440E+08, 9.214935380493926E+07, -1.560961036842257E+04);
    position_[4] = vec3 (-1.528793878916806E+08, 1.925541334868633E+08, 7.784109759604024E+06);
    position_[5] = vec3 (-1.308404872563110E+08, 7.618480179221750E+08, -2.483656477704518E+05);
    position_[6] = vec3 (-1.062988073620396E+09, -1.022591702447227E+09, 6.008502801056857E+07);
    position_[7] = vec3 (2.946083841697208E+09, 5.529078700946414E+08, -3.611419689746372E+07);
    position_[8] = vec3 (4.036568780165731E+09, -1.954926579794473E+09, -5.276881058283679E+07);
    position_[9] = vec3 (9.074663629135412E+08, -4.775885697847751E+09, 2.485565548180889E+08);
    position_[10] = vec3 (0, 0, 0);

    speed_[0] = vec3 (1.068407615399651E-02, 2.825998901862247E-03, -2.451813399270692E-04);
    speed_[1] = vec3 (-3.712878513620788E+01, 4.226944115449314E+01, 6.860766793768278E+00);
    speed_[2] = vec3 (2.163430099495670E+00, 3.478693145745870E+01, 3.521473884663605E-01);
    speed_[3] = vec3 (-1.903255921308336E+01, 2.319272201812972E+01, -9.394261929108316E-04);
    speed_[4] = vec3 (-1.805947412894641E+01, -1.298811643007845E+01, 1.711362049767525E-01);
    speed_[5] = vec3 (-1.303624071468951E+01, -1.591321557884244E+00, 2.983270239243695E-01);
    speed_[6] = vec3 (6.170956639472989E+00, -6.985778300302598E+00, -1.241099702155237E-01);
    speed_[7] = vec3 (-1.305921743797635E+00, 6.375718119862134E+00, 4.060658963891937E-02);
    speed_[8] = vec3 (2.332992134044233E+00, 4.924012112924557E+00, -1.551671101516667E-01);
    speed_[9] = vec3 (5.444809029005323E+00, -8.040967446408351E-02, -1.566356777898360E+00);
    speed_[10] = vec3 (0, 0, 0);

    //================================================================
    //================================================================
    //================================================================
    for (int i = 0; i < planet_num_; i++)
        MovePlanetPosition(i);

    SetSkyMapRadius();
    
    color_[0] = vec4 (255, 255, 0, 1);
    color_[1] = vec4 (137, 132, 138, 1);
    color_[2] = vec4 (254, 215, 136, 1);
    color_[3] = vec4 (100, 132, 85, 1);
    color_[4] = vec4 (229, 129, 67, 1);
    color_[5] = vec4 (239, 239, 185, 1);
    color_[6] = vec4 (223, 168, 88, 1);
    color_[7] = vec4 (190, 254, 254, 1);
    color_[8] = vec4 (78, 104, 181, 1);
    color_[9] = vec4 (242, 235, 225, 1);
    color_[10] = vec4 (255, 255, 255, 1);

    // Material characteristics for light
    light_source_[0] = 1;
    for (int i = 1; i < planet_num_; ++i)
        light_source_[i] = 0;

    int i = 0;
    for (i = 0; i < planet_num_; ++i)
        background_light_koef_[i] = vec3(0.15, 0.15, 0.15);
    background_light_koef_[10] = vec3(1, 1, 1);

    diffuse_light_koef_[0] = vec3 (1, 1, 1); // sun
    for (i = 1; i <= 4 && i < planet_num_; ++i)
        diffuse_light_koef_[i] = vec3 (0.9, 0.9, 0.9);
    for (; i < planet_num_; ++i)
        diffuse_light_koef_[i] = vec3 (0.8, 0.8, 0.8);
    diffuse_light_koef_[10] = vec3(0, 0, 0);

    for (i = 0; i <= 4 && i < planet_num_; ++i)
        reflect_light_koef_ [i] = vec3 (0.01, 0.01, 0.01);
    for (; i < planet_num_; ++i)
        reflect_light_koef_ [i] = vec3 (0.005, 0.005, 0.005);
    reflect_light_koef_[10] = vec3(0, 0, 0);

    for (i = 0; i <= 4 && i < planet_num_; ++i)
        shine_koef_ [i] = 100;
    for (; i < planet_num_; ++i)
        shine_koef_ [i] = 120;
    shine_koef_[10] = 1;

    SetStep();
}

//=====================================================================================================================DESTRUCTOR====================
Serializator::~Serializator()
{
    for (int i = 0; i < planet_num_ +1; i++)
        if (use_texture_[i])
        {
            delete texture_[i];
            texture_[i] = NULL;
            use_texture_[i] = false;
        }
}

//=====================================================================================================================INITIALIZE=TEXTURES===========
void Serializator::InitializeTextures ()
{
    texture_[0] = new Texture (string("textures/sun.bmp"));
    texture_[1] = new Texture (string("textures/mercury.bmp"));
    texture_[2] = new Texture (string("textures/venus.bmp"));
    texture_[3] = new Texture (string("textures/earth.bmp"));
    texture_[4] = new Texture (string("textures/mars.bmp"));
    texture_[5] = new Texture (string("textures/jupiter.bmp"));
    texture_[6] = new Texture (string("textures/saturn.bmp"));
    texture_[7] = new Texture (string("textures/uranus.bmp"));
    texture_[8] = new Texture (string("textures/neptune.bmp"));
    texture_[9] = new Texture (string("textures/pluton.bmp"));
    texture_[10] = new Texture (string("textures/skymap.bmp"));
    texture_[11] = new Texture (string("textures/saturn rings.bmp"));
    
    for (int i = 0; i < planet_num_ +1; i++)
        use_texture_[i] = true;
}

//=====================================================================================================================GET=PLANET=POSITION===========
mat4x4 Serializator::GetPlanetModelMatrix (const int i_planet)
{
    if (i_planet < 0 || planet_num_ <= i_planet)
        throw string ("Incorrect planet number.");

    mat4x4 scale = glm::mat4() * GetPlanetRadius(i_planet);
    scale [3][3] = 1;

    mat4x4 rotate = glm::mat4();
    rotate[0][0] = rotate[2][2] = glm::cos(angle_[i_planet]);
    rotate[0][2] = glm::sin(angle_[i_planet]);
    rotate[2][0] = -rotate[0][2];

    vec3 new_pos = GetPlanetPosition(i_planet);
    mat4x4 pos = glm::translate(glm::mat4(), new_pos);

    mat4x4 res = pos*scale*rotate;

    return res;
}

//=====================================================================================================================GET=PLANET=ORBIT=SCALE=MATRIX=
mat4x4 Serializator::GetPlanetOrbitScaleMatrix (const int i_planet)
{
    if (i_planet < 0 || planet_num_ <= i_planet)
        throw string ("Incorrect planet number.");

    if (i_planet == 0 || i_planet == 10)
        return mat4x4(0);

    mat4x4 res;
    if (enable_schema_mode_)
        res = mat4x4 (1/schema_orbits_scale_);
    else
        res = mat4x4 (1/real_mode_scale_);

    res[3][3] = 1;

    return res;
}

//=====================================================================================================================SET=STEP======================
void Serializator::SetStep()
{
    float min_len = FLT_MAX;
    float len;
    vec3 pos = GetPlanetPosition(0);
    len = glm::length (pos - eye_) - GetPlanetRadius(0);
    if (len > 0 && min_len > len)
        min_len = len;
    for (int i = 1; i < planet_num_ -1; ++i)
    {
        pos = GetPlanetPosition (i);
        len = glm::length (pos - eye_) - GetPlanetRadius(i);
        if (len > 0 && min_len > len)
            min_len = len;
    }
    
    step_ = min_len /5;
}

//=====================================================================================================================MOVE=PLANETS==================
void Serializator::MovePlanets ()
{
    // Gravitation Model

    const unsigned int current_time = glutGet(GLUT_ELAPSED_TIME); // Time in milliseconds
    elapsed_time_ = current_time - last_time_measure_;
    last_time_measure_ = current_time;
    const float elapsed_time = static_cast<float>(elapsed_time_) / (1000 * 60 * 60 * 24) * game_speed_;

    for (int i = 0; i < planet_num_; i++)
    {
        const float koef = 2*glm::pi<float>();
        angle_[i] += elapsed_time * koef / axis_rotation_[i];
        while (angle_[i] > koef)
            angle_[i] -= koef;
    }

    // Gravity
    /*float gravitation_constant = 6.6738480E-11;

    vec3 new_pos [planet_num_ -1];
    vec3 new_speed [planet_num_ -1];
    for (int i = 0; i < planet_num_ -1; ++i)
    {
        vec3 acceleration = vec3 (0, 0, 0);
        for (int j = 0; j < planet_num_ -1; ++j)
            if (j != i)
            {
                float radius = ( glm::length (position_[i] - position_[j]) );
                vec3 norm = glm::normalize (position_[j] - position_[i]);
                acceleration += norm * ( mass_[j] / ( radius * radius) );
            }
        acceleration *= gravitation_constant;

        new_pos [i] = position_[i] + speed_[i] * elapsed_time + acceleration * (elapsed_time * elapsed_time /2);
        new_speed [i] = speed_[i] + acceleration * elapsed_time;
    }

    for (int i = 0; i < planet_num_ -1; ++i)
    {
        position_[i] = new_pos[i];
        speed_[i] = new_speed[i];
    }*/

    //====================================================
    //====================================================
    //====================================================

    // Kepler
    for (int i = 0; i < planet_num_; i++)
        MovePlanetPosition(i);

    SetSkyMapRadius();

    // Skymap
    /*float new_sky_rad = 0;
    for (int i = 0; i < planet_num_ -1; ++i)
    {
        float len = glm::length(position_[i]);
        if (new_sky_rad < len)
            new_sky_rad = len;
    }
    if (9*new_sky_rad > 10*radius_[10])
        radius_[10] = new_sky_rad * 5;*/
}

//=====================================================================================================================MOVE=PLANET=POSITION==========
vec3 Serializator::MovePlanetPosition (const int i_planet, const bool calculation_mode, const float angle_shift)
{
    if (i_planet < 0 || planet_num_ <= i_planet)
        throw string ("Incorrect planet number in function \"MovePlanetPosition ()\".");

    if (i_planet == 0) // sun
    {
        position_[i_planet] = vec3(0, 0, 0);
        return vec3(0, 0, 0);
    }
    if (i_planet == 10) // skymap
    {
        position_[i_planet] = vec3(0, 0, 0);
        return vec3(0, 0, 0);
    }

    float M;
    if (!calculation_mode)
    {
        float n = ( 2*glm::pi<float>() ) / rotation_speed_[i_planet];
        const float elapsed_time = static_cast<float>(elapsed_time_) / (1000 * 60 * 60 * 24) * game_speed_;
        M = M0_[i_planet] + n * elapsed_time;
        M0_[i_planet] = static_cast<float>(M);
    }
    else
    {
        M = M0_[i_planet] + angle_shift;
    }

    while (M < 0 || M > 2 * glm::pi<float>())
    {
        if (M < 0)
            M += 2 * glm::pi<float>();
        else
            M -= 2 * glm::pi<float>();
    }
    float E = M + e_[i_planet] * glm::sin(M) * ( 1 + e_[i_planet] * glm::cos(M) );
    float E_new = 1000; // Заведомо не уложится в ограничения
    int iterations_num = 0;
    while (iterations_num < 100)
    {
        float koef = (1 - e_[i_planet] * glm::cos(E) );
        E_new = E - (E - e_[i_planet] * glm::sin(E) - M) / koef;
        if (glm::abs(E - E_new) < 0.001)
        {
            E = E_new;
            break;
        }
        else
            E = E_new;
        iterations_num ++;
    }

    float x_flat = a_[i_planet] * (glm::cos(E) - e_[i_planet]);
    float y_flat = a_[i_planet] * (glm::sqrt(1 - e_[i_planet] * e_[i_planet]) * glm::sin (E));
    float v = glm::atan2 (y_flat, x_flat);
    float r = glm::sqrt(x_flat * x_flat + y_flat * y_flat);
    
    float cosN = glm::cos(N_[i_planet]);
    float sinN = glm::sin(N_[i_planet]);

    float cosI = glm::cos(i_[i_planet]);
    float sinI = glm::sin(i_[i_planet]);

    float k = v + w_[i_planet];

    float cosE = glm::cos(k);
    float sinE = glm::sin(k);

    vec3 pos = vec3 (
        (cosN * cosE - sinN * sinE * cosI),
        (sinE * sinI),
        (sinN * cosE + cosN * sinE * cosI)
        );

    /*
    vec3 pos = vec3 (
        r * glm::cos(N_[i_planet]* (glm::pi<float>() / 180)) * glm::cos(true_anomaly + w_[i_planet]* (glm::pi<float>() / 180)) - glm::sin(N_[i_planet]* (glm::pi<float>() / 180)) * glm::sin(true_anomaly + w_[i_planet]* (glm::pi<float>() / 180)) * glm::cos(i_[i_planet]* (glm::pi<float>() / 180)),
        r * glm::sin(true_anomaly + w_[i_planet]* (glm::pi<float>() / 180)) * glm::sin(i_[i_planet]* (glm::pi<float>() / 180)),
        r * glm::sin(N_[i_planet]* (glm::pi<float>() / 180)) * glm::cos(true_anomaly + w_[i_planet]* (glm::pi<float>() / 180)) + glm::cos(N_[i_planet]* (glm::pi<float>() / 180)) * glm::sin(true_anomaly + w_[i_planet]* (glm::pi<float>() / 180)) * glm::cos(i_[i_planet]* (glm::pi<float>() / 180))
        );
    */

    if (!calculation_mode)
        position_[i_planet] = pos * static_cast<float>(r);
    return pos * static_cast<float>(r);
}

//=====================================================================================================================GET=PLANET=POSITION===========
vec3 Serializator::GetPlanetPosition (const int i_planet)
{
    if (i_planet < 0 || planet_num_ <= i_planet)
        throw string ("Incorrect planet number in function \"GetPlanetPosition ()\".");

    vec3 pos;

    if (enable_schema_mode_)
    {
        float radius = glm::length(position_[i_planet]);
        float scale;
        if (radius > 1)
            //scale = glm::log(radius, schema_orbits_scale_) / radius;
            scale = glm::pow(radius, schema_orbits_scale_) / radius;
        else
            scale = 1 / schema_orbits_scale_;
        
        pos = position_[i_planet] * scale;

        pos = position_[i_planet] / schema_orbits_scale_;
    }
    else
    {
        pos = position_[i_planet] / real_mode_scale_;
    }

    return pos;
}

//=====================================================================================================================GET=PLANET=RADIUS=============
float Serializator::GetPlanetRadius (const int i_planet)
{
    if (i_planet < 0 || planet_num_ <= i_planet)
        throw string ("Incorrect planet number in function \"GetPlanetRadius ()\".");

    float radius = 0;
    if (enable_schema_mode_)
    {
        if (i_planet == 0)
            radius = radius_[0] * static_cast<float>(0.04) / schema_radius_scale_; // Special for Sun, it will be 0.04 times smaller
        else if (i_planet == 10)
            radius = (radius_[i_planet] / schema_radius_scale_) / 2; // Special for Skymap, it will be 10 times greater
        else
            radius = radius_[i_planet] / schema_radius_scale_;
    }
    else
    {
        if (i_planet == 10)
            radius = (radius_[i_planet] / real_mode_scale_); // Special for Skymap, it will be 10 times greater
        else radius = radius_[i_planet] / real_mode_scale_;
    }

    float len = glm::length(position_[9]);

    return radius;
}

//=====================================================================================================================SET=SKY=MAP=RADIUS============
void Serializator::SetSkyMapRadius ()
{
    float radius = 0;
    float len;
    for (int i = 0; i < planet_num_ -1; ++i)
    {
        len = glm::length(position_[i]) + radius_[i];
        if (radius < len)
            radius = len;
    }

    /*if (enable_schema_mode_)
    {
        vec3 camera = vec3 ( glm::pow(schema_orbits_scale_, glm::abs(eye_.x) ), glm::pow(schema_orbits_scale_, glm::abs(eye_.y) ), glm::pow(schema_orbits_scale_, glm::abs(eye_.z) ) );
        len = glm::length (camera);
    }
    else
        len = real_mode_scale_ * glm::length(eye_);

    if (radius < len)
            radius = len;*/
    if (radius_[10] < 2 * radius)
        radius_[10] = 2 * radius;

    /*if (9*new_sky_rad > 10*radius_[10])
        radius_[10] = new_sky_rad * 5;*/
}
