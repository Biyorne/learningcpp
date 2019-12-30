// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

//

struct Monster
{
    std::string rules;
    std::string canon;
    std::string setting;
    std::string release;
    std::string source;
    std::string page;
    std::string url;
    std::string entry;
    std::string monster;
    std::string size;
    std::string origin;
    std::string type;
    std::string keywords;
    std::string level;
    std::string role;
    std::string xp;
    std::string subtypes;
    std::string clas;
    std::string environment;
    std::string cr;
    std::string la;
    std::string classify1;
    std::string classify2;
    std::string classify3;
    std::string classify4;
    std::string classify5;
    std::string alternativeNames;
};

inline bool operator==(const Monster & left, const Monster & right)
{
    // try optimize by monster/page/source

    return (
        std::tie(
            left.monster,
            left.source,
            left.page,
            left.rules,
            left.canon,
            left.setting,
            left.release,
            left.url,
            left.entry,
            left.size,
            left.origin,
            left.type,
            left.keywords,
            left.level,
            left.role,
            left.xp,
            left.subtypes,
            left.clas,
            left.environment,
            left.cr,
            left.la,
            left.classify1,
            left.classify2,
            left.classify3,
            left.classify4,
            left.classify5,
            left.alternativeNames) ==
        std::tie(
            right.monster,
            right.source,
            right.page,
            right.rules,
            right.canon,
            right.setting,
            right.release,
            right.url,
            right.entry,
            right.size,
            right.origin,
            right.type,
            right.keywords,
            right.level,
            right.role,
            right.xp,
            right.subtypes,
            right.clas,
            right.environment,
            right.cr,
            right.la,
            right.classify1,
            right.classify2,
            right.classify3,
            right.classify4,
            right.classify5,
            right.alternativeNames));
}

inline bool operator!=(const Monster & left, const Monster & right) { return !(left == right); }

inline bool operator<(const Monster & left, const Monster & right)
{
    return (
        std::tie(
            left.monster,
            left.source,
            left.page,
            left.rules,
            left.canon,
            left.setting,
            left.release,
            left.url,
            left.entry,
            left.size,
            left.origin,
            left.type,
            left.keywords,
            left.level,
            left.role,
            left.xp,
            left.subtypes,
            left.clas,
            left.environment,
            left.cr,
            left.la,
            left.classify1,
            left.classify2,
            left.classify3,
            left.classify4,
            left.classify5,
            left.alternativeNames) <
        std::tie(
            right.monster,
            right.source,
            right.page,
            right.rules,
            right.canon,
            right.setting,
            right.release,
            right.url,
            right.entry,
            right.size,
            right.origin,
            right.type,
            right.keywords,
            right.level,
            right.role,
            right.xp,
            right.subtypes,
            right.clas,
            right.environment,
            right.cr,
            right.la,
            right.classify1,
            right.classify2,
            right.classify3,
            right.classify4,
            right.classify5,
            right.alternativeNames));
}

void readLine(std::istringstream & iss, std::vector<std::string> & fields)
{
    std::string field;
    while (std::getline(iss, field, '\t'))
    {
        fields.push_back(field);
    }

    if (fields.size() < 26)
    {
        throw std::runtime_error(
            "found row with less than the required/expected 27 fields: \"" + iss.str() + "\"");
    }
}

void readFile(std::ifstream & fstream, std::vector<std::vector<std::string>> & fieldsVec)
{
    fieldsVec.reserve(100'000);

    std::vector<std::string> fields;
    fields.reserve(32);

    std::string line;
    while (std::getline(fstream, line, '\n'))
    {
        readLine(std::istringstream(line), fieldsVec.emplace_back());
    }
}

int main()
{
    // read the file
    std::ifstream fstream("monsters.tsv", std::fstream::in);
    if (!fstream.is_open())
    {
        throw std::runtime_error("Failed to open monsters.tsv");
    }

    std::vector<std::vector<std::string>> fieldsVec;
    readFile(fstream, fieldsVec);

    // verify
    const std::size_t lineCount{ fieldsVec.size() };
    std::cout << "Read " << lineCount << " lines." << std::endl;

    std::map<std::size_t, std::size_t> fieldsCount;
    for (const std::vector<std::string> & vec : fieldsVec)
    {
        fieldsCount[vec.size()]++;
    }

    for (const auto & [fieldCount, linesCount] : fieldsCount)
    {
        std::cout << linesCount << " lines with " << fieldCount << " fields." << std::endl;
    }

    // analyze
    // TODO

    // create monsters container
    std::vector<Monster> monsters;
    monsters.reserve(fieldsVec.size());

    Monster monster;
    for (const std::vector<std::string> & fields : fieldsVec)
    {
        // clang-format off
        monster.rules            = fields[0];
        monster.canon            = fields[1];
        monster.setting          = fields[2];
        monster.release          = fields[3];
        monster.source           = fields[4];
        monster.page             = fields[5];
        monster.url              = fields[6];
        monster.entry            = fields[7];
        monster.monster          = fields[8];
        monster.size             = fields[9];
        monster.origin           = fields[10];
        monster.type             = fields[11];
        monster.keywords         = fields[12];
        monster.level            = fields[13];
        monster.role             = fields[14];
        monster.xp               = fields[15];
        monster.subtypes         = fields[16];
        monster.clas             = fields[17];
        monster.environment      = fields[18];
        monster.cr               = fields[19];
        monster.la               = fields[20];
        monster.classify1        = fields[21];
        monster.classify2        = fields[22];
        monster.classify3        = fields[23];
        monster.classify4        = fields[24];
        monster.classify5        = fields[25];
        monster.alternativeNames = fields[26];
        // clang-format on

        monsters.push_back(monster);
    }

    // detect duplicates
    std::sort(std::begin(monsters), std::end(monsters));
    monsters.erase(std::unique(std::begin(monsters), std::end(monsters)), std::end(monsters));
    const std::size_t origCount{ fieldsVec.size() };

    std::cout << "There were " << (lineCount - origCount) << " duplicates." << std::endl;

    return EXIT_SUCCESS;
}