 
#include "../include/akinator.h"

int main(void)
{
   //SetConsoleCP(1251);
   //SetConsoleOutputCP(1251);
   //setlocale(LC_ALL, "Rus");
   //setlocale (LC_ALL, "ru_RU.cp1251");

   struct Tree *tree = TreeCtor();
   ERROR_CHECK(tree == NULL, 1);

   int insert_err = treeInsert(tree, &tree->root, "is animal");
   ERROR_CHECK(insert_err, 2);
   TREEDUMP(tree, "kuku");

   insert_err = treeInsert(tree, &tree->root->rght_no, "is human");
   ERROR_CHECK(insert_err, 2);
   TREEDUMP(tree, "kukusprava");

   insert_err = treeInsert(tree, &tree->root->left_yes, "poltorashka");
   ERROR_CHECK(insert_err, 2);
   TREEDUMP(tree, "kukusleva");

   struct TreeNode *curr_node = tree->root->rght_no;
   insert_err = treeInsert(tree, &curr_node->left_yes, "mudak");
   ERROR_CHECK(insert_err, 2);
   TREEDUMP(tree, "kukusverhu");

   insert_err = treeInsert(tree, &curr_node->rght_no, "pidaras");
   ERROR_CHECK(insert_err, 2);
   TREEDUMP(tree, "Pidarasi");

   int save_node_err = treeSave(tree);
   ERROR_CHECK(save_node_err, 4);

   struct Tree *readed_tree = ReadTree(TREE_IO);
   ERROR_CHECK(readed_tree == NULL, 5);
   TREEDUMP(readed_tree, "EBANiy");


   int mode = 0;
   int greet_err = Greeting(&mode);
   ERROR_CHECK(greet_err, 6);

   int guess_err = GuessConcept(tree);
   ERROR_CHECK(guess_err, 7);

   TREEDUMP(tree, "SUDA GORILLU");

   return 0;
}
