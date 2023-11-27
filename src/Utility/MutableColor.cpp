#include "MutableColor.hpp"

///CONSTRUCTEUR
MutableColor::MutableColor(j::Value const& config) //constructeur de RGBA_ A REVOIR PEUT ETRE ?????
    :RGBA_( {MutableNumber(config["r"]), MutableNumber(config["g"]), MutableNumber(config["b"]), MutableNumber(config["a"])} )
{}

MutableColor::~MutableColor()
{}

void MutableColor::mutate()
{

    for(auto& comp : RGBA_) {
        comp.mutate();
    }
    return;
}

sf::Color MutableColor::get() const
{
    return { static_cast<sf::Uint8>(RGBA_[0].get() * 255),
             static_cast<sf::Uint8>(RGBA_[1].get() * 255),
             static_cast<sf::Uint8>(RGBA_[2].get() * 255),
             static_cast<sf::Uint8>(RGBA_[3].get() * 255) };

}
