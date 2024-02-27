#pragma once

#include <type_traits>
#include <iostream>


struct Command{
    virtual void Execute() = 0;
};

template<class C>
concept DerivedFromCommand = std::is_base_of<Command, C>::value;