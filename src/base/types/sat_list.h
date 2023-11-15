#ifndef sat_list_included
#define sat_list_included
//----------------------------------------------------------------------

/*

doubly-linked 'intrusive' list..

  a templates (ala array)data-container)
  b multiple inheritance (ala widgetListener)
  * non-invasive list.. nodes = next, prev, ptr

*/

//----------------------------------------------------------------------

class SAT_ListNode {
private:
  SAT_ListNode*  MPrev = nullptr;
  SAT_ListNode*  MNext = nullptr;
public:
  SAT_ListNode() {}
  virtual ~SAT_ListNode() {}
public:
  SAT_ListNode*  prev() { return MPrev; }
  SAT_ListNode*  next() { return MNext; }
  void prev(SAT_ListNode* ANode) { MPrev = ANode; }
  void next(SAT_ListNode* ANode) { MNext = ANode; }
};

//----------------------------------------------------------------------

class SAT_List {

//------------------------------
private:
//------------------------------

  SAT_ListNode*  MHead = nullptr;
  SAT_ListNode*  MTail = nullptr;
  uint32_t       MSize = 0;

//------------------------------
public:
//------------------------------

  SAT_List() {}
  virtual ~SAT_List() {}

//------------------------------
public:
//------------------------------

public:

  SAT_ListNode*  head() { return MHead; }
  SAT_ListNode*  tail() { return MTail; }

  void head(SAT_ListNode* ANode) { MHead = ANode; }
  void tail(SAT_ListNode* ANode) { MTail = ANode; }

  //----------

  void clear() {
    SAT_ListNode* node = MHead;
    while (node) {
      SAT_ListNode* next = node->next();
      delete node;
      node = next;
    }
  }

  //----------

  bool isEmpty() {
    return (MHead == nullptr);
  }

  //----------

  bool isNotEmpty() {
    return (MHead != nullptr);
  }

  //----------

  bool hasNode(SAT_ListNode* ANode) {
    SAT_ListNode* node = MHead;
    while (node) {
      if (node == ANode) return true;
      node = node->next();
    }
    return false;
  }

  //----------

  void insertHead(SAT_ListNode* ANode) {
    MSize += 1;
    if (MHead) {
      // -H -> -*H
      MHead->prev(ANode);
      ANode->next(MHead);
      ANode->prev(nullptr);
      MHead = ANode;
    }
    else {
      // - -> -*-
      ANode->next(nullptr);
      ANode->prev(nullptr);
      MTail = ANode;
      MHead = ANode;
    }
  }

  //----------

  void insertTail(SAT_ListNode* ANode) {
    MSize += 1;
    if (MTail) {
      // T- -> T*-
      ANode->next(nullptr);
      ANode->prev(MTail);
      MTail->next(ANode);
      MTail = ANode;
    }
    else {
      // empty list
      // - -> -*-
      ANode->next(nullptr);
      ANode->prev(nullptr);
      MTail = ANode;
      MHead = ANode;
    }
  }

  //----------

  void removeHead() {
    MSize -= 1;
    if (MHead) {
      // -H?
      if (MHead==MTail) {
        // -H- -> -
        MHead = nullptr;
        MTail = nullptr;
      }
      else {
        // -Hn -> n-
        MHead->next()->prev(nullptr);
        MHead = MHead->next();
      }
    }
  }

  //----------

  void removeTail() {
    MSize -= 1;
    if (MTail) {
      // ?T-
      if (MTail==MHead) {
        // -T- -> -
        MHead = nullptr;
        MTail = nullptr;
      }
      else {
        // pT- -> p-
        MTail->prev()->next(nullptr);
        MTail = MTail->prev();
      }
    }
  }

  //----------

  void insertBefore(SAT_ListNode* ANode, SAT_ListNode* ANewNode) {
    MSize += 1;
    if (ANode->prev()) {
      // pN?
      //if (ANode->next()) {
        // pNn -> p*Nn
        ANode->prev(ANewNode);
        ANewNode->next(ANode);
        ANewNode->prev(ANode->prev());
        ANode->prev()->next(ANewNode);
      //}
      //else {
      //  // pN- -> p*N-
      //  // (similar to the above)
      //  ANode->prev(ANewNode);
      //  ANewNode->next(ANode);
      //  ANewNode->prev(ANode->prev());
      //  ANode->prev()->next(ANewNode);
      //}
    }
    else {
      // -N?
      //if (ANode->next()) {
        // -Nn -> -*Nn
        ANode->prev(ANewNode);
        ANewNode->next(ANode);
        ANewNode->prev(nullptr);
        MHead = ANewNode;
      //}
      //else {
      //  // -N- -> -*N-
      //  // (similar to the above)
      //  ANode->prev(ANewNode);
      //  ANewNode->next(ANode);
      //  ANewNode->prev(nullptr);
      //  MHead = ANewNode;
      //}
    }
  }

  //----------

  void insertAfter(SAT_ListNode* ANode, SAT_ListNode* ANewNode) {
    MSize += 1;
    if (ANode->next()) {
      // ?Nn
      //if (ANode->prev()) {
        // pNn -> pN*n
        ANode->next()->prev(ANewNode);
        ANewNode->next(ANode->next());
        ANewNode->prev(ANode);
        ANode->next(ANewNode);
      //}
      //else {
      //  // -Nn -> -N*n
      //  // (similar to the above)
      //  ANode->next()->prev(ANewNode);
      //  ANewNode->next(ANode->next());
      //  ANewNode->prev(ANode);
      //  ANode->next(ANewNode);
      //}
    }
    else {
      // ?N-
      //if (ANode->prev()) {
        // pN- -> pN*-
        ANewNode->next(nullptr);
        ANewNode->prev(ANode);
        ANode->next(ANewNode);
        MTail = ANode;
      //}
      //else {
      //  // -N- -> -N*-
      //  // (similar to the above)
      //  ANewNode->next(nullptr);
      //  ANewNode->prev(ANode);
      //  ANode->next(ANewNode);
      //  MTail = ANode;
      //}
    }
  }

  //----------

  void removeNode(SAT_ListNode* ANode) {
    MSize -= 1;
    if (ANode->next()) {
      // ?Nn
      if (ANode->prev()) {
        // pNn -> pn
        ANode->next()->prev(ANode->prev());
        ANode->prev()->next(ANode->next());
      }
      else {
        // -Nn -> -n
        ANode->next()->prev(nullptr);
        MHead = ANode->next();
      }
    }
    else {
      // ?N-
      if (ANode->prev()) {
        // pN- -> p-
        ANode->prev()->next(nullptr);
        MTail = ANode->prev();
      }
      else {
        // -N- -> -
        MHead = nullptr;
        MTail = nullptr;
      }
    }
  }

  //----------

  //void sortNodes() {
  //}

  //void findNode() {
  //}

  //void swapNodes(SAT_ListNode* ANode1, SAT_ListNode* ANode2) {
  //}

  //void moveNodeToHead(SAT_ListNode* ANode) {
  //}

  //void moveNodeToTail(SAT_ListNode* ANode) {
  //}

  //----------

};

//----------------------------------------------------------------------
#endif

#if 0

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

#endif // 0
