/*
 * This file is part of ATSDB.
 *
 * ATSDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ATSDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ATSDB.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef JASTERIX_FILES_H
#define JASTERIX_FILES_H

#include <string>
#include <vector>

namespace jASTERIX
{

namespace Files
{

bool fileExists(const std::string& path);
size_t fileSize(const std::string& path);
bool directoryExists(const std::string& path);
std::vector<std::string> getFilesInDirectory (const std::string& path);

}
}

#endif // JASTERIX_FILES_H
