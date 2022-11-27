#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace std::chrono;

int NUM_OF_INTS = 0;        // number of random integers
int NUM_OF_THREADS = 0;     // number of threads
double duration_of_exec;            // duration of the execution
vector<int> my_array;       // our global array

struct two_functions{
    void (*function_1)();
    void (*function_2)();
};

struct five_functions{
    void (*function_1)();
    void (*function_2)();
    void (*function_3)();
    void (*function_4)();
    void (*function_5)();
};

// Following variables will keep the outputs:
// 0- (int)min, 1- (int)max, 2- (int)range, 3- (int)mode, 4- median,
// 5- (int)sum, 6- art_mean, 7- harm_mean, 8- st_deviation, 9 - interquartile_range
double outputs[10];
void print_outputs(ofstream& print_to_file, double duration);
void find_min();
void find_max();
void find_range();
void find_mode();
void find_median();
void find_sum();
void find_arithmetic_mean();
void find_st_deviation();
void find_interquartile_range();
void find_harmonic_mean();
void calculate_mode_and_harm_mean();
void empty_func();

void (*functionPointers[12])() = {find_min,                     // 0
                                find_max,                       // 1
                                find_range,                     // 2
                                find_median,                    // 3
                                find_sum,                       // 4
                                find_harmonic_mean,             // 5
                                find_arithmetic_mean,           // 6
                                find_st_deviation,              // 7
                                find_mode,                      // 8
                                find_interquartile_range,       // 9
                                calculate_mode_and_harm_mean,   // 10
                                empty_func};                    // 11

void *executor_two(void *param) {
    two_functions strct = *((two_functions *)param);
    strct.function_1();
    strct.function_2();
    pthread_exit(0);
}

void *executor_five(void *param) {
    five_functions strct = *((five_functions *)param);
    strct.function_1();
    strct.function_2();
    strct.function_3();
    strct.function_4();
    strct.function_5();
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    NUM_OF_INTS = stoi(argv[1]);
    NUM_OF_THREADS = stoi(argv[2]);  
    if (NUM_OF_THREADS > 10 or NUM_OF_THREADS < 1) {
        cout << "Invalid number of threads. Please specify the number of threads as 1, 5, or 10.\n";
        return 0;
    }

    for(int i = 0; i < NUM_OF_INTS; i ++) {
        int random_int = rand() % 9001 + 1000; // range is 1000-10000
        my_array.push_back(random_int);
    }
    sort(my_array.begin(), my_array.end());     // Sorts in NlogN complexity
    pthread_t workers[NUM_OF_THREADS];
    two_functions struct_arr[NUM_OF_THREADS];
    int indexes[2 * NUM_OF_THREADS];
    for (int i = 1; i < 2*NUM_OF_THREADS ; i+=2){
        indexes[i] = 11;
    }
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
    if(NUM_OF_THREADS == 1) {
        five_functions five_function_struct;
        five_function_struct.function_1 = functionPointers[2];             // range: min & max
        five_function_struct.function_2 = functionPointers[3];             // median
        five_function_struct.function_3 = functionPointers[7];             // st_dev : sum & art_mean
        five_function_struct.function_4 = functionPointers[9];             // interquartile_range
        five_function_struct.function_5 = functionPointers[10];            // harm_mean, mode
        pthread_create(&workers[0], NULL, executor_five, &five_function_struct);
    }
    else if(NUM_OF_THREADS == 2) {
        five_functions five_function_struct;
        two_functions two_function_struct;

        five_function_struct.function_1 = functionPointers[2];             // range: min & max
        five_function_struct.function_2 = functionPointers[7];             // st_dev : sum & art_mean
        five_function_struct.function_3 = functionPointers[9];             // interquartile_range
        five_function_struct.function_4 = functionPointers[11];            // empty function
        five_function_struct.function_5 = functionPointers[11];            // empty function

        two_function_struct.function_1 = functionPointers[10];             // harm_mean, mode
        two_function_struct.function_2 = functionPointers[3];              // median

        pthread_create(&workers[0], NULL, executor_five, &five_function_struct);
        pthread_create(&workers[1], NULL, executor_two, &two_function_struct);

    }
    else { 
        indexes[0] = 2;
        indexes[2] = 7;
        indexes[4] = 9;
        switch (NUM_OF_THREADS)
        {
        case 3: {
            indexes[0] = 2;
            indexes[1] = 7;
            indexes[2] = 9;
            indexes[3] = 10;
            indexes[4] = 3;
            indexes[5] = 11;
            break;
            }
        case 4: {
            indexes[1] = 3;
            indexes[6] = 10;
            break;
        }    
        case 5: {
            for(int i = 0; i < NUM_OF_THREADS; i++) {
                indexes[2*i] = 2*i;
                indexes[2*i+1] = 2*i+1;
            }
            break;
        }
        case 10: {
            indexes[18] = 6;
        } 
        case 9: {
            indexes[16] = 4;
        }  
        case 8: {
            indexes[14] = 0;
        }   
        case 7: {
            indexes[12] = 1;
        }   
        case 6: {
            indexes[6] = 3;
            indexes[8] = 5;
            indexes[10] = 8;
            break;
        }   
        }
        for(int i = 0; i < NUM_OF_THREADS; i ++) { 
            struct_arr[i].function_1 = functionPointers[indexes[2*i]];
            struct_arr[i].function_2 = functionPointers[indexes[2*i+1]];            // empty function
            pthread_create(&workers[i], NULL, executor_two, &struct_arr[i]);
        }
    }
    for(int i = 0; i < NUM_OF_THREADS; i ++) {
        pthread_join(workers[i], NULL);
    }
    ofstream outfile ("output.txt");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
    duration_of_exec = ( 1000.0*end_time.tv_sec + 1e-6*end_time.tv_nsec
                - (1000.0*start_time.tv_sec + 1e-6*start_time.tv_nsec) ) / 1000;
    print_outputs(outfile, duration_of_exec);
    return 0;
}

void print_outputs(ofstream& print_to_file, double duration) {
    print_to_file << setprecision(5) << fixed;
    for(int i = 0; i < 10; i++) {
    double result = outputs[i];
        if(result - (int) result < 0.00001) {
            print_to_file << (int)result << "\n";
        }
        else{
            print_to_file << result << "\n";
        }
    }
    print_to_file << duration << "\n";
    print_to_file.close();
}

void find_min() {    // O(1)
    outputs[0] = my_array.at(0);
    return;
}

void find_max() {    // O(1)
    outputs[1] = my_array.at(NUM_OF_INTS - 1);
    return;
}

void find_range() {    // O(1)
    find_min();
    find_max();
    outputs[2] = outputs[1] - outputs[0];
    return;
}

void find_mode() {      // O(N)
    int max_count = 0;
    int count = 1;
    int mode_of = my_array.at(0);
    for(int i = 0; i < NUM_OF_INTS - 1; i ++) {
        if(my_array.at(i) == my_array.at(i+1)) {
            count++;
            if(count > max_count) {
                max_count = count;
                mode_of = my_array.at(i);
            }
        }
        else {
            count = 1;
        }
    }
    outputs[3] = mode_of;
    return;
}

void find_median() {     // O(1)
    if(NUM_OF_INTS % 2 == 1) {  // odd
        outputs[4] = my_array.at(NUM_OF_INTS/2);
    }
    else{   // even
        outputs[4] = ( (double)my_array.at(NUM_OF_INTS/2) + (double)my_array.at(NUM_OF_INTS/2 - 1) ) / 2;
    }
    return;
}

void find_sum() {    // O(N)
    int sum = 0;
    for(int i = 0; i < NUM_OF_INTS; i ++) {
        sum += my_array.at(i);
    }
    outputs[5] = sum;
    return;
}

void find_arithmetic_mean() {      // O(1)
    find_sum();     // arithmetic mean calculation depends on the sum
    outputs[6] = (double) outputs[5] / NUM_OF_INTS;
    return;
}

void find_harmonic_mean() {        // O(N)
    double denominator = 0;
    for(int i = 0; i < NUM_OF_INTS; i ++) {
        denominator += (double)1/(double)my_array.at(i);
    }
    outputs[7] = (double)NUM_OF_INTS / denominator;
    return;
}

void find_st_deviation() {        // O(N)
    double numerator = 0;
    find_arithmetic_mean();     // standard deviation calculation depends on the arithmetic mean
    double art_mean = outputs[6];
    for(int i = 0; i < NUM_OF_INTS; i ++) {
        numerator += pow(my_array.at(i) - art_mean, 2);
    }
    outputs[8] = pow(numerator / (double)(NUM_OF_INTS-1), 0.5);
    return;
}

void find_interquartile_range() {     // O(1)
    int half = NUM_OF_INTS / 2;
    int first_quarter = half/2;
    int third_quarter = NUM_OF_INTS - first_quarter;
    double left_median, right_median;
    if(half % 2 == 1 ){
        left_median = my_array.at(first_quarter);
        right_median = my_array.at(third_quarter - 1);
    }
    else {
        right_median = (double)(my_array.at(third_quarter) + my_array.at(third_quarter - 1))/2;
        left_median =  (double)(my_array.at(first_quarter) + my_array.at(first_quarter - 1))/2;
    }
    outputs[9] = right_median - left_median;
    return;
}

void calculate_mode_and_harm_mean() {   // harm_mean, mode
    double harm_denominator = 0;
    int max_count = 0;
    int count = 1;
    int mode_of = my_array.at(0);
    int i;
    for(i = 0; i < NUM_OF_INTS - 1; i ++) {
        if(my_array.at(i) == my_array.at(i+1)) {
            count++;
            if(count > max_count) {
                max_count = count;
                mode_of = my_array.at(i);
            }
        }
        else {
            count = 1;
        }

        harm_denominator += (double)1/(double)my_array.at(i);
    }
    outputs[3] = mode_of;
    harm_denominator += (double)1/(double)my_array.at(i);
    outputs[7] = (double)NUM_OF_INTS / harm_denominator;
    return;
}

void empty_func() {return;}
