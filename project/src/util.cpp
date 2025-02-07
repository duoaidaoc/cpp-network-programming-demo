#include "util.hpp"

void errif(bool condition, const std::string &msg){
    if(condition)
        std::cout << msg << '\n';
}