/// @file reader.cpp
/// @brief
///
/// @author Roland Abel
/// @date August 30, 2024
///
/// Copyright (c) 2024 Roland Abel
///
/// This software is released under the MIT License.
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.

#include <istream>
#include <sstream>
#include <fstream>
#include "reader.h"
#include "itemset.h"

using namespace fim;

namespace fim::data {

    auto read_csv(std::istream &is, const read_csv_config_t &config) -> read_result_t {
        database_t database{};
        std::string line;

        if ((is.bad() || is.fail())) {
            return std::unexpected{io_error_t::UNKNOWN_ERROR};
        }

        for (auto i = 0; i < config.skip_rows; ++i) {
            std::getline(is, line);
        }

        while (std::getline(is, line)) {
            itemset_t itemset{};

            std::stringstream line_stream(line);
            std::string number;

            try {
                while (std::getline(line_stream, number, config.separator)) {
                    size_t value = std::stoull(number);
                    itemset.push_back(value);
                }
            } catch (const std::invalid_argument &) {
                return std::unexpected{io_error_t::INVALID_FORMAT};
            }
            catch (const std::out_of_range &) {
                return std::unexpected{io_error_t::VALUE_OUT_OF_RANGE};
            }

            if (line_stream.fail() && !line_stream.eof()) {
                return std::unexpected{io_error_t::INVALID_FORMAT};
            }

            database.push_back(std::move(itemset));
        }

        if (database.empty()) {
            return std::unexpected{io_error_t::EMPTY_ERROR};
        }

        return std::move(database);
    }

    auto read_csv(const std::string_view &file_path, const read_csv_config_t &config) -> read_result_t {
        std::ifstream ifs(file_path.data());
        if (!ifs.is_open()) {
            return std::unexpected{io_error_t::FILE_NOT_FOUND};
        }

        return read_csv(ifs, config);
    }

    auto read_result_csv(std::istream &is, const read_csv_config_t &config) -> result_t {

        // Hilfsfunktion zum Entfernen von Anführungszeichen
        auto remove_quotes = [](std::string &str) -> void {
            // Entfernt führende und abschließende Anführungszeichen, falls vorhanden
            if (!str.empty() && str.front() == '"') {
                str.erase(0, 1);  // Führendes "
                str.erase(str.find_last_of('"'), 1);  // Abschließendes "
            }
        };

        std::vector<itemset_support_t> result;  // Vektor für die Resultate
        std::string line;

        if (is.bad() || is.fail()) {
            throw std::runtime_error("Error with the input stream");
        }

        // Zeilen überspringen, wenn notwendig
        for (size_t i = 0; i < config.skip_rows; ++i) {
            std::getline(is, line);
        }

        // Datei Zeile für Zeile einlesen
        while (std::getline(is, line)) {
            std::stringstream line_stream(line);
            std::string itemset_str, support_str;

            try {
                // Einlesen des Itemsets und des Supports
                if (!std::getline(line_stream, itemset_str, config.separator) ||
                    !std::getline(line_stream, support_str)) {
                    throw std::invalid_argument("Invalid format in CSV line");
                }

                // Entferne mögliche Anführungszeichen aus dem Itemset
                remove_quotes(itemset_str);

                // Itemset im Format "x, y" umwandeln (z.B. "3, 50" in {3, 50})
                itemset_t itemset;
                std::stringstream itemset_stream(itemset_str);
                std::string number;

                // Extrahiere alle durch Komma getrennten Zahlen
                while (std::getline(itemset_stream, number, ',')) {
                    size_t value = std::stoull(number);  // Umwandeln in size_t
                    itemset.push_back(value);
                }

                // Support-Wert als float umwandeln
                float support = std::stof(support_str);

                // Ein Itemset und den Support-Wert in die Result-Liste speichern
                result.push_back(itemset_support_t{std::move(itemset), support});
            } catch (const std::exception &e) {
                // Fehlerbehandlung bei ungültigem Format
                // std::cerr << "Error parsing line: " << e.what() << std::endl;
                throw std::runtime_error("Error parsing the CSV file");
            }
        }

        // Rückgabe des Vektors mit den Itemsets und Support-Werten
        return result;
    }


    auto read_result_csv(const std::string_view &file_path, const read_csv_config_t &config) -> result_t {
        std::ifstream ifs(file_path.data());
        if (!ifs.is_open()) {
            return result_t{};
        }

        return read_result_csv(ifs, config);
    }
}