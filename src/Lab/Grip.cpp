#include "Grip.hpp"

Grip::Grip(Vec2d const& coord, double const& rayon)
    :CircularBody(coord, rayon)
{}

Grip::~Grip()
{}

void Grip::moveGrip(Vec2d const& position)
{
    move(position);
    return;
}
