#include <Stats/Stats.hpp>
#include "Application.hpp"
#include <string>
#include <vector>


Stats::Stats()
    :time_(sf::Time::Zero)
{}

Stats::~Stats()
{}


///Affichage et update
void Stats::drawOn(sf::RenderTarget& target) const
{
    //on sélectionne le graph à l'id actif pour le draw dans target (at() lance aussi une erreur si out of range)
    graphs_.at(id_)->drawOn(target);
    return;
}


void Stats::update(sf::Time dt)
{

    //On update que si délai suffisant
    time_+=dt;
    if(time_<=sf::seconds(getAppConfig()["stats"]["refresh rate"].toDouble())){
        return;
    }

    graphs_[id_]->updateData(time_, getAppEnv().fetchData(libelles_[id_])); //update via update du graph
    time_=sf::Time::Zero;

}


std::string Stats::getCurrentTitle() const
{
    auto paire = libelles_.find(id_);

    //paire->fisrt renvoie la clé de paire et second la valeur de paire

    if(paire==libelles_.end()) {
        throw std::invalid_argument("id_ de libellé introuvable !");
    }

    return paire->second;
}



///INCREMENTER / DECREMENTER CURRENT ID

void Stats::setActive(const int& id)
{
    id_=id;
}

void Stats::next()
{
    int i(graphs_.size()-1);
    if(id_==i){
        setActive(0);
        return;
    }
    id_+=1;
}


void Stats::previous()
{
    if(id_-1 <0){
        setActive(graphs_.size()-1);
        return;
    }
   setActive(id_-1);
}


void Stats::addGraph(const int& id, const std::string& title, const std::vector<std::string>& graphTitles, const double& min, const double& max, const Vec2d& size)
{
    //on reset le ptr associé à l'id et on créer un nouv graph
    graphs_[id].reset( new Graph(graphTitles, size, min, max ));
    libelles_[id] = title;
    setActive(id); //update de l'id actif
}



void Stats::reset()
{
    //On itère sur les graphs et on les reset
    for(auto& graph : graphs_)
        (graph.second)->Graph::reset();
}
