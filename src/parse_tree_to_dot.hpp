// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#pragma once

#include <cassert>
#include <ostream>
#include <string>

#include <tao/pegtl/contrib/parse_tree.hpp>

#include "proto3_grammar.hpp"

namespace
TAO_PEGTL_NAMESPACE::proto3
{
namespace internal
{
void print_dot_node(std::ostream& os, const parse_tree::node& n, const std::string& s);

void print_field_node(std::ostream& os, const parse_tree::node& n, const std::string& s)
{
    auto& x = std::find_if(n.children.begin(), n.children.end(),
        [](auto& child) {return child->is<field_name>(); });

    std::string name = "undefined";
    if(x != n.children.end()) {
        name = x->get()->string_view();
    }

    auto& type = std::find_if(n.children.begin(), n.children.end(),
        [](auto& child) {return child->is<builtin_type>(); });

    if(type == n.children.end()) {
        type = std::find_if(n.children.begin(), n.children.end(),
            [](auto& child) {return child->is<defined_type>(); });
        os << "  x" << &n << " -> id_" << type->get()->string_view() << std::endl;
    }

    os << "  x" << &n << " [ label=\"this is a field node '" << name << "'\"]" << std::endl;
        //for (auto& c : n.children) {
    //    os << c->name() << std::endl;
    //}
}

void print_dot_node(std::ostream& os, const parse_tree::node& n, const std::string& s)
{
    if (n.has_content()) {
        if(n.is<field>()) {
            print_field_node(os, n, s);
            return;
        }
        else {
            os << "  x" << &n << " [ label=\"" << s << "\\n\\\"" << n.string_view() << "\\\"\" ]\n";
        }
    } else {
        os << "  x" << &n << " [ label=\"" << s << "\" ]\n";
    }
    if (!n.children.empty()) {
        os << std::endl;
        os << "  x" << &n << " -> { ";
        for (auto& up : n.children) {
            os << "x" << up.get() << ((up == n.children.back()) ? " }\n" : ", ");
        }
        for (auto& up : n.children) {
            print_dot_node(os, *up, up->name());
        }
    }
}
}  // namespace internal

void print_dot(std::ostream& os, const parse_tree::node& n)
{
    assert( n.is_root() );
    os << "digraph parse_tree\n{\n";
    internal::print_dot_node(os, n, "ROOT");
    os << "}\n";
}
}
