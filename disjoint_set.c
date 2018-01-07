#include "disjoint_set.h"

/**
 *
 * @param roots
 * @param item
 * @return
 */
int find_root(int *roots, int item) {
    int parent = roots[item];

    // if parent is the same as item, we found the set root,
    // else try to look recursively up the tree until we find root
    return parent == item ? item : find_root(roots, parent);
}

/**
 *
 * @param roots
 * @param x
 * @param y
 */
void unite_sets(int *roots, int x, int y) {
    int root_x = find_root(roots, x);
    int root_y = find_root(roots, y);

    // both colors already belong to the same set
    if (root_x == root_y) {
        return;
    }

    // set the root with bigger index to the value
    // of the root with smaller index
    if (root_x < root_y) {
        roots[root_y] = roots[root_x];
    } else {
        roots[root_x] = roots[root_y];
    }
}