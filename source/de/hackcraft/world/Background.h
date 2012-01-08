/**
 * File:     cBackground.h
 * Project:  LinWarrior 3D
 * Home:     hackcraft.de
 *
 * Created on March 28, 2008, 21:25 PM (1999)
 *
 * Virtual Horizon with Sky and Ground
 * (SkySphere / SkyBox).
 */

#ifndef _CBACKGROUND_H
#define _CBACKGROUND_H

class Background;

#include <map>
#include <list>

#include "de/hackcraft/psi3d/Particle.h"

typedef std::map<std::string,std::string> BackgroundProps;

/**
 * A background in form of a SkySphere just like
 * those virtual horizons found in aviation instruments.
 * Sets lightsource 0 as skylight according to time.
 */
class Background {
public:
    float hour;
    std::map<int, unsigned int> textures;
    std::list<Particle*> rain;
    int rainstrength;
    int seed;
    float heightshift;
    vec3 light;
public:
    Background();
    Background(BackgroundProps* properties);
    void init(BackgroundProps* properties);
    void drawBackground(float h = 12.00f); // Calls other draw*()
    void drawGalaxy();
    void drawUpperDome();
    void drawLowerDome();
    void drawGround();
    void drawClouds();
    //void drawMountains();
    void drawSun();
    void drawOrbit();
    void drawRain();
};

#endif

