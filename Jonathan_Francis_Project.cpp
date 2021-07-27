// Jonathan Francis Project
// PHYS 30762 Programming in C++
// AC Circuit Main Programme

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <map>
#include "Jonathan_Francis_Project.h"
#include "Jonathan_Francis_Non_Member_Functions.h"
#include "Jonathan_Francis_Component_Functions.h"
#include "Jonathan_Francis_Complex_Functions.h"
#include "Jonathan_Francis_Circuit_Functions.h"

int main()
{
	// Boolean to determine whether user wants to restart the circuit program
	char restart = 'y';
	while (restart == 'y') {
		
		// Get lead inductance and parasitic capacitance from the wire contributions
		auto [lead_inductance, parasitic_capacitance] = get_wire_contributions();

		// Create a map to hold components
		std::map<std::string, std::shared_ptr<component>> components = select_components(parasitic_capacitance, lead_inductance);

		// Print out keys and impedances of all components
		std::cout << "List of components: Note! Some components may have infinite or zero impedance before a frequency is specified! " << std::endl;
		for(auto comp: components) {
			std::cout << comp.first << " Impedance: " << comp.second->get_impedance() << " \u03A9, " << "Magnitude: " << comp.second->get_impedance_magnitude() << " \u03A9" << std::endl;
		}

		std::cout << std::endl << "Thanks! Now you can build the circuit." << std::endl;

		// Loop for making new circuits
		char make_new_circuit = 'y';
		while (make_new_circuit == 'y') {

			// Add components to the circuit in pairs
			std::cout << std::endl << "1) Please build your circuit in pairs of components: for example, to connect 3 components in series, input 'component1 component2', 'component2 component3', using the names you selected. " << std::endl;
			std::cout << "2) Build these connections as you would build a circuit by hand. If, in real life, you wanted to connect component1 to component2 with a wire, you would input here: 'component1 component2'. " << std::endl;
			std::cout << "3) Make sure to connect your components sequentially! Please! " << std::endl;
			std::cout << "4) It might help to draw the circuit by hand first (excluding a voltage source)." << std::endl;
			std::cout << "5) Also, you do not need to complete the circuit by adding a link back to the first component - that will be done for you." << std::endl;
			std::cout << "6) Make sure to start with the first component i.e. the component directly after the cell in a real-life circuit, or it won't work!" << std::endl;
			std::cout << "7) Finally, make sure not to repeat any connections! Please!!!" << std::endl;

			// Boolean to indicate if user wants to add links 
			char add_link = 'y';

			// A vector of links (see project header file) to create circuit with
			std::vector<link> links; 

			// Loop allows the user to add links to the circuit
			while(add_link == 'y') {
	
				// Add links to the vector of links
				links = add_links(add_link, links, components);

				// Loop to allow the user to change the frequency and update the circuit
				char change_frequency = 'y';
				while (change_frequency == 'y') {

					// Set circuit frequency
					components = set_frequency(components);

					// Construct the circuit
					circuit circuit(links, links.size());

					// Print out the circuit (With impedance and phase shift for each component) and components list
					print_circuit(circuit, links.size(), links, circuit.get_circuit_segments(links[0].start));

					// Print out list of components in each circuit segment (at user's request)
					print_circuit_segments(circuit.get_circuit_segments(links[0].start));

					// Ask user if they would like to change frequency
					std::cout << std::endl << "Would you like to change the frequency of the circuit (y/n)? ";
					std::cin >> change_frequency;
					// Check for fail, clear, and replace
					while (std::cin.fail() || (change_frequency != 'y' && change_frequency != 'n')) {
						std::cout << "Sorry, your input was not valid. Please enter y or n. ";
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
						std::cin >> change_frequency;
					}
				}

				// Ask user if they want to add another link
				std::cout << std::endl << "Would you like to add another pair to the circuit (y/n)? Just so you know, you are limited by the number of components in your components list! If you want to add more components, please enter 'n': ";
				std::cin >> add_link;
				// Check for fail, clear, and replace
				while (std::cin.fail() || (add_link != 'y' && add_link != 'n')) {
					std::cout << "Sorry, your input was not valid. Please enter y or n. ";
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
					std::cin >> add_link;
				}
			}

			// Ask user if they want to make a new circuit (using same components list)
			std::cout << std::endl << "Would you like to make a new circuit (y/n)? Just so you know, you are limited by the number of components in your components list! " << std::endl;
			std::cout << "If you want to add more components, please enter 'n', then restart the programme: ";
			std::cin >> make_new_circuit;
			// Check for fail, clear, and replace
			while (std::cin.fail() || (make_new_circuit != 'y' && make_new_circuit != 'n')) {
				std::cout << "Sorry, your input was not valid. Please enter y or n. ";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
				std::cin >> make_new_circuit;
			}
		}

		// Ask user if they want to restart the program
		std::cout << std::endl << "Would you like to restart the program (y/n)? ";
		std::cin >> restart;
		// Check for fail, clear, and replace
		while (std::cin.fail() || (restart != 'y' && restart != 'n')) {
			std::cout << "Sorry, your input was not valid. Please enter y or n. ";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
			std::cin >> restart;
		}
	}

	return 0;
}

