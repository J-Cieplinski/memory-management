#pragma once

#include <stdexcept>
#include <string>

class NullPtrException : public std::runtime_error {
public:
    NullPtrException(const std::string& what)
        : std::runtime_error(what){};
};
