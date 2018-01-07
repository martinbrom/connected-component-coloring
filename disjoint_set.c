#include "disjoint_set.h"

/**
 * Recursively finds a root element of a disjoint set,
 * parent is found when roots[item] and item are the same
 * @param roots
 * @param item
 * @return Root of item's disjoint set
 */
unsigned int find_root(unsigned int *roots, unsigned int item) {
    unsigned int parent = roots[item];

    // if parent is the same as item, we found the set root,
    // else try to look recursively up the tree until we find root
    return parent == item ? item : find_root(roots, parent);
}

/**
 * Unites two sets, the set with greater root value
 * is added to the other set (with smaller root value)
 * @param roots
 * @param x
 * @param y
 */
void unite_sets(unsigned int *roots, unsigned int x, unsigned int y) {
    unsigned int root_x = find_root(roots, x);
    unsigned int root_y = find_root(roots, y);

    // set the root with bigger index to the value
    // of the root with smaller index
    if (root_x < root_y) {
        roots[root_y] = roots[root_x];
    } else if (root_y > root_x) {
        roots[root_x] = roots[root_y];
    }

    // nothing is done when root_x == root_y because
    // that means they both are already in the same set
}