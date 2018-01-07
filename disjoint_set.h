#ifndef ___DISJOINT_SET__
#define ___DISJOINT_SET__

/**
 *
 * @param roots
 * @param item
 * @return
 */
int find_root(int *roots, int item);

/**
 *
 * @param roots
 * @param x
 * @param y
 */
void unite_sets(int *roots, int x, int y);

#endif //___DISJOINT_SET__
