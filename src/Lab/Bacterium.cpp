#include "Bacterium.hpp"
#include "Application.hpp"
#include "./Utility/Utility.hpp"
#include <cmath>
#include "Nutriment.hpp"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"


///CONSTRUCTEUR
Bacterium::Bacterium(Quantity const& energy, Vec2d const& coord, Vec2d const& direction, double const& rayon, MutableColor const& color)
    :CircularBody(coord, rayon), color_(color), direction_(direction), isHungry_(true), energy_(energy), time_(sf::Time::Zero), rotation_(direction_.angle()), isCannibal_(false)
{}

Bacterium::~Bacterium()
{}


///AFFICHAGE ET UPDATE
void Bacterium::drawOn(sf::RenderTarget& target) const
{
    //option de debug (affichage texte que si pas morte)
    if(isDebugOn() and (energy_>0)) {

        //On doit décaler la position du texte en fct de la taille de la bactérie
        Vec2d positionText = getPosition();
        Vec2d Decal(0,getRadius()+1);
        positionText=positionText+Decal;

        //Convertir Quantity en int pour ne garder que la partie entière
        int energy(energy_);

        auto const text = buildText(std::to_string(energy), positionText, getAppFont(), 15, sf::Color::Magenta);
        target.draw(text);
    }

    //Affichage de la bactérie (un cercle de couleur) dans target
    auto const circle = buildCircle(coord_, rayon_, color_.get());
    target.draw(circle);
}



void Bacterium::update(sf::Time dt)
{

    //Update de l'angle de rotation de la bactérie

    auto const angleDiff = angleDelta(direction_.angle(), rotation_); // calcule la différence entre le nouvel
    // angle de direction et l'ancien
    auto dalpha = PI * dt.asSeconds();    // calcule dα
    dalpha = std::min(dalpha, std::abs(angleDiff)); // on ne peut tourner plus que de angleDiff

    dalpha = std::copysign(dalpha, angleDiff); // on tourne dans la direction indiquée par angleDiff
    rotation_ += dalpha; // angle de rotation mis à jour



    //Update de la position/consommation

    time_+=dt;
    //Faire bouger bactérie sur un intervalle dt
    move(dt);


    //Rebondissment sur une paroi ?
    if(getAppEnv().doesCollideWithDish(*this)) { //si elle se cogne contre la paroi elle rebondit en sens inverse
        direction_= -direction_;
    }

    //Mode de nutrition de la bactérie

    if(not(isCannibal_)){
        //Collision avec nutriments ?
        Nutriment* ptr_nutriment(getAppEnv().getNutrimentColliding(*this));
        if(ptr_nutriment != nullptr) { //Si il y a collision la bactérie consomme le nutriment percuté
    
            //critère de consommation : isHungry et si l'intervalle de temps entre deux consommations répond au délai fixé pour le type de bactérie
            if(isHungry_ and (time_ > getDelay() )) {
                eat((*ptr_nutriment));
    
                time_=sf::Time::Zero; //on remet le commpteur à zero
                ptr_nutriment = nullptr;
                delete ptr_nutriment;

            }
        }
       }

    if(isCannibal_){
        //Collision avec bactéries ?
        Bacterium* ptr_bact(getAppEnv().getBacteriumColliding(*this));
        if(ptr_bact != nullptr) { //Si il y a collision la bactérie consomme la bact percutée

            //critère de consommation : isHungry et si l'intervalle de temps entre deux consommations répond au délai fixé pour le type de bactérie
            if(isHungry_ and (time_ > getDelay() )) {
                eat((*ptr_bact));

                time_=sf::Time::Zero; //on remet le commpteur à zero
                ptr_bact = nullptr;
                delete ptr_bact;
            }
        }
    }

    //Bactérie dans zone d'effet d'antibiotiques ?
    double antibioDammage(getAppEnv().getAntibioticScore(coord_));
    if(antibioDammage>0){
        energy_-=antibioDammage/getAntibioticResistanceFactor();  //la résistance aux antibios entre en jeu ici
    }

    //Divison de la bactérie
    divide();

    return;
}



///DIVISION
void Bacterium::divide()
{
    //énergie suffisante ppour divison ?
    if(energy_>getEnergyDivision()) {
        //perte energie de bactérie originelle
        energy_=energy_/2;

        Bacterium* ptr_bact(this->clone());
        this->updateNumber(); //on doit incrémenter le compteur de bactérie

        //Pour un meilleur rendu visuel, on déplace juste la nouvelle bactérie divisée de l'originelle
        Vec2d new_pos(ptr_bact->getPosition() - direction_* 10);
        ptr_bact->CircularBody::move(new_pos - ptr_bact->getPosition());

        ptr_bact->energy_=energy_;                  //la nouvelle bactérie a aussi la moitié de l'energie
        ptr_bact->direction_=-this->direction_;     //la bactérie divisée change de direction

        //mutation bactérie divisée
        ptr_bact->mutate();

        //On ajoute la nvelle bactérie à la collection de bactérie du petri (en attente)
        getAppEnv().addNewBacterium(ptr_bact);

    }

    return;
}



///AUTRES METHODES
bool Bacterium::isDead() const
{
    if(energy_<=0.0) {
        return true;
    }
    return false;
}


void Bacterium::consumeEnergy(Quantity const& qt)
{
    energy_-=qt;
}


void Bacterium::eat(Nutriment& nutriment)
{
 Quantity eaten(nutriment.eatenBy(*this));
 energy_+=eaten;

}

void Bacterium::eat(Bacterium& bact){
    Quantity eaten(bact.takeQuantity(getMaxEatableQuantity()));
    energy_+=eaten;
}


///GETTER ET SETTER DE PROPRIETE MUTABLE
void Bacterium::addProperty(const std::string& name, const MutableNumber& nb_mut)
{

    auto paire = mutableParameters_.find(name);
    if(not(paire==mutableParameters_.end())) {
        throw std::invalid_argument("Clé déjà existante pour ce paramètre mutable");
    }

    mutableParameters_[name]=nb_mut;

}


MutableNumber Bacterium::getProperty(const std::string& name) const
{

    auto paire = mutableParameters_.find(name);

    //paire->fisrt renvoie la clé de paire et second la valeur de paire

    if(paire==mutableParameters_.end()) {
        throw std::invalid_argument("Clé de paramètre mutable numérique introuvable");
    }

    return paire->second;

}

///MODIFIER L'ENERGIE DES BACTERIES LORS CONSOMMATION PAR AUTRES BACTERIES

Quantity Bacterium::takeQuantity(const Quantity& quantite)   //Prélever une qtité de nutriment
{

    if(quantite<0.) { //Erreur si prélevement négatif
        throw std::invalid_argument("erreur la qtité à prélever est négative");
    }
    if(quantite<=energy_) { //Si prélevement est en dessous de la qtité dispo
        energy_=energy_-quantite; //On mets a jour l'énergie de la bactérie
        return quantite;
    }
    energy_=0.;
    return quantite;   //Si la qtité dispo est trop petite, on ne peut que prélever toute la qtité dispo
}



///GETTERS UTILITAIRES
Quantity Bacterium::getEnergyDivision() const
{
    return getConfig()["energy"]["division"].toDouble();
}

sf::Time  Bacterium::getDelay() const
{
    return sf::seconds(getConfig()["meal"]["delay"].toDouble());
}

Quantity  Bacterium::getEnergyMoveRate() const
{
    return getConfig()["energy"]["consumption factor"].toDouble();
}

Quantity Bacterium::getMaxEatableQuantity() const
{
    return getConfig()["meal"]["max"].toDouble();
}


double Bacterium::getAntibioticResistanceFactor() const
{
    return getProperty("antibiotic resistance").get();
}

bool Bacterium::isInCannibalMode() const
{
    return isCannibal_;
}
