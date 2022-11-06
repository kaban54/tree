#include "tree.h"
#include "akinator.h"

FILE *LOG = NULL;

int main()
{
    LOG = fopen (LOGFILENAME, "w");
    if (LOG == nullptr) return 0;
    fprintf (LOG, "<pre>\n");

    RunAkinator ();

    fclose (LOG);
    return 0;
}