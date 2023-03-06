//***************************************************************************
//
//  Hung Nguyen
//  Z1924897
//  CSCI340 - 0002
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************
#ifndef PARKING_H
#define PARKING_H

#include <iostream>
#include <deque>
#include <stack>
#include <string>
#include <cstring>

class car
{
public:
    car(int id, const std::string &license) : id(id), license(license) {}

    /// Call this to increment the number of times the car has been moved.
    void move() {
        ++num_moves;
    }

    /// @return the number of times that the car has been moved.
    int get_num_moves() const { return num_moves; }

    /// @return A reference to the license for this car.
    const std::string &get_license() const { return license; }

    /// Overload the << so this can print itself.
    friend std::ostream& operator<<(std::ostream& lhs, const car& rhs)
    {
        lhs << "Car " << rhs.id << " with license plate \"" << rhs.license << "\"";
        return lhs;
    }

private:
    int id;                 ///< The ID number for this car (assigned by the garage)
    std::string license;    ///< license plate of this car.
    int num_moves = {0};    ///< how many times the car was moved within the garage.
};

/**
 * Helper function that extracts the individual components from the input line
 *
 * @param line input line
 * @param xact_type indicates if the line corresponds to an arrival or a departure
 * @param license license plate number
 *
 *********************************************************************************/
void get_input_vals(const std::string &line, char &xact_type, std::string &license);

class garage
{
public:
        garage(size_t limit = 10) : parking_lot_limit(limit) {}

        /// @param license The license of the car that has arrived.
        void arrival(const std::string &license);

        /// @param license The license of the car that has departed.
        void departure(const std::string &license);

private:
        int next_car_id = { 1 };
        std::deque<car> parking_lot;
        size_t parking_lot_limit;
};

#endif // PARKING_H
