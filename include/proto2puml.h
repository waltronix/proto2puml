#pragma once

#include <string>

extern "C" const char* proto_to_puml(const char* proto_input);

namespace proto2puml
{
    std::string convert(const std::string& proto_input);
}

