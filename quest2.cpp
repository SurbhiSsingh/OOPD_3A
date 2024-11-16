#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include <cmath> // Include cmath for math functions
#include <stdexcept> // For exception handling

// Custom exception for invalid platform
class PlatformNotFoundException : public std::runtime_error {
public:
    PlatformNotFoundException(const std::string& message)
        : std::runtime_error(message) {}
};

// Custom exception for scheduling conflict
class SchedulingConflictException : public std::runtime_error {
public:
    SchedulingConflictException(const std::string& message)
        : std::runtime_error(message) {}
};

class Line {
public:
    std::string name;

    Line(const std::string& lineName) : name(lineName) {}
};

class Platform {
private:
    int id;
    std::vector<std::time_t> stoppageTimings;
    std::vector<std::time_t> throughTimings;

public:
    Platform(int platformID) : id(platformID) {}

    bool canAccommodateStoppage(std::time_t time) {
        for (const auto& t : stoppageTimings) {
            if (std::abs(std::difftime(t, time)) < 30 * 60) {
                throw SchedulingConflictException("Stoppage time conflicts with an existing stoppage.");
            }
        }
        stoppageTimings.push_back(time);
        return true;
    }

    bool canAccommodateThrough(std::time_t time) {
        for (const auto& t : throughTimings) {
            if (std::abs(std::difftime(t, time)) < 10 * 60) {
                throw SchedulingConflictException("Through train time conflicts with an existing through train.");
            }
        }
        throughTimings.push_back(time);
        return true;
    }

    int getID() const {
        return id;
    }
};

class Station {
private:
    std::string idStr;
    int idInt;
    bool isStringID;
    std::vector<std::shared_ptr<Line>> lines;
    std::vector<Platform> platforms;

public:
    Station(const std::string& stationID) : idStr(stationID), isStringID(true) {}
    Station(int stationID) : idInt(stationID), isStringID(false) {}

    void addLine(const std::string& lineName) {
        lines.push_back(std::make_shared<Line>(lineName));
    }

    void addPlatform(int platformID) {
        platforms.emplace_back(platformID);
    }

    bool scheduleStoppage(int platformID, std::time_t time) {
        for (auto& platform : platforms) {
            if (platform.getID() == platformID) {
                try {
                    return platform.canAccommodateStoppage(time);
                } catch (const SchedulingConflictException& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    return false;
                }
            }
        }
        throw PlatformNotFoundException("Platform ID not found.");
    }

    bool scheduleThrough(int platformID, std::time_t time) {
        for (auto& platform : platforms) {
            if (platform.getID() == platformID) {
                try {
                    return platform.canAccommodateThrough(time);
                } catch (const SchedulingConflictException& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    return false;
                }
            }
        }
        throw PlatformNotFoundException("Platform ID not found.");
    }

    void displayID() const {
        if (isStringID) {
            std::cout << "Station ID (String): " << idStr << "\n";
        } else {
            std::cout << "Station ID (Integer): " << idInt << "\n";
        }
    }

    void displayLines() const {
        std::cout << "Lines:\n";
        for (const auto& line : lines) {
            std::cout << "- " << line->name << "\n";
        }
    }
};

std::time_t getTestTime(int hour, int minute) {
    std::tm timeStruct = {};
    timeStruct.tm_hour = hour;
    timeStruct.tm_min = minute;
    return std::mktime(&timeStruct);
}

int main() {
    try {
        std::shared_ptr<Station> station = std::make_shared<Station>(1001);
        station->addLine("Blue Line");
        station->addLine("Yellow Line");
        station->addPlatform(1);
        station->addPlatform(2);

        // Test correct stoppage time
        std::time_t stoppageTime1 = getTestTime(10, 0);
        if (station->scheduleStoppage(1, stoppageTime1)) {
            std::cout << "Stoppage scheduled successfully.\n";
        }

        // Test scheduling a through train at a different time
        std::time_t throughTime1 = getTestTime(10, 30);
        if (station->scheduleThrough(1, throughTime1)) {
            std::cout << "Through train scheduled successfully.\n";
        }

        // Attempt to schedule a conflicting stoppage
        std::time_t stoppageTime2 = getTestTime(10, 15);
        if (!station->scheduleStoppage(1, stoppageTime2)) {
            std::cout << "Conflict: Could not schedule stoppage.\n";
        }

        // Attempt to schedule a conflicting through train
        std::time_t throughTime2 = getTestTime(10, 5);
        if (!station->scheduleThrough(1, throughTime2)) {
            std::cout << "Conflict: Could not schedule through train.\n";
        }

    } catch (const PlatformNotFoundException& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }

    return 0;
}
