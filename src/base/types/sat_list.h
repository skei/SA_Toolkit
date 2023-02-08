#ifndef sat_list_included
#define sat_list_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// intrusive list
//
//----------------------------------------------------------------------

class SAT_IListNode {
  friend class SAT_IList;
private:
  SAT_IListNode* MPrevNode = nullptr;
  SAT_IListNode* MNextNode = nullptr;
};

//----------

class SAT_IList {
private:
  SAT_IListNode* MHeadNode = nullptr;
  SAT_IListNode* MTailNode = nullptr;
};

//----------------------------------------------------------------------
//
// templated list
//
//----------------------------------------------------------------------

template <typename T>
class SAT_ListNode {
  //friend class SAT_List<T>
private:
  SAT_ListNode*  MPrevNode  = nullptr;
  SAT_ListNode*  MNextNode  = nullptr;
  T              MData      = nullptr;
};

//----------

template <typename T>
class SAT_List {
private:
  SAT_ListNode<T>* MHeadNode = nullptr;
  SAT_ListNode<T>* MTailNode = nullptr;
};

//----------------------------------------------------------------------
#endif
