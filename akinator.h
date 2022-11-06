#include "tree.h"

const size_t BUFSIZE = 512;

const char *const TREE_FILE_NAME = "akinator_tree.txt";

const char *const YES = "yes";
const char *const  NO =  "no";

int RunAkinator ();

int RunGuess (Tree_t *tree);

int Question (Tree_t *tree, TreeElem_t *elem);

int Add_new_question (Tree_t *tree, TreeElem_t *elem);

int Get_new_question (char *buf);

int RunDefinition (Tree_t *tree);

int RunDifference (Tree_t *tree);

int ShowTree (Tree_t *tree);

int LoadTree (Tree_t *tree, const char *filename);

int Read_tree (FILE *file, TreeElem_t *elem, int *size);

int SkipSpaces (FILE *file);

int Read_value (FILE *file, TreeElem_t *elem);

int SaveTree (Tree_t *tree, const char *filename);

void Print_tree (FILE *file, TreeElem_t *elem, int rank);