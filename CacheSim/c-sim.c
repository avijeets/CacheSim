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
#include <math.h>

unsigned int logBased(int x) {
    unsigned int ans = 0 ;
    while(x>>=1){
        ans++;
    }
    return ans;
}

// MARK: Deciding if Cache Size and Block Size input is valid (power of two)
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

//MARK: Hex -> Int, Char -> Int, Int -> Binary, Binary -> Int
int hexToInt (char* hex) { // strtol?
    int i, k, value, coeff, sum, len;
    //initial values
    len = strlen(hex);
    value = 0;
    coeff = 1;
    
    for (i = 0; i < len; i++) {
        if (hex[i] <= '9' && hex[i] >= '0') {
            value = hex[i] - '0'; //ASCII trick to get value
        }
        else if (hex[i] <= 'f' && hex[i] >= 'a') {
            value = hex[i] - 'W'; // ASCII trick to get value
        }
        coeff = 1;
        for (k = (len - 1 - i); k > 0; k--){
            coeff *= 16; // base 16
        }
        sum += (coeff * value);
    }
    return sum;
}

//# of sets based on cache formula: (cache = b * e * s)
int calculateNumOfSets (int blockSize, int associative, int cacheSize){
    int numOfSets = cacheSize / (blockSize * associative);
    return numOfSets;
}

// MARK: Associative or Direct Cache Type
int cacheType (char* exp) {
    if (strcmp(exp, "assoc") == 0){
        return 4;
    }
    else if (strcmp(exp, "direct") == 0) {
        return 1;
    }
    else {
        fprintf(stderr,"Not valid cache type.\n");
        exit(0);
        return 0;
    }
}

Cache* createCache (int size, int blockSize, int numOfSets, int associative, int tBits){
    Cache *sim = malloc(sizeof(Cache));
    sim -> hits         = 0;
    sim -> misses       = 0;
    sim -> reads        = 0;
    sim -> writes       = 0;
    sim -> size         = size;
    sim -> numOfSets    = numOfSets;
    sim -> blockSize    = blockSize;
    sim -> associative  = associative;
    sim -> arrSets = calloc(sizeof(Set) * numOfSets, 0);
    return sim;
}

int main(int argc, char ** argv){
    //populating the cache and keeping track of arguments
    int cacheSize       = atoi(argv[1]);
    int associative     = cacheType(argv[2]); // will return assoc or direct
    int blockSize       = atoi(argv[3]);
    char* traceFile     = argv[4];
    int numOfSets       = calculateNumOfSets(blockSize, associative, cacheSize); // s = c/(be)
    
    //cache bit calculation
    int bBits           = logBased(blockSize);
    int sBits           = logBased(numOfSets);
    int tBits           = 32 - bBits - sBits;
    
    Cache* L1cache = createCache(cacheSize, blockSize, numOfSets, associative, tBits);
    
    FILE *new = fopen(traceFile, "r");
    char *line = (char *)malloc(50);
    char *p;
    //int hits = 0, miss = 0, writes = 0, memRead = 0;
    int i = 0;
    
    int setIndex = 0, tagInt = 0, addressCorrect = 0;
    //printf("addr: '%s'\n", addr);
    
    Set* currentSet;
    int lineSpec; //specific line
    
    //MAIN LOOP
    while(fgets(line, 50, new) != NULL) {
        if (line[0] == '#'){
            break;
        }
        p = line + 13;
        //address count
        addressCorrect = strtol(p, NULL, 16);
        //printf("P AND ADDR: %s, %i\n", p, addressCorrect);
        addressCorrect /= (pow(2, bBits));
        
        //set index
        setIndex = addressCorrect % (int)((pow(2, sBits)));
        addressCorrect /= (pow(2, sBits));
        
        //tagInt from mem
        tagInt = addressCorrect % (int)((pow(2, tBits)));
        
        int hitBool = 0;
        
        //printf("setIndex: %i\n", setIndex);
        //printf("address: %i\n", addressCorrect);
        
        currentSet = &L1cache->arrSets[setIndex];
        //read from 12th char
        if (line[11] == 'R') {
            for (i = 0; i < associative; i++){
                if (currentSet->arrLines[i].validBits == 1 &&
                    tagInt == currentSet->arrLines[i].tagBits) {
                    //printf("READ SET BITS: %i, TAG BITS: %i, BLOCK BITS: %i\n", sBits, tBits, bBits);
                    //printf("currentSet->arrLines[i].validBits: %i\n", currentSet->arrLines[i].validBits);
                    //printf("LOOK AT THIS FROM READ: %i, %i\n", addressCorrect, currentSet->arrLines[i].tagBits);
                    L1cache->hits++;
                    hitBool = 1;
                    break;
                }
            }
            if(!hitBool) {
                L1cache->misses++;
                lineSpec = currentSet->fifo % associative;
                currentSet->arrLines[lineSpec].validBits = 1;
                currentSet->arrLines[lineSpec].tagBits = tagInt;
                currentSet->fifo++;
                L1cache->reads++;
            }
         }
        
        //write from 12th char
        if (line[11] == 'W'){
            L1cache->writes++;
            for (i = 0; i < associative; i++){
                if (currentSet->arrLines[i].validBits == 1 &&
                    tagInt == currentSet->arrLines[i].tagBits) {
                    //printf("WRITE SET BITS: %i, TAG BITS: %i, BLOCK BITS: %i\n", sBits, tBits, bBits);
                    //printf("LOOK AT THIS FROM WRITE: %i, % i\n", tagInt, currentSet->arrLines[i].tagBits);
                    L1cache->hits++;
                    hitBool = 1;
                    break;
                }
            }
            if(!hitBool){
                L1cache->misses++;
                lineSpec = currentSet->fifo % associative;
                currentSet->arrLines[lineSpec].validBits = 1;
                currentSet->arrLines[lineSpec].tagBits = tagInt;
                currentSet->fifo++;
                L1cache->reads++;
            }
         }
    }
    printf("Memory reads: %i\n", L1cache->reads);
    printf("Memory writes: %i\n", L1cache->writes);
    printf("Cache hits: %i\n", L1cache->hits);
    printf("Cache misses: %i\n", L1cache->misses);
    
    return 0;
}
