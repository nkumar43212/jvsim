//
//  PathValidator.cpp
//  Test_programs
//
//  Created by Abbas Sakarwala on 5/24/16.
//
//

#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <cmath>
#include "PathValidator.hpp"

// PathInformation store
PathInformationDB path_information_db;

bool
PathValidator::_read_path_information_file (std::string filename,
                                           Json::Value &path_information_json)
{
    // Open file and read the file
    std::ifstream file(filename);
    std::stringstream path_information_buffer;
    path_information_buffer << file.rdbuf();
    std::string path_information_str = path_information_buffer.str();
    // std::cout << path_information_str << std::endl;

    Json::StyledWriter styledWriter;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(path_information_str,
                                          path_information_json);
    if (parsingSuccessful) {
        _logger->log("Json file \"" + filename + "\" parsed successfully");
        // _logger->log("JSONized output:");
        // _logger->log(styledWriter.write(path_information_json));
    } else {
        _logger->log("Json file \"" + filename + "\" parsing error");
        return false;
    }
    return true;
}

uint32_t
PathValidator::_roundToThousandths (uint32_t num, uint32_t max)
{
    num += 500;
    num /= 1000;
    num = num * 1000;
    num = (num > max) ? max: num;
    return num;
}

bool
PathValidator::build_path_information_db (std::string filename)
{
    Json::Value path_information_json;

    // Read from file and conver to json
    bool status;
    status = _read_path_information_file(filename, path_information_json);
    if (!status) {
        return status;
    }

    // Look for valid_paths
    path_information_json = path_information_json["valid_paths"];
    if (path_information_json == Json::nullValue) {
        _logger->log("\'valid_paths\' list keyword missing");
        return false;
    }

    // Extract from array
    const Json::Value default_value = Json::nullValue;
    if (path_information_json.isArray()) {
        // Size of array = ?
        int size = path_information_json.size();
        _logger->log("Number of paths provided in json file = " +
                     std::to_string(size));

        for (int i = 0; i < size; i++) {
            Json::Value obj = path_information_json.get(i, default_value);
            Json::StyledWriter styledWriter;
            _logger->log("Path " + std::to_string(i) +
                         " -->" + styledWriter.write(obj));

            // First check if "path" is present
            if (obj["path"] == Json::nullValue) {
                _logger->log("Path " + std::to_string(i) +
                             " --> Missing \'path\' keyword");
                // continue;
                return false;
            } else {
                PathInformationPtr pPathInformation =
                    std::make_shared<PathInformation>(obj["path"].asString());

                if (obj["filter_support"] != Json::nullValue) {
                    pPathInformation->filter_support =
                                obj["filter_support"].asBool();
                }

                if (obj["suppress_unchanged_support"] != Json::nullValue) {
                    pPathInformation->suppress_unchanged_support =
                                obj["suppress_unchanged_support"].asBool();
                }

                if (obj["max_silent_interval_support"] != Json::nullValue) {
                    pPathInformation->max_silent_interval_support =
                                obj["max_silent_interval_support"].asBool();
                }

                if (obj["sample_frequency"]["min"] != Json::nullValue) {
                    pPathInformation->sample_frequency_min =
                                obj["sample_frequency"]["min"].asInt();
                }

                if (obj["sample_frequency"]["max"] != Json::nullValue) {
                    pPathInformation->sample_frequency_max =
                                obj["sample_frequency"]["max"].asInt();
                }

                // Now add in the db store
                path_information_db[pPathInformation->path] = pPathInformation;
            }
        }
    } else {
        _logger->log("Invalid json format");
        return  false;
    }
    return true;
}

void
PathValidator::print_path_information_db (void)
{
    // Iterate
    for (PathInformationDB::iterator itr = path_information_db.begin();
         itr != path_information_db.end();
         itr++) {
        std::string pathinfo_str = "<PathInformation DB>" + itr->first + "--->";
        PathInformationPtr pPathInfo = itr->second;
        pathinfo_str.append("path:" + pPathInfo->path + ",");
        pathinfo_str.append("filter_support:" +
                std::to_string(pPathInfo->filter_support) + ",");
        pathinfo_str.append("suppress_unchanged_support:" +
                std::to_string(pPathInfo->suppress_unchanged_support) + ",");
        pathinfo_str.append("max_silent_interval_support:" +
                std::to_string(pPathInfo->max_silent_interval_support) + ",");
        pathinfo_str.append("sample_frequency_min:" +
                std::to_string(pPathInfo->sample_frequency_min) + ",");
        pathinfo_str.append("sample_frequency_max:" +
                std::to_string(pPathInfo->sample_frequency_max));
        _logger->log(pathinfo_str);
    }
}

void
PathValidator::delete_path_information_db (void)
{
    // Iterate
    _logger->log("Erase the path information db");

    PathInformationDB::iterator itr = path_information_db.begin();
    while (itr != path_information_db.end()) {
        itr = path_information_db.erase(itr);
    }
}

void
PathValidator::validate_path (Telemetry::Path &path)
{
    // Search the db
    PathInformationPtr pPathInformation = path_information_db[path.path()];
    if (pPathInformation == NULL) {
        path.Clear();
        return;
    }

    // Check filter
    if (!pPathInformation->filter_support) {
        path.clear_filter();
    }

    // Check suppress_unchanged
    if (!pPathInformation->suppress_unchanged_support) {
        path.clear_suppress_unchanged();
    }

    // Check max_silent_interval
    if (!pPathInformation->max_silent_interval_support) {
        path.clear_max_silent_interval();
    }

    // Check sample_frequency
    uint32_t sample_frequency = path.sample_frequency();
    uint32_t sample_frequency_min = pPathInformation->sample_frequency_min;
    uint32_t sample_frequency_max = pPathInformation->sample_frequency_max;
    if (sample_frequency < sample_frequency_min) {
        path.set_sample_frequency(sample_frequency_min);
    } else if (sample_frequency > sample_frequency_max) {
        path.set_sample_frequency(sample_frequency_max);
    } else {
        uint32_t rounded = _roundToThousandths(sample_frequency,
                                               sample_frequency_max);
        path.set_sample_frequency(rounded);
    }
}
