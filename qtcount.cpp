#include "qtcount.h"

bool qtcount::prunable(Node* node, const int tol) const {
    return 
       prunableRecurser(node, node->NE, tol) 
    && prunableRecurser(node, node->SW, tol)
    && prunableRecurser(node, node->SE, tol)
    && prunableRecurser(node, node->NW, tol);
}

int qtcount::distance(Node* n1, Node* n2) const {
return  ((n1->avg.g - n2->avg.g)*(n1->avg.g - n2->avg.g)) +
        ((n1->avg.r - n2->avg.r)*(n1->avg.r - n2->avg.r)) +
        ((n1->avg.b - n2->avg.b)*(n1->avg.b - n2->avg.b));
}

bool qtcount::prunableRecurser(Node* node1, Node* node2, const int tol) const {

    // return (prunableRecurser(node1, node2->NE, tol) &&
    //         prunableRecurser(node1, node2->NW, tol) &&
    //         prunableRecurser(node1, node2->SW, tol) &&
    //         prunableRecurser(node1, node2->SE, tol));
    if (node2 == NULL) return false;
    if (distance(node1, node2) <= tol) {
        return true;
    } 
    return (prunableRecurser(node1, node2->NE, tol)) &&
    (prunableRecurser(node1, node2->NW, tol)) &&
    (prunableRecurser(node1, node2->SW, tol)) &&
    (prunableRecurser(node1, node2->SE, tol));
}       
