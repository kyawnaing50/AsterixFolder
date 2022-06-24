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


#ifndef SPFEDITION_H
#define SPFEDITION_H

#include "json.hpp"
#include <jasterix/spf.h>

#include <string>

namespace jASTERIX
{

class Record;

class SPFEdition
{
public:
    SPFEdition(const std::string& number, const nlohmann::json& definition, const std::string& definition_path);
    virtual ~SPFEdition();

    std::string number() const;
    std::string document() const;
    std::string date() const;
    std::string file() const;

    std::shared_ptr<SpecialPurposeField> specialPurposeField() const;
    std::string definitionPath() const;

protected:
    std::string number_;
    std::string document_;
    std::string date_;
    std::string file_;

    std::string edition_definition_path_;
    nlohmann::json definition_; // from file
    std::shared_ptr<SpecialPurposeField> spf_;
};

}
#endif // SPFEDITION_H
