#include <filesystem>
#include <fstream>
#include <iostream>

#include "proto2puml.h"

int main(int argc, char** argv)
{
    auto sample_dir = std::filesystem::path(__FILE__).parent_path() / ".." / "samples";

    if (argc == 2)
    {
        sample_dir = std::string(argv[1]);
    }

    auto proto_file_count = 0;

    for (auto& file : std::filesystem::directory_iterator(sample_dir))
    {
        if (file.path().extension() == ".proto")
        {
            try
            {
                std::cout << "Processing " << file.path().lexically_relative(sample_dir).generic_string() << std::
                    endl;
                auto proto_file = file.path();
                std::ifstream proto_stream(proto_file);
                std::string proto_string((std::istreambuf_iterator<char>(proto_stream)),
                                         std::istreambuf_iterator<char>());

                auto puml_string = proto2puml::convert(proto_string);

                auto puml_file = proto_file.replace_extension("puml");
                std::ofstream puml_stream(puml_file);
                puml_stream << puml_string;
                proto_file_count++;
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
    }
    std::cout << "Please specify a directory with proto files to convert." << std::endl;

    return 1;
}
