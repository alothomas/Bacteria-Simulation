#include "MutableNumber.hpp"
#include "./Random/Random.hpp"

///CONSTRUCTEURS
MutableNumber::MutableNumber(double const& value, double const& prob, double const& ecartType, bool const& hasMin, double const& min, bool const& hasMax, double const& max)
    :prob_(prob), hasMin_(hasMin), hasMax_(hasMax), min_(min), max_(max), ecartType_(ecartType)
{
    set(value);
}

MutableNumber::MutableNumber(j::Value const& config)
    :prob_(config["rate"].toDouble()), hasMin_(config["clamp min"].toBool()), hasMax_(config["clamp max"].toBool()), min_(config["min"].toDouble()), max_(config["max"].toDouble()), ecartType_(config["sigma"].toDouble())
{
    set(config["initial"].toDouble());
}

MutableNumber::~MutableNumber()
{}


///GETTERS ET SETTERS
double MutableNumber::get() const
{
    return value_;
}


void MutableNumber::set(double const& value)
{

    if( (hasMin_==true) and (value<min_) ) {
        value_=min_;
        return;
    }
    if( (hasMax_==true) and (value>max_) ) {
        value_=max_;
        return;
    }
    value_=value;

    return;
}


///AUTRES METHODES
void MutableNumber::mutate()
{
    if(bernoulli(prob_)) {
        set(value_+ normal(0., ecartType_*ecartType_));
    }
    return;
}



///CREATION DE NOMBRES MUTABLES PLUS SPECIFIQUES
MutableNumber MutableNumber::probability(double const& initialValue, double const& mutationProbability, double const& sigma)
{
    return MutableNumber(initialValue, mutationProbability, sigma, true, 0.0, true, 1.0);
}

MutableNumber MutableNumber::probability(j::Value const& config)
{
    return MutableNumber(config["initial"].toDouble(), config["rate"].toDouble(), config["sigma"].toDouble(), true, 0.0, true, 1.0);
}


MutableNumber MutableNumber::positive(double const& initialValue, double const& mutationProbability, double const& sigma, bool const& hasMax, double const& max)
{
    return MutableNumber(initialValue, mutationProbability, sigma, true, 0.0, hasMax, max);
}

MutableNumber MutableNumber::positive(j::Value const& config, bool const& hasMax, double const& max)
{
    return MutableNumber(config["initial"].toDouble(), config["rate"].toDouble(), config["sigma"].toDouble(), true, 0.0, hasMax, max);
}
