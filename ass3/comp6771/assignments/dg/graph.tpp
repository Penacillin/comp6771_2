#include "graph.h"

#include <algorithm>
#include <iostream>

namespace gdwg {

template <typename N, typename E>
Graph<N, E>::Graph() {}

template <typename N, typename E>
Graph<N, E>::Graph(const typename std::vector<N>::const_iterator start,
                   const typename std::vector<N>::const_iterator end) {
  for (auto it = start; it != end; ++it) {
    auto node_ptr = std::make_shared<N>(*it);
    this->adj_list_.insert(std::make_pair(node_ptr, GraphEdges()));
  }
}

template <typename N, typename E>
Graph<N, E>::Graph(const typename std::vector<std::tuple<N, N, E>>::const_iterator start,
                   const typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
  for (auto it = start; it != end; ++it) {
    this->InsertNode(std::get<0>(*it));
    this->InsertNode(std::get<1>(*it));
    this->InsertEdge(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
  }
}

template <typename N, typename E>
Graph<N, E>::Graph(const std::initializer_list<N> init_list) {
  for (const auto& it : init_list) {
    auto node_ptr = std::make_shared<N>(it);
    this->adj_list_.insert(std::make_pair(node_ptr, GraphEdges()));
  }
}

template <typename N, typename E>
Graph<N, E>& Graph<N, E>::_copy_graph(const Graph<N, E>& rhs) noexcept {
  for (const auto& it : rhs.adj_list_) {
    this->InsertNode(*it.first);
  }
  for (const auto& it : rhs.adj_list_) {
    for (const auto& edge : it.second) {
      for (const auto& weight : edge.second) {
        this->InsertEdge(*it.first, *edge.first, weight);
      }
    }
  }

  return *this;
}

template <typename N, typename E>
Graph<N, E>::Graph(const Graph<N, E>& rhs) {
  _copy_graph(rhs);
}

template <typename N, typename E>
Graph<N, E>::Graph(Graph<N, E>&& rhs) {
  std::swap(this->adj_list_, rhs.adj_list_);
}

template <typename N, typename E>
Graph<N, E>::~Graph() {}

template <typename N, typename E>
Graph<N, E>& Graph<N, E>::operator=(const Graph<N, E>& rhs) noexcept {
  if (this == &rhs)
    return *this;
  this->Clear();

  _copy_graph(rhs);

  return *this;
}

template <typename N, typename E>
Graph<N, E>& Graph<N, E>::operator=(Graph<N, E>&& rhs) noexcept {
  std::swap(this->adj_list_, rhs.adj_list_);
  rhs.adj_list_.clear();
  return *this;
}

template <typename N, typename E>
bool Graph<N, E>::InsertNode(const N& val) noexcept {
  auto node_ptr = std::make_shared<N>(val);
  auto ret =
      this->adj_list_.insert(typename GraphRepresentation::value_type(node_ptr, GraphEdges()));

  return ret.second;
}

template <typename N, typename E>
bool Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w) {
  // std::make_shared<N>(src)
  auto src_ptr = std::shared_ptr<N>(const_cast<N*>(&src), [](N*) {});
  auto dst_ptr = std::shared_ptr<N>(const_cast<N*>(&dst), [](N*) {});
  auto src_node = this->adj_list_.find(src_ptr);
  auto dst_node = this->adj_list_.find(dst_ptr);

  if (src_node == this->adj_list_.end() || dst_node == this->adj_list_.end()) {
    throw std::runtime_error("Cannot call Graph::InsertEdge when"
                             " either src or dst node does not exist");
  }

  auto src_dst_node = src_node->second.find(dst_ptr);
  if (src_dst_node == src_node->second.end()) {
    src_node->second.insert(typename GraphEdges::value_type(dst_node->first, std::set<E>({w})));

    return true;
  } else {
    auto ret = src_dst_node->second.insert(w);
    return ret.second;
  }
}

template <typename N, typename E>
bool Graph<N, E>::Replace(const N& oldData, const N& newData) {
  auto oldData_ptr = std::shared_ptr<N>(const_cast<N*>(&oldData), [](N*) {});
  auto newData_ptr = std::shared_ptr<N>(const_cast<N*>(&newData), [](N*) {});
  auto oldData_node = this->adj_list_.find(oldData_ptr);
  auto newData_node = this->adj_list_.find(newData_ptr);

  if (oldData_node == this->adj_list_.end()) {
    throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
  }
  if (newData_node != this->adj_list_.end()) {
    return false;
  }

  auto newData_alloced_ptr = std::make_shared<N>(newData);
  auto extracted_oldData_node = this->adj_list_.extract(oldData_ptr);
  extracted_oldData_node.key() = newData_alloced_ptr;
  this->adj_list_.insert(std::move(extracted_oldData_node));

  for (auto it = this->adj_list_.begin(); it != this->adj_list_.end(); ++it) {
    auto edge_node = it->second.find(oldData_ptr);
    if (edge_node != it->second.end()) {
      auto extracted_oldData_edge_node = it->second.extract(edge_node);
      extracted_oldData_edge_node.key() = newData_alloced_ptr;
      it->second.insert(std::move(extracted_oldData_edge_node));
    }
  }

  return true;
}

// Deletes a node and all references to it
template <typename N, typename E>
bool Graph<N, E>::DeleteNode(const N& val) noexcept {
  bool removed = false;
  for (auto it = this->adj_list_.begin(); it != this->adj_list_.end();) {
    if (!(*it->first < val || val < *it->first)) {
      it = this->adj_list_.erase(it);
      removed = true;
    } else {
      for (auto edge = it->second.begin(); edge != it->second.end();) {
        if (*edge->first == val) {
          edge = it->second.erase(edge);
          removed = true;
        } else {
          ++edge;
        }
      }

      ++it;
    }
  }
  return removed;
}

// Merges a node into a another one
template <typename N, typename E>
void Graph<N, E>::MergeReplace(const N& oldData, const N& newData) {
  auto oldData_ptr = std::shared_ptr<N>(const_cast<N*>(&oldData), [](N*) {});
  auto newData_ptr = std::shared_ptr<N>(const_cast<N*>(&newData), [](N*) {});
  auto oldData_node = this->adj_list_.find(oldData_ptr);
  auto newData_node = this->adj_list_.find(newData_ptr);

  if (oldData_node == this->adj_list_.end() || newData_node == this->adj_list_.end()) {
    throw std::runtime_error("Cannot call Graph::MergeReplace on old or new data"
                             " if they don't exist in the graph");
  }
  if (oldData == newData)
    return;

  newData_node->second.insert(oldData_node->second.begin(), oldData_node->second.end());
  this->adj_list_.erase(oldData_node);
}

// Clears the graph
template <typename N, typename E>
void Graph<N, E>::Clear() noexcept {
  this->adj_list_.clear();
}

// Checks if a node exists in a graph
template <typename N, typename E>
bool Graph<N, E>::IsNode(const N& val) const noexcept {
  const auto val_ptr = std::shared_ptr<N>(const_cast<N*>(&val), [](N*) {});
  return this->adj_list_.find(val_ptr) != this->adj_list_.end();
}

// Checks if two nodes are connected by at least 1 edge
template <typename N, typename E>
bool Graph<N, E>::IsConnected(const N& src, const N& dst) const {
  auto src_ptr = std::shared_ptr<N>(const_cast<N*>(&src), [](N*) {});
  auto src_node = this->adj_list_.find(src_ptr);
  auto dst_ptr = std::shared_ptr<N>(const_cast<N*>(&dst), [](N*) {});
  auto dst_node = this->adj_list_.find(dst_ptr);

  if (src_node == this->adj_list_.end() || dst_node == this->adj_list_.end()) {
    throw std::runtime_error("Cannot call Graph::InsertEdge when"
                             " either src or dst node does not exist");
  }

  return src_node->second.find(dst_ptr) != src_node->second.end();
}

// Gets a vector of all nodes in the graph
template <typename N, typename E>
std::vector<N> Graph<N, E>::GetNodes() const noexcept {
  std::vector<N> res;

  for (const auto& node : this->adj_list_) {
    res.push_back(*node.first);
  }

  return res;
}

// Gets all connected destination nodes from a source node
template <typename N, typename E>
std::vector<N> Graph<N, E>::GetConnected(const N& src) const {
  auto src_ptr = std::shared_ptr<N>(const_cast<N*>(&src), [](N*) {});
  auto src_node = this->adj_list_.find(src_ptr);
  if (src_node == this->adj_list_.end()) {
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }
  std::vector<N> res;

  for (const auto& node : src_node->second) {
    res.push_back(*node.first);
  }

  return res;
}

// Gets weights between two nodes
template <typename N, typename E>
std::vector<E> Graph<N, E>::GetWeights(const N& src, const N& dst) const {
  auto src_ptr = std::shared_ptr<N>(const_cast<N*>(&src), [](N*) {});
  auto src_node = this->adj_list_.find(src_ptr);
  auto dst_ptr = std::shared_ptr<N>(const_cast<N*>(&dst), [](N*) {});
  auto dst_node = this->adj_list_.find(dst_ptr);
  if (src_node == this->adj_list_.cend() || dst_node == this->adj_list_.cend()) {
    throw std::out_of_range("Cannot call Graph::GetWeights if "
                            " src or dst node don't exist in the graph");
  }

  auto src_dst_node = src_node->second.find(dst_ptr);
  std::vector<E> res;
  for (const auto& node : src_dst_node->second) {
    res.push_back(node);
  }

  return res;
}

// Gets an iterator to an edge described by params
// Returns end if no such edge could be found
template <typename N, typename E>
typename Graph<N, E>::const_iterator
Graph<N, E>::find(const N& src, const N& dst, const E& weight) const noexcept {
  const auto src_ptr = std::shared_ptr<N>(const_cast<N*>(&src), [](N*) {});
  typename GraphRepresentation::const_iterator src_node = this->adj_list_.find(src_ptr);
  const auto dst_ptr = std::shared_ptr<N>(const_cast<N*>(&dst), [](N*) {});
  if (src_node == this->adj_list_.cend())
    return cend();
  typename GraphEdges::const_iterator dst_node = src_node->second.find(dst_ptr);
  if (dst_node == src_node->second.cend())
    return cend();
  typename std::set<E>::const_iterator weight_node = dst_node->second.find(weight);
  if (weight_node == dst_node->second.cend())
    return cend();

  return {src_node, dst_node, weight_node, this->adj_list_.begin(), this->adj_list_.end()};
}

// Erases an edge that is described by the parameters
// Returns false if successfully removed an edge, false otherwise
template <typename N, typename E>
bool Graph<N, E>::erase(const N& src, const N& dst, const E& weight) noexcept {
  if (erase(find(src, dst, weight)) == cend())
    return false;
  return true;
}

// Erases the value pointed to by iterator, and returns an iterator pointing to
// the value next in the container
// Returns end iterator if iterator invalid
template <typename N, typename E>
typename Graph<N, E>::const_iterator Graph<N, E>::erase(const_iterator _position) noexcept {
  if (_position == cend())
    return _position;

  const auto src_ptr = std::shared_ptr<N>(const_cast<N*>(&std::get<0>(*_position)), [](N*) {});
  typename GraphRepresentation::iterator src_node = this->adj_list_.find(src_ptr);
  const auto dst_ptr = std::shared_ptr<N>(const_cast<N*>(&std::get<1>(*_position)), [](N*) {});
  if (src_node == this->adj_list_.cend())
    return cend();
  typename GraphEdges::iterator dst_node = src_node->second.find(dst_ptr);
  if (dst_node == src_node->second.cend())
    return cend();
  typename std::set<E>::iterator weight_node = dst_node->second.find(std::get<2>(*_position));
  if (weight_node == dst_node->second.cend())
    return cend();

  weight_node = dst_node->second.erase(weight_node);
  if (weight_node == dst_node->second.cend()) {
    // either erase edge and increment, or just increment
    if (dst_node->second.empty()) {
      dst_node = src_node->second.erase(dst_node);
      if (dst_node == src_node->second.cend()) {
        if (src_node->second.empty()) {
          src_node = this->adj_list_.erase(src_node);
        } else {
          ++src_node;
        }
        // look for next non empty src
        while (src_node != this->adj_list_.cend() && src_node->second.empty())
          ++src_node;

        if (src_node != this->adj_list_.cend()) {
          dst_node = src_node->second.begin();
          weight_node = dst_node->second.begin();
        }
      } else {
        weight_node = dst_node->second.begin();
      }
    } else {
      ++dst_node;
      weight_node = dst_node->second.begin();
    }
  }
  return {src_node, dst_node, weight_node, this->adj_list_.cbegin(), this->adj_list_.cend()};
}



// const_iterator definitions

template <typename N, typename E>
typename Graph<N, E>::const_iterator Graph<N, E>::cbegin() const noexcept {
  typename GraphRepresentation::const_iterator node_iterator = this->adj_list_.cbegin();
  typename GraphEdges::const_iterator edge_iterator;
  typename std::set<E>::const_iterator weight_iterator;
  while (node_iterator != this->adj_list_.end() && node_iterator->second.size() == 0)
    ++node_iterator;
  if (node_iterator != this->adj_list_.cend()) {
    edge_iterator = node_iterator->second.begin();
  }
  if (node_iterator != this->adj_list_.end() && edge_iterator != node_iterator->second.end()) {
    weight_iterator = edge_iterator->second.begin();
  }
  return {node_iterator, edge_iterator, weight_iterator, this->adj_list_.begin(),
          this->adj_list_.end()};
}

template <typename N, typename E>
typename Graph<N, E>::const_iterator Graph<N, E>::cend() const noexcept {
  typename GraphRepresentation::const_iterator node_iterator = this->adj_list_.end();
  typename GraphEdges::const_iterator edge_iterator;
  typename std::set<E>::const_iterator weight_iterator;

  auto node_reverse_iter = this->adj_list_.crbegin();
  if (node_reverse_iter != this->adj_list_.crend())
    edge_iterator = node_reverse_iter->second.cend();
  while (node_reverse_iter != this->adj_list_.crend() && node_reverse_iter->second.size() == 0) {
    ++node_reverse_iter;
  }

  if (node_reverse_iter != this->adj_list_.crend()) {
    auto edge_rev_iter = node_reverse_iter->second.crbegin();
    weight_iterator = edge_rev_iter->second.cend();
  }

  return {node_iterator, edge_iterator, weight_iterator, this->adj_list_.cbegin(),
          this->adj_list_.cend()};
}


template <typename N, typename E>
typename Graph<N, E>::const_iterator& Graph<N, E>::const_iterator::operator++() {
  ++this->weight_iterator_;
  if (this->weight_iterator_ == this->edge_iterator_->second.cend()) {
    ++this->edge_iterator_;
    while (this->edge_iterator_ == this->node_iterator_->second.cend()) {
      ++this->node_iterator_;
      if (this->node_iterator_ == this->node_iterator_end_) {
        return *this;
      }
      this->edge_iterator_ = this->node_iterator_->second.begin();
    }
    this->weight_iterator_ = this->edge_iterator_->second.begin();
  }
  return *this;
}

template <typename N, typename E>
typename Graph<N, E>::const_iterator& Graph<N, E>::const_iterator::operator--() {
  // std::cout << "operator--" << std::endl;
  if (this->node_iterator_ == this->node_iterator_end_) {
    --this->node_iterator_;
    while (this->node_iterator_ != this->node_iterator_begin_ &&
           this->node_iterator_->second.size() == 0) {
      --this->node_iterator_;
    }

    this->edge_iterator_ = this->node_iterator_->second.end();
    --this->edge_iterator_;
    this->weight_iterator_ = this->edge_iterator_->second.end();
    --this->weight_iterator_;
  } else if (this->weight_iterator_ == this->edge_iterator_->second.begin()) {
    if (this->edge_iterator_ == this->node_iterator_->second.begin()) {
      --this->node_iterator_;
      while (this->node_iterator_ != this->node_iterator_begin_ &&
             this->node_iterator_->second.size() == 0) {
        --this->node_iterator_;
      }
      this->edge_iterator_ = this->node_iterator_->second.end();
      --this->edge_iterator_;
      this->weight_iterator_ = this->edge_iterator_->second.end();
      --this->weight_iterator_;
    } else {
      --this->edge_iterator_;
      this->weight_iterator_ = this->edge_iterator_->second.end();
      --this->weight_iterator_;
    }
  } else {
    --this->weight_iterator_;
  }

  return *this;
}

template <typename N, typename E>
typename Graph<N, E>::const_iterator::reference Graph<N, E>::const_iterator::operator*() const {
  return {*this->node_iterator_->first, *this->edge_iterator_->first, *this->weight_iterator_};
}

}  // namespace gdwg