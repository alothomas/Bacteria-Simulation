#include "NutrimentB.hpp"
#include "Application.hpp"


NutrimentB::NutrimentB(const Quantity& quantite, const Vec2d& coord)  //Constructeur initialisant le rayon et la position seulement en fct de la qtité
    :Nutriment(quantite,coord)
{}

NutrimentB::~NutrimentB()
{}

void NutrimentB::drawOn(sf::RenderTarget& target) const
{
    Nutriment::drawOn(target);
}

void NutrimentB::update(sf::Time dt)
{
    Nutriment::update(dt);
}

//CONSOMMATION DE NUTRIMENT PAR BACTÉRIE

Quantity NutrimentB::eatenBy(Bacterium& bact)
{
    return bact.eatableQuantity(*this);
}

Quantity NutrimentB::eatenBy(SimpleBacterium& bact)
{
    double factor(getConfig()["resistance factor"].toDouble());
    return takeQuantity(bact.getMaxEatableQuantity() / factor);
}

Quantity NutrimentB::eatenBy(TwitchingBacterium& bact)
{
    double factor1(getConfig()["nutritive factor"].toDouble());
    return takeQuantity(bact.getMaxEatableQuantity()) * factor1;
}

Quantity NutrimentB::eatenBy(SwarmBacterium& bact)
{
    double factor2(getConfig()["poison factor"].toDouble());
    return takeQuantity(bact.getMaxEatableQuantity())* (-factor2);
}


j::Value const& NutrimentB::getConfig() const
{
    return getAppConfig()["nutriments"]["B"];
}



