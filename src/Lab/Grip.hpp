#pragma once
#include "CircularBody.hpp"

//Modélise un grappin

class Grip : public CircularBody
{

public:

    Grip(Vec2d const& coord, double const& rayon);
    ~Grip();

    void moveGrip(Vec2d const& position);


};
