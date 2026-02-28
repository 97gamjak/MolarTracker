#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cerr << "Usage: generator <output.hpp> <src_dir>...\n";
        return 1;
    }

    const fs::path        output = argv[1];
    std::set<std::string> categories;

    std::regex pattern("REGISTER_LOG_CATEGORY\\s*\\(\\s*\"([^\"]+)\"\\s*\\)");

    for (int i = 2; i < argc; ++i)
    {
        for (const auto& path : fs::recursive_directory_iterator(argv[i]))
        {
            if (!path.is_regular_file())
                continue;

            if (path.path().extension() != ".cpp" &&
                path.path().extension() != ".hpp" &&
                path.path().extension() != ".tpp")
                continue;

            std::ifstream in(path.path());
            std::string   content(
                (std::istreambuf_iterator<char>(in)),
                std::istreambuf_iterator<char>()
            );

            std::smatch match;
            auto        begin = content.cbegin();

            while (std::regex_search(begin, content.cend(), match, pattern))
            {
                categories.insert(match[1].str());
                begin = match.suffix().first;
            }
        }
    }

    std::ofstream out(output);

    out << "#ifndef __LOG_CATEGORIES_GEN_HPP__\n";
    out << "#define __LOG_CATEGORIES_GEN_HPP__\n";
    out << "#include <set>\n";
    out << "#include <string_view>\n\n";
    out << "namespace logging\n";
    out << "{\n";
    out << "    class CategoryRegistry{\n";
    out << "      public:\n";
    out << "        static CategoryRegistry& getInstance()\n";
    out << "        {\n";
    out << "            static CategoryRegistry registry;\n";
    out << "            return registry;\n";
    out << "        }\n\n";
    out << "        std::set<std::string_view> categories = {\n";

    for (auto const& category : categories)
        out << "            \"" << category << "\",\n";

    out << "        };\n";
    out << "    };\n\n";
    out << "}\n\n";
    out << "#endif // __LOG_CATEGORIES_GEN_HPP__\n";
}