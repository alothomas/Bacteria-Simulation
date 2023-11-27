#pragma once
#include "PetriDish.hpp"
#include <SFML/Graphics.hpp>
#include "NutrimentGenerator.hpp"


class Lab
{

public :
    Lab();   //constructeur
    ~Lab();  //Destructeur

    //Dessin et update du lab
    void drawOn(sf::RenderWindow& targetWindow) const;   //Dessine le contenu du Lab : un unique petri_
    void update(sf::Time& dt);

    void reset(); //efface le contenu du lab : un unique petri_

    //Tests de collision entre circular body
    bool contains(CircularBody const& body) const;
    bool doesCollideWithDish(CircularBody const& body) const;           //body en collision avec petri?
    Nutriment* getNutrimentColliding(CircularBody const& body) const;   //retourne un nutriment en collision avec body
    Bacterium* getBacteriumColliding(Bacterium const& bacteria) const;  //retourne une bactérie en collision avec une autre


    //Ajouter des nutriments / bactéries / swarms / antibiotiques
    void addNutriment(Nutriment* nutriment); //ajoute un nutriment au lab si il est contenu dans le petri
    void addBacterium(Bacterium* bacterie);  //ajout une bactérie au lab si il est contenu dans le petri
    void addNewBacterium(Bacterium* bacterie); //ajout d'une bactérie (en attente de fusion avec celles du petri)
    void addSwarm(Swarm* swarm); //ajout d'un swarm au petri
    void addAntibiotic(Antibiotic* antibiotic); //ajout d'un antibio au lab

    //Trouver un swarm avec un ID
    Swarm* getSwarmWithId(std::string const& id);


    //Température
    double getTemperature() const; //Getter de la température du lab
    void increaseTemperature();     //Augmenter ou diminuer température
    void decreaseTemperature();
    void resetTemperature();

    //Gradient de nutriment
    double getPositionScore(const Vec2d& coord) const; //renvoie le gradient de nutriment à une position donnée du petri
    double getGradientExponent() const;
    void increaseGradientExponent();
    void decreaseGradientExponent();
    void resetGradientExponent();

    //Gradiant d'antibiotique
    double getAntibioticScore(const Vec2d& coord) const; //renvoie le gradiant d'antibio à la position coord

    //Gradiant de bactérie
    double getBacteriumScore(const Vec2d& coord) const;  //renvoie le gradiant de bactérie vu par bact
    

    //Récupérer les data du lab
    std::unordered_map<std::string, double> fetchData(const std::string& s) const;

    //Augmenter ou décrémenter le compteur de bactérie par type
    void increaseSimpleBacteriumNumber();
    void increaseTwitchingBacteriumNumber();
    void increaseSwarmBacteriumNumber();

    void decreaseSimpleBacteriumNumber();
    void decreaseTwitchingBacteriumNumber();
    void decreaseSwarmBacteriumNumber();


    //Update la vitesse/longueur moyenne des tentacules
    void updateTentacleSpeed(double const& speed);
    void updateTentacleLength(double const& length);


private :

    PetriDish petri_;
    NutrimentGenerator nutrimentGenerator_;

};
