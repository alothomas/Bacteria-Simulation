#include "Lab.hpp"
#include "PetriDish.hpp"
#include "Application.hpp"
#include <SFML/Graphics.hpp>


///CONSTRUCTEUR DU LAB

Lab::Lab()
    :petri_(getApp().getCentre(),  0.95*(getApp().getLabSize().x)/2)
{}

Lab::~Lab()
{}



///METHODES DE DESSIN/ UPDATE DU LAB

void Lab::drawOn(sf::RenderWindow& targetWindow) const    //Afficher contenu du lab -> PetriDish
{
    petri_.drawOn(targetWindow);
}

void Lab::update(sf::Time& dt)   //Update le PetriDish
{
    petri_.update(dt);
    nutrimentGenerator_.update(dt);
}



void Lab::reset()    //reset l'UNIQUE assiette petri du lab et son générateur de nutriment
{
    petri_.reset();
    nutrimentGenerator_.reset();
}



///TESTS DE COLLISION

//Le body est il contenu dans l'assiette du lab ?
bool Lab::contains(CircularBody const& body) const
{
    return petri_.contains(body);
}

//Le body est il en collision avec le bord du petri ?
bool Lab::doesCollideWithDish(CircularBody const& body) const
{
    return ((petri_.isColliding(body)) and (not(petri_.contains(body)))); //si le body n'est pas dans le petri et est en collision avec le petri = true
}

//Le body est il en collision avec un nutriment du petri du lab ?
Nutriment* Lab::getNutrimentColliding(CircularBody const& body) const
{
    return petri_.getNutrimentColliding(body);
}


Bacterium* Lab::getBacteriumColliding(Bacterium const& bacteria) const
{
    return petri_.getBacteriumColliding(bacteria);
}



///AJOUT D'ELEMENTS AU PETRI DU LAB
void Lab::addNutriment(Nutriment* nutriment)   //ajout de nutriment au lab seulement si il est contenu dans son petriDIsh -> petri
{
    if(contains(*nutriment)) {
        petri_.addNutriment(nutriment);
    }
}


void Lab::addBacterium(Bacterium* bacterie)  //ajout une bactérie au lab si il est contenu dans le petri
{
    if(contains(*bacterie)) {
        petri_.addBacterium(bacterie);
    }
}

void Lab::addNewBacterium(Bacterium* bacterie)
{
    if(contains(*bacterie)) {
        petri_.addNewBacterium(bacterie);
    }

}

void Lab::addSwarm(Swarm* swarm) //ajout d'un swarm au petri
{

    if(not(petri_.addSwarm(swarm))) {
        throw std::invalid_argument("ERREUR pas de swarm associé à cet ID ! ");
    }

}


void Lab::addAntibiotic(Antibiotic* antibiotic)
{
    if(contains(*antibiotic)){
        petri_.addAntibiotic(antibiotic);
    }
}


///GETTER DE SWARM
Swarm* Lab::getSwarmWithId(std::string const& id)
{
    return petri_.getSwarmWithId(id);
}




///TEMPERATURE
void Lab::increaseTemperature()
{
    petri_.increaseTemperature(); //C'est au petri d'augmenter sa temp car le lab peut avoir plusieurs petri a differentes températures
}


void Lab::decreaseTemperature()
{
    petri_.decreaseTemperature();
}

//getter de température de petri
double Lab::getTemperature() const //Getter de la température du lab
{
    return petri_.getTemperature();
}

void Lab::resetTemperature()   //reset la température du Petri à Default
{
    petri_.resetTemperature();
}


///GRADIENT DE NUTRIMENT
//Renvoie le gradient au point coord du petri
double Lab::getPositionScore(const Vec2d& coord) const
{
    return petri_.getPositionScore(coord);
}

double Lab::getGradientExponent() const
{
    return petri_.getGradientExponent();
}

void Lab::increaseGradientExponent()
{
    petri_.increaseGradientExponent();
}

void Lab::decreaseGradientExponent()
{
    petri_.decreaseGradientExponent();
}

void Lab::resetGradientExponent()
{
    petri_.resetGradientExponent();
}


///GRADIANT DE ANTIBIOTIQUE
double Lab::getAntibioticScore(const Vec2d& coord) const
{
    return petri_.getAntiboticScore(coord);
}

///GRADIANT DE BACTERIE
double Lab::getBacteriumScore(const Vec2d& coord) const
{
    return petri_.getBacteriumScore(coord);
}

///RECUPERER NOUVELLES DATA DU LAB
std::unordered_map<std::string, double> Lab::fetchData(const std::string& s) const
{
    std::unordered_map<std::string, double> new_data;

    if(s == s::GENERAL){
        new_data[s::SIMPLE_BACTERIA] = petri_.getSimpleBacteriumNumber();
        new_data[s::TWITCHING_BACTERIA] = petri_.getTwitchingBacteriumNumber();
        new_data[s::SWARM_BACTERIA] = petri_.getSwarmBacteriumNumber();
        new_data[s::NUTRIMENT_SOURCES] = petri_.getNutrimentsNumber();
        new_data[s::DISH_TEMPERATURE]= getTemperature();
        return new_data;
    }


    if(s == s::NUTRIMENT_QUANTITY){
        new_data[s::NUTRIMENT_QUANTITY] = petri_.getNutrimentQuantity();
        return new_data;
     }

    if(s == s::BACTERIA){
        new_data[s::SPEED] = petri_.getAverageSpeed();
        return new_data;
    }


   if(s == s::TWITCHING_BACTERIA){
       new_data[s::TENTACLE_LENGTH] = petri_.getAverageTentacleLength();
       new_data[s::TENTACLE_SPEED] = petri_.getAverageTentacleSpeed();
       return new_data;
    }

   if(s == s::ANTIBIOTIC_RESISTANCE){
       new_data[s::RESISTANCE] = petri_.getAverageAntibioticResistance();
       return new_data;
    }

    throw std::invalid_argument("impossible de récupérer la data du lab: mauvais titre demandé !");

}


///AUGMENTER/DIMINUER LE NOMBRE DE BACTERIES PAR TYPE DU PETRI
void Lab::increaseSimpleBacteriumNumber()
{
    petri_.increaseSimpleBacteriumNumber();
}

void Lab::increaseTwitchingBacteriumNumber()
{
    petri_.increaseTwitchingBacteriumNumber();
}

void Lab::increaseSwarmBacteriumNumber()
{
    petri_.increaseSwarmBacteriumNumber();
}



void Lab::decreaseSimpleBacteriumNumber()
{
    petri_.decreaseSimpleBacteriumNumber();
}

void Lab::decreaseTwitchingBacteriumNumber()
{
    petri_.decreaseTwitchingBacteriumNumber();
}

void Lab::decreaseSwarmBacteriumNumber()
{
    petri_.decreaseSwarmBacteriumNumber();
}



///Update tentacle speed/length average
void Lab::updateTentacleLength(double const& length)
{
    petri_.updateTentacleLength(length);
}

void Lab::updateTentacleSpeed(double const& speed)
{
    petri_.updateTentacleSpeed(speed);
}
