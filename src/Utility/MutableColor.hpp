#pragma once
#include <array>
#include "MutableNumber.hpp"

class MutableColor
{

public:

    MutableColor(j::Value const& config);
    ~MutableColor();

//Faire muter les couleurs du tableau
    void mutate();
    sf::Color get() const;



private:

    std::array<MutableNumber,4>RGBA_;   //couleur





};
