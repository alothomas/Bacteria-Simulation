#pragma once
#include "CircularBody.hpp"
#include "./Interface/Drawable.hpp"
#include "./Interface/Updatable.hpp"
#include "./Utility/MutableColor.hpp"
#include "./Utility/MutableNumber.hpp"
#include <string>

class Nutriment;
class NutrimentA;
class NutrimentB;

class Bacterium : public CircularBody, public Drawable, public Updatable
{

public:

    Bacterium(Quantity const& energy, Vec2d const& coord, Vec2d const& direction, double const& rayon, MutableColor const& color);
    virtual  ~Bacterium();

    //Affichage et update
    virtual void update(sf::Time dt) override;
    virtual void drawOn(sf::RenderTarget& target) const override;


    //Faire bouger et diviser et muter une bactérie
    virtual void move(sf::Time dt) = 0; //Méthode virtuelle pure : fais bouger une bactérie
    virtual Bacterium* clone() const = 0; //clone une bacterie
    virtual void mutate() = 0;  //mutation des param mutables de la bactérie
    virtual void divide(); //divise bactérie


    //Utilitaires
    bool isDead() const;
    void consumeEnergy(Quantity const& qt);
    void eat(Nutriment& nutriment); //consommation de nutriment par la bactérie
    void eat(Bacterium& bact);      //consommation de bactérie par bactérie
    virtual Quantity eatableQuantity(NutrimentA& nutriment) = 0; //retourne la qtité mangeable selon le type de nutriment et selon la bactérie
    virtual Quantity eatableQuantity(NutrimentB& nutriment) = 0; //retourne la qtité mangeable selon le type de nutriment et selon la bactérie

    void addProperty(const std::string& name, const MutableNumber& nb_mut); //ajoute un paramètre num mutable
    MutableNumber getProperty(const std::string& name) const; //getter de param mutable num

    //Mettre à jour le nombre de bactérie (polymortphique pour que les compteurs s'update selon les types de bactérie
    virtual void updateNumber() = 0;

    //Getter de vitesse d'une bactérie
    virtual double getBacteriumSpeed() const = 0;

    virtual j::Value const& getConfig() const = 0;


    //Modifier les quantités d'énergie de la bactérie lors de cannibalisme
    Quantity takeQuantity(const Quantity& quantite);   //Prélever une qtité de bactérie

    //Getters
    Quantity getEnergyDivision() const;
    sf::Time getDelay() const;
    Quantity getEnergyMoveRate() const;
    Quantity getMaxEatableQuantity() const;
    double getAntibioticResistanceFactor() const;
    bool isInCannibalMode() const;


protected:

    MutableColor color_;
    Vec2d direction_;
    bool isHungry_;   //true si la bacterie a faim, false si elle est au régime
    Quantity energy_;
    std::map<std::string,MutableNumber> mutableParameters_;
    sf::Time time_;
    double rotation_; //angle de rotation de la bactérie en RAD (angle horaire)
    bool isCannibal_; //true si la bactérie est cannibale, false si non

};
