#ifndef JELINEK_H
#define JELINEK_H

struct node {
    unsigned long encstate; // Encoder state
    unsigned long encstate_highbits;     // least significant 50 bits are the data
    int gamma;		         // Cumulative metric to this node
    unsigned int depth;               // depth of this node
    unsigned int jpointer;
};

struct node *stack;

int jelinek(unsigned int *metric,
            unsigned int *cycles,
            unsigned char *data,
            unsigned char *symbols,
            unsigned int nbits,
            unsigned int stacksize,
            struct node *stack,
            int mettab[2][256],
            unsigned int maxcycles);

#endif

