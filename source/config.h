#pragma once

#include <optional>
#include <string>

#include <clrie/method_info.h>

namespace appmap {
    struct config {
        std::optional<std::string> module_list_path;
        std::optional<std::string> appmap_output_path;

        std::vector<std::string> classes;
        std::vector<std::string> modules;

        bool generate_classmap = false;

        static config &instance();
        bool should_instrument(clrie::method_info method);
    };
}
