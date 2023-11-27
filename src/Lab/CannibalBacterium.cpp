#include "CannibalBacterium.hpp"
#include "Application.hpp"
#include "./Random/Random.hpp"
#include "./Utility/Utility.hpp"


CannibalBacterium::CannibalBacterium(Vec2d const& coord)
    :SimpleBacterium(coord)
{}

CannibalBacterium::~CannibalBacterium()
{}


///AFFICHAGE ET UPDATE
void CannibalBacterium::drawOn(sf::RenderTarget& target) const
{
     SimpleBacterium::drawOn(target);

    //Affichage d'un rond sur le corps de la bactérie cannibale pour la différencier de la SimpleBacterium
    auto const circle = buildCircle(coord_, rayon_/4, getWarningColor());
    target.draw(circle);

}

void CannibalBacterium::update(sf::Time dt)
{
    //Si la bactérie détecte plus de bactéries que de nutriments elle devient cannibale

    double bactScore(getAppEnv().getBacteriumScore(coord_));
    double nutScore(getAppEnv().getPositionScore(coord_));

    if(bactScore < nutScore){
        isCannibal_ = false;
        SimpleBacterium::update(dt);
        return;
    }

    isCannibal_ = true;
    SimpleBacterium::update(dt);
}


///DEPLACEMENT
void CannibalBacterium::move(sf::Time dt)
{
    //Si la bactérie est cannibale
    if(isCannibal_){

        //BASCULEMENT

            //Tentative de basculement
            t_basculement_+=dt.asSeconds(); //Temps depuis le dernier basculement
            score_ = getAppEnv().getBacteriumScore(this->getPosition()); //calcul du nouveau score
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

                        if(getAppEnv().getBacteriumScore(this->getPosition() + dir_al) >= getAppEnv().getBacteriumScore(this->getPosition() + dir) ) {
                            dir=dir_al;
                        }
                        dir_al=Vec2d::fromRandomAngle();
                    }
                    direction_=dir;

                }

                t_basculement_=0.; //il ya eu basculement on reset le timer
            }

            //NOUVELLE POSITION ET VITESSE
        computeNewPosAndSpeed(dt);

        return;
    }
    //Si la bactérie n'est pas cannibale elle bouge comme une simpleBacterium
    SimpleBacterium::move(dt);
    return;

}




Bacterium* CannibalBacterium::clone() const
{
    return new CannibalBacterium(*this);
}



///UTILITAIRE
sf::Color CannibalBacterium::getWarningColor() const
{
    if(isCannibal_){
        return sf::Color::Red;
    }
    return sf::Color::Green;
}

