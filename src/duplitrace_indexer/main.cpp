/*
This source file is part of DupliTrace
For the latest info, see https://github.com/SwatKat1977/DupliTrace

Copyright 2024 DupliTrace Development Team

    This program is free software : you can redistribute it and /or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see < https://www.gnu.org/licenses/>.
*/
#include <cstdlib>
#include <filesystem>
#include "argparse.hpp"

const std::string DEFAULT_CONFIG_FILE = "./config.cfg";

int main(int argc, char **argv) {

    bool verbose = false;

    argparse::ArgumentParser arguments_parser(argv[0]);
    arguments_parser.add_argument("-c", "--config")
        .default_value(DEFAULT_CONFIG_FILE)
        .help("Configuration file");
    arguments_parser.add_argument ("--verbose")
        .help ("increase output verbosity")
        .implicit_value(true)
        .flag ();

    try {
        arguments_parser.parse_args (argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << err.what () << std::endl;
        std::cerr << arguments_parser;
        return EXIT_FAILURE;
    }

    auto config_file = arguments_parser.get<std::string> ("--config");
    std::cout << "[INFO] Config File : " << config_file << std::endl;

    if (arguments_parser["--verbose"] == true) {
        std::cout << "[INFO] Setting verbose mode" << std::endl;
        verbose = true;
    }

    if (!std::filesystem::is_regular_file(config_file)) {
        std::cout << "[ERROR] Config file '" << config_file
            << "' is not a valid file/cannot be read" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
