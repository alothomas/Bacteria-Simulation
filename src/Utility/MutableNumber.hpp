#pragma once
#include "Utility.hpp"

class MutableNumber
{

public:

//constructeurs / destructeur
    MutableNumber(double const& value, double const& prob, double const& ecartType, bool const& hasMin = false, double const& min = 0.0,  bool const& hasMax = false,  double const& max = 0.0);
    MutableNumber(j::Value const& config);
    MutableNumber() = default;

    ~MutableNumber();

//getter et setters
    double get() const;
    void set(const double& value);

//fonction pour muter un nombre
    void mutate();

//Creation de nombres mutables plus spécifiques
    static MutableNumber probability(const double& initialValue, const double& mutationProbability, const double& sigma);
    static MutableNumber probability(j::Value const& config);

    static MutableNumber positive(const double& initialValue, const double& mutationProbability, const double& sigma, const bool& hasMax = false, const double& max = 0.);
    static MutableNumber positive(j::Value const& config, bool const& hasMax=false, double const& max=0.);


private:

    double value_;          //valeur du nombre
    double prob_;           //caractéristiques de la valeur
    bool hasMin_;
    bool hasMax_;
    double min_;
    double max_;
    double ecartType_;

};
