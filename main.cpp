#include "tree.h"

FILE *LOG = NULL;

int main()
{
    LOG = fopen (LOGFILENAME, "w");
    if (LOG == nullptr) return 0;
    fprintf (LOG, "<pre>\n");

    Tree_t tree = {};

    TreeCtor (&tree);

    TreeDump (&tree);

    TreeAddLeft  (&tree, &tree.data, "1");
    TreeAddRight (&tree, &tree.data, "2");
    TreeAddRight (&tree, tree.data.left, "3");
    TreeAddLeft (&tree, tree.data.left, "4");
    TreeAddLeft (&tree, tree.data.right, "5");

    TreeDump (&tree);

    TreeDtor (&tree);

    fclose (LOG);
    return 0;
}