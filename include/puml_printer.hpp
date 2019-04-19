// Parts are taken from from parse_tree_to_dot.hpp:
//
// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#pragma once

#include <string>

#include <tao/pegtl/contrib/parse_tree.hpp>

#include "proto3_grammar.hpp"

namespace proto2puml
{
    namespace parse_tree = tao::pegtl::parse_tree;

    class puml_printer
    {
        std::stringstream definitions;
        std::stringstream dependencies;


        template <typename TItem>
        static bool is(const std::unique_ptr<parse_tree::node>& node)
        {
            return node->id == typeid(TItem);
        }

        template <typename TItem>
        bool has_child(const parse_tree::node& n)
        {
            const auto& found_items = std::find_if(
                n.children.begin(), n.children.end(), puml_printer::is<TItem>);

            return found_items != n.children.end();
        }

        template <typename TItem>
        const parse_tree::node& get_first_child(const parse_tree::node& n)
        {
            const auto& found_items = std::find_if(
                n.children.begin(), n.children.end(), puml_printer::is<TItem>);

            if (found_items != n.children.end())
            {
                return *(found_items->get());
            }

            const auto error_message = std::string("No item of type '") + typeid(TItem).name() + "' available.";
            throw std::invalid_argument(error_message.c_str());
        }

        template <typename TItem>
        const std::string_view get_string_view(const parse_tree::node& n)
        {
            const auto& found_items = std::find_if(
                n.children.begin(), n.children.end(), puml_printer::is<TItem>);

            if (found_items == n.children.end())
            {
                return "";
            }
            return found_items->get()->string_view();
        }

        void print_field(const parse_tree::node& n, const std::string& message_name);

        void print_repeated(const parse_tree::node& n, const std::string& message_name);

        void print_oneof(const parse_tree::node& n, const std::string& message_name);

        void print_map_field(const parse_tree::node& n, const std::string& message_name);

        void print_message(const parse_tree::node& n);

        void print_enum(const parse_tree::node& n);


        void print(const parse_tree::node& n);

        std::string get_puml();

    public:
        static std::string run(const parse_tree::node& root_node);
    };
}
