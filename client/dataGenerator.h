#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <cstdint>

class DataGenerator {
public:
    std::string generatePassengerData();
    std::string generatePassportNumber();

private:
    std::mt19937 generator;
    DataGenerator();


};



