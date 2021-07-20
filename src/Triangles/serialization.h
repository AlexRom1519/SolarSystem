#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

using glm::vec3;
using glm::vec4;
using glm::mat4x4;

class Serializator
{
public:
    int windowWidth_;
    int windowHeight_;

    //last mouse coordinates
    int mouseX_;
    int mouseY_;

    // camera step
    float step_;
    // step in radians
    float vision_step_;

    //camera position
    vec3 eye_;
    //reference point position.
    vec3 cen_;
    //up vector direction (head of observer)
    vec3 up_;

    //matrices
    /*mat4x4 modelViewMatrix_;
    mat4x4 projectionMatrix_;
    mat4x4 modelViewProjectionMatrix_;
    mat4x4 normalMatrix_;*/

    mat4x4 projectionMatrix_; // It initializes in ReSizeGLScene

    bool enable_schema_mode_;
    bool move_planets_;

    float real_mode_scale_;
    float schema_radius_scale_;
    float schema_orbits_scale_;

    float real_mode_scale_step_;
    float schema_radius_scale_step_;
    float schema_orbits_scale_step_;

    float game_speed_;

    static const int planet_num_ = 11;

    vec3 light_spectrum_;
    int light_source_ [planet_num_];

    float angle_[planet_num_];
    float radius_ [planet_num_];
    float mass_ [planet_num_];
    vec3 position_ [planet_num_];
    vec3 speed_ [planet_num_];
    vec4 color_ [planet_num_];

    // vec3 - because of RGB
    vec3 background_light_koef_ [planet_num_];
    vec3 diffuse_light_koef_ [planet_num_];
    vec3 reflect_light_koef_ [planet_num_];
    float shine_koef_ [planet_num_];
    Texture* texture_ [planet_num_ +1]; // Because of Saturn ring
    bool use_texture_ [planet_num_ +1]; // Because of Saturn ring

    float N_ [planet_num_];
    float i_ [planet_num_];
    float w_ [planet_num_];
    float a_ [planet_num_];
    float e_ [planet_num_];
    float M0_ [planet_num_];
    float rotation_speed_ [planet_num_];    // speed of rotation in orbit
    float axis_rotation_ [planet_num_];     // speed of rotation around own axis

    unsigned int last_time_measure_;// In miliseconds
    unsigned int elapsed_time_;     // In miliseconds between last calc

public:
    Serializator();
    ~Serializator();

    void InitializeTextures ();

    mat4x4 GetPlanetModelMatrix (const int i_planet);
    mat4x4 GetPlanetOrbitScaleMatrix (const int i_planet);
    void MovePlanets ();
    vec3 MovePlanetPosition (const int i_planet, const bool calculation_mode = false, const float angle_shift = 0);
    void SetStep();
    void SetSkyMapRadius ();
    
    vec3 GetPlanetPosition (const int i_planet);
    float GetPlanetRadius (const int i_planet);
};
