#define TAO_PEGTL_PRETTY_DEMANGLE 1

#include "proto2puml.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "proto3_grammar.hpp"
#include "puml_printer.hpp"

extern "C" const char* proto_to_puml(const char* proto_input)
{
    std::string output = "No result available";
    try
    {
        output = proto2puml::convert(std::string(proto_input));
    }
    catch (std::exception& e)
    {
        output = "Exception occured: ";
        output += e.what();
    }
    catch (...)
    {
        output = "Undefined Error";
    }

#ifdef __EMSCRIPTEN__
    return output.c_str();
#else
    char* return_buffer = static_cast<char*>(malloc(output.size() + 1));
    strcpy_s(return_buffer, output.size() + 1, output.data());
    return return_buffer;
#endif
}

namespace proto2puml
{
    using namespace tao::pegtl;
    using namespace proto3;

    // clang-format off
    template <typename Rule>
    using selector = parse_tree::selector<
        Rule,
        parse_tree::store_content::on<
            ident,

            message,

            oneof,
            oneof_name,
            oneof_field,

            map_field,
            key_type,
            map_name,

            field,
            field_number,
            field_name,

            repeated,

            type,
            builtin_type,
            defined_type,

            enum_,
            enum_name,
            enum_field,
            int_lit
        >>;
    // clang-format on
}

std::string proto2puml::convert(const std::string& proto_input)
{
    using namespace TAO_PEGTL_NAMESPACE; // NOLINT

    if (analyze<proto3::proto>() != 0)
    {
        throw std::invalid_argument("analyze<proto3::proto>() failed");
    }

    memory_input in(proto_input, "");
    const auto root = tao::pegtl::parse_tree::parse<proto3::proto, selector>(in);

    auto os = std::ostringstream();
    try
    {
        os << puml_printer::run(*root);
    }
    catch (std::exception& e)
    {
        os << e.what() << std::endl;
    }
    catch (...)
    {
        os << "Undefined error." << std::endl;
    }
    return os.str();
}
