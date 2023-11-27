#pragma once
#include "CircularBody.hpp"
#include "./Utility/Types.hpp"
#include <SFML/Graphics.hpp>
#include "./Interface/Drawable.hpp"
#include "./Interface/Updatable.hpp"
#include "Bacterium.hpp"
#include "SimpleBacterium.hpp"
#include "TwitchingBacterium.hpp"
#include "SwarmBacterium.hpp"

class Nutriment : public CircularBody, public Drawable, public Updatable
{


public:

    Nutriment(const Quantity& quantite, const Vec2d& coord);  //Constructeur initialisant le rayon et la position seulement en fct de la qtité
    virtual ~Nutriment();  //destructeur

    //Prendre et régler une qtité de nutriment
    Quantity takeQuantity(const Quantity& quantite);
    void setQuantity(Quantity quantite);

    virtual void drawOn(sf::RenderTarget& target) const override;
    virtual void update(sf::Time dt) override;

    //retourne la qtité prise par un type de bactérie sur un nutriment
    virtual Quantity eatenBy(Bacterium& bact) = 0;
    virtual Quantity eatenBy(SimpleBacterium& bact) = 0;
    virtual Quantity eatenBy(TwitchingBacterium& bact) = 0;
    virtual Quantity eatenBy(SwarmBacterium& bact) = 0;

    Quantity getQuantity() const;


    virtual j::Value const& getConfig() const = 0;



protected:
    Quantity quantite_;


};
