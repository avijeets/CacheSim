//
//  c-sim.h
//  CacheSim
//
//  Created by Avijeet Sachdev on 8/9/16.
//  Copyright © 2016 Avijeet Sachdev.
//  All rights reserved.
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
    struct set* arrSets;
    int associative;
} Cache;

typedef struct line{
    int validBits;
    int tagBits;
} Line;

typedef struct set {
    Line arrLines[4];
    int fifo;
} Set;

typedef struct address {
    char* tagBits;
    char* setBits;
} Address;

#endif /* c_sim_h */
