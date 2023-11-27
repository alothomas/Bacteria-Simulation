#include "Swarm.hpp"
#include "Application.hpp"
#include "Utility/Utility.hpp"
#include "SwarmBacterium.hpp"

Swarm::Swarm(std::string const& id)
    :id_(id), leader_(nullptr)
{}


Swarm::~Swarm()
{
    leader_ = nullptr;
    swarm_.clear();
}


///UPDATE
void Swarm::update(sf::Time dt)
{
    updateLeader();
}



void Swarm::updateLeader()
{
    if(not(swarm_.empty())) {
        if (leader_ == nullptr) leader_ = swarm_[0];
        for(auto& bacterie : swarm_) {
            //On définit le leader selon gradiant de nutriments
            if(getAppEnv().getPositionScore(bacterie->getPosition()) >= getAppEnv().getPositionScore(getLeaderPosition()))
               leader_= bacterie;
        }
    }

    return;
}


///UTILITAIRE
void Swarm::addSwarmBacterium(SwarmBacterium* bacterie)
{
    if(swarm_.empty()){
        leader_ = bacterie;
    }
    swarm_.push_back(bacterie);
}


void Swarm::removeSwarmBacterium(SwarmBacterium* bacterie)
{
    for (auto& bact: swarm_){
            if (bact == bacterie) bact = nullptr;
        }
         swarm_.erase(std::remove(swarm_.begin(), swarm_.end(), nullptr), swarm_.end());
         if(bacterie == leader_) {
            leader_ = nullptr;
            updateLeader();
        }
}


Vec2d Swarm::getLeaderPosition() const
{
    return leader_->getPosition();
}


std::string Swarm::getId() const
{
    return id_;
}


j::Value const& Swarm::getSwarmInitialColor() const
{
    return getAppConfig()["swarms"][id_]["color"];
}

