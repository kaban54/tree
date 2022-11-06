#include "tree.h"
#include "akinator.h"

FILE *LOG = NULL;

int main()
{
    LOG = fopen (LOGFILENAME, "w");
    if (LOG == nullptr) return 0;
    fprintf (LOG, "<pre>\n");

    Tree_t tree = {};

    TreeCtor (&tree);

    LoadTree (&tree, "save.txt");

    TreeDump (&tree);

    TreeDtor (&tree);

    fclose (LOG);
    return 0;
}