#pragma once
#include "./Interface/Updatable.hpp"
#include "./Utility/Vec2d.hpp"


class SwarmBacterium;


class Swarm : public Updatable
{


public:

    Swarm(std::string const& id);
    virtual ~Swarm();


    //Update le leader
    virtual void update(sf::Time dt) override;
    void updateLeader();


    //Utilitaire
    void addSwarmBacterium(SwarmBacterium* bacterie);    //ajoute une bactérie au swarm
    void removeSwarmBacterium(SwarmBacterium* bacterie); //enleve une bactérie au swarm
    Vec2d getLeaderPosition() const;                     //retourne la position du leader
    std::string getId() const;                           //retourne l'ID du swarm

    j::Value const& getSwarmInitialColor() const;


private:

    std::string id_;                     //numéro de groupe
    std::vector<SwarmBacterium*> swarm_; //ensemble du groupe
    SwarmBacterium* leader_;             //bactérie leader


};
