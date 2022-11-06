#include "tree.h"

const char *const IMGNUMFILE = "imgnum.txt";
const char *const  GRAPHFILE =  "graph.txt";

const char *const NEXTCOLOR = "blue";
const char *const PREVCOLOR = "red";

void Tree_txt_dmup (Tree_t *tree, FILE *stream, const char *func_name, const char *file_name, int line)
{
    if (stream == nullptr) stream = stdout;

    if (func_name == nullptr) func_name = "(NULL)";
    if (file_name == nullptr) file_name = "(NULL)";

    fprintf (stream, "\nTree dump from (%s) at (%s) at line (%d):\n", func_name, file_name, line);

    if (tree == nullptr)
    {
        fprintf (stream, "Unknown tree (nullptr).\n");
        return;
    }

    if (tree -> err & TREE_INFO_CORRUPTED) fprintf (stream, "Unknown tree (info corrupted):\n");
    else fprintf (stream, "tree [%p] \"%s\" at (%s) at (%s)(%d):\n", tree, tree -> info.     name, tree -> info.func_name, 
                                                                           tree -> info.file_name, tree -> info.     line);
    fprintf (stream, "{\n");
    fprintf (stream, "\tstatus   = %d\n", tree -> status);
    fprintf (stream, "\terror    = %d\n", tree -> err);
    fprintf (stream, "\tsize     = %d\n", tree -> size);
    fprintf (stream, "\n\t");
    
    if (tree -> err & TREE_DATA_CORRUPTED) fprintf (stream, "data is corrupted");
    else Tree_print_data (stream, &(tree -> data));

    fprintf (stream, "\n}\n");
}

void Tree_print_data (FILE *stream, TreeElem_t *elem)
{
    fprintf (stream, "(");
    if (elem ->  left) Tree_print_data (stream, elem ->  left);
    TreePrintVal (stream, elem);
    if (elem -> right) Tree_print_data (stream, elem -> right);
    fprintf (stream, ")");
}

void Tree_dump (Tree_t *tree, const char *func_name, const char *file_name, int line)
{
    fprintf (LOG, "<pre>\n");
    Tree_txt_dmup (tree, LOG, func_name, file_name, line);
/*
    if (tree != nullptr && tree -> data != nullptr)
    {
        int imgnum = 0;
        FILE *numfile = fopen (IMGNUMFILE, "r");
        if (numfile != nullptr)
        {
            fscanf (numfile, "%d", &imgnum);
            fclose (numfile);
            numfile = fopen (IMGNUMFILE, "w");
            fprintf (numfile, "%d", imgnum + 1);
            fclose (numfile);
        }
        Tree_generate_img (tree, imgnum);
        fprintf (LOG, "<img src=\"./images/dumpimg%d.png\", width=\"80%%\">", imgnum);
    }
*/
}


void Tree_generate_img (Tree_t *tree, int imgnum)
{/*
    FILE *graph = fopen (GRAPHFILE, "w");
    if (graph == nullptr) return;

    fprintf (graph, "digraph {\n rankdir = TB;\n"
                    "node [shape = record, fontsize = 12, style = \"rounded, filled\", fillcolor = lightblue];\n"
                    "graph [splines = true];\n");
    
    int size = tree -> size;

    Tree_draw_data (graph, &(tree -> data), 0, &size);


    fprintf (graph, "elem0 [color = red, style = \"rounded, filled\", fillcolor = white, label = \"index: 0|");
    if (tree -> data [0].value == POISON_VAL)   fprintf (graph, "value = PSN");
    else                                        fprintf (graph, "value = %d", tree -> data -> value);
    fprintf (graph, "|adress: %p\"];\n", tree -> data);

    int index = 1;
    TreeElem_t *elem = tree -> data -> next;    
    while (elem != tree -> data)
    {
        fprintf (graph, "elem%d -> elem%d [style = invis, weight = 1000];\n", index - 1, index);
        fprintf (graph, "elem%d [label = \"index: %d|", index, index);
        if (tree -> data [index].value == POISON_VAL)   fprintf (graph, "value = PSN");
        else                                            fprintf (graph, "value = %d", elem -> value);
        fprintf (graph, "|adress: %p\"];\n", elem);
        fprintf (graph, "elem%d -> elem%d [color = %s, weight = 1];\n", index - 1, index, NEXTCOLOR);
        fprintf (graph, "elem%d -> elem%d [color = %s, weight = 1];\n", index, index - 1, PREVCOLOR);

        elem = elem -> next;
        index++;
    }

    index -= 1;
    fprintf (graph, "elem%d -> elem%d [color = %s, weight = 1];\n", index, 0, NEXTCOLOR);
    fprintf (graph, "elem%d -> elem%d [color = %s, weight = 1];\n", 0, index, PREVCOLOR);

    fprintf (graph, "}");

    fclose (graph);

    char cmd [64] = "";
    sprintf (cmd, "dot -T png -o ./images/dumpimg%d.png %s", imgnum, GRAPHFILE);
    system (cmd);*/
}
/*
void Tree_draw_data (FILE *graph, TreeElem_t *elem, int rank, int *size)
{
    if (*size >= 0 || elem == nullptr) return;

    fprintf ();
}
*/