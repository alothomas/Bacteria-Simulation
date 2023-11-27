#pragma once
#include "./Interface/Drawable.hpp"
#include "./Interface/Updatable.hpp"
#include "CircularBody.hpp"
#include "./Utility/Types.hpp"


class Antibiotic : public Drawable, public Updatable, public CircularBody
{

public:

    Antibiotic(Vec2d const& coord);
    virtual ~Antibiotic();

    //Affichage et update
    virtual void drawOn(sf::RenderTarget& target) const override;
    virtual void update(sf::Time dt) override;


    bool isInEffectZone(Vec2d const& coord) const;  //savoir si un objet est dans la zone d'effet de l'antibio
    double getPower() const; //retourne la puissance de l'antibio


private:

    Quantity dose_;             //Quantité d'antibiotique
    double power_;              //puissance de l'antibiotique sur les bactéries : small number = big effect / big number = small effect
    double effectZoneRadius_;   //rayon où l'antibiotique fait effet
    double timer_;

};
