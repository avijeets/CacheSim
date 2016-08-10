//
//  c-sim.h
//  CacheSim
//
//  Created by Avijeet Sachdev on 8/9/16.
//  Copyright © 2016 Avijeet Sachdev. All rights reserved.
//

#ifndef c_sim_h
#define c_sim_h

#include <stdio.h>
typedef struct cache {
    int hits;
    int misses;
    int reads;
    int writes;
    int size;
    int blockSize;
    int numOfSets;
    struct set** arrSets;
    int associative;
} Cache;

typedef struct set {
    struct line** arrSets;
} Set;

typedef struct address {
    char* tagBits;
    char* setBits;
} Address;

typedef struct line{
    int validBits;
    char* tagBits;
} Line;

#endif /* c_sim_h */
