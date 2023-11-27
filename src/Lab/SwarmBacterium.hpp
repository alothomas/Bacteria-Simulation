#pragma once
#include "Bacterium.hpp"
#include "./Utility/DiffEqSolver.hpp"
#include "Swarm.hpp"

class Nutriment;
class NutrimentA;
class NutrimentB;


class SwarmBacterium : public Bacterium, public DiffEqFunction
{

public:


    SwarmBacterium(Vec2d const& coord, Swarm* swarm);

    virtual ~SwarmBacterium();

    //Affichage et Update
    virtual void update(sf::Time dt) override;
    virtual void drawOn(sf::RenderTarget& target) const override;


    //Déplacement
    virtual void move(sf::Time dt) override;

    //Clonage et mutation
    virtual Bacterium* clone() const override; //clone une bacterie
    virtual void mutate() override;            //mutation des param mutables de la bactérie

    //Utilitaire
    virtual Vec2d f(Vec2d position, Vec2d speed) const override; //définit la force subie par la bactérie
    Vec2d getSpeedVector() const;                                //retourne vitesse
    std::string getSwarmBacteriumId() const;                     //retourne l'id du swarm de la bactérie

    //Consommation de nutriment
    virtual Quantity eatableQuantity(NutrimentA& nutriment);
    virtual Quantity eatableQuantity(NutrimentB& nutriment);

    virtual void updateNumber() override;    //update du nombre de bactérie
    //Getter de vitesse d'une bactérie
    virtual double getBacteriumSpeed() const override;

    virtual j::Value const& getConfig() const override;


private:

    std::string swarmId_;  //id du swarm auquel la bactérie appartient
};
