#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <model_loader.hpp>

struct shapes{

    mesh cube;
    mesh cylinder;
    mesh sphere;

    shapes();
    static shapes& instance();
};

#endif