#include "dataGenerator.h"

std::string DataGenerator::generatePassengerData() {
    std::stringstream ss;
    ss << "Passenger Name: John Doe, Passport Number: " << generatePassportNumber() << "\n";
    return ss.str();
}

std::string DataGenerator::generatePassportNumber() {
    static const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::uniform_int_distribution<std::size_t> distribution(0, sizeof(charset) - 2);
    std::string passportNumber;

    for (int i = 0; i < 9; ++i) {
        passportNumber += charset[distribution(generator)];
    }

    return passportNumber;
}
