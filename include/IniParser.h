/*

ini format:

[section]
a = 1
b = 2

[section2]
array = 1.0, 2.0, 3.0
another = 1, 0, 1

*/

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <string>

template<typename T>
std::vector<T> to_vector(const std::string& s){
    std::vector<T> result;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, ',')){
        boost::algorithm::trim(item);
        result.push_back(boost::lexical_cast<T>(item));
    }
    return result;
}

class IniParser {
    boost::property_tree::ptree pt;
public:
    IniParser(std::string filename){
        boost::property_tree::ini_parser::read_ini(filename, pt);
    }

    template<typename T>
    T get(std::string sectionvar){
        return pt.get<T>(sectionvar);
    }

    template<typename T>
    std::vector<T> get_array(std::string sectionvar){
        return to_vector<T>(pt.get<std::string>(sectionvar));
    }
};