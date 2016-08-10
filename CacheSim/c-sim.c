//
//  c-sim.c
//  CacheSim
//
//  Created by Avijeet Sachdev on 8/9/16.
//  Copyright © 2016 Avijeet Sachdev. All rights reserved.
//

#include "c-sim.h"

int cacheSizeValid(int a) {
    while (((a % 2) == 0) && (a > 1)) { // even, and above 1
        a /= 2; // keep looping to check if power of 2
    }
    return (a == 1); // returns 1, if 1
}

int blockSizeValid(int a) {
    while (((a % 2) == 0) && (a > 1)) { // even, and above 1
        a /= 2; // keep looping to check if power of 2
    }
    return (a == 1); // returns 1, if 1
}

int calculateNumSets (int blockSize, int associative, int cacheSize){
    int numSets = cacheSize / (blockSize * associative);
    return numSets;
}

Cache *createCache (int size, int blockSize, int numOfSets, int associative){
    Cache *sim = malloc(sizeof(Cache));
    
}

int main(int argc, char ** argv){
    int associative = cacheType(argv[2]);
    int numOfSets = calculateNumSets(0, 0, 0);
    
    char* trace = argv[4];
    
    FILE *new = fopen(trace, "r");
    while (fgets()) {}
    
    return 0;
}
