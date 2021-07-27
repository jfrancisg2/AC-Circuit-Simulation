// Jonathan Francis Project
// PHYS 30762 Programming in C++
// AC Circuit
// Circuit Function Definitions

#pragma once
#include <vector>
#include "Jonathan_Francis_Project.h"
#include "Jonathan_Francis_Non_Member_Functions.h"

// Define global constants for useful complex numbers
inline const complex one{1,0};
inline const complex nil{0,0};

//                          //
// CIRCUIT MEMBER FUNCTIONS //
//                          //

// Get impedance of of the circuit segments
complex const circuit::get_total_impedance(std::vector<std::vector<std::shared_ptr<component>>> circuit_segments, std::vector<std::shared_ptr<component>> parallel_starter, std::vector<std::shared_ptr<component>> parallel_ender) const
{
    // Define the impedance which we will be calculating
    complex impedance;

    // Define a vector of vector of vector of base pointers to hold different parallel segments
    std::vector<std::vector<std::vector<std::shared_ptr<component>>>> mini_circuits;
    mini_circuits.resize(circuit_segments.size());

    // Get all the parallel segments
    std::vector<std::vector<std::shared_ptr<component>>> parallel_segments = get_parallel_segments(circuit_segments,parallel_starter,parallel_ender);

    // If there are parallel segments, add up the impedance of all components which aren't inside the parallel segments
    if (parallel_segments.size() != 0) {
        impedance = impedance + get_impedance_of_segment(circuit_segments[0],parallel_starter,parallel_ender);
    }

    // Create placeholder impedance to keep track of impedances from different parallel segments
    complex auxiliary_impedance;
    // If there are no parallel segments (e.g a purely serial circuit, or at the bottom level of a parallel circuit) add up all segments of the circuit in parallel 
    // For a series circuit, this is equivalent to 1/(1/Z) = Z, where Z is the impedance of the circuit
    if (parallel_segments.size() == 0) {
        for (int i = 0; i < circuit_segments.size(); i++) {
            if (circuit_segments[i].size() == 0) {
                return impedance;
            }
            auxiliary_impedance = auxiliary_impedance + one/get_impedance_of_segment(circuit_segments[i], parallel_starter, parallel_ender);
        }

        impedance = impedance + one/auxiliary_impedance;

        return impedance;
    }
    // Set placeholder impedance to zero to avoid "impedance leak"
    auxiliary_impedance = nil;

    // This section works out if there are parallel segments in series on the same "level"
    // If non_duplicates = 0, then there are no parallel segments in series
    int parallels_in_series = 0;
    for (int i = 0; i < parallel_segments.size(); i++) {
        for (int j = i + 1; j < parallel_segments.size(); j++) {
            if (parallel_segments[i][0] != parallel_segments[j][0]) {
                parallels_in_series++;
            }
        }
    }
    if (parallels_in_series != 0) {
        parallels_in_series = 1;
    }

    // The order in which segments are added to the "mini circuit" changes if there are parallel segments in series, therefore the switch is required
    switch (parallels_in_series) {
        // This is the case where there are parallel segments in series
        case 1: {
            // A vector of indices to avoid double counting
            std::vector<int> indices;
            // Add parallel segments to different elements of the "mini circuit" container depending on where they are in the circuit
            // Segments which correspond to the same start and end components are added to the same element in the "mini circuit"
            for (int i = 0; i < parallel_segments.size(); i++) {
                for (int j = 0; j < parallel_segments.size(); j++) {
                    if (parallel_segments[i][0] == parallel_segments[j][0] && parallel_segments[i][parallel_segments[i].size()-1] == parallel_segments[j][parallel_segments[j].size()-1] && std::find(indices.begin(), indices.end(), j) == indices.end()) {
                        indices.push_back(j);
                        mini_circuits[i].push_back(parallel_segments[j]);
                    }
                }
            }

            // Erase the first and last components to prevent the function looping indefinitely
            for (int i=0; i < mini_circuits.size(); i++) {
                std::transform(mini_circuits[i].begin(), mini_circuits[i].end(), mini_circuits[i].begin(), [](std::vector<std::shared_ptr<component>> segment) {
                    segment.erase(segment.begin());
                    segment.erase(segment.begin() + segment.size() - 1);
                    return segment;
                });
            }

            // Remove empty vectors
             for (int i = 0; i < mini_circuits.size(); i++) {
                if (mini_circuits[i].size() == 0) {
                    mini_circuits.erase(mini_circuits.begin() + i);
                    i = -1;
                }
            }

            // The function recursively calculates the impedance of each segment and eventually the circuit
            for (int i = 0; i < mini_circuits.size(); i++) {
                auxiliary_impedance = auxiliary_impedance + (get_total_impedance(mini_circuits[i], parallel_starter, parallel_ender));
            }

            impedance = impedance + auxiliary_impedance;

            return impedance;

            break;
        }
        // This is the case where there is only one parallel splitting on this level of the circuit
        default: {
            // This time we have to erase the first and last components of the parallel segments before adding them to the mini circuit
            std::transform(parallel_segments.begin(), parallel_segments.end(), parallel_segments.begin(), [](std::vector<std::shared_ptr<component>> segment) {
                segment.erase(segment.begin());
                segment.erase(segment.begin() + segment.size() - 1);
                return segment;
            });

            // Same procedure as before
            std::vector<int> indices;
            for (int i = 0; i < parallel_segments.size(); i++) {
                for (int j = 0; j < parallel_segments.size(); j++) {
                    if (parallel_segments[i][0] == parallel_segments[j][0] && parallel_segments[i][parallel_segments[i].size()-1] == parallel_segments[j][parallel_segments[j].size()-1] && std::find(indices.begin(), indices.end(), j) == indices.end()) {
                        indices.push_back(j);
                        mini_circuits[i].push_back(parallel_segments[j]);
                    }
                }
            }
    
            // Remove empty vectors
            for (int i = 0; i < mini_circuits.size(); i++) {
                if (mini_circuits[i].size() == 0) {
                    mini_circuits.erase(mini_circuits.begin() + i);
                    i = -1;
                }
            }

            // If one of the parallel segments has no components between the start and end points (i.e no impedance in that segment)
            // return the impedance of the parallel section as just the sum of the start and end components
            for (int i = 0; i < mini_circuits.size(); i++) {
                for (int j = 0; j < mini_circuits[i].size(); j++) {
                    if (mini_circuits[i][j].size() == 0) {
                        return impedance;
                    }
                }
            }

            // The function recursively calculates the impedance of each segment and eventually the circuit
            auxiliary_impedance = nil;
            for (int i = 0; i < mini_circuits.size(); i++) {
                auxiliary_impedance = auxiliary_impedance + one/(get_total_impedance(mini_circuits[i], parallel_starter, parallel_ender));
            }

            impedance = impedance + one/auxiliary_impedance;

            return impedance;

            break;
        }
    }
}


// Get total phase shift
double const circuit::get_total_phase_shift(complex total_impedance) const
{
    return total_impedance.get_argument();
}

// Function to find all paths from a component
std::vector<std::shared_ptr<component>> const circuit::find_paths(std::shared_ptr<component> comp) const
{
    // Create vector to hold possible paths
    std::vector<std::shared_ptr<component>> paths;

    // Find all possible next steps from this point
    for (int i = 0; i < adjacency_list.size(); i++) {
        if (adjacency_list[i][0] == comp) {
            paths.push_back(adjacency_list[i][1]);
        }
    }
    return paths;
}

// Find all possible circuit segments
std::vector<std::vector<std::shared_ptr<component>>> const circuit::get_circuit_segments(std::shared_ptr<component> comp) const
{
    // Make vector of vector of circuit segments
    std::vector<std::vector<std::shared_ptr<component>>> circuit_segments;
    circuit_segments.resize(1);
    circuit_segments[0].push_back(comp);

    // Create vector to hold possible paths
    std::vector<std::vector<std::shared_ptr<component>>> group_of_paths;
    group_of_paths.resize(1);

    // Find all possible next steps from this point
    group_of_paths[0] = find_paths(comp);

    // Build segments
    while (1) {
        // Define circuit segments size (it gets updated in the loop)
        int circuit_segments_size = circuit_segments.size();
        for (int i = 0; i < circuit_segments_size; i++) {
            // Define group_of_paths size (it gets updated in the loop)
            int group_of_paths_size = group_of_paths[i].size();
            // Create a placeholder to retain current paths after overwriting
            std::vector<std::shared_ptr<component>> placeholder = group_of_paths[i];
            for (int j = 0; j < group_of_paths_size; j++) {
                if (j == 0) {
                    // Add next paths to segment and find new paths
                    circuit_segments[i].push_back(group_of_paths[i][j]);
                    group_of_paths[i] = find_paths(group_of_paths[i][j]);
                }

                else {
                    // Resize to add additional segment and copy contents from previous segment
                    circuit_segments.resize(circuit_segments.size()+1);
                    group_of_paths.resize(group_of_paths.size()+1);
                    circuit_segments[circuit_segments.size()-1].insert(std::end(circuit_segments[circuit_segments.size()-1]), std::begin(circuit_segments[i]), std::prev(std::end(circuit_segments[i]), 1));

                    // Add next paths to segment and find new paths
                    circuit_segments[circuit_segments.size()-1].push_back(placeholder[j]);
                    group_of_paths[group_of_paths.size()-1] = find_paths(placeholder[j]);
                }
            }
        }

        // Add condition to stop when no new paths are possible for any segment
        int counter = 0;
        for (int i = 0; i < circuit_segments.size(); i++) {
            if (find_paths(circuit_segments[i].back()).size() != 0) {
                counter++;
            }
        }
        if (counter == 0) {
            return circuit_segments;
        }
    }
}

// Get parallel starters
std::vector<std::shared_ptr<component>> const circuit::get_parallel_starters(std::vector<std::vector<std::shared_ptr<component>>> circuit_segments, int N) const
{
    // Create vector of components which start a parallel section
    std::vector<std::shared_ptr<component>> parallel_starter;

    // Find all components which start a parallel section
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (adjacency_list[i][0] == adjacency_list[j][0]) {
                parallel_starter.push_back(adjacency_list[i][0]);
            }
        }
    }

    return parallel_starter;
}

// Get parallel enders
std::vector<std::shared_ptr<component>> const circuit::get_parallel_enders(std::vector<std::vector<std::shared_ptr<component>>> circuit_segments, int N) const
{
    // Create vector of components which end a parallel section
    std::vector<std::shared_ptr<component>> parallel_ender;

    // Find all components which end a parallel section
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (adjacency_list[i][1] == adjacency_list[j][1]) {
                parallel_ender.push_back(adjacency_list[i][1]);
            }
        }
    }

    return parallel_ender;
}

// Function to return all parallel segments in a circuit
std::vector<std::vector<std::shared_ptr<component>>> const circuit::get_parallel_segments(std::vector<std::vector<std::shared_ptr<component>>> circuit_segments,std::vector<std::shared_ptr<component>> parallel_starter, std::vector<std::shared_ptr<component>> parallel_ender) const
{
    // Create a container to hold all parallel segments and set it initially to hold one element
    std::vector<std::vector<std::shared_ptr<component>>> parallel_segments;
    parallel_segments.resize(1);

    // A counter to determine whether a component starts or finishes a parallel section
    int counter = 0;
    for (int i = 0; i < circuit_segments.size(); i++) {
        for (int j = 0; j < circuit_segments[i].size(); j++) {
            // If a component starts a parallel section, increment the counter
            if (std::find(parallel_starter.begin(), parallel_starter.end(), circuit_segments[i][j]) != parallel_starter.end()) {
                counter++;
            }
            // Add components which form part of a parallel segment to the parallel segments holder
            if (counter != 0) {
                parallel_segments[parallel_segments.size()-1].push_back(circuit_segments[i][j]);
            }
            // If a component ends a parallel section, decrement the counter
            if (std::find(parallel_ender.begin(), parallel_ender.end(), circuit_segments[i][j]) != parallel_ender.end()) {
                counter--;
                // Conditions to handle counters going below 0
                if (counter == 0) {
                    parallel_segments.resize(parallel_segments.size()+1);
                }
                if (counter == -1) {
                    counter = 0;
                }
            }
        }
    }

    // Remove empty vectors
    for (int i = 0; i < parallel_segments.size(); i++) {
        if (parallel_segments[i].size() == 0) {
            parallel_segments.erase(parallel_segments.begin() + i);
            i = -1;
        }
    }

    // Transfer elements from vector to set to eliminate duplicates, then transfer back
    std::set<std::vector<std::shared_ptr<component>>> temp_set;
    for (int i = 0; i < parallel_segments.size(); i++) {
        temp_set.insert(parallel_segments[i]);
    }
    parallel_segments.clear();
    parallel_segments.resize(temp_set.size());
    std::copy(temp_set.begin(), temp_set.end(), parallel_segments.begin());

    return parallel_segments;

}

// Add up impedance of a segment
complex const circuit::get_impedance_of_segment(std::vector<std::shared_ptr<component>> segment, std::vector<std::shared_ptr<component>> parallel_starter, std::vector<std::shared_ptr<component>> parallel_ender) const
{
    // Define the impedance of the segment
    complex segment_impedance;

    // A counter to keep track of whether a component starts or finishes a parallel segment
    int counter = 0;
    for (int i = 0; i < segment.size(); i++) {
        // If the component is the end of a parallel section, decrement the counter
        if (std::find(parallel_ender.begin(), parallel_ender.end(), segment[i]) != parallel_ender.end()) {
            counter--;
        }
        // If the counter = 0, add the impedance of the component to the impedance of the segment
        if (counter == 0) {
            segment_impedance = segment_impedance + segment[i]->get_impedance();
        }
        // If the component is at the start of a parallel section, increment the counter
        if (std::find(parallel_starter.begin(), parallel_starter.end(), segment[i]) != parallel_starter.end()) {
            counter++;
        }

        // The ordering of this function is done such that the parallel starters and enders contribute to the impedance of the segment

    }
    
    return segment_impedance;
}

// Return set of circuit components
std::set<std::shared_ptr<component>> const circuit::get_circuit_components() const
{
    return circuit_components;
}

// Return vector of links/adjacency list
std::vector<std::vector<std::shared_ptr<component>>> const circuit::get_adjacency_list() const
{
    return adjacency_list;
}

