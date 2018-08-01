#ifndef K_GRAPH_H
#define K_GRAPH_H

#include <map>

namespace mtm {

class KGraphPrivateMembers;
class NodePrivateMembers;
class IteratorPrivateMembers;
class ConstIteratorPrivateMembers;

// Requirements: KeyType::opertor<,
//               KeyType::operator==,
//               KeyType and ValueType copy c'tor
template<typename KeyType, typename ValueType, int k> class KGraph {
 protected:
  class Node;

 public:
  class const_iterator;  // forward declaration
  
  // An iterator. Used to iterate over the data in a kGraph. At every given 
  // moment, the iterator points either to one of the nodes in the graph, or to
  // the end of the graph.
  class iterator {
   public:
    // Constructs a new iterator that points to a given node in the given graph.
    //
    // @param node the node the new iterator points to.
    // @param graph the kGraph over which the iterator iterates.
    iterator(Node* node, KGraph* graph);
 
    // A copy constructor.
    //
    // @param it the iterator to copy.
    iterator(const iterator& it);

    // A destructor.
    ~iterator();

    // Moves the iterator to point to the node that is connected to the current
    // node through edge i.
    //
    // @param i the edge over which to move.
    // @return a reference to *this (the same iterator) after moving it.
    // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
    // @throw KGraphIteratorReachedEnd when trying to move an iterator that
    //        points to the end of the graph.
    iterator& Move(int i);

    // Dereferne operator. Return the key of the node pointed by the iterator.
    //
    // @return the key of the node to which the iterator points.
    // @throw KGraphIteratorReachedEnd when trying to dereference an iterator
    //        that points to the end of the graph.
    KeyType const& operator*() const;

    // Equal operator. Two iterators are equal iff they either point to the same
    // node in the same graph, or to the end of the same graph.
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are equal.
    bool operator==(const iterator& rhs) const;

    // Not equal operator (see definition of equality above).
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are not equal.
    bool operator!=(const iterator& rhs) const;

    // Equal operator for a const iterator as the right-hand side operand.
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are equal.
    bool operator==(const const_iterator& rhs) const;

    // Not equal operator for a const iterator as the right-hand side operand.
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are not equal.
    bool operator!=(const const_iterator& rhs) const;

   private:
    IteratorPrivateMembers* private_;
  };

  // A const iterator. Used to iterate over the data in a constant kGraph.
  // Similarly to a regular iterator, at every given moment, the iterator points
  // either to one of the nodes in the graph, or to the end of the graph.
  class const_iterator {
   public:
    // Constructs a new const iterator that points to a given node in the given
    // graph.
    //
    // @param node the node the new iterator points to.
    // @param graph the kGraph over which the iterator iterates.
    const_iterator(const Node* node, const KGraph* graph);

    // A copy constructor.
    //
    // @param it the iterator to copy.
    const_iterator(const const_iterator& it);

    // Conversion from a regular iterator. Constucts a new const iterator that
    // points to the same node as the given iterator.
    //
    // @param it the iterator we would like to convert to const iterator.
    const_iterator(const iterator& it);
    
    // A destructor.
    ~const_iterator();

    // Moves the iterator to point to the node that is connected to the current
    // node through edge i.
    //
    // @param i the edge over which to move.
    // @return a reference to *this (the same iterator) after moving it.
    // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
    // @throw KGraphIteratorReachedEnd when trying to move an iterator that
    //        points to the end of the graph.
    const_iterator& Move(int i);

    // Dereferne operator. Return the key of the node pointed by the iterator.
    //
    // @return the key of the node to which the iterator points.
    // @throw KGraphIteratorReachedEnd when trying to dereference an iterator
    //        that points to the end of the graph.
    KeyType const& operator*() const;

    // Equal operator. Two iterators are equal iff they either point to the same
    // node in the same graph, or to the end of the same graph.
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are equal.
    bool operator==(const const_iterator& rhs) const;

    // Not equal operator (see definition of equality above).
    //
    // @param rhs righ hand side operand.
    // @return true iff the iterators are not equal.
    bool operator!=(const const_iterator& rhs) const;
   
   private:
    ConstIteratorPrivateMembers* private_;
  };

  // Constructs a new empty kGraph with the given default value.
  //
  // @param default_value the default value in the graph.
  explicit KGraph(ValueType const& default_value);

  // A copy constructor. Copies the given graph. The constructed graph will have
  // the exact same structure with copied data.
  //
  // @param k_graph the graph to copy.
  KGraph(const KGraph& k_graph);

  // A destructor. Destroys the graph together with all resources allocated.
  ~KGraph();

  // Returns an iterator to the node with the given key.
  //
  // @param i the key of the node which the returned iterator points to.
  // @return iterator the newly constructed iterator.
  // @throw KGraphKeyNotFoundException when the given key is not found in the
  //        graph.
  iterator BeginAt(KeyType const& i);
  const_iterator BeginAt(KeyType const& i) const;

  // Returns an iterator to the end of the graph.
  //
  // @return iterator an iterator to the end of the graph.
  const_iterator End() const;

  // Inserts a new node with the given data to the graph.
  //
  // @param key the key to be assigned to the new node.
  // @param value the value to be assigned to the new node.
  // @throw KGraphKeyAlreadyExistsExpection when trying to insert a node with a
  //        key that already exists in the graph.
  void Insert(KeyType const& key, ValueType const& value);

  // Inserts a new node with the given key and the default value to the graph.
  //
  // @param key the key to be assigned to the new node.
  // @throw KGraphKeyAlreadyExistsExpection when trying to insert a node with a
  //        key that already exists in the graph.
  void Insert(KeyType const& key);

  // Removes the node with the given key from the graph.
  //
  // @param key the key of the node to be removed.
  // @throw KGraphKeyNotFoundException when trying to remove a key that cannot
  //        be found in the graph.
  void Remove(KeyType const& key);

  // Removes the node pointed by the given iterator from the graph. If the
  // given iterator neither points to a node in this graph nor to the end of
  // this graph, the behaviour of this function is undefined.
  //
  // @param it the iterator that points to the node to be removed.
  // @throw KGraphIteratorReachedEnd when the given iterator points to the end
  //        of the graph.
  void Remove(const iterator& it);

  // The subscript operator. Returns a reference to the value assigned to
  // the given key in the graph. If the key does not exist, inserts a new node
  // to the graph with the given key and the default value, then returns a
  // refernce to its value.
  //
  // @param key the key to return its value.
  // @return the value assigned to the given key.
  ValueType& operator[](KeyType const& key);

  // A const version of the subscript operator. Returns the value assigned to
  // the given key in the graph. If the key does not exist, throws an exception.
  //
  // @param key the key to return its value.
  // @return the value assigned to the given key.
  // @throw KGraphKeyNotFoundException if the given key cannot be found in the 
  //        graph.
  ValueType const& operator[](KeyType const& key) const;

  // Checks whether the graph contains the given key.
  //
  // @param key
  // @return true iff the graph contains the given key.
  bool Contains(KeyType const& key) const;

  // Connects two nodes in the graph with an edge.
  //
  // @param key_u the key of the first node.
  // @param key_v the key of the second node.
  // @param i_u the index of the new edge at the first node.
  // @param i_v the index of the new edge at the second node.
  // @throw KGraphKeyNotFoundException if at least one of the given keys cannot
  //        be found in the graph.
  // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1].
  // @throw KGraphNodesAlreadyConnected if the two nodes are already connected.
  // @throw KGraphEdgeAlreadyInUse if at least one of the indices of the edge at
  //        one of the nodes is already in use.
  void Connect(KeyType const& key_u, KeyType const& key_v, int i_u, int i_v);

  // Connects a node to itself via a self loop.
  //
  // @param key the key of the node.
  // @param i the index of the self loop.
  // @throw KGraphKeyNotFoundException if the given keys cannot be found in the
  //        graph.
  // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
  // @throw KGraphNodesAlreadyConnected if the node is already self connected.
  // @throw KGraphEdgeAlreadyInUse if the index of the self loop is already in
  //        use.
  void Connect(KeyType const& key, int i);

  // Disconnects two connected nodes.
  //
  // @param key_u the key of the first node.
  // @param key_v the key of the second node.
  // @throw KGraphKeyNotFoundException if at least one of the given keys cannot
  //        be found in the graph.
  // @throw kGraphNodesAreNotConnected if the two nodes are not connected.
  void Disconnect(KeyType const& key_u, KeyType const& key_v);
 
 protected:
  std::map<KeyType, Node*> nodes_;

 private:
  friend class KGraphPrivateMembers;
  friend class NodePrivateMembers;
  friend class IteratorPrivateMembers;
  friend class ConstIteratorPrivateMembers;

  KGraphPrivateMembers* private_;
};

}  // namespace mtm

#endif  // K_GRAPH_H