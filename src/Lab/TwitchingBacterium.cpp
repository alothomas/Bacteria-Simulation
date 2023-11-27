#include "TwitchingBacterium.hpp"
#include "Application.hpp"
#include "./Random/Random.hpp"
#include "./Utility/Utility.hpp"
#include "Nutriment.hpp"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"


TwitchingBacterium::TwitchingBacterium(Vec2d const& coord)
    :Bacterium(uniform(getConfig()["energy"]), coord, Vec2d::fromRandomAngle(), uniform(getConfig()["radius"]), MutableColor(getConfig()["color"])), grip_(Grip(coord, rayon_/4)), state_(IDLE) //score_(getAppEnv().getPositionScore(coord)), ancien_score_(score_),
{
    addProperty("tentacle length", MutableNumber::positive(getConfig()["tentacle"]["length"]));
    addProperty("tentacle speed", MutableNumber::positive(getConfig()["tentacle"]["speed"]));
    addProperty("antibiotic resistance", MutableNumber::positive(getConfig()["antibiotic resistance"]));

    updateNumber();
}


TwitchingBacterium::~TwitchingBacterium()
{
    getAppEnv().decreaseTwitchingBacteriumNumber();
}




///AFFICHAGE ET UPDATE
void TwitchingBacterium::update(sf::Time dt)
{
    Bacterium::update(dt);
    getAppEnv().updateTentacleSpeed(getProperty("tentacle speed").get());
    getAppEnv().updateTentacleLength(getProperty("tentacle length").get());
}


void TwitchingBacterium::drawOn(sf::RenderTarget& target) const
{

    //Dessin du tentacule
    auto line = buildLine(grip_.getPosition(), coord_, color_.get(), 1);
    target.draw(line);

    //Dessin du grappin
    auto border = buildAnnulus(grip_.getPosition(), grip_.getRadius(), color_.get(), 1);
    target.draw(border);

    //Dessin du crops de la bactérie
    Bacterium::drawOn(target);
}





///DEPLACEMENT
void TwitchingBacterium::move(sf::Time dt) //ne fait rien pour l'instant
{

    Nutriment* nut_ptr = nullptr;

    //selon le state_ de la bactérie, elle fera certaines actions :
    switch(state_) {


    //tentacule au repos
    case IDLE :

        state_ = WAIT_TO_DEPLOY; //on passe à l'état suivant
        break;



    //Tentacule en préparation pour déploiement
    case WAIT_TO_DEPLOY : {

        Vec2d dir(Vec2d::fromRandomAngle()), dir_al(dir);
        for(int i(0); i<20; ++i) {
            //La bactérie va se diriger la ou il y a le plus de nutriments
            if(getAppEnv().getPositionScore(coord_ + dir_al) >= getAppEnv().getPositionScore(coord_ + dir) ) {
                dir=dir_al;
            }
            dir_al=Vec2d::fromRandomAngle();
        }
        direction_=dir;
        state_ = DEPLOY;
        break;
    }



    //Tentacule se déploie
    case DEPLOY : {
        //on bouge grappin si c'est possible
        if(not(getAppEnv().doesCollideWithDish(grip_)) and ((coord_-grip_.getPosition()).length() <= getTentacleLength())) {

            moveGrip( (direction_*getTentacleSpeed()*dt.asSeconds()) );

            //consomation d'énergie du grappin
            consumeEnergy(getEnergyTentacle()* getTentacleSpeed()* dt.asSeconds());


            //on teste si il y a des nutriments au grappin
            nut_ptr = getAppEnv().getNutrimentColliding(grip_);
            if( nut_ptr != nullptr) {
                state_= ATTRACT; // il y a de la nourriture!
                break;
            }
            break; //on continue deploy
        }
        state_ = RETRACT; // si on ne peut plus deploy, on retract
    }


    //Tentacule attire bactérie vers nourriture
    case ATTRACT : {
        nut_ptr =  getAppEnv().getNutrimentColliding(grip_);

        //nutriment toujours vivant ?
        if(nut_ptr != nullptr) {

            //bactérie en collision avec nutriment ?
            if(isColliding(*nut_ptr)) {
                state_ = EAT;
                break;
            }

            //sinon la bactérie va vers le nutriment
            Vec2d last_coord(coord_);
            Vec2d dir_tentacule( (grip_.getPosition() - coord_).normalised() );
            CircularBody::move( (dir_tentacule * getTentacleSpeed() * getSpeedFactor()* dt.asSeconds()));
            consumeEnergy( (coord_-last_coord).length() * getEnergyMoveRate() );

            break;
        }

        //si le nutriment est mort
        state_ = RETRACT;
        break;
    }



    //La bactérie fait ripaille
    case EAT :
        //La bactérie arrête de manger si plus de contact avec nutriment
        if(getAppEnv().getNutrimentColliding(*this) == nullptr) {
            state_ = IDLE;
            break;
        }
        break;



    //Tentacule se rétracte
    case RETRACT : {
        //tentacule complètement rétracté ?
        if(distance(coord_, grip_.getPosition()) <= rayon_) {
            state_ = IDLE;
            break;
        }

        //sinon on replie le tentacule
        Vec2d dir_tentacule( (coord_ - grip_.getPosition()).normalised() );
        moveGrip( (dir_tentacule * getTentacleSpeed() * getSpeedFactor()* dt.asSeconds()));
        consumeEnergy(getEnergyTentacle()* getTentacleSpeed()* dt.asSeconds());
        break;
    }

    default :
        break;

    }
}



//DEPLACEMENT DU GRAPPIN
void TwitchingBacterium::moveGrip(Vec2d const& position) //bouger de delta la position
{
    grip_.moveGrip(position);
}


///DIVISON ET MUTATION
Bacterium* TwitchingBacterium::clone() const
{
    return new TwitchingBacterium(*this);
}


void TwitchingBacterium::mutate()
{
    //TwitchingBacterium peut muter sa couleur, longueur max tentacule et sa vitesse
    color_.mutate();
    mutableParameters_["tentacle length"].mutate();
    mutableParameters_["tentacle speed"].mutate();
    mutableParameters_["antibiotic resistance"].mutate();

}

//Consommation de nutriment
Quantity TwitchingBacterium::eatableQuantity(NutrimentA& nutriment)
{
    return nutriment.eatenBy(*this);
}

Quantity TwitchingBacterium::eatableQuantity(NutrimentB& nutriment)
{
    return nutriment.eatenBy(*this);
}


///GETTERS UTILES
Quantity TwitchingBacterium::getEnergyTentacle() const
{
    return getConfig()["energy"]["consumption factor"]["tentacle"].toDouble();
}


Quantity TwitchingBacterium::getEnergyMoveRate() const
{
    return getConfig()["energy"]["consumption factor"]["move"].toDouble();
}


double TwitchingBacterium::getTentacleSpeed() const
{
    return getProperty("tentacle speed").get();
}

double TwitchingBacterium::getTentacleLength() const
{
    return getProperty("tentacle length").get();
}

double TwitchingBacterium::getSpeedFactor() const
{
    return getConfig()["speed factor"].toDouble();
}

void TwitchingBacterium::updateNumber()
{
    getAppEnv().increaseTwitchingBacteriumNumber();

}



//Getter de vitesse d'une bactérie
 double TwitchingBacterium::getBacteriumSpeed() const
 {
     return 0.;
 }



j::Value const& TwitchingBacterium::getConfig() const
{
    return getAppConfig()["twitching bacterium"];
}

