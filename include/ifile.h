#include <fstream>
#include <iostream>
#include <sstream>

class ifile {
public:
    ifile(){}
    ifile(const std::string& filename){
        read(filename);
    }
    bool read(const std::string& filename){
        if (filename.empty())
        {
            std::cout << "No filename provided" << std::endl;
            return false;
        }

        std::fstream input_file(filename, std::fstream::in);

        if (!input_file.is_open())
        {
            std::cout << "Could not open file " << filename << std::endl;
            return false;
        }

        std::stringstream ss;
        ss << input_file.rdbuf();
        _contents = ss.str();

        return true;
    }

    const std::string get_contents() const {
        return _contents;
    }
private:
    std::string _contents;
};
