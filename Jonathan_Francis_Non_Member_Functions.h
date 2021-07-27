// Jonathan Francis Project
// PHYS 30762 Programming in C++
// AC Circuit
// Non Member Function Definitions

#pragma once
#include <iostream>
#include <set>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <map>
#include <type_traits>
#include "Jonathan_Francis_Project.h"

//                      //
// NON MEMBER FUNCTIONS //
//                      //


// Find the contributions from the wires
std::pair<double, double> get_wire_contributions()
{
    // Set wire specifications
    std::cout << "Please specify the length, thickness, and spacing of your wires in the following form '40 1 10', where units are given in mm. If you would not like to make these specifications, type '1 1 1' ";
    double wire_length;
    double wire_thickness;
    double wire_spacing;
    std::cin >> wire_length >> wire_thickness >> wire_spacing;
    // Check for fail, clear, and replace
    while (std::cin.fail() || wire_length <= 0 || wire_thickness <= 0 || wire_spacing <= 0) {
        std::cout << "Sorry, your input was not valid. Please enter appropriate wire specifications. ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        std::cin >> wire_length >> wire_thickness >> wire_spacing;
    }

    // Set default specifications
    if (wire_length == wire_thickness == wire_spacing == 1) {
        wire_length = 40;
        wire_thickness = 1;
        wire_spacing = 10;
    }

    // Define lead inductance and parasitic capacitance for the circuit
    double lead_inductance = 2 * wire_length * 0.001 * (2.303 * log10(4*wire_length/wire_thickness) - 0.75 * (wire_thickness / (2 * wire_length)));
    double parasitic_capacitance = M_PI * 8.85 * pow(10,-12) * wire_length * 0.001 / log(2 * wire_spacing / wire_thickness);

    return std::pair(lead_inductance, parasitic_capacitance);
}

// Create a map of components based on user input
std::map<std::string, std::shared_ptr<component>> select_components(double parasitic_capacitance, double lead_inductance)
{
    // Declare a char to hold user response: 'y' or 'n'
	char yesno;

    // Ask user for number of components
    std::cout << std::endl << "How many components would you like to add to the list? You don't have to use them all in your circuit; they can be added later on: ";
    int n;
    std::cin >> n;
    // Check for fail, clear, and replace
    while (std::cin.fail() || n < 1) {
        std::cout << "Sorry, your input was not valid. Please enter at least one component. ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        std::cin >> n;
    }

    // Ask user to add components to the map
    std::cout << std::endl << "Please provide a list of all the components you want to use: " << std::endl;

    // Create a map to hold components
    std::map<std::string, std::shared_ptr<component>> components;

    // Variable to keep track of how many components the user has added
    int component_number = 0;

    while (component_number < n) {
        // User indicates type of component
        std::cout << std::endl << "Would you like to add a resistor, capacitor, or inductor (1,2,3)? One at a time, please: ";
        int component_choice;
        std::cin >> component_choice;
        // Check for fail, clear, and replace
        while (std::cin.fail() || component_choice < 1 || component_choice > 3) {
            std::cout << "Sorry, your input was not valid. Please enter 1, 2, or 3. ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cin >> component_choice;
        }

        // User gives unique component names as keys for the map
        std::cout << "Please provide a UNIQUE name tag to component: ";
        std::string component_name;
        std::cin >> component_name;
        while (std::cin.fail() || components.find(component_name) != components.end()) {
            std::cout << "Sorry, your input was not valid. Please enter a unique name tag. ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cin >> component_name;
        }

        // Switch statement for initialisation of different component types
        switch (component_choice) {
            // RESISTOR
            case 1: {
                // Add resistance
                std::cout << "Please provide a resistance (\u03A9): ";
                double resistance;
                std::cin >> resistance;
                while (std::cin.fail() || resistance <= 0) {
                    std::cout << "Sorry, your input was not valid. Please enter a valid resistance. ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                    std::cin >> resistance;
                }

                // Choose quality of resistor
                std::cout << "Would you like to use a non-ideal resitor (y/n)? ";
                std::cin >> yesno;
                // Check for fail, clear, and replace
                while (std::cin.fail() || (yesno != 'y' && yesno != 'n')) {
                    std::cout << "Sorry, your input was not valid. Please enter y or n. ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                    std::cin >> yesno;
                }

                if (yesno == 'y') {
                    // Add non-ideal resistor and key to map
                    // Exception handling block as memory is dynamically allocated
                    try
                    {
                        components.insert(std::pair(component_name, new non_ideal_resistor(parasitic_capacitance,lead_inductance,resistance,component_name)));
                    }
                    catch (const std::bad_alloc& b) {
                        std::cerr << "Memory allocation failure" << std::endl;
                        throw;
                    }
                }
                else if (yesno == 'n') {
                    // Add ideal resistor and key to map
                    // Exception handling block as memory is dynamically allocated
                    try
                    {
                        components.insert(std::pair(component_name, new resistor(resistance,component_name)));
                    }
                    catch (const std::bad_alloc& b) {
                        std::cerr << "Memory allocation failure" << std::endl;
                        throw;
                    }
                }

                // Increment component number
                component_number++;

                break;
            }
            // CAPACITOR
            case 2: {
                // Add capacitance
                std::cout << "Please provide a capacitance (F): ";
                double capacitance;
                std::cin >> capacitance;
                while (std::cin.fail() || capacitance <= 0) {
                    std::cout << "Sorry, your input was not valid. Please enter a valid resistance. ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                    std::cin >> capacitance;
                }

                // Choose quality of capacitor
                std::cout << "Would you like to use a non-ideal capacitor (y/n)? ";
                std::cin >> yesno;
                // Check for fail, clear, and replace
                while (std::cin.fail() || (yesno != 'y' && yesno != 'n')) {
                    std::cout << "Sorry, your input was not valid. Please enter y or n. ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                    std::cin >> yesno;
                }

                if (yesno == 'y') {
                    // Ask user for plate resistance
                    std::cout << "Please provide the plate resistance (Suggested range: 1-10% of resistor resistances) (\u03A9): ";
                    double plate_resistance;
                    std::cin >> plate_resistance;
                    while (std::cin.fail() || plate_resistance <= 0) {
                        std::cout << "Sorry, your input was not valid. Please enter a valid resistance. ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                        std::cin >> plate_resistance;
                }

                    // Add non-ideal capacitor and key to map
                    // Exception handling block as memory is dynamically allocated
                    try 
                    {
                        components.insert(std::pair(component_name, new non_ideal_capacitor(plate_resistance,lead_inductance,capacitance,component_name)));
                    } 
                    catch (const std::bad_alloc& b) {
                        std::cerr << "Memory allocation failure" << std::endl;
                        throw;
                    }
                }
                else if (yesno == 'n') {
                    // Add capacitor and key to map
                    // Exception handling block as memory is dynamically allocated
                    try
                    {
                        components.insert(std::pair(component_name, new capacitor(capacitance,component_name)));
                    }
                    catch (const std::bad_alloc& b) {
                        std::cerr << "Memory allocation failure" << std::endl;
                        throw;
                    }
                }

                // Increment component number
                component_number++;

                break;
            }
            // INDUCTOR
            case 3: {
                // Add inductance
                std::cout << "Please provide an inductance (H): ";
                double inductance;
                std::cin >> inductance;
                while (std::cin.fail() || inductance <= 0) {
                    std::cout << "Sorry, your input was not valid. Please enter a valid resistance. ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                    std::cin >> inductance;
                }

                // Choose quality of inductor
                std::cout << "Would you like to use a non-ideal inductor (y/n)? ";
                std::cin >> yesno;
                // Check for fail, clear, and replace
                while (std::cin.fail() || (yesno != 'y' && yesno != 'n')) {
                    std::cout << "Sorry, your input was not valid. Please enter y or n. ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                    std::cin >> yesno;
                }

                if (yesno == 'y') {
                    // Ask user for parasitic resistance
                    std::cout << "Please provide the parasitic resistance (Suggested range: 1-10% of resistor resistances) (\u03A9): ";
                    double parasitic_resistance;
                    std::cin >> parasitic_resistance;
                    while (std::cin.fail() || parasitic_resistance <= 0) {
                        std::cout << "Sorry, your input was not valid. Please enter a valid resistance. ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                        std::cin >> parasitic_resistance;
                }

                    // Add non-ideal inductor and key to map
                    // Exception handling block as memory is dynamically allocated
                    try 
                    {
                        components.insert(std::pair(component_name, new non_ideal_inductor(parasitic_resistance,parasitic_capacitance,inductance,component_name)));
                    }
                    catch (const std::bad_alloc& b) {
                        std::cerr << "Memory allocation failure" << std::endl;
                        throw;
                    }
                }
                else if (yesno == 'n') {
                    // Add inductor and key to map
                    // Exception handling block as memory is dynamically allocated
                    try
                    {
                        components.insert(std::pair(component_name, new inductor(inductance,component_name)));
                    }
                    catch(const std::bad_alloc& b)
                    {
                        std::cerr << "Memory allocation failure" << std::endl;
                        throw;
                    }
                }

                // Increment component number
                component_number++;

                break;
            }
        }
    }

    // Create space in the terminal
    std::cout << "\n\n\n\n\n\n\n\n";

    return components;
}
   

// Add links to the circuit
std::vector<link> add_links(char add_link, std::vector<link> links, std::map<std::string, std::shared_ptr<component>> components)
{
    while (add_link == 'y') {
        // User inputs two component names
        std::cout << std::endl << "Please add a pair: " << std::endl;
        std::string input_name1;
        std::string input_name2;
        std::cin >> input_name1 >> input_name2;
        while (std::cin.fail() || components.find(input_name1) == components.end() || components.find(input_name2) == components.end()) {
            std::cout << "Sorry, your input was not valid. Please enter a unique name tag. ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cin >> input_name1 >> input_name2;
        }

        // Search for these components in the map and add to a temporary link 
        link temp_link;
        temp_link.start = components.find(input_name1)->second;
        temp_link.end = components.find(input_name2)->second;

        // Add link to vector of links
        links.push_back(temp_link);

        // Ask user if they want to add another link
        std::cout << "Would you like to add another pair to the circuit (y/n)? ";
        std::cin >> add_link;
        // Check for fail, clear, and replace
        while (std::cin.fail() || (add_link != 'y' && add_link != 'n')) {
            std::cout << "Sorry, your input was not valid. Please enter y or n. ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cin >> add_link;
        }
    }

    return links;
}

// Update the map of components after changing circuit frequency
std::map<std::string, std::shared_ptr<component>> set_frequency(std::map<std::string, std::shared_ptr<component>> components)
{
    std::cout << std::endl << "What would you like the frequency of the circuit to be (Hz)? ";
    std::cin >> component::frequency;
    // Check for fail, clear, and replace
    while (std::cin.fail() || component::frequency < 0) {
        std::cout << "Sorry, your input was not valid. Please enter a positive frequency. ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        std::cin >> component::frequency;
    }

    // Change impedances of all components to match new frequency
    std::for_each(components.begin(),components.end(), [](auto comp) {
        comp.second->set_impedance();
    });

    return components;
}

// Print out values pointed to by a component*
void const print_pointer(const std::shared_ptr<component> c)
{
    std::cout << c->get_name() << std::endl;
    std::cout << " Impedance: " << c->get_impedance() << " \u03A9" << std::endl; 
    std::cout << " Magnitude of impedance: " << c->get_impedance_magnitude() << " \u03A9" << std::endl;
    std::cout << " Phase shift: " << c->get_phase_difference() << " rad" << std::endl;
}

// Template function to print a list of components with their impedances and phase shifts
template <typename DataType>
void print_components(DataType components)
{
    std::cout << "Frequency of the circuit: " << component::frequency << " Hz" << std::endl;

    for(auto it = components.begin(); it != components.end(); it++) {
        print_pointer(*it);
    }

    std::cout << std::endl;
}

// Print out a list of all the components within a circuit segment (at user's request)
void print_circuit_segments(std::vector<std::vector<std::shared_ptr<component>>> circuit_segments)
{
    // Declare a char to hold user response: 'y' or 'n'
	char yesno;

    // Ask user if they would like to look at components within each possible path (Potentially useful for longer circuits)?  
    std::cout << std::endl << "Would you like to see the components of each circuit path (y/n)? ";
    std::cin >> yesno;
    while (std::cin.fail() || (yesno != 'y' && yesno != 'n')) {
        std::cout << "Sorry, your input was not valid. Please enter y or n. ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        std::cin >> yesno;
    }


    if (yesno == 'y') {
        for (int i = 0; i < circuit_segments.size(); i++) {
            std::cout << std::endl << "Path: " << i + 1 << std::endl;
            print_components(circuit_segments[i]);

            // Create space in the terminal
            std::cout << "\n\n";

            // visualise the segment
            std::cout << "Printing the segment: " << std::endl;
            for (int j = 0; j < circuit_segments[i].size(); j++) {
		        // Break if at the end of the circuit
                if (j == circuit_segments[i].size() - 1) {
                    std::cout << circuit_segments[i][j]->get_name() << " ——> " << circuit_segments[i][0]->get_name() << "\n\n" << std::endl;
                }
                else {
                    std::cout << circuit_segments[i][j]->get_name() << " ——> " << circuit_segments[i][j+1]->get_name() << "  ";
                }
	        }
        }
    } 
}

// Print the circuit circuit
void print_circuit(circuit const &circuit, int N, std::vector<link> links, std::vector<std::vector<std::shared_ptr<component>>> circuit_segments)
{
    // Find starts and ends of parallel segments
    std::vector<std::shared_ptr<component>> parallel_starter = circuit.get_parallel_starters(circuit_segments, links.size());
    std::vector<std::shared_ptr<component>> parallel_ender = circuit.get_parallel_enders(circuit_segments, links.size());

    // Create space in the terminal
    std::cout << "\n\n\n\n\n";

    // Print out a list of components along with their impedances
    print_components(circuit.get_circuit_components());

    std::cout << "Printing circuit (the final component connects back to the first): " << std::endl;
	for (int i = 0; i < N; i++) {
		// Print each link
        // If the circuit enters a parallel section, split up the circuit
        if (std::find(parallel_starter.begin(), parallel_starter.end(), circuit.get_adjacency_list()[i][0]) != parallel_starter.end()) {
            std::cout << std::endl;
        }
        std::cout << circuit.get_adjacency_list()[i][0]->get_name() << " ——> " << circuit.get_adjacency_list()[i][1]->get_name() << "  ";
	}

    // Create space in the terminal
    std::cout << "\n\n";

    // Find total impedance and phase shift of the circuit
    complex total_impedance = circuit.get_total_impedance(circuit_segments, parallel_starter, parallel_ender);
    std::cout << "Total impedance of circuit: " << total_impedance << " \u03A9" << std::endl;
    std::cout << "Magnitude of total impedance of circuit: " << total_impedance.get_modulus() << " \u03A9" << std::endl;
    std::cout << "Total phase shift of circuit: " << circuit.get_total_phase_shift(total_impedance) << " rad" << std::endl;
}








