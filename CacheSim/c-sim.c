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
//Char -> Int
int charToInt (char* str) {
    int i = atoi(str);
    return i;
}
//MARK: FIX Int -> Binary
char* intToBinary (int num) {
    int i = 0;
    char response[32];
    while (num > 0) {
        num *= 2;
        if (num >= 1) {
            response[i] = '1';
            num -=1;
            i++;
        }
        else {
            response[i] = '0';
            i++;
        }
    }
    char* respPtr = response;
    return respPtr;
}
// MARK: FIX ERROR CHECKS FOR Binary -> Int
unsigned int binaryToInt(char* bin){
    int i, binaryVal;
    double n;
    unsigned int sum = 0;
    n = 0;
    for(i = strlen(bin) - 1; i>=0; i--){
        binaryVal = 0;
        if(bin[i] == '1')
            binaryVal = 1;
        sum += (binaryVal*((unsigned int)(pow(2.0, n))));
        n++;
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

Cache *createCache (int size, int blockSize, int numOfSets, int associative, int tBits){
    Cache *sim = malloc(sizeof(Cache));
    sim -> hits         = 0;
    sim -> misses       = 0;
    sim -> reads        = 0;
    sim -> writes       = 0;
    sim -> size         = size;
    sim -> numOfSets    = numOfSets;
    sim -> blockSize    = blockSize;
    sim -> associative  = associative;
    
    sim -> arrSets = malloc(sizeof(Set) * numOfSets);
    //go through every set, allocate the lines
    int i, j;
    for (i = 0; i <= numOfSets; i++){
        //allocate lines
        sim -> arrSets[i].arrLines = malloc(sizeof(Line) * associative);
        //loop for allocateing char* tag for every line
        for (j = 0; j <= associative; j++){
            //allocate char* tag to every line
            sim -> arrSets[i].arrLines -> tagBits = malloc(sizeof(char) * tBits);
        }
    }
    return sim;
}

unsigned int getTag(char* bin, int blockBits, int setBits){
    char *p;
    unsigned int tag;
    int i;
    
    p = (char *)malloc(strlen(bin) - blockBits - setBits);
    for(i=0; i < strlen(bin) - blockBits - setBits; i++){
        *(p+i) = bin[i];
    }
    
    *(p+i) = '\0';
    tag = binaryToInt(p);
    return tag;
}


int main(int argc, char ** argv){
    //populating the cache and keeping track of arguments
    int cacheSize       = argv[1];
    int associative     = cacheType(argv[2]); // will return assoc or direct
    int blockSize       = argv[3];
    char* traceFile     = argv[4];
    int numOfSets       = calculateNumOfSets(blockSize, associative, cacheSize); // s = c/(be)
    
    //cache bit calculation
    int bBits = logBased(blockSize);
    int sBits = logBased(numOfSets);
    int tBits = 32 - bBits - sBits;
    
    Cache* L1cache = createCache(cacheSize, blockSize, numOfSets, associative, tBits);
    
    
    FILE *new = fopen(traceFile, "r");
    char *line = (char *)malloc(32);
    char *p;
    while(fgets(line, 32, new) != NULL) {
        p = line;
        while (strlen(p) > 0) {
            while (*p == ' ') { //whitespace
                if (strlen(p) == 1)
                    break; // p can't be that short
                p++; // if it's applicable length, keep traversing
            }
            int readCount, writeCount, cacheHitCount = 0;
            int hits, miss, writes, memRead = 0;
            if (*p == 'R') {
               /*readCount++;
                if (hits) {
                    hits++;
                }
                else if (miss){
                    miss++;
                    memRead++;
                }*/
             }
             if (*p == 'W'){
               /* writeCount++;
                if (hit) {
                    hits++;
                    write++;
                if (miss) {
                    miss++;
                    read++;
                    write++;
                }*/
             }/*
             
             printf("Memory reads: %i", memRead);
             printf("Memory writes: %i",
            */
            
        }
    }
    
    return 0;
}
