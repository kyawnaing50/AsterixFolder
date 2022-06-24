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


#include "jasterix.h"
#include "jsonwriter.h"
#include "jsonfilewritetask.h"
#include "logger.h"

#include <vector>
#include <tbb/tbb.h>

#include <archive.h>

#include <chrono>
#include <thread>

using namespace tbb;
using namespace std;

namespace jASTERIX
{

size_t JSONTextZipFileWriteTask::entry_cnt_ = 0;


JSONWriter::JSONWriter(JSON_OUTPUT_TYPE json_output_type, const std::string& json_path)
    : json_output_type_{json_output_type}, json_path_ {json_path}
{
    switch (json_output_type_)
    {
    case JSON_TEXT:
        openJsonFile();
        break;
    case JSON_ZIP_TEXT:
        openJsonZipFile();
        break;
    default:
        throw runtime_error ("unhandled JSON output type "+to_string(json_output_type_)+" during construction");
    }
}

JSONWriter::~JSONWriter ()
{
    if (json_data_.size())
        writeData();

    while (file_write_in_progress_)
    {
        //loginf << "JSONWriter: dtor: waiting for file write" << logendl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    if (json_file_open_)
        closeJsonFile ();

    if (json_zip_file_open_)
        closeJsonZipFile();
}

void JSONWriter::write(std::unique_ptr<nlohmann::json> data)
{
    switch (json_output_type_)
    {
    case JSON_TEXT:
    case JSON_ZIP_TEXT:
    {
        json_data_.push_back(std::move(data));
    }
        break;
    default:
        throw runtime_error ("unhandled JSON output type "+to_string(json_output_type_)+" write");
    }

    if (data_write_size > 0 && json_data_.size() > static_cast<size_t>(data_write_size))
        writeData();
}

void JSONWriter::writeData()
{
    assert (json_data_.size());

    for (std::unique_ptr<nlohmann::json>& j_it : json_data_)
        (*j_it)["rec_num"] = rec_num_cnt_++;

    // convert to string or binary data

    switch (json_output_type_)
    {
    case JSON_TEXT:
    case JSON_ZIP_TEXT:
        convertJSON2Text();
        break;
    default:
        throw runtime_error ("unhandled JSON output type "+to_string(json_output_type_)+" write data");
    }

    assert (!json_data_.size());

    switch (json_output_type_)
    {
    case JSON_TEXT:
        writeTextToFile ();
        break;
    case JSON_ZIP_TEXT:
        writeTextToZipFile();
        break;
    default:
        throw runtime_error ("unhandled JSON output type "+to_string(json_output_type_)+" write data");
    }

    assert (!text_data_.size());
    assert (!binary_data_.size());
}

void JSONWriter::convertJSON2Text ()
{
    assert (!text_data_.size());
    text_data_.resize(json_data_.size());

    size_t size = json_data_.size();

    tbb::parallel_for( size_t(0), size, [&]( size_t cnt )
    {
        text_data_[cnt] = json_data_.at(cnt)->dump(print_dump_indent) + "\n";
    } );

//    size_t cnt = 0;

//    for (nlohmann::json& j_it : json_data_)
//        text_data_[cnt++] = j_it.dump(4) + "\n";

    assert (text_data_.size() == json_data_.size());
    json_data_.clear();
}


void JSONWriter::openJsonFile ()
{
    assert (!json_file_open_);

    switch (json_output_type_)
    {
    case JSON_TEXT:
        json_file_.open (json_path_);
        break;
    case JSON_ZIP_TEXT:
        json_file_.open (json_path_, ios::out | ios::binary);
        break;
    default:
        throw runtime_error ("unhandled JSON output type "+to_string(json_output_type_)+" open");
    }

    json_file_open_ = true;
}

void JSONWriter::writeTextToFile ()
{
    assert (json_file_open_);
    assert (text_data_.size());

//    for (const std::string str_it : text_data_)
//        json_file_ << str_it;

//    text_data_.clear();

    while (file_write_in_progress_)
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

    file_write_in_progress_ = true;

    //loginf << "JSONWriter: new write task" << logendl;

    JSONTextFileWriteTask* write_task = new (tbb::task::allocate_root()) JSONTextFileWriteTask (
                json_file_, std::move(text_data_), *this);
    tbb::task::enqueue(*write_task);

    text_data_.clear();

    assert (!text_data_.size());
}

void JSONWriter::writeBinaryToFile ()
{
    assert (json_file_open_);
    assert (binary_data_.size());

//    for (const std::vector<std::uint8_t>& bin_it : binary_data_)
//        json_file_.write (reinterpret_cast<const char*>(bin_it.data()), bin_it.size());

//    binary_data_.clear();

    while (file_write_in_progress_)
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

    file_write_in_progress_ = true;

    JSONBinaryFileWriteTask* write_task = new (tbb::task::allocate_root()) JSONBinaryFileWriteTask (
                json_file_, std::move(binary_data_), *this);
    tbb::task::enqueue(*write_task);

    binary_data_.clear();

    assert (!binary_data_.size());
}

void JSONWriter::closeJsonFile ()
{
    json_file_.close();
    json_file_open_ = false;
}

void JSONWriter::openJsonZipFile ()
{
    assert (!json_zip_file_open_);

    json_zip_file_ = archive_write_new();
    archive_write_set_format_zip(json_zip_file_);
    archive_write_open_filename(json_zip_file_, json_path_.c_str());

    json_zip_file_open_ = true;
}

void JSONWriter::writeTextToZipFile ()
{
    assert (json_zip_file_open_);
    assert (text_data_.size());

//    for (const std::string str_it : text_data_)
//        archive_write_data (json_zip_file_, str_it.c_str(), str_it.size());

    while (file_write_in_progress_)
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

    file_write_in_progress_ = true;

    JSONTextZipFileWriteTask* write_task = new (tbb::task::allocate_root()) JSONTextZipFileWriteTask (
                json_zip_file_, std::move(text_data_), *this);
    tbb::task::enqueue(*write_task);

    text_data_.clear();
}

void JSONWriter::writeBinaryToZipFile ()
{
    assert (json_zip_file_open_);
    assert (binary_data_.size());

//    for (const std::vector<std::uint8_t> bin_it : binary_data_)
//        archive_write_data (json_zip_file_, reinterpret_cast<const void*>(bin_it.data()), bin_it.size());

//    binary_data_.clear();

    while (file_write_in_progress_)
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

    file_write_in_progress_ = true;

    JSONBinaryZipFileWriteTask* write_task = new (tbb::task::allocate_root()) JSONBinaryZipFileWriteTask (
                json_zip_file_, std::move(binary_data_), *this);
    tbb::task::enqueue(*write_task);

    assert (!binary_data_.size());
}

void JSONWriter::closeJsonZipFile ()
{
    archive_write_close(json_zip_file_); // Note 4
    archive_write_free(json_zip_file_); // Note 5

    json_zip_file_open_ = false;
}

void JSONWriter::fileWritingDone ()
{
    //loginf << "JSONWriter: fileWritingDone" << logendl;
    file_write_in_progress_ = false;
}

}
