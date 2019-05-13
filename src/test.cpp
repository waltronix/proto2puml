#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct node
{
    using TChild = std::unique_ptr<node>;

    std::string name = "";
    uint32_t id = 0;
    std::vector<TChild> children = {};

    node(std::string name_in, uint32_t id_in = 0) 
    : name(name_in), id(id_in) {};

    node& add(TChild&& child_in)
    {
        children.push_back(std::move(child_in));
        return *(children.back());
    }
};

void print(const node& n)
{
    for(auto& child : n.children)
    {
        std::cout << child->name << ", ";
        print(*child);
    }
    std::cout << std::endl;
}

void print2(const node& n)
{
    auto child = std::find_if(n.children.begin(), n.children.end(),
        [](const auto& child) {return child->id == 2; });

    while (child != n.children.end()) {
        std::cout << child->get()->name << ", ";
        print2(*(child->get()));
        
        child = std::find_if(++child, n.children.end(),
            [](const auto& child) {return child->id == 2; });
    }
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    std::cout << "Hello World!" << std::endl;

    auto a = std::make_unique<node>("a", 1);
    auto& b1 = a->add(std::make_unique<node>("b1", 2));
    auto& b2 = a->add(std::make_unique<node>("b2", 2));
    auto& b3 = a->add(std::make_unique<node>("b3", 2));
    
    auto& c1 = b1.add(std::make_unique<node>("c1", 3));
    auto& c2 = b1.add(std::make_unique<node>("c2", 3));

    print(*a);
    print2(*a);


    // node& n = *(a->get());

    // const auto& child = std::find_if(n.children.begin(), n.children.end(),
    //     [](const auto& child) {return child->id == 2; });

    // while (child != n.children.end()) {
    //     // f(*child->get());
        
    //     child = std::find_if(++child, n.children.end(),
    //         [](const auto& child) {return child.id == 2; });
    // }
}