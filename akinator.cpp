#include "akinator.h"

int RunAkinator (Tree_t *tree)
{

    return 0;
}


int LoadTree (Tree_t *tree, const char *filename)
{
    TreeVerify (tree);
    if (filename == nullptr) TreeErr (tree, TREE_NULLPTR_ARG);

    if (tree -> data.left || tree -> data.right) TreeErr (tree, TREE_INCORRECT_POSITION);

    FILE *file = fopen (filename, "r");
    if (file == nullptr) TreeErr (tree, TREE_NULLPTR_ARG);

    tree -> size = 0;
    Read_tree (file, &(tree -> data), &(tree -> size));

    TreeVerify (tree);
    return TREE_OK;
}

int Read_tree (FILE *file, TreeElem_t *elem, int *size)
{
    int ch = SkipSpaces (file);
    if (ch != '{') return TREE_INCORRECT_FORMAT;

    int err = Read_value (file, elem);
    if (err) return err;

    *size += 1;

    ch = SkipSpaces (file);

    if (ch == '{')
    {
        ungetc ('{', file);

        elem ->  left = TreeAllocElem ();
        elem -> right = TreeAllocElem ();
        if (elem ->  left == nullptr) return TREE_ALLOC_ERROR;
        if (elem -> right == nullptr) return TREE_ALLOC_ERROR;
        
        err = Read_tree (file,  elem -> left, size);
        if (err) return err;

        err = Read_tree (file, elem -> right, size);
        if (err) return err;

        ch = SkipSpaces (file);
    }

    if (ch != '}') return TREE_INCORRECT_FORMAT;

    return TREE_OK;
}

int SkipSpaces (FILE *file)
{
    if (file == nullptr) return EOF;

    int ch = fgetc (file);

    while (isspace (ch)) ch = fgetc (file);

    return ch;
}

int Read_value (FILE *file, TreeElem_t *elem)
{
    int ch = SkipSpaces (file);
    if (ch == 'P')
    {
        if (fgetc (file) != 'S') return TREE_INCORRECT_FORMAT;
        if (fgetc (file) != 'N') return TREE_INCORRECT_FORMAT;

        elem -> value = POISON_VAL;
    }
    else if (ch == '"')
    {
        char buf [BUFSIZE];
        size_t len = 0;

        ch = fgetc (file);
        while (ch != '"')
        {
            if (ch == EOF || len >= BUFSIZE - 1) return TREE_INCORRECT_FORMAT;
            buf [len++] = (char) ch;
            ch = fgetc (file);
        }
        buf [len++] = '\0';

        elem -> value = (char *) calloc (len, 1);
        if (elem -> value == nullptr) return TREE_ALLOC_ERROR;

        strcpy (elem -> value, buf);
    }
    else return TREE_INCORRECT_FORMAT;

    return TREE_OK;
}


int SaveTree (Tree_t *tree, const char *filename)
{
    TreeVerify (tree);
    if (filename == nullptr) TreeErr (tree, TREE_NULLPTR_ARG);

    FILE *file = fopen (filename, "w");
    if (file == nullptr) TreeErr (tree, TREE_NULLPTR_ARG);

    Print_tree (file, &(tree -> data), 0);

    fclose (file);
    return TREE_OK;
}

void Print_tree (FILE *file, TreeElem_t *elem, int rank)
{
    fputc ('\n', file);
    for (int n = 0; n < rank; n++) fputc ('\t', file);

    fputc ('{', file);
    TreePrintVal (file, elem);

    if (elem ->  left) Print_tree (file, elem ->  left, rank + 1);
    if (elem -> right) Print_tree (file, elem -> right, rank + 1);

    if (elem -> left || elem -> right)
    {
        fputc ('\n', file);
        for (int n = 0; n < rank; n++) fputc ('\t', file);
    }
    fputc ('}', file);
}
