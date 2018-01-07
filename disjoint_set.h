#ifndef ___DISJOINT_SET__
#define ___DISJOINT_SET__

/**
 * Finds a root element of a disjoint set,
 * that the given item is located in
 * @param roots
 * @param item
 * @return A root element of a disjoint set
 */
int find_root(int *roots, int item);

/**
 * Performs a union on two disjoint sets
 * Disjoint set with a greater root element
 * is united under the other disjoint set
 * @param roots
 * @param x
 * @param y
 */
void unite_sets(int *roots, int x, int y);

#endif //___DISJOINT_SET__
