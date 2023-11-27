#pragma once
#include "Bacterium.hpp"
#include "Grip.hpp"
class Nutriment;
class NutrimentA;
class NutrimentB;

enum States_list { IDLE, WAIT_TO_DEPLOY, DEPLOY, ATTRACT, RETRACT, EAT}; //états possible de la batérie en mouvement

class TwitchingBacterium : public Bacterium
{

public:


    TwitchingBacterium(Vec2d const& coord);
    virtual ~TwitchingBacterium();


    //Affichage et déplacement
    virtual void update(sf::Time dt) override;
    virtual void drawOn(sf::RenderTarget& target) const override;


    //Faire bouger muter et diviser bactérie
    virtual void move(sf::Time dt) override;
    virtual Bacterium* clone() const override;
    virtual void mutate() override;


    //Faire bouger le grappin de delta
    void moveGrip(Vec2d const& position);

    //Consommation de nutriment
    virtual Quantity eatableQuantity(NutrimentA& nutriment);
    virtual Quantity eatableQuantity(NutrimentB& nutriment);

    virtual void updateNumber() override;     //update du nombre de bactéries

    //Getter de vitesse d'une bactérie
    virtual double getBacteriumSpeed() const override;

    virtual j::Value const& getConfig() const override;

private:

    Grip grip_;         // un grappin
    States_list state_; //état de la bactérie en mouvement

    //getters utiles
    Quantity getEnergyTentacle() const;
    Quantity getEnergyMoveRate() const;
    double getTentacleSpeed() const;
    double getSpeedFactor() const;
    double getTentacleLength() const;

};
