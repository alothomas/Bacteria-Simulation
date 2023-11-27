#pragma once
#include "Nutriment.hpp"

class NutrimentA : public Nutriment
{

public :

    NutrimentA(const Quantity& quantite, const Vec2d& coord);  //Constructeur initialisant le rayon et la position seulement en fct de la qtité
    ~NutrimentA();

    void drawOn(sf::RenderTarget& target) const;
    void update(sf::Time dt);


    //Consommation de nutriment par bactérie
    virtual Quantity eatenBy(Bacterium& bact) override;
    virtual Quantity eatenBy(SimpleBacterium& bact) override;
    virtual Quantity eatenBy(TwitchingBacterium& bact) override;
    virtual Quantity eatenBy(SwarmBacterium& bact) override;

    virtual j::Value const& getConfig() const override;

};
