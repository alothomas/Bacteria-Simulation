#include "Antibiotic.hpp"
#include "Application.hpp"
#include "Random/Random.hpp"

Antibiotic::Antibiotic(Vec2d const& coord)
    :CircularBody(coord, uniform(getShortConfig().antibiotic_min_qty, getShortConfig().antibiotic_max_qty)), dose_(rayon_), power_(uniform(getShortConfig().antibiotic_min_power, getShortConfig().antibiotic_max_power)), effectZoneRadius_(3*rayon_), timer_(0.0)
{}

Antibiotic::~Antibiotic()
{}



///AFFICHAGE ET UPDATE
void Antibiotic::drawOn(sf::RenderTarget& target) const
{
    //Option de debug
    if(isDebugOn() and (getRadius()>0)) {

        //Affichage dose
        //On doit décaler la position du texte en fct de la taille du nutriment
        Vec2d positionText = getPosition();
        Vec2d Decal(0,getRadius()+1);
        positionText=positionText+Decal;

        //Convertir Quantity en int pour ne garder que la partie entière
        int qtite(dose_);

        auto const text = buildText(std::to_string(qtite), positionText, getAppFont(), 15, sf::Color::Blue);
        target.draw(text);

        //Affichage de la zone d'effet
        auto border = buildAnnulus(coord_, effectZoneRadius_, sf::Color::White, 2);
        target.draw(border);
    }


    //affichage de l'antibiotique
    auto const& texture = getAppTexture(getAppConfig()["antibiotic"]["texture"].toString());

    //buildSprite(centre, taille, texture)
    auto antibioticSprite = buildSprite(getPosition(), 6, texture);

    //adapte la taille du Sprite au rayon de l'antibiotic:
    antibioticSprite.setScale(((2*getRadius()) / (texture.getSize().x)), ((2*getRadius()) / (texture.getSize().y)) );
    target.draw(antibioticSprite);

}



void Antibiotic::update(sf::Time dt)
{
    timer_+=dt.asSeconds();
    if(timer_>getAppConfig()["antibiotic"]["delay"].toDouble()){
        dose_-=1;
        setRadius(dose_);
        effectZoneRadius_ = rayon_*3;
        timer_=0.;
    }
    return;

}


///ZONE D'EFFET ET GRADIANT
bool Antibiotic::isInEffectZone(Vec2d const& coord) const
{
    if(distance(coord_,coord)<=effectZoneRadius_){
        return true;
    }
    return false;


}

double Antibiotic::getPower() const
{
    return power_;
}


