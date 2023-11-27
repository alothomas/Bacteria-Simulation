#include "NutrimentGenerator.hpp"
#include "Application.hpp"
#include "./Random/Random.hpp"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"



///CONSTRUCTEUR ET DESTRUCTEUR
NutrimentGenerator::NutrimentGenerator()
    :time_(sf::Time::Zero)
{}


NutrimentGenerator::~NutrimentGenerator()
{}


///METHODES UPDATE ET RESET
void NutrimentGenerator::update(sf::Time dt)
{
    time_+=dt;
    //Si l'intervalle de temps réponds au délai fixé dans la config, on autorise génération de nutriment sinon rien
    if(time_ > sf::seconds(getAppConfig()["generator"]["nutriment"]["delay"].toDouble()) ) {
        time_=sf::Time::Zero;
        double x = getApp().getLabSize().x;
        double y = getApp().getLabSize().y;

        //On génère un nutriment A ou sinon un B
        if(bernoulli( getAppConfig()["generator"]["nutriment"]["prob"].toDouble()) ) {

            Quantity quantite( uniform(getShortConfig().nutrimentA_min_qty, getShortConfig().nutrimentA_max_qty) );

            double coord_x( normal(x/2,x/4*x/4));
            double coord_y( normal(y/2, y/4*y/4));
            Vec2d coord(coord_x,coord_y);

            getAppEnv().addNutriment(new NutrimentA(quantite, coord)); //On ajoute un nutriment A


        } else {


            Quantity quantite( uniform(getShortConfig().nutrimentB_min_qty, getShortConfig().nutrimentB_max_qty) );

            double coord_x( normal(x/2,x/4*x/4));
            double coord_y( normal(y/2, y/4*y/4));
            Vec2d coord(coord_x,coord_y);

            getAppEnv().addNutriment(new NutrimentB( quantite, coord) ) ; //On ajoute un nutriment B

        }

        return;
    }

    return;
}



void NutrimentGenerator::reset()
{
    time_=sf::Time::Zero;
}
