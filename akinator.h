#include "tree.h"
#include "stack.h"
#include <unistd.h>

const size_t BUFSIZE = 512;

const char *const TREE_FILE_NAME = "akinator_tree.txt";

const char *const YES = "yes";
const char *const  NO =  "no";

int RunAkinator ();

int RunGuess (Tree_t *tree);

int Question (Tree_t *tree, TreeElem_t *elem);

int Add_new_question (Tree_t *tree, TreeElem_t *elem);

int Get_new_question (char *buf);

char *Get_ans (char *ans);

int RunDefinition (Tree_t *tree);

int Get_definition (TreeElem_t *elem, Stack_t *stk, char *name);

void Print_definition (TreeElem_t *elem, Stack_t *stk, char *name);

int RunDifference (Tree_t *tree);

void Print_difference (TreeElem_t *elem, Stack_t *stk1, Stack_t *stk2, char *name1, char *name2);

int ShowTree (Tree_t *tree);

int LoadTree (Tree_t *tree, const char *filename);

int Read_tree (FILE *file, TreeElem_t *elem, int *size);

int SkipSpaces (FILE *file);

int Read_value (FILE *file, TreeElem_t *elem);

int SaveTree (Tree_t *tree, const char *filename);

void Print_tree (FILE *file, TreeElem_t *elem, int rank);