#include "tree.h"

const size_t BUFSIZE = 512;


int RunAkinator (Tree_t *tree);

int LoadTree (Tree_t *tree, const char *filename);

int Read_tree (FILE *file, TreeElem_t *elem, int *size);

int SkipSpaces (FILE *file);

int Read_value (FILE *file, TreeElem_t *elem);

int SaveTree (Tree_t *tree, const char *filename);

void Print_tree (FILE *file, TreeElem_t *elem, int rank);