#pragma once
#include "Bacterium.hpp"
#include "./Utility/DiffEqSolver.hpp"
class Nutriment;
class NutrimentA;
class NutrimentB;

class SimpleBacterium : public Bacterium, public DiffEqFunction
{

public:

    SimpleBacterium(Vec2d const& coord);
    virtual ~SimpleBacterium();


    //Affichage et déplacement
    virtual void update(sf::Time dt) override;
    virtual void drawOn(sf::RenderTarget& target) const override;


    //Faire bouger muter et diviser bactérie
    virtual void move(sf::Time dt) override; //fait bouger la bactérie
    virtual Bacterium* clone() const override; //divise une bacterie
    virtual void mutate() override; //mutation des param mutables de la bactérie simple

    //Consommation de nutriment
    virtual Quantity eatableQuantity(NutrimentA& nutriment);
    virtual Quantity eatableQuantity(NutrimentB& nutriment);


    Vec2d getSpeedVector() const;  //retourne vitesse
    void computeNewPosAndSpeed(sf::Time dt); //calcule la nouvelle position et vitesse
    virtual Vec2d f(Vec2d position, Vec2d speed) const override; //définit la force subie par la bactérie

    virtual void updateNumber() override;                 //update du nombre de bactérie
    virtual double getBacteriumSpeed() const override;    //Getter de vitesse d'une bactérie

    j::Value const& getConfig() const override;

protected:

    double t_; //compteur pour flagelle
    double t_basculement_; //compteur pour déplacement
    double score_;
    double ancien_score_;

};
