// Copyright (c) 2017-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#define TAO_PEGTL_PRETTY_DEMANGLE 1

#include "proto3_grammar.hpp"
#include "parse_tree_to_dot.hpp"

#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
// #include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

namespace TAO_PEGTL_NAMESPACE::proto3 {
// clang-format off
template< typename Rule >
   using selector = parse_tree::selector<
      Rule,
      parse_tree::store_content::on<
         message,

         oneof,
         oneof_name,
         oneof_field,

         key_type,
         map_name,
         map_field,

         field,
         builtin_type,
         defined_type,
         field_name
     > >;

// clang-format on

} // namespace TAO_PEGTL_NAMESPACE::proto3

int main(int argc, char **argv) {
  using namespace TAO_PEGTL_NAMESPACE; // NOLINT

  if (analyze<proto3::proto>() != 0) {
    return 1;
  }

  for (int i = 1; i < argc; ++i) {
    file_input in(argv[i]);
    auto root = tao::pegtl::parse_tree::parse<proto3::proto, proto3::selector>(in);
    parse_tree::print_dot(std::cout, *root);
  }
  return 0;
}
