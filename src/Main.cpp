/* Copyright 2016 Marleson Graf <aszdrick@gmail.com> */

#include <iostream>
#include "mbe/Engine.hpp"
#include "multimedia/Gomoku.hpp"
#include "gomoku/Demo.hpp"
#include "Traits.hpp"

int main(int argc, char** argv) {
    // Agora só precisamos criar um novo mbe::Game...
    mbe::Engine e(new mbe::Game(new Demo()), 1024, 768, "Demo");
    // ...e rodar a engine
    e.run();

    return 0;
}