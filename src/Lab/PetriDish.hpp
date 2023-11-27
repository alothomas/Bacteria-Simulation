#pragma once
#include <SFML/Graphics.hpp>
#include "./Utility/Vec2d.hpp"
#include <vector>
#include "Swarm.hpp"
#include "Bacterium.hpp"
#include "Nutriment.hpp"
#include "Antibiotic.hpp"
#include "CircularBody.hpp"
#include "./Interface/Drawable.hpp"
#include "./Interface/Updatable.hpp"


//Cette classe hérite de la superclasse CircularBody (une assiette de petri est un circularBody)
class PetriDish : public CircularBody, public Drawable, public Updatable
{

public:

    PetriDish(const Vec2d& coord, double rayon);  //constructeur

    virtual ~PetriDish(); //Destructeur

    //Ajouter des bactéries/ nutriments / swarm
    bool addBacterium(Bacterium* bacterie);
    bool addNewBacterium(Bacterium* bacterie); //ajouter des bactéries divisées.
    bool addNutriment(Nutriment* nutriment);
    bool addSwarm(Swarm* swarm);       //ajouter un swarm au petri
    bool addAntibiotic(Antibiotic* antibio);    //ajouter un antibio au petri

    //Trouver un swarm avec un ID
    Swarm* getSwarmWithId(std::string const& id);


    //Update et dessine le petri
    virtual void update(sf::Time dt) override;
    virtual void drawOn(sf::RenderTarget& targetWindow) const override;

    void reset();

    //Tests de collision avec nutriments et bactéries
    Nutriment* getNutrimentColliding(CircularBody const& body) const;   //retourne un nutriment en collision avec body
    Bacterium* getBacteriumColliding(Bacterium const& bacteria) const;      //retourne une bactérie en collision avec une autre

    //Température
    double getTemperature() const; //getter
    void increaseTemperature(); //augmenter la temperature du petri
    void decreaseTemperature();
    void resetTemperature();

    //Gradient de nutriment
    double getPositionScore(const Vec2d& coord) const; //Renvoie le gradient de nutriment au point coord
    double getGradientExponent() const;                //Renvoie l'exposant définisant le gradient : high = faible perception / low = grande perception
    void increaseGradientExponent();
    void decreaseGradientExponent();
    void resetGradientExponent();


    //Gradiant d'antibiotique
    double getAntiboticScore(const Vec2d& coord) const;

    //Gradiant de bactéries
    double getBacteriumScore(const Vec2d& coord) const;


    PetriDish& operator=(PetriDish const& assiette) = delete; //On supprime l'operateur = pour les PetriDish
    PetriDish(PetriDish const& assiette) = delete;  //On supprime le constructeur de copie


    //Getter de compteur de bactérie ou nutriment

    Quantity getNutrimentQuantity() const;
    int getNutrimentsNumber() const;
    int getSimpleBacteriumNumber() const;
    int getTwitchingBacteriumNumber() const;
    int getSwarmBacteriumNumber() const;

    //augmenter ou diminuer le compteur de bactérie
    void increaseSimpleBacteriumNumber();
    void increaseTwitchingBacteriumNumber();
    void increaseSwarmBacteriumNumber();

    void decreaseSimpleBacteriumNumber();
    void decreaseTwitchingBacteriumNumber();
    void decreaseSwarmBacteriumNumber();

    void resetBacteriumTypeNumber(); //reset le compteur de bactéries


    //Calculer la vitesse moyenne des bactéries
    double getAverageSpeed() const;

    //Calculer vitesse moyenne tentacule
    double getAverageTentacleSpeed() const;
    //Calculer longueur moyenne tentacule
    double getAverageTentacleLength() const;

    //Update tentacule speed et length average
    void updateTentacleSpeed(double const& speed) ;
    void updateTentacleLength(double const& length);

    void resetTentacleAverages(); //reset les moyennes du tentacule

    //Getter de facteur de resistance antibio
    double getAverageAntibioticResistance() const;





private:

    std::vector<Bacterium*> bacteries_;     //ensemble de bacteries
    std::vector<Nutriment*> nutriments_;    //ensemble de nutriments
    double temperature_;
    double puissance_;                      //liée au gradient de nutriment du petri
    std::vector<Bacterium*> new_bacteries_; //collection de nouvelles bactéries en attente de fusion avec celles déjà existantes
    std::vector<Swarm*> swarms_;            //ensemble de swarms du petri

    //Nombre de bactéries par type dans le petri
    int SimpleBacteriumNumber_;
    int TwitchingBacteriumNumber_;
    int SwarmBacteriumNumber_;

    //Vitesse et longueur moyenne des tentacules
    double tentacleSpeed_;
    double tentacleLength_;

    //Collection d'antibio
    std::vector<Antibiotic*> antibiotiques_;


};
