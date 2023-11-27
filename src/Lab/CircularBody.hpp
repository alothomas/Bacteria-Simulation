#pragma once
#include "./Utility/Vec2d.hpp"

class CircularBody
{
public:

    Vec2d const& getPosition() const;             //getters et setters

    double getRadius() const;


    CircularBody& operator=(CircularBody const&); //Operateur =


    void move(const Vec2d& pos);  //deplacer les coord de l'objet

    bool contains(CircularBody const& other) const; //teste si objet ciruclaire dans instance consideree

    bool isColliding(CircularBody const& other) const;  //teste si other est en collision

    bool contains(Vec2d const& point) const;  //teste si un point est a interieur de corps circulaire


    bool operator>(CircularBody const& body) const;  //operateur > : true si body est dans objet
    bool operator&(CircularBody const& body) const;  //operateur & : true si body est en collison avec objet
    bool operator>(const Vec2d& p) const;  //operateur & : true si point est dans objet



protected:

    Vec2d coord_;
    double rayon_;

    //On veut que ces méthodes ne soient accesibles que dans les ss-classes de CircularBody
    CircularBody(Vec2d coord, double rayon);
    CircularBody(CircularBody const& other);
    void setPosition(const Vec2d& coord);
    void setRadius(const double& rayon);

};


///AFFICHAGE
std::ostream& operator<<(std::ostream& out, CircularBody const& body);


