#include "SwarmBacterium.hpp"
#include "Application.hpp"
#include "./Utility/Utility.hpp"
#include "./Random/Random.hpp"
#include "Nutriment.hpp"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"


SwarmBacterium::SwarmBacterium(Vec2d const& coord, Swarm* swarm)
    :Bacterium(uniform(getConfig()["energy"]), coord, Vec2d::fromRandomAngle(), uniform(getConfig()["radius"]), MutableColor(swarm->getSwarmInitialColor())), swarmId_(swarm->getId())
{
    addProperty("antibiotic resistance", MutableNumber::positive(getConfig()["antibiotic resistance"]));
    swarm->addSwarmBacterium(this);
    updateNumber();
}


SwarmBacterium::~SwarmBacterium()
{
    getAppEnv().decreaseSwarmBacteriumNumber();
    getAppEnv().getSwarmWithId(swarmId_)->removeSwarmBacterium(this);
}




///UPDATE ET AFFICHAGE
void SwarmBacterium::update(sf::Time dt)
{
    Bacterium::update(dt);
}


void SwarmBacterium::drawOn(sf::RenderTarget& target) const
{

    //Dessin anneau rouge pour le leader en mode debug
    if(isDebugOn() and (coord_ == getAppEnv().getSwarmWithId(swarmId_)->getLeaderPosition())) {

        auto border = buildAnnulus(coord_, 30, sf::Color::Red, 2);
        target.draw(border);
    }

    //Dessin du corps de la bactérie
    Bacterium::drawOn(target);
}



///DEPLACEMENT
void SwarmBacterium::move(sf::Time dt) //Méthode virtuelle pure : fais bouger une bactérie
{

    //CALCUL NOUVELLE POSITION ET VITESSE

    Vec2d last_coord(coord_); //ancienne position
    DiffEqResult result(stepDiffEq(coord_, getSpeedVector(), dt, *this, DiffEqAlgorithm::EC));

    //nouvelle position et vitesse que si déplacement assez important (pour éviter tremblements)
    if(result.position.lengthSquared()>0.001) {

        CircularBody::move(result.position - last_coord); //nouvelle position

        //Baisse d'énergie liée au déplacement
        consumeEnergy( (coord_-last_coord).length() * getEnergyMoveRate() );
    }


    //Si la bactérie est le leader elle choisit nouvelle direction
    if(coord_==getAppEnv().getSwarmWithId(swarmId_)->getLeaderPosition()) {

        Vec2d dir(Vec2d::fromRandomAngle()), dir_al(dir);
        for(int i(0); i<20; ++i) {

            if(getAppEnv().getPositionScore(this->getPosition() + dir_al) >= getAppEnv().getPositionScore(this->getPosition() + dir) ) {
                dir=dir_al;
            }
            dir_al=Vec2d::fromRandomAngle();
        }
        direction_=dir;

    }

    return;
}



///CLONAGE ET MUTATION
Bacterium* SwarmBacterium::clone() const
{
    //Ajout de la bactérie divisée au swarm
    SwarmBacterium* new_bact(new SwarmBacterium(*this));
    getAppEnv().getSwarmWithId(swarmId_)->addSwarmBacterium(new_bact);
    return new_bact;
}


void SwarmBacterium::mutate()  //mutation des param mutables de la bactérie
{
    color_.mutate();
    mutableParameters_["antibiotic resistance"].mutate();

}



///UTILITAIRE
Vec2d SwarmBacterium::f(Vec2d position, Vec2d speed) const  //la force exercée par le leader dépend du type de swarm auquel la bactérie appartient
{
    return getAppConfig()["swarms"][swarmId_]["force factor"].toDouble() * (getAppEnv().getSwarmWithId(swarmId_)->getLeaderPosition() - coord_);
}


//Calcule et renvoie la vitesse de la bactérie
Vec2d SwarmBacterium::getSpeedVector() const
{
    return (direction_ * getConfig()["speed"]["initial"].toDouble());
}


//Consommation de nutriment
Quantity SwarmBacterium::eatableQuantity(NutrimentA& nutriment)
{
    return nutriment.eatenBy(*this);
}

Quantity SwarmBacterium::eatableQuantity(NutrimentB& nutriment)
{
    return nutriment.eatenBy(*this);
}

std::string SwarmBacterium::getSwarmBacteriumId() const
{
    return swarmId_;
}


void SwarmBacterium::updateNumber()
{
    getAppEnv().increaseSwarmBacteriumNumber();

}



//Getter de vitesse d'une bactérie
double SwarmBacterium::getBacteriumSpeed() const
{
    return getConfig()["speed"]["initial"].toDouble();
}


j::Value const& SwarmBacterium::getConfig() const
{
    return getAppConfig()["swarm bacterium"];
}
