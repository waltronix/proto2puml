// Parts are taken from from parse_tree_to_dot.hpp:
//
// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#pragma once

#include <cassert>
#include <functional>
#include <ostream>
#include <string>

#include <tao/pegtl/contrib/parse_tree.hpp>

#include "proto3_grammar.hpp"

namespace
TAO_PEGTL_NAMESPACE::proto3
{
class puml_printer
{
    std::stringstream definitions;
    std::stringstream dependencies;

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

    template <typename TItem>
    void for_every_child_do(const parse_tree::node& n, std::function<void(const parse_tree::node& n)> f)
    {
        auto predicate = [](const parse_tree::node& child) {return child.is<TItem>(); };

        auto& child = std::find_if(n.children.begin(), n.children.end(),
            [](const auto& child) {return child->is<TItem>(); });

        while (child != n.children.end()) {
            f(*child->get());

            child = std::find_if(++child, n.children.end(),
                [](const auto& child) {return child->is<TItem>(); });
        }
    }

    void print_message_thing(const parse_tree::node& n, const std::string& message)
    {
        std::string name = "undefined name";
        std::string type_id = "undefined type";

        if (n.is<field>()) {
            name = get_single_child<field_name>(n).string_view();

            auto& type = get_single_child<proto3::type>(n);
            type_id = type.string_view();

            definitions << "  " << name << ": " << type_id << std::endl;

            if (has_child<defined_type>(type)) {
                dependencies << "  " << message << " o-- " << type_id << std::endl;
            }
        }
        else if (n.is<repeated>()) {
            auto& f = get_single_child<field>(n);
            name = get_single_child<field_name>(f).string_view();

            auto& type = get_single_child<proto3::type>(f);
            type_id = type.string_view();

            definitions << "  " << name << ": " << type_id << "[]" << std::endl;

            if (has_child<defined_type>(type)) {
                dependencies << "  " << message << " \"1\" *-- \"0 .. n\" " << type.string_view() << std::endl;
            }
        }
        else if (n.is<oneof>()) {
            name = get_single_child<oneof_name>(n).string_view();

            definitions << "  " << name << ": union<";

            for_every_child_do<oneof_field>(n, [&](const auto& child) {
                definitions << get_single_child<type>(child).string_view() << ", ";
            });
            definitions.seekp(-2, std::ios_base::end);

            definitions << ">" << std::endl;
        }
        else {
            print(n);
        }

    }

    void print_message(const parse_tree::node& n)
    {
        std::string name = std::string(get_single_child<ident>(n).string_view());
        definitions << "class " << name << " <<(M,LightGreen)>>  {" << std::endl;

        for_every_child_do<message_thing>(n, [&](const auto& child)
        {
            print_message_thing(*(child.children.begin()->get()), name);
        });

        definitions << "}" << std::endl;
    }

    void print_enum(const parse_tree::node& n)
    {
        std::string name = std::string(get_single_child<enum_name>(n).string_view());
        definitions << "enum " << name << " {" << std::endl;

        for_every_child_do<enum_field>(n, [&](const auto& child) {
            definitions << "  " << get_single_child<ident>(child).string_view() << " = "
                << get_single_child<int_lit>(child).string_view() << std::endl;
        });

        definitions << "}" << std::endl;
    }

    void print(const parse_tree::node& n)
    {
        if (n.has_content()) {
            if (n.is<message>()) {
                print_message(n);
                return;
            }
            if (n.is<enum_>()) {
                print_enum(n);
                return;
            }
        }
        if (!n.children.empty()) {
            for (auto& up : n.children) {
                print(*up);
            }
        }
    }

    std::string get_puml()
    {
        std::stringstream puml;
        puml << "@startuml" << std::endl;
        puml << definitions.str() << std::endl;;
        puml << dependencies.str() << std::endl;;
        puml << "@enduml" << std::endl;
        return puml.str();
    }

public:
    static std::string run(const parse_tree::node& root_node)
    {
        assert(root_node.is_root());

        puml_printer pp;
        pp.print(root_node);
        return pp.get_puml();
    }
};

void print_puml(std::ostream& os, const parse_tree::node& n)
{
    try {
        os << puml_printer::run(n);
    }
    catch (std::exception& e) {
        os << e.what() << std::endl;
    }
    catch(...) {
        os << "Something terrible happened." << std::endl;
    }
}
}
