#ifndef sat_tree_included
#define sat_tree_included
//----------------------------------------------------------------------

class SAT_Tree;
class SAT_TreeNode;
typedef SAT_Array<SAT_TreeNode*> SAT_TreeNodes;

//----------------------------------------------------------------------

class SAT_TreeNode {
private:
//SAT_Tree*     MOwner;
  SAT_TreeNode* MParent;
  SAT_TreeNodes MChildren;
};

//----------------------------------------------------------------------

class SAT_Tree {
private:
  SAT_TreeNode MRoot;
};

//----------------------------------------------------------------------
#endif
