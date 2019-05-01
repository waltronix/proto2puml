// Copyright (c) 2017-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#define TAO_PEGTL_PRETTY_DEMANGLE 1

#include "proto3_grammar.hpp"
#include "parse_tree_to_puml.hpp"

#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

#include <filesystem>
#include <fstream>

namespace
TAO_PEGTL_NAMESPACE::proto3
{
// clang-format off
template <typename Rule>
using selector = parse_tree::selector<
    Rule,
    parse_tree::store_content::on<
        ident,

        message,
        message_thing,

        oneof,
        oneof_name,
        oneof_field,

        key_type,
        map_name,
        map_field,

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
} // namespace TAO_PEGTL_NAMESPACE::proto3

int main(int argc, char** argv)
{
    using namespace TAO_PEGTL_NAMESPACE; // NOLINT

    if (analyze<proto3::proto>() != 0) {
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::filesystem::path proto_file = argv[i];

        if (exists(proto_file)) {
            file_input in(argv[i]);

            try {
                auto root = tao::pegtl::parse_tree::parse<proto3::proto, proto3::selector>(in);

                {
                    auto dot_file = proto_file.replace_extension("dot");
                    std::ofstream dot_stream(dot_file);
                    dot_stream << "@startuml" << std::endl;
                    parse_tree::print_dot(dot_stream, *root);
                    dot_stream << "@enduml" << std::endl;
                }

                 {
                     auto puml_file = proto_file.replace_extension("puml");
                     std::ofstream puml_stream(puml_file);
                     proto3::print_puml(puml_stream, *root);
                 }
            }
            catch(std::exception& e) {
                std::cerr << e.what()<< std::endl;
            }
        }
        else{
            std::cerr << "File '" << proto_file << "' does not exist" << std::endl;
        }
    }
    return 0;
}
