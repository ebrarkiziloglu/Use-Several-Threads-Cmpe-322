#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;
typedef void *(*RunnerFunction) (void *param);

int NUM_OF_INTS = 0;        // number of random integers
int NUM_OF_THREADS = 0;     // number of threads
vector<int> my_array;       // our global array

// Following variables will keep the outputs:
int min_of, max_of, range_of, mode_of, sum_of;
double median_of, art_mean, st_deviation, harm_mean, interquartile_range;

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

void *one_thread(void *param);

void *thread51(void *param);
void *thread52(void *param);
void *thread53(void *param);
void *thread54(void *param);
void *thread55(void *param);

void *thread1(void *param);
void *thread2(void *param);
void *thread3(void *param);
void *thread4(void *param);
void *thread5(void *param);
void *thread6(void *param);
void *thread7(void *param);
void *thread8(void *param);
void *thread9(void *param);
void *thread10(void *param);

RunnerFunction five_functions[5] = {thread51, thread52, thread53, thread54, thread55};
RunnerFunction ten_functions[10] = {thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8, thread9, thread10};

int main(int argc, char *argv[]) {
    NUM_OF_INTS = stoi(argv[1]);
    NUM_OF_THREADS = stoi(argv[2]);
    for(int i = 0; i < NUM_OF_INTS; i ++) {
        int random_int = rand() % 9001 + 1000; // range is 1000-10000
        my_array.push_back(random_int);
    }
    sort(my_array.begin(), my_array.end());     // Sorts in NlogN complexity
    pthread_t workers[NUM_OF_THREADS];

    auto start = high_resolution_clock::now();
    switch(NUM_OF_THREADS) {
        case 1: {
            pthread_create(&workers[0], NULL, &one_thread, NULL);   // create a thread
            pthread_join(workers[0], NULL);    // wait for the thread to exit
            break;
        }
        case 5: {
            for(int i = 0; i < NUM_OF_THREADS; i ++) {
                pthread_create(&workers[i], NULL, five_functions[i], NULL);
            }
            for(int i = 0; i < NUM_OF_THREADS; i ++) {
                pthread_join(workers[i], NULL);
            }
            break;
        }
        case 10: {
            for(int i = 0; i < NUM_OF_THREADS; i ++) {
                pthread_create(&workers[i], NULL, ten_functions[i], NULL);
            }
            for(int i = 0; i < NUM_OF_THREADS; i ++) {
                pthread_join(workers[i], NULL);
            }
            break;
        }
        default: {
            cout << "Invalid number of threads. Please specify the number of threads as 1, 5, or 10.\n";
            break;
        }
    }
    cout << min_of << "\n" << max_of << "\n" <<
            range_of << "\n" << mode_of << "\n" <<
            median_of << "\n" << sum_of << "\n" <<
            art_mean << "\n" << harm_mean << "\n" <<
            st_deviation << "\n" << interquartile_range << "\n";
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count()/(1e6) << "\n";
    return 0;
}

void *one_thread( void *param ) {
    find_min();
    find_max();
    find_range();
    find_mode();
    find_median();
    find_sum();
    find_arithmetic_mean();
    find_harmonic_mean();
    find_st_deviation();
    find_interquartile_range();
    pthread_exit(0);
}

void *thread51(void *param) {   // min and max
    find_min();
    find_max();
    pthread_exit(0);
}

void *thread52(void *param) {   // range and median
    find_range();
    find_median();
    pthread_exit(0);
}

void *thread53(void *param) {   // sum and art_mean
    find_sum();
    find_arithmetic_mean();
    pthread_exit(0);
}

void *thread54(void *param) {   // mode, inter_range
    find_mode();
    find_interquartile_range();
    pthread_exit(0);
}

void *thread55(void *param) {   // harm_mean, st_deviation
    double harm_denominator = 0;
    double st_numerator = 0;
    for(int i = 0; i < NUM_OF_INTS; i ++) {
        harm_denominator += (double)1/(double)my_array.at(i);
        st_numerator += pow(my_array.at(i) - art_mean, 2);
    }
    harm_mean = (double)NUM_OF_INTS / harm_denominator;
    st_deviation = pow(st_numerator / (double)(NUM_OF_INTS-1), 0.5);
    pthread_exit(0);
}

void *thread1(void *param) {
    find_min();
    pthread_exit(0);
}

void *thread2(void *param) {
    find_max();
    pthread_exit(0);
}

void *thread3(void *param) {
    find_range();
    pthread_exit(0);
}

void *thread4(void *param) {
    find_mode();
    pthread_exit(0);
}

void *thread5(void *param) {
    find_median();
    pthread_exit(0);
}

void *thread6(void *param) {
    find_sum();
    pthread_exit(0);
}

void *thread7(void *param) {
    find_arithmetic_mean();
    pthread_exit(0);
}

void *thread8(void *param) {
    find_st_deviation();
    pthread_exit(0);
}

void *thread9(void *param) {
    find_interquartile_range();
    pthread_exit(0);
}

void *thread10(void *param) {
    find_harmonic_mean();
    pthread_exit(0);
}

void find_min() {    // O(1)
    min_of = my_array.at(0);
    return;
}

void find_max() {    // O(1)
    max_of = my_array.at(NUM_OF_INTS - 1);
    return;
}

void find_range() {    // O(1)
    range_of = my_array.at(NUM_OF_INTS - 1) - my_array.at(0);
    return;
}

void find_mode() {      // O(N)
    int max_count = 0;
    int count = 1;
    mode_of = my_array.at(0);
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
    return;
}

void find_median() {     // O(1)
    if(NUM_OF_INTS % 2 == 1) {  // odd
        median_of = my_array.at(NUM_OF_INTS/2);
    }
    else{   // even
        median_of = ( (double)my_array.at(NUM_OF_INTS/2) + (double)my_array.at(NUM_OF_INTS/2 - 1) ) / 2;
    }
    return;
}

void find_sum() {    // O(N)
    sum_of = 0;
    for(int i = 0; i < NUM_OF_INTS; i ++) {
        sum_of += my_array.at(i);
    }
    return;
}

void find_arithmetic_mean() {      // O(1)
    // depends on find_sum()
    art_mean = (double) sum_of / NUM_OF_INTS;
    return;
}

void find_harmonic_mean() {        // O(N)
    double denominator = 0;
    for(int i = 0; i < NUM_OF_INTS; i ++) {
        denominator += (double)1/(double)my_array.at(i);
    }
    harm_mean = (double)NUM_OF_INTS / denominator;
    return;
}

void find_st_deviation() {        // O(N)
    double numerator = 0;
    for(int i = 0; i < NUM_OF_INTS; i ++) {
        numerator += pow(my_array.at(i) - art_mean, 2);
    }
    st_deviation = pow(numerator / (double)(NUM_OF_INTS-1), 0.5);
    return;
}

void find_interquartile_range() {     // O(1)
    int half = NUM_OF_INTS / 2;
    double left_median, right_median;
    if(half % 2 == 1 ){
        left_median = my_array.at(half/2);
        right_median = my_array.at(NUM_OF_INTS - half/2 - 1);
    }
    else {
        right_median = (double)(my_array.at(NUM_OF_INTS - half/2) + my_array.at(NUM_OF_INTS - half/2 - 1))/2;
        left_median =  (double)(my_array.at(half/2) + my_array.at(half/2 - 1))/2;
    }
    interquartile_range = right_median - left_median;
    return;
}
