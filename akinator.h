#define TX_USE_SPEAK
#include <TXLib.h>
#include "tree.h"
#include "stack.h"
#include <unistd.h>

const size_t BUFSIZE = 256;

const char *const TREE_FILE_NAME = "akinator_tree.txt";

const char *const     YES_STR = "yes";
const char *const     NO_STR  = "no";
const char *const UNS_YES_STR = "rather yes";
const char *const UNS_NO_STR  = "rather no";

enum AKINATOR_ANSWERS
{
        YES =  2,
        NO  = -2,
    UNS_YES =  1,
    UNS_NO  = -1,
};


int RunAkinator ();

int RunGuess (Tree_t *tree);

int Question (Tree_t *tree);

TreeElem_t *Get_last_unsure (Tree_t *tree, Stack_t *ans_stk);

int Add_new_question (Tree_t *tree, TreeElem_t *elem);

int Get_new_question (char *buf);

char *Get_ans (char *ans);

int Get_YN_ans ();

int AnsYes (int ans);

int AnsNo (int ans);

int AnsUnsure (int ans);

int RunDefinition (Tree_t *tree);

int Get_definition (TreeElem_t *elem, Stack_t *stk, char *name);

void Print_definition (TreeElem_t *elem, Stack_t *stk, int *index, char *msg);

int RunDifference (Tree_t *tree);

void Print_difference (TreeElem_t *elem, Stack_t *stk1, Stack_t *stk2, char *name1, char *name2);

int ShowTree (Tree_t *tree);

int LoadTree (Tree_t *tree, const char *filename);

int Read_tree (FILE *file, TreeElem_t *elem, int *size);

int SkipSpaces (FILE *file);

int Read_value (FILE *file, TreeElem_t *elem);

int Read_poison (FILE *file);

int SaveTree (Tree_t *tree, const char *filename);

void Print_tree (FILE *file, TreeElem_t *elem, int rank);