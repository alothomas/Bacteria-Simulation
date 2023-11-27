#pragma once
#include "SFML/Graphics.hpp"
#include "./Interface/Updatable.hpp"

class NutrimentGenerator : public Updatable
{
public:

    NutrimentGenerator();
    virtual ~NutrimentGenerator();

    virtual void update(sf::Time dt) override;

    void reset(); //reset son time à zero

private:

    sf::Time time_;
};
