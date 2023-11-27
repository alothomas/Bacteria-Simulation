#pragma once
#include "./Interface/Updatable.hpp"
#include "./Interface/Drawable.hpp"
#include "Graph.hpp"
#include "Utility/Utility.hpp"


class Stats : public Updatable, public Drawable
{

public:

    Stats();
    virtual ~Stats();
    
    //Affichage et update
    virtual void drawOn(sf::RenderTarget& target) const override;
    virtual void update(sf::Time dt) override;


    std::string getCurrentTitle() const; //renvoie le libellé du graph actif

    //Setter et Incrémenter ou décrémenter l'id_ actif
    void setActive(const int& id);
    void next();
    void previous();

    //Ajouter un graph
    void addGraph(const int& id, const std::string& title, const std::vector<std::string>& graphTitles, const double& min, const double& max, const Vec2d& size);

    void reset();


private:

    std::unordered_map<int,std::unique_ptr<Graph>> graphs_;   //ensemble des graphs
    std::unordered_map<int, std::string> libelles_;           //ensemble des libéllés (associés à un graph via un numéro int)
    int id_;                                                  //identifiant actif
    sf::Time time_;                                           //compteur

};

