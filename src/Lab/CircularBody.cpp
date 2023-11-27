#include "CircularBody.hpp"
#include "./Utility/Vec2d.hpp"


///CONSTRUCTEURS
CircularBody::CircularBody(Vec2d coord, double rayon)
    :coord_(coord), rayon_(rayon)
{}

CircularBody::CircularBody(const CircularBody &other)
    :coord_(other.coord_), rayon_(other.rayon_)
{}



///SETTERS ET GETTERS
Vec2d const& CircularBody::getPosition() const
{
    return coord_;
}

void CircularBody::setPosition(const Vec2d& coord)  //ajouter erreurs
{
    coord_=coord;
}

double CircularBody::getRadius() const
{
    return rayon_;
}

void CircularBody::setRadius(const double& rayon) //ajouter erreurs
{
    rayon_=rayon;
}



CircularBody& CircularBody::operator=(CircularBody const& source) //Operateur = entre 2 corps circulaires
{
    coord_=source.coord_;
    rayon_=source.rayon_;
    return (*this);
}



void CircularBody::move(Vec2d const& pos)  //faire delpacer coord du body
{
    coord_+=pos;
}




///METHODES POUR TESTER SI COLLISION/CONTAINS AVEC 2 BODY OU AVEC UN POINT
bool CircularBody::contains(CircularBody const& other) const //objet ciruclaire dans instance consideree
{

    if((rayon_>=other.rayon_) and ((distance(coord_,other.coord_) <= (rayon_-other.rayon_)))) { //on teste si other est a l'interieur de instance consideree
        return true;
    }

    return false;   //other n'est pas a l'interieur de instance consideree

}


bool CircularBody::isColliding(CircularBody const& other) const
{
    if(distance(other.coord_, coord_) <= (rayon_+ other.rayon_)) {
        return true;
    }

    return false;
}


bool CircularBody::contains(const Vec2d& p) const  //teste si un point est a interieur de corps circulaire
{
    if(distance(p,coord_) <= rayon_) {
        return true;
    }
    return false;
}



///OPERATEURS POUR TESTER COLLISION/CONTAINS

bool CircularBody::operator>(CircularBody const& body) const  //operateur > : true si body est dans objet
{
    return (contains(body));
}


bool CircularBody::operator&(CircularBody const& body) const  //operateur & : true si body est en collison avec objet
{
    return (isColliding(body));
}


bool CircularBody::operator>(const Vec2d& p) const  //operateur > : true si point est dans objet
{
    return (contains(p));
}



///AFFICHAGE
std::ostream& operator<<(std::ostream& out, CircularBody const& body)
{
    out << "CircularBody: position = " << body.getPosition() << " radius = " << body.getRadius();
    return out;
}
