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

template <typename TItem>
bool has_child(const parse_tree::node& n)
{
    auto& found_items = std::find_if(n.children.begin(), n.children.end(),
        [](auto& child) {
        return child->is<TItem>();
    });

    return found_items != n.children.end();
}

template <typename TItem>
const parse_tree::node& get_single_child(const parse_tree::node& n)
{
    auto& found_items = std::find_if(n.children.begin(), n.children.end(),
        [](auto& child) {
            return child->is<TItem>();
        });

    if (found_items != n.children.end()) {
        return *(found_items->get());
    }

    throw std::exception("item not found");
}

void print_field_node(std::ostream& os, const parse_tree::node& n, const std::string& s)
{
    auto name = get_single_child<field_name>(n).string_view();

    os << " - " << name << std::endl;
}

void print_message_thing(std::ostream& os, const parse_tree::node& n, const std::string& s, std::string& dependency)
{
    std::string name = "undefined name";
    std::string type_id = "undefined type";

    try {
        if (n.is<field>()) {
            name = get_single_child<field_name>(n).string_view();

            auto& type = get_single_child<proto3::type>(n);
            type_id = type.string_view();

            if(has_child<defined_type>(type)) {
                dependency = s + " *-- " + type_id + "\n";
            }
        }
    }catch (std::exception& e) {
        os << e.what() << std::endl;
    }

    os << "  " << name << ": " << type_id << std::endl;
}

void print_message(std::ostream& os, const parse_tree::node& n, const std::string& s)
{
    std::string dependencies = "";

    std::string name = std::string( get_single_child<ident>(n).string_view());
    os << "class " << name << " {" << std::endl; // " [ label=\"message: '" << s << "'\\n" << std::endl;

    auto& fields = std::find_if(n.children.begin(), n.children.end(),
        [](auto& child) {
            return child->is<field>();
        });

    while (fields != n.children.end()) {
        std::string dependency;
        print_message_thing(os, *fields->get(), name, dependency);
        dependencies += dependency;

        fields = std::find_if(++fields, n.children.end(),
            [](auto& child) {
                return child->is<field>();
            });
    }

    os << "}" << std::endl;

    os << dependencies << std::endl;
}

void print_dot_node(std::ostream& os, const parse_tree::node& n, const std::string& s)
{
    if (n.has_content()) {
        if (n.is<field>()) {
            print_field_node(os, n, s);
            return;
        }
        if (n.is<message>()) {
            print_message(os, n, s);
            return;
        } else {
            os << "  x" << &n << " [ label=\"" << s << "\\n\\\"" << n.string_view() << "\\\"\" ]\n";
        }
    }
    if (!n.children.empty()) {
        for (auto& up : n.children) {
            print_dot_node(os, *up, up->name());
        }
    }
}
}  // namespace internal

void print_dot(std::ostream& os, const parse_tree::node& n)
{
    assert( n.is_root() );
    os << "@startuml" << std::endl;
    internal::print_dot_node(os, n, "");
    os << "@enduml" << std::endl;
}
}
