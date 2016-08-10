//
//  c-sim.c
//  CacheSim
//
//  Created by Avijeet Sachdev on 8/9/16.
//  Copyright © 2016 Avijeet Sachdev. All rights reserved.
//

#include "c-sim.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int logBased(int x) {
    unsigned int ans = 0 ;
    while(x>>=1){
        ans++;
    }
    return ans;
}

int cacheSizeValid(int a) { // checking to see if power of two
    while (((a % 2) == 0) && (a > 1)) { // even, and above 1
        a /= 2; // keep looping to check if power of 2
    }
    return (a == 1); // returns 1, if 1
}

int blockSizeValid(int a) { // checking to see if power of two
    while (((a % 2) == 0) && (a > 1)) { // even, and above 1
        a /= 2; // keep looping to check if power of 2
    }
    return (a == 1); // returns 1, if 1
}

int calculateNumSets (int blockSize, int associative, int cacheSize){
    int numSets = cacheSize / (blockSize * associative);
    return numSets;
}

int cacheType (char* exp) {
    if (strcmp(exp, "assoc") == 0){
        return 4;
    }
    else if (strcmp(exp, "direct") == 0) {
        return 1;
    }
    else {
        //stderr
        exit(0);
        return 0;
    }
}

Cache *createCache (int size, int blockSize, int numOfSets, int associative){
    Cache *sim = malloc(sizeof(Cache));
    return sim;
}

int main(int argc, char ** argv){
    int associative = cacheType(argv[2]);
    int numOfSets = calculateNumSets(0, 0, 0);
    
    char* traceFile = argv[4];
    
    FILE *new = fopen(traceFile, "r");
    //while (fgets()) {}
    
    return 0;
}
