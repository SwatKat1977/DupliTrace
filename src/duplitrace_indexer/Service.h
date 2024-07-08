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
#ifndef SERVICE_H_
#define SERVICE_H_
#include "ConfigManager.h"

namespace duplitrace { namespace indexer {

class Service {
 public:

     Service();

     bool Initialise(common::SectionsMap* layout, std::string file);

     void Execute();

 private:
     bool initialised_;
     std::string config_file_;
     common::SectionsMap *config_layout_;
     common::ConfigManager config_manager_;
     bool shutdown_requested_;

     bool ReadConfiguration();

     bool InitialiseLogger();
};

}   // namespace indexer
}   // namespace duplitrace

#endif  // SERVICE_H_
