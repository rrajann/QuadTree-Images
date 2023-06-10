
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
#include <cmath>
using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// quadtree destructor, given.
quadtree::~quadtree() {
    clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree& other) {
    copy(other);
}
// quadtree assignment operator, given.
quadtree& quadtree::operator=(const quadtree& rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

quadtree::quadtree(PNG& imIn) {
    stats s(imIn);
    int dim = log2(imIn.height());
    pair<int,int> ul(0,0);
    edge = imIn.height();  
    buildTree(s, ul, dim);
}

quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    if (dim < 0) return nullptr;
    Node *noder = new Node(ul, dim, s.getAvg(ul,dim), s.getVar(ul, dim));
    if (root ==  NULL) root = noder;

    int dimension = pow(2, dim-1);
    pair<int,int> northeast(ul.first + dimension, ul.second);
    pair<int,int> northwest(ul.first, ul.second);
    pair<int,int> southwest(ul.first, ul.second + dimension);
    pair<int,int> southeast(ul.first + dimension, ul.second + dimension);
    noder->NE = buildTree(s, northeast, dim-1);
    noder->NW = buildTree(s, northwest, dim-1);
    noder->SE = buildTree(s, southeast, dim-1);
    noder->SW = buildTree(s, southwest, dim-1);
    
    return noder;
}

PNG quadtree::render() const {
    PNG data;
    data.resize(edge, edge);
    return renderRecursion(data, root);
}

PNG quadtree::renderRecursion(PNG data, Node* node) const {
    PNG img = data;
    if (node == NULL) return img;
    int dimension = pow(2, node->dim);

    if (node->NE == NULL && node->NW == NULL && 
        node->SE == NULL && node->SW == NULL) {
        for (int x = node->upLeft.first; x < node->upLeft.first + dimension; x++) {
            for (int y = node->upLeft.second; y < node->upLeft.second+ dimension; y ++) {
            RGBAPixel* p = img.getPixel(x, y);
            *p = node->avg;
            }
        }
    }
    img = renderRecursion(img, node->NE);
    img = renderRecursion(img, node->SE);
    img = renderRecursion(img, node->SW);
    img = renderRecursion(img, node->NW);
    return img;
}

int quadtree::idealPrune(const int leaves) const {
    return 0;
}

int quadtree::pruneSize(const int tol) const {
    int count = 0;
    return pruneSizeRecursion(tol, count, root);
}


int quadtree::pruneSizeRecursion(const int tol, int count, Node* node) const {
    int newcount = count;
    if (node == NULL) return newcount;
    if (node->NE == NULL && node->NW == NULL && node->SE == NULL && node->SW == NULL) {
        return ++newcount;
    }
    if (prunable(node, tol)) {
        return ++newcount;
    }
    newcount = pruneSizeRecursion(tol, newcount, node->NE);
    newcount = pruneSizeRecursion(tol, newcount, node->NW);
    newcount = pruneSizeRecursion(tol, newcount, node->SE);
    return pruneSizeRecursion(tol, newcount, node->SW);
}

void quadtree::prune(const int tol) {
    pruneTreeRecurse(tol, root);
}

void quadtree::pruneTreeRecurse(const int tol, Node* node)  {
    if (node == NULL) return;
    if (node->NE==NULL) return;
    if (prunable(node, tol)) {
    node->NE = NULL;
    node->NW = NULL;
    node->SE = NULL;
    node->SW = NULL;
    }
    pruneTreeRecurse(tol, node->NE);
    pruneTreeRecurse(tol, node->NW);
    pruneTreeRecurse(tol,  node->SE);
    pruneTreeRecurse(tol,  node->SW);
}

void quadtree::clear() {
    clearRecurse(root);
}

void quadtree::clearRecurse(Node* node) {
    if (node == NULL) return;
    clearRecurse(node->NE);
    clearRecurse(node->SE);
    clearRecurse(node->SW);
    clearRecurse(node->NW);
    if (node->NE == NULL && node->NW == NULL && 
        node->SE == NULL && node->SW == NULL) {
    delete(node);
    node = NULL;
    }
}

void quadtree::copy(const quadtree& orig) {
    // clear();
    // this->edge = orig.edge;
    // this->root = orig.root;
}