#include "SimpleBacterium.hpp"
#include "Application.hpp"
#include "./Random/Random.hpp"
#include "./Utility/Utility.hpp"
#include "Nutriment.hpp"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"

///CONSTRUCTEUR
SimpleBacterium::SimpleBacterium(Vec2d const& coord)
    :Bacterium(uniform(getConfig()["energy"]), coord, Vec2d::fromRandomAngle(), uniform(getConfig()["radius"]), MutableColor(getConfig()["color"])), t_(uniform(0., PI)), t_basculement_(0.), score_(getAppEnv().getPositionScore(coord)), ancien_score_(score_)
{
    addProperty("speed", MutableNumber::positive(getConfig()["speed"]));
    addProperty("tumble better", MutableNumber::positive(getConfig()["tumble"]["better"]));
    addProperty("tumble worse",MutableNumber::positive(getConfig()["tumble"]["worse"]));
    addProperty("antibiotic resistance", MutableNumber::positive(getConfig()["antibiotic resistance"]));

    updateNumber();
}

SimpleBacterium::~SimpleBacterium()
{
    getAppEnv().decreaseSimpleBacteriumNumber();
}



///UPDATE ET AFFICHAGE
void SimpleBacterium::update(sf::Time dt)
{
    Bacterium::update(dt);
}


void SimpleBacterium::drawOn(sf::RenderTarget& target) const
{
    //Dessin du flagelle
    auto set_of_points = sf::VertexArray(sf::LinesStrip);
    // ajout de points à l'ensemble:

    double x(0.), y(0.);
    set_of_points.append( {{0,0}, color_.get()} );

    for(int i(1); i<=30; ++i) { //On complete l'ensemble de points pour faire un sinus
        x= -i * rayon_/10.0;
        y= rayon_ * sin(t_) * sin(2*i/10.0);

        set_of_points.append( {{static_cast<float>(x), static_cast<float>(y)}, color_.get()} );
    }

    auto transform = sf::Transform(); // déclare une matrice de transformation

    // ici ensemble d'opérations comme des translations ou rotations faites  sur transform:
    transform.translate(coord_); //on mets les points sur le centre de la bactérie
    transform.rotate(rotation_/DEG_TO_RAD); //angle en degré

    // puis:
    target.draw(set_of_points, transform); // dessin de l'ensemble des points
    // fait après leur transformation
    //selon la matrice transform


    //Dessin du corps de la bactérie simple
    Bacterium::drawOn(target);
}



///DEPLACEMENT
void SimpleBacterium::move(sf::Time dt)
{
//BASCULEMENT

    //Tentative de basculement
    t_basculement_+=dt.asSeconds(); //Temps depuis le dernier basculement
    score_ = getAppEnv().getPositionScore(this->getPosition()); //calcul du nouveau score
    double lambda(0.);

    if(score_ >= ancien_score_) {
        lambda=getProperty("tumble better").get(); //meilleur cas
    } else {
        lambda=getProperty("tumble worse").get();  //pire cas
    }

    ancien_score_ = score_; //on enregistre l'ancien score

    //Basculement
    if(bernoulli(1-exp(-t_basculement_/lambda))) {
        std::string name_algo(getConfig()["tumble"]["algo"].toString());
        //Différents algos (on pourra en ajouter dans JSON -> ajouter ici le nouvel algo)

        //algo 1
        if( name_algo=="single random vector") {
            direction_ = Vec2d::fromRandomAngle();
        }


        //algo 2
        if( name_algo=="best of N") {
            Vec2d dir(Vec2d::fromRandomAngle()), dir_al(dir);
            for(int i(0); i<20; ++i) {

                if(getAppEnv().getPositionScore(this->getPosition() + dir_al) >= getAppEnv().getPositionScore(this->getPosition() + dir) ) {
                    dir=dir_al;
                }
                dir_al=Vec2d::fromRandomAngle();
            }
            direction_=dir;

        }

        t_basculement_=0.; //il ya eu basculement on reset le timer
    }



//CALCUL NOUVELLE POSITION ET VITESSE

    computeNewPosAndSpeed(dt);
    return;
}



///UTILITAIRE


void SimpleBacterium::computeNewPosAndSpeed(sf::Time dt)
{
    Vec2d last_coord(coord_); //ancienne position
    DiffEqResult result(stepDiffEq(coord_, getSpeedVector(), dt, *this, DiffEqAlgorithm::EC));

    //nouvelle position et vitesse que si déplacement assez important (pour éviter tremblements)
    if(result.position.lengthSquared()>0.001) {
        t_+= 2*dt.asSeconds(); //compteur pour flagelle / à augmenter si on veut flagelle plus "agitée"

        CircularBody::move(result.position - last_coord); //nouvelle position
        //pas de nouvelle vitesse car la bactérie simple se déplace à v=const

        //Baisse d'énergie liée au déplacement
        consumeEnergy( (coord_-last_coord).length() * getEnergyMoveRate() );
        return;
    }
    return;
}


//Calcule et renvoie la vitesse de la bactérie
Vec2d SimpleBacterium::getSpeedVector() const
{
    return (direction_ * getProperty("speed").get());
}


//Redéfinition de la méthode de calcul de la force de la classe DiffEqFunction (= une force)
Vec2d SimpleBacterium::f(Vec2d position, Vec2d speed) const
{
    return Vec2d(0.,0.);   //On fixe la force a 0 car déplacement rect uniforme
}


//Clone une bactérie simple
Bacterium* SimpleBacterium::clone() const
{
    return new SimpleBacterium(*this); //copie polymorphique
}


void SimpleBacterium::mutate()
{
    //Une bactérie simple peut muter sa couleur/vitesse/lamba
    color_.mutate();
    mutableParameters_["speed"].mutate();
    mutableParameters_["tumble better"].mutate();
    mutableParameters_["tumble worse"].mutate();
    mutableParameters_["antibiotic resistance"].mutate();
    return;
}


//Consommation de nutriment
Quantity SimpleBacterium::eatableQuantity(NutrimentA& nutriment)
{
    return nutriment.eatenBy(*this);
}

Quantity SimpleBacterium::eatableQuantity(NutrimentB& nutriment)
{
    return nutriment.eatenBy(*this);
}


void SimpleBacterium::updateNumber()
{
    getAppEnv().increaseSimpleBacteriumNumber();
}

//Getter de vitesse d'une bactérie
double SimpleBacterium::getBacteriumSpeed() const
{
    return getProperty("speed").get();
}



j::Value const& SimpleBacterium::getConfig() const
{
    return getAppConfig()["simple bacterium"];
}
