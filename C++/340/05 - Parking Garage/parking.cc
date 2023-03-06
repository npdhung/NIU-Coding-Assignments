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
#include "parking.h"

void get_input_vals(const std::string &line, char &xact_type, std::string &license) {
    if ( line.length() == 0 ) exit(1);
    char ch[1];
    line.copy(ch, 1, 0);
    xact_type = ch[0];
    license = line.substr(2, line.length() - 3);
}

/**
 * Create a car object and use it to print itself as part of a message announcing its arrival.
 * If the garage is already full of cars (as defined by parking_lot_limit) then print a message
 * so indicating that fact (see the reference output). If the garage is not already full then
 * add the car to the parking_lot deque.
 *
 * @param license license plate number
 */
void garage::arrival(const std::string &license) {
       
    // Create an instance of car
    car carInst(next_car_id,license);
    ++next_car_id;
   
    if (parking_lot.size() == parking_lot_limit) {
        std::cout << carInst << " has arrived." << std::endl;
        std::cout << "    But the garage is full!" << std::endl;
    }
    else {
        parking_lot.push_front(carInst);
        std::cout << carInst << " has arrived." << std::endl;
    }
    std::cout << std::endl;
}

/**
 * Check each of the car elements in the parking_lot deque to see if any of them have
 * the given license. If no such car is found then print a message stating that fact.
 * Otherwise (the car has been found), move the car elements that are in the way
 * (that are closer to the exit) one at-a-time out of the deque and into a std::stack<car>
 * (increasing the number of times each have been moved along the way, and then
 * remove the car with given license from the parking_lot, print the car information
 * and how many time it has been moved, and move all the cars back to the parking_lot
 * so they remain the same order as they were before.
 *
 * @param license license plate number
 */
void garage::departure(const std::string &license) {
    auto i = parking_lot.begin();
    std::stack<car> carStack;
    while ( ( i != parking_lot.end() ) && (i->get_license() != license ) ) {
        ++i;
    }
    if (i->get_license() == license) {
        auto movePos = i;
        // start moving cars
        for (auto i = parking_lot.end()-1; i != movePos; --i) {
            i->move();
            parking_lot.pop_back();
            // move cars to a stack
            carStack.push(*i);
        }
        // output details and pop car
        auto j = parking_lot.back();
        std::cout << j << " has departed," << std::endl;
        j.move();
        int numMove = j.get_num_moves();
        std::cout << "    car was moved " << numMove;
        std::cout << (numMove > 1 ? " times": " time");
        std::cout << " in the garage." << std::endl;
        parking_lot.pop_back();
        
        // move cars from stack back to the deque
        while (!carStack.empty()) {
            parking_lot.push_back(carStack.top());
            carStack.pop();
        }
    }
    else { // No car with that license plate is in the garage
        std::cout << "No car with license plate \"" << license
            << "\" is in the garage." << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    
    std::string lic;
    char type;
    std::string input;
    garage pGarage;
    
    while (!std::cin.eof()) {
        std::getline(std::cin, input);
        get_input_vals(input, type, lic);
        if (type == 'A') {
            pGarage.arrival(lic);
        }
        else
            if (type == 'D') {
                pGarage.departure(lic);
            }
            else {
                std::cout << "'" << type << "': invalid action!"
                    << std::endl << std::endl;
            }
    }
    return 0;
}
