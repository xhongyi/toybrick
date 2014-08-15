#!/usr/bin/python

import pdb

LENGTH = 0
INDEX = 1

def num_loc_generated(seed_freq_matrix, seed_len_min, seeds_list):
    locations = 0
    for seed in seeds_list:
        locations += float(seed_freq_matrix[seed[LENGTH] - seed_len_min][seed[INDEX]])
    return locations

def find_optimal_seeds(seed_freq_matrix, read_len, seed_len_min, seed_len_max, allowed_errors):
    # initialize the optimal location array
    num_seeds = allowed_errors + 1


    optimal_seeds_loc = float("inf")
    optimal_seeds_list = []

    for seed_len in range(seed_len_min, seed_len_max + 1):
        for seed_index in range(read_len - seed_len):
            # initialize the list
            curr_seeds_list = [[seed_len, seed_index]]

            # remove the locations from the freq matrix
            reduced_freq_matrix = []
            for i in range(len(seed_freq_matrix)):
                reduced_freq_matrix.append(seed_freq_matrix[i][seed_index + seed_len + 1:])

            tmp_seeds = find_optimal_seeds_helper(reduced_freq_matrix, read_len - (seed_index + seed_len), seed_len_min, seed_len_max, num_seeds - 1)[:]

            for i in range(len(tmp_seeds)):
                tmp_seeds[i][INDEX] += (seed_index + seed_len)

            curr_seeds_list += tmp_seeds

            curr_seeds_loc = num_loc_generated(seed_freq_matrix, seed_len_min, curr_seeds_list)
            if(len(curr_seeds_list) == num_seeds and optimal_seeds_loc  > curr_seeds_loc):
                optimal_seeds_loc = curr_seeds_loc
                optimal_seeds_list = curr_seeds_list[:]
                print optimal_seeds_loc, optimal_seeds_list
                                          
    return optimal_seeds_loc, optimal_seeds_list

def find_optimal_seeds_helper(seed_freq_matrix, read_len, seed_len_min, seed_len_max, num_seeds):
    optimal_seeds_loc = float("inf")
    optimal_seeds_list = []
    
    if(num_seeds == 0 or read_len <=0):
        return []

    for seed_len in range(seed_len_min, seed_len_max + 1):
        for seed_index in range(read_len - seed_len):
            # initialize the list
            curr_seeds_list = [[seed_len, seed_index]]

            # remove the locations from the freq matrix
            reduced_freq_matrix = []
            for i in range(len(seed_freq_matrix)):
                reduced_freq_matrix.append(seed_freq_matrix[i][seed_index + seed_len + 1:])
            
            tmp_seeds = find_optimal_seeds_helper(reduced_freq_matrix, read_len - (seed_index + seed_len), seed_len_min, seed_len_max, num_seeds - 1)[:]

            for i in range(len(tmp_seeds)):
                tmp_seeds[i][INDEX] += (seed_index + seed_len)

            curr_seeds_list += tmp_seeds

            curr_seeds_loc = num_loc_generated(seed_freq_matrix, seed_len_min, curr_seeds_list)
            if(len(curr_seeds_list) == num_seeds and optimal_seeds_loc  > curr_seeds_loc):
                optimal_seeds_loc = curr_seeds_loc
                optimal_seeds_list = curr_seeds_list[:]
                
               
    return optimal_seeds_list

    
