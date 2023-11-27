#include "Nutriment.hpp"
#include "Application.hpp"
#include "./Utility/Utility.hpp"
#include <SFML/Graphics.hpp>




///CONSTRUCTEUR ET DESTRUCTEUR
Nutriment::Nutriment(const Quantity& quantite, const Vec2d& coord)  //Constructeur initialisant le rayon et la position seulement en fct de la qtité
    :CircularBody(coord,quantite), quantite_(quantite)
{}

Nutriment::~Nutriment()  //destructeur
{}



///MODIFIER DES QTITE DE NUTRIMENT ET LES METTRE A JOUR

Quantity Nutriment::takeQuantity(const Quantity& quantite)   //Prélever une qtité de nutriment
{

    if(quantite<0.) { //Erreur si prélevement négatif
        throw std::invalid_argument("erreur la qtité à prélever est négative");
    }
    if(quantite<=quantite_) { //Si prélevement est en dessous de la qtité dispo
        setQuantity(quantite_-quantite); //On mets a jour le nutriment
        return quantite;
    }
    setQuantity(0.);
    return quantite_;   //Si la qtité dispo est trop petite, on ne peut que prélever toute la qtité dispo
}



void Nutriment::setQuantity(Quantity quantite)   //Mets a jour le rayon du nutriment
{

    if(quantite<=0.) {
        quantite_=0.;
        setRadius(0.);   //On adapte le rayon
        return;
    }

    quantite_=quantite;
    setRadius(quantite_);
    return;

}




///AFFICHAGE DES NUTRIMENTS AVEC OPTION DEBUGGING

void Nutriment::drawOn(sf::RenderTarget& target) const
{

    //option de debug (affichage texte que si rayon>0)   ON POURRA EN FAIRE UNE METHODE A PART SI JAMAIS

    if(isDebugOn() and (getRadius()>0)) {

        //On doit décaler la position du texte en fct de la taille du nutriment
        Vec2d positionText = getPosition();
        Vec2d Decal(0,getRadius()+1);
        positionText=positionText+Decal;

        //Convertir Quantity en int pour ne garder que la partie entière
        int qtite(quantite_);

        auto const text = buildText(std::to_string(qtite), positionText, getAppFont(), 15, sf::Color::Black);
        target.draw(text);
    }


    //affichage des nutriments
    auto const& texture = getAppTexture(getConfig()["texture"].toString());   //toSpring + nom de "texture"

    //buildSprite(centre, taille, texture)
    auto nutrimentSprite = buildSprite(getPosition(), 6, texture);

    //adapte la taille du Sprite au rayon du nutriment:
    nutrimentSprite.setScale(((2*getRadius()) / (texture.getSize().x)), ((2*getRadius()) / (texture.getSize().y)) );
    target.draw(nutrimentSprite);
}


void Nutriment::update(sf::Time dt)
{
//Croissance que si certaines conditions

    //Si température est dans bonnes limites
    double temp(getAppEnv().getTemperature());

    if( (getConfig()["growth"]["min temperature"].toDouble() <= temp ) and ( getConfig()["growth"]["max temperature"].toDouble() >= temp) ) {

        auto growth = getConfig()["growth"]["speed"].toDouble() * dt.asSeconds();

        //Si le nutriment ne dépasse pas le petri et la qtité ne dépasse pas le maximum*2 : on autorise la croissance
        if((getAppEnv().contains(*this)) and ((quantite_) <= getConfig()["quantity"]["max"].toDouble()*2)) {

            setQuantity(growth+quantite_);
            return;
        }
        return;
    }

    return;
}


Quantity Nutriment::getQuantity() const
{
    return quantite_;
}


