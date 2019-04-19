#include "puml_printer.hpp"

#include <cassert>
#include <ostream>

namespace proto2puml
{
    using namespace tao::pegtl;

    void puml_printer::print_field(const parse_tree::node& n, const std::string& message_name)
    {
        auto& name = get_string_view<proto3::field_name>(n);

        auto& type = get_first_child<proto3::type>(n);
        auto type_id = type.string_view();

        definitions << "  " << name << ": " << type_id << std::endl;

        if (has_child<proto3::defined_type>(type))
        {
            dependencies << "  " << message_name << " o-- " << type_id << std::endl;
        }
    }

    void puml_printer::print_repeated(const parse_tree::node& n, const std::string& message_name)
    {
        auto& field = get_first_child<proto3::field>(n);
        auto& name = get_string_view<proto3::field_name>(field);

        auto& type = get_first_child<proto3::type>(field);
        auto type_id = type.string_view();

        definitions << "  " << name << ": " << type_id << "[]" << std::endl;

        if (has_child<proto3::defined_type>(type))
        {
            dependencies << "  " << message_name << " *-- " << type.string_view() << std::endl;
        }
    }

    void puml_printer::print_oneof(const parse_tree::node& n, const std::string& message_name)
    {
        auto& name = get_string_view<proto3::oneof_name>(n);
        definitions << "  " << name << ": union<";

        for (const auto& child : n.children)
        {
            if (child->is<proto3::oneof_field>())
            {
                const auto child_type = get_string_view<proto3::type>(*child);

                definitions << child_type << ",\\n ";
                dependencies << "  " << message_name << " o-- " << child_type << std::endl;
            }
        }
        definitions.seekp(-2, std::ios_base::end);
        definitions << ">" << std::endl;
    }

    void puml_printer::print_map_field(const parse_tree::node& n, const std::string& message_name)
    {
        auto& name = get_string_view<proto3::map_name>(n);

        auto key_type = get_string_view<proto3::key_type>(n);
        auto type_id = get_string_view<proto3::type>(n);

        definitions << "  " << name << ": map<" << key_type << ", " << type_id << ">" << std::endl;
        dependencies << "  " << message_name << " *-- " << type_id << std::endl;
    }

    void puml_printer::print_message(const parse_tree::node& n)
    {
        std::string name = std::string(get_first_child<proto3::ident>(n).string_view());
        definitions << "class " << name << " <<(M,LightGreen)>>  {" << std::endl;

        for (const auto& child : n.children)
        {
            if (child->is<proto3::field>())
            {
                print_field(*child, name);
            }
            else if (child->is<proto3::map_field>())
            {
                print_map_field(*child, name);
            }
            else if (child->is<proto3::repeated>())
            {
                print_repeated(*child, name);
            }
            else if (child->is<proto3::oneof>())
            {
                print_oneof(*child, name);
            }
            else
            {
                //print(n);
            }
        }
        definitions << "}" << std::endl;
    }

    void puml_printer::print_enum(const parse_tree::node& n)
    {
        std::string name = std::string(get_first_child<proto3::enum_name>(n).string_view());
        definitions << "enum " << name << " {" << std::endl;

        for (const auto& child : n.children)
        {
            if (child->is<proto3::enum_field>())
            {
                definitions << "  " << get_first_child<proto3::ident>(*child).string_view() << " = "
                    << get_first_child<proto3::int_lit>(*child).string_view() << std::endl;
            }
        }
        definitions << "}" << std::endl;
    }

    void puml_printer::print(const parse_tree::node& n)
    {
        if (n.has_content())
        {
            if (n.is<proto3::message>())
            {
                print_message(n);
                return;
            }
            if (n.is<proto3::enum_>())
            {
                print_enum(n);
                return;
            }
        }
        if (!n.children.empty())
        {
            for (auto& up : n.children)
            {
                print(*up);
            }
        }
    }

    std::string puml_printer::get_puml()
    {
        std::stringstream puml;
        puml << "@startuml" << std::endl;
        puml << "hide empty members" << std::endl;
        puml << std::endl;
        puml << definitions.str() << std::endl;;
        puml << std::endl;
        puml << dependencies.str() << std::endl;;
        puml << std::endl;
        puml << "@enduml" << std::endl;
        return puml.str();
    }

    std::string puml_printer::run(const parse_tree::node& root_node)
    {
        assert(root_node.is_root());

        try
        {
            puml_printer pp;
            pp.print(root_node);
            return pp.get_puml();
        }
        catch (std::exception& e)
        {
            return e.what();
        }
        catch (...)
        {
            return "Something terrible happened.";
        }
    }
}
