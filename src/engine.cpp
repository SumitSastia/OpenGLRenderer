#include <engine.hpp>

engine* engine::pointer = nullptr;

engine::~engine(){
    delete pointer;
}