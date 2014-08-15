#!/usr/bin/python

LENGTH = 0
INDEX = 1

def num_loc_generated(seed_freq_matrix, seed_len_min, seeds_list):
    locations = 0
    for seed in seeds_list:
        locations += float(seed_freq_matrix[seed[LENGTH] - seed_len_min][seed[INDEX]])
    return locations

def is_valid(read_len, seeds_list):
    valid_loc = [ True for _ in range(read_len) ]
    for seed in seeds_list:
        for i in range(seed[LENGTH]):
            curr_index = i + seed[INDEX]
            if(curr_index >= read_len or not valid_loc[i + seed[INDEX]]):
                return False
            else:
                valid_loc[i + seed[INDEX]] = False
    return True
                 

def find_optimal_seeds(seed_freq_matrix, read_len, seed_len_min, seed_len_max, allowed_errors):
    # initialize the optimal location array
    num_required_seeds = allowed_errors + 1

    optimal_seed_loc = None
    optimal_loc = float("inf")

    curr_seed_loc = [ [ seed_len_min, 0 ] for _ in range(num_required_seeds) ]

    end_seed_loc = [ [seed_len_max, read_len - 1 ] for _ in range(num_required_seeds) ]

    counter = 0.0
    max_count = ((seed_len_max - seed_len_min + 1) * read_len) ** num_required_seeds

    # begin the main loop
    while(curr_seed_loc != end_seed_loc):
        counter += 1
        if(counter % int(max_count / 1000) == 0):
            print round(100 * counter / max_count, 2), "% complete" 

        # check to see if a better location was found
        if(is_valid(read_len, curr_seed_loc) and num_loc_generated(seed_freq_matrix, seed_len_min, curr_seed_loc) < optimal_loc):
            optimal_seed_loc = [curr_seed_loc[i][:] for i in range(len(curr_seed_loc))]
            optimal_loc = num_loc_generated(seed_freq_matrix, seed_len_min, curr_seed_loc)
            print "NEW OPTIMAL SEED", optimal_seed_loc, optimal_loc
        
        # update the seed locations
        for seed in curr_seed_loc:
            if(seed[INDEX] < read_len - 1):
                seed[INDEX] += 1
                break
            else:
                seed[INDEX] = 0
                if(seed[LENGTH] < seed_len_max):
                    seed[LENGTH] += 1
                    break
                else:
                    seed[LENGTH] = seed_len_min

    print optimal_seed_loc, optimal_loc
    return optimal_seed_loc, optimal_loc


