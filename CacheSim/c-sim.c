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

//MARK: # of sets based on cache formula: (cache = b * e * s)
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
    sim -> arrSets = calloc(numOfSets, sizeof(Set)); // malloc w/ 0s
    return sim;
}

int main(int argc, char ** argv){
    //populating the cache and keeping track of arguments
    int cacheSize = 0, associative = 0, blockSize = 0, numOfSets = 0;

    //MARK: Cache size if valid
    if (cacheSizeValid(atoi(argv[1]))) { // if power of two
        cacheSize       = atoi(argv[1]);
    }
    else { // not power of two
        fprintf(stderr,"Invalid cache size.\n");
        exit(0);
        return 0;
    }
    
    //assoc or direct
    associative         = cacheType(argv[2]); // will return assoc or direct
    

    //MARK: Block size if valid
    if (blockSizeValid(atoi(argv[3]))){ // if power of two
        blockSize       = atoi(argv[3]);
    }
    else { // not power of two
        fprintf(stderr,"Invalid block size.\n");
        exit(0);
        return 0;
    }

    //trace file specified
    char* traceFile     = argv[4];

    //# of sets based on input
    numOfSets           = calculateNumOfSets(blockSize, associative, cacheSize); // s = c/(be)

    //cache bit calculation
    int bBits           = logBased(blockSize);
    int sBits           = logBased(numOfSets);
    int tBits           = 32 - bBits - sBits;
    
    Cache* L1cache = createCache(cacheSize, blockSize, numOfSets, associative, tBits);
    
    FILE *new = fopen(traceFile, "r");
    char *line = (char *)malloc(50);
    char *p;
    int i = 0;
    
    int setIndex = 0, tagInt = 0, hitBool = 0;
    long addressCorrect = 0;
    
    Set* currentSet;
    int lineSpec = 0; //specific line
    
    //MAIN LOOP
    while(fgets(line, 50, new) != NULL) {
        if (line[0] == '#'){
            break;
        }
        p = line + 15; // traverse 13 forward
        
        //address count
        addressCorrect = strtol(p, NULL, 16); //str to long (hex -> int)
        addressCorrect /= (pow(2, bBits)); // truncate
        
        //set index
    	int tmp = pow(2,sBits);
    	setIndex = addressCorrect % tmp; // get setIndex
        addressCorrect /= (pow(2, sBits)); // truncate
        
        //tagInt from mem
        tagInt = (int)addressCorrect; // get tagInt
        
        hitBool = 0; // for furthering flow
        
        currentSet = &(L1cache->arrSets[setIndex]); // convenience-sake

        //read from 12th char
        if (line[11] == 'R') {
            for (i = 0; i < associative; i++){
                if (currentSet->arrLines[i].validBits == 1 &&
                    tagInt == currentSet->arrLines[i].tagBits) {
                    (L1cache->hits)++;
                    hitBool = 1;
                    break;
                }
            }
            if(hitBool==0) { // no hit
                L1cache->misses++;
                lineSpec = currentSet->fifo;
                currentSet->arrLines[lineSpec].validBits = 1;
                ((currentSet->arrLines)[lineSpec]).tagBits = tagInt;
                currentSet->fifo =(currentSet->fifo + 1)%associative;
                L1cache->reads++;
            }
         }
        
        //write from 12th char
        else if (line[11] == 'W'){
            L1cache->writes++;
            for (i = 0; i < associative; i++){
                if (currentSet->arrLines[i].validBits == 1 &&
                    tagInt == currentSet->arrLines[i].tagBits) {
                    (L1cache->hits)++;
                    hitBool = 1;
                    break;
                }
            }
            if(hitBool==0){ // no hit
                L1cache->misses++;
                lineSpec = currentSet->fifo;
                currentSet->arrLines[lineSpec].validBits = 1;
                currentSet->arrLines[lineSpec].tagBits = tagInt;
                currentSet->fifo = (currentSet->fifo+1)%associative;
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
