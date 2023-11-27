#include "NutrimentA.hpp"
#include "Application.hpp"


NutrimentA::NutrimentA(const Quantity& quantite, const Vec2d& coord)  //Constructeur initialisant le rayon et la position seulement en fct de la qtité
    :Nutriment(quantite,coord)
{}

NutrimentA::~NutrimentA()
{}

void NutrimentA::drawOn(sf::RenderTarget& target) const
{
    Nutriment::drawOn(target);
}

void NutrimentA::update(sf::Time dt)
{
    Nutriment::update(dt);
}


///CONSOMMATION DE NUTRIMENT PAR BACTÉRIE

Quantity NutrimentA::eatenBy(Bacterium& bact)
{
 return  bact.eatableQuantity(*this);
}

Quantity NutrimentA::eatenBy(SimpleBacterium& bact)
{
  return takeQuantity(bact.getMaxEatableQuantity());
}

Quantity NutrimentA::eatenBy(TwitchingBacterium& bact)
{
    return takeQuantity(bact.getMaxEatableQuantity());
}

Quantity NutrimentA::eatenBy(SwarmBacterium& bact)
{
    return takeQuantity(bact.getMaxEatableQuantity());

}



j::Value const& NutrimentA::getConfig() const
{
    return getAppConfig()["nutriments"]["A"];
}
