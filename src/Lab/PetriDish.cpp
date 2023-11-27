#include "PetriDish.hpp"
#include <vector>
#include "./Utility/Utility.hpp"
#include <SFML/Graphics.hpp>
#include "Application.hpp"
#include <cmath>


///CONSTRUCTEURS ET DESTRUCTEURS

PetriDish::PetriDish(const Vec2d& coord, double rayon)
    :CircularBody(coord,rayon), temperature_(getAppConfig()["petri dish"]["temperature"]["default"].toDouble()), puissance_( (getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble() + getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble())/2 )
{
    resetBacteriumTypeNumber();
    resetTentacleAverages();
}


PetriDish::~PetriDish()  //Libère mémoire
{
    if(not(bacteries_.empty())) {
        for(auto& bacterie : bacteries_) {
            delete bacterie;
        }
    }

    if(not(nutriments_.empty())) {
        for(auto& nutriment : nutriments_) {
            delete nutriment;
        }
    }

    //on supprime les nouvelles bactéries si elles n'ont pas encore eu le temps d'etre fusionnées
    if(not(new_bacteries_.empty())) {
        for(auto& bacterie : new_bacteries_) {
            delete bacterie;
        }
    }

    if(not(antibiotiques_.empty())) {
        for(auto& antibio : antibiotiques_) {
            delete antibio;
        }
    }

}


///AJOUTER DES BACTERIES / NUTRIMENTS

bool PetriDish::addBacterium(Bacterium* bacterie)
{
    if(contains(bacterie->getPosition())) {  //on ajoute la bacterie si sa position est a l'interieur du petri
        bacteries_.push_back(bacterie);

        return true;
    }
    delete bacterie;
    return false;
}


bool PetriDish::addNutriment(Nutriment* nutriment)
{

    if(contains(nutriment->getPosition())) {  //on ajoute le nutriment si sa position est a l'interieur du petri
        nutriments_.push_back(nutriment);
        return true;
    }
    delete nutriment;
    return false;
}

bool PetriDish::addNewBacterium(Bacterium* bacterie)
{
    new_bacteries_.push_back(bacterie);  //pas obligé de tester si bactérie est dans petri
    return true;                         //car la nvelle bactérie apparait à la position de celle qui se divise
}

bool PetriDish::addSwarm(Swarm* swarm)
{
    swarms_.push_back(swarm);
    return true;
}


bool PetriDish::addAntibiotic(Antibiotic* antibio)
{
    if(contains(antibio->getPosition())) {  //on ajoute l'antibio si sa position est a l'interieur du petri
        antibiotiques_.push_back(antibio);
        return true;
    }
    delete antibio;
    return false;
}



///GETTER DE SWARM
Swarm* PetriDish::getSwarmWithId(std::string const& id)
{
    if(swarms_.empty()) {
        throw std::invalid_argument("ERREUR Collection de swarms vide !");
    }

    for(const auto& swarm : swarms_) {
        if(swarm->getId() == id) {
            return swarm;
        }

    }

    throw std::invalid_argument("ERREUR Pas de swarm avec l'ID correspondant ! ");

}


///TESTS DE COLLISION
Nutriment* PetriDish::getNutrimentColliding(CircularBody const& body) const
{
    for(const auto& nutriment : nutriments_)
        if(nutriment->isColliding(body)) {
            return nutriment;
        }
    return nullptr;
}

Bacterium* PetriDish::getBacteriumColliding(Bacterium const& bacteria) const
{
    for(const auto& bact : bacteries_){
        if(bact != nullptr){
            if((bact->isColliding(bacteria)) and (bact->getPosition() != bacteria.getPosition())) {
                return bact;
            }
        }
    }
    return nullptr;
}

///TEMPERATURE
double PetriDish::getTemperature() const //getter de temperature du petri
{
    return temperature_;
}

void PetriDish::increaseTemperature()
{
    if( (temperature_ + getAppConfig()["petri dish"]["temperature"]["delta"].toDouble()) > getAppConfig()["petri dish"]["temperature"]["max"].toDouble()) {
        temperature_=getAppConfig()["petri dish"]["temperature"]["max"].toDouble();
        return;
    }

    if( (temperature_ + getAppConfig()["petri dish"]["temperature"]["delta"].toDouble()) < getAppConfig()["petri dish"]["temperature"]["min"].toDouble()) {
        temperature_=getAppConfig()["petri dish"]["temperature"]["min"].toDouble();
        return;
    }

    temperature_ += getAppConfig()["petri dish"]["temperature"]["delta"].toDouble();
}

void PetriDish::decreaseTemperature()
{
    if( (temperature_ - getAppConfig()["petri dish"]["temperature"]["delta"].toDouble()) > getAppConfig()["petri dish"]["temperature"]["max"].toDouble()) {
        temperature_=getAppConfig()["petri dish"]["temperature"]["max"].toDouble();
        return;
    }

    if( (temperature_ - getAppConfig()["petri dish"]["temperature"]["delta"].toDouble()) < getAppConfig()["petri dish"]["temperature"]["min"].toDouble()) {
        temperature_=getAppConfig()["petri dish"]["temperature"]["min"].toDouble();
        return;
    }

    temperature_ -= getAppConfig()["petri dish"]["temperature"]["delta"].toDouble();

}

void PetriDish::resetTemperature()
{
    temperature_=getAppConfig()["petri dish"]["temperature"]["default"].toDouble();
}



///GRADIENT DE NUTRIMENTS
double PetriDish::getPositionScore(const Vec2d& coord) const
{
    double score(0.);
    if(not(nutriments_.empty())) {
        for(const auto& nutriment : nutriments_) {
            if(nutriment != nullptr) {
                score+= (nutriment->getRadius() / pow((distance(coord, nutriment->getPosition())), puissance_) );
            }
        }
    }
    return score;
}

double PetriDish::getGradientExponent() const
{
    return puissance_;
}

void PetriDish::increaseGradientExponent()
{
    if((puissance_ + getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble()) > getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble() ) {
        puissance_=getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble();
        return;
    }

    if((puissance_ + getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble()) < getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble() ) {
        puissance_=getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble();
        return;
    }

    puissance_+=getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble();
}

void PetriDish::decreaseGradientExponent()
{
    if((puissance_ - getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble()) > getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble() ) {
        puissance_=getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble();
        return;
    }

    if((puissance_ - getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble()) < getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble() ) {
        puissance_=getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble();
        return;
    }

    puissance_-=getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble();

}

void PetriDish::resetGradientExponent()
{
    puissance_=((getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble() + getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble())/2);

}

///GRADIANT D'ANTIBIOTIQUE
double PetriDish::getAntiboticScore(const Vec2d& coord) const
{
    double score(0.);
    if(not(antibiotiques_.empty())) {
        for(const auto& antibio : antibiotiques_) {
            if((antibio != nullptr) and (antibio->isInEffectZone(coord))) {
                score+= (antibio->getRadius() / pow((distance(coord, antibio->getPosition())), antibio->getPower()) );
                //le score dépend de la dose d'antibio, de sa puissance et de la distance de la bactérie à l'antibio
            }
        }
    }
    return score;
}


///GRADIANT DE BACTERIES
double PetriDish::getBacteriumScore(const Vec2d& coord) const
{
    //REMARQUE : la perception du gradiant avec la puissance est la même que pour les nutriments
    // la perception de la bactérie ne change pas (elle ne "voit" pas mieux les bactéries que les nutriments)
    double score(0.);
    if(not(bacteries_.empty())) {
        for(const auto& bacteria : bacteries_) {
            if((bacteria != nullptr) and (bacteria->getPosition() != coord)) {
                score+= (bacteria->getRadius() / pow((distance(coord, bacteria->getPosition())), puissance_) );
            }
        }
    }
    return score;

}



///DESSIN ET UPDATE DES BACTERIES ET NUTRIMENTS

void PetriDish::update(sf::Time dt)  //UPDATE TOUS LES ELEMENTS DU PETRI
{

    //On réinitialise les vitesses des tentacules pour que les stats s'appliquent à chaque intervalle dt
    resetTentacleAverages();

    //Update les nutriments de l'assiette de petri (supprimes les nutriments morts/Croissance)
    for(auto& nutriment : nutriments_) {
        //Si le nutriment a une qtité nulle (=rayon nul), on le détruit sinon on l'update.
        if((nutriment->getRadius())<=0.0) {
            delete nutriment;
            nutriment = nullptr;
        }
        if(nutriment != nullptr) {
            nutriment->update(dt);
        }
    }
    nutriments_.erase(std::remove(nutriments_.begin(), nutriments_.end(), nullptr), nutriments_.end());


    //Update des antibiotiques
    for(auto& antibio : antibiotiques_) {
        //Si l'antibio a une qtité nulle (=rayon nul), on le détruit sinon on l'update.
        if((antibio->getRadius())<=0.0) {
            delete antibio;
            antibio = nullptr;
        }
        if(antibio != nullptr) {
            antibio->update(dt);
        }
    }
    antibiotiques_.erase(std::remove(antibiotiques_.begin(), antibiotiques_.end(), nullptr), antibiotiques_.end());


    //fusion des nouv bactéries avec les anciennes
    append(new_bacteries_,bacteries_);
    new_bacteries_.clear();


    //Update les bactéries de l'assiette de petri
    for(auto& bacterie : bacteries_) {
        //Si la bactérie a une energie nulle elle est morte
        if(bacterie->isDead()) {
            delete bacterie;
            bacterie = nullptr;
        }
        if(bacterie != nullptr) {
            bacterie->update(dt);
        }
    }
    bacteries_.erase(std::remove(bacteries_.begin(), bacteries_.end(), nullptr), bacteries_.end());


    //Update des swarms
    for(auto& swarm : swarms_) {
        swarm->update(dt);
    }
}



void PetriDish::drawOn(sf::RenderTarget& targetWindow) const //AFFICHE LE PETRI (Anneau / Nutriments / Bacteries)  dans targetWindow
{

    //Afficher le cercle du petri
    auto border = buildAnnulus(coord_, rayon_, sf::Color::Black, 5);
    targetWindow.draw(border);

    //Afficher les nutriments de l'assiette de petri
    for(const auto& nutriment : nutriments_) { //On doit afficher tous les nutriments (qui ont un rayon >0) donc on itere sur tout le tableau
        nutriment->drawOn(targetWindow);
    }

    //Afficher les antibiotiques de l'assiette de petri
    for(const auto& antibio : antibiotiques_) {
        antibio->drawOn(targetWindow);
    }

    //Afficher les bactéries de l'assiette de petri
    for(const auto& bacterie : bacteries_) {
        bacterie->drawOn(targetWindow);
    }

}



///SUPPRIMER NUTRIMENTS/BACTERIES DE PETRI

void PetriDish::reset()   //supprimes toutes les bactéries et nutriments de l'assiette et remets température à default
{

    if(not(bacteries_.empty())) {
        for(auto& bacterie : bacteries_) {
            delete bacterie;
        }
    }
    bacteries_.clear();

    if(not(nutriments_.empty())) {
        for(auto& nutriment : nutriments_) {
            delete nutriment;
        }
    }
    nutriments_.clear();

    //on supprime les nouvelles bactéries si elles n'ont pas encore eu le temps d'etre fusionnées
    if(not(new_bacteries_.empty())) {
        for(auto& bacterie : new_bacteries_) {
            delete bacterie;
        }
    }
    new_bacteries_.clear();

    if(not(antibiotiques_.empty())) {
        for(auto& antibio : antibiotiques_) {
            delete antibio;
        }
    }
    antibiotiques_.clear();

    resetTemperature();
    resetGradientExponent();
    resetBacteriumTypeNumber();
}


///AUTRES METHODES

void PetriDish::resetBacteriumTypeNumber()
{
    SimpleBacteriumNumber_ = 0;
    TwitchingBacteriumNumber_ = 0;
    SwarmBacteriumNumber_ = 0;
}

Quantity PetriDish::getNutrimentQuantity() const
{
    Quantity totalQuantity(0.);
    for(const auto& nutriment : nutriments_){
        totalQuantity +=nutriment->getQuantity();
    }

    return totalQuantity;
}

int PetriDish::getNutrimentsNumber() const
{
    return nutriments_.size();
}

int PetriDish::getSimpleBacteriumNumber() const
{
    return SimpleBacteriumNumber_;
}

int PetriDish::getTwitchingBacteriumNumber() const
{
    return TwitchingBacteriumNumber_;

}
int PetriDish::getSwarmBacteriumNumber() const
{
    return SwarmBacteriumNumber_;

}


void PetriDish::increaseSimpleBacteriumNumber()
{
    SimpleBacteriumNumber_+=1;
}

void PetriDish::increaseTwitchingBacteriumNumber()
{
    TwitchingBacteriumNumber_+=1;
}

void PetriDish::increaseSwarmBacteriumNumber()
{
    SwarmBacteriumNumber_+=1;
}



void PetriDish::decreaseSimpleBacteriumNumber()
{
    SimpleBacteriumNumber_-=1;
}

void PetriDish::decreaseTwitchingBacteriumNumber()
{
    TwitchingBacteriumNumber_-=1;
}

void PetriDish::decreaseSwarmBacteriumNumber()
{
    SwarmBacteriumNumber_-=1;
}


//Vitesse moyenne des bactéries
double PetriDish::getAverageSpeed() const
{
    double speed(0.);
    //les twitching bacterium n'ont pas vraiment de vitesse donc on ne les comptabilise pas

    for(const auto& bacterie : bacteries_){
        speed+=bacterie->getBacteriumSpeed();
    }
    return (speed/(SimpleBacteriumNumber_ + SwarmBacteriumNumber_));

}


//Calcul vitesse moyenne tentacules et longueurs
void PetriDish::updateTentacleSpeed(double const& speed)
{
    tentacleSpeed_ += speed;
}

void PetriDish::updateTentacleLength(double const& length)
{
    tentacleLength_ += length;

}

//getter of tentacule average of the petri
double PetriDish::getAverageTentacleSpeed() const
{
    return tentacleSpeed_/TwitchingBacteriumNumber_;
}

double PetriDish::getAverageTentacleLength() const
{
    return tentacleLength_/TwitchingBacteriumNumber_;
}



void PetriDish::resetTentacleAverages()
{
    tentacleLength_ = 0.;
    tentacleSpeed_ = 0.;
}



//Getter de facteur résistance antibio
double PetriDish::getAverageAntibioticResistance() const
{
    double average(0.);
    double total(0.);
    for(const auto& bacterie : bacteries_){
        if(bacterie !=nullptr){
            average += bacterie->getProperty("antibiotic resistance").get();
            total+=1;
        }
    }
    return average/total;
}
