#pragma once
#include "SimpleBacterium.hpp"



class CannibalBacterium : public SimpleBacterium
{

public:

    CannibalBacterium(Vec2d const& coord);
    virtual ~CannibalBacterium();

    //Affichage et update
    virtual void update(sf::Time dt) override;
    virtual void drawOn(sf::RenderTarget& target) const override;
    
    //Déplacement et division
    virtual void move(sf::Time dt) override; //fait bouger la bactérie
    virtual Bacterium* clone() const override; //divise une bacterie

    sf::Color getWarningColor() const; //retourne la couleur du motif de la bactérie selon si elle est cannibale

};
