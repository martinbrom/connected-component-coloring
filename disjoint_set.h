#ifndef ___DISJOINT_SET__
#define ___DISJOINT_SET__

/**
 * Finds a root element of a disjoint set,
 * that the given item is located in
 * @param roots
 * @param item
 * @return A root element of a disjoint set
 */
unsigned int find_root(unsigned int *roots, unsigned int item);

/**
 * Performs a union on two disjoint sets
 * Disjoint set with a greater root element
 * is united under the other disjoint set
 * @param roots
 * @param x
 * @param y
 */
void unite_sets(unsigned int *roots, unsigned int x, unsigned int y);

#endif //___DISJOINT_SET__
