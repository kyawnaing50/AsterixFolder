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

#include "refedition.h"
#include "files.h"
#include "ref.h"

#include <fstream>

namespace jASTERIX
{

using namespace Files;
using namespace std;
using namespace nlohmann;

REFEdition::REFEdition(const std::string& number, const nlohmann::json& definition, const std::string& definition_path)
  : number_(number)
{
    //    "document": "SUR.ET1.ST05.2000-STD-04-01",
    if (!definition.contains("document"))
        throw runtime_error ("REFEdition '"+number_+"' has no document reference");

    document_ = definition.at("document");

    //    "date": "April 2007",
    if (!definition.contains("date"))
        throw runtime_error ("REFEdition '"+number_+"' has no date");

    date_ = definition.at("date");

    //    "file": "048/cat048_1.15.json"
    if (!definition.contains("file"))
        throw runtime_error ("REFEdition '"+number_+"' has no file");

    file_ = definition.at("file");

    edition_definition_path_ = definition_path+"/categories/"+file_;

    if (!fileExists(edition_definition_path_))
        throw invalid_argument ("REFEdition "+number_+" file '"+edition_definition_path_+"' not found");

    definition_ = json::parse(ifstream(edition_definition_path_));

    ref_.reset(new ReservedExpansionField(definition_));
}

REFEdition::~REFEdition()
{

}

std::string REFEdition::document() const
{
    return document_;
}

std::string REFEdition::date() const
{
    return date_;
}

std::string REFEdition::file() const
{
    return file_;
}

std::shared_ptr<ReservedExpansionField> REFEdition::reservedExpansionField() const
{
    return ref_;
}

std::string REFEdition::definitionPath() const
{
    return edition_definition_path_;
}

std::string REFEdition::number() const
{
    return number_;
}

}
