#include "graph.h"

#include <iostream>

template <typename N, typename E>
gdwg::Graph<N, E>::Graph() {}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const typename std::vector<N>::const_iterator start,
                         const typename std::vector<N>::const_iterator end) {
  for (auto it = start; it != end; ++it) {
    auto node_ptr = std::make_shared<N>(*it);
    this->adj_list_.insert(std::make_pair(node_ptr, graph_edges()));
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const typename std::vector<std::tuple<N, N, E>>::const_iterator start,
                         const typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
  for (auto it = start; it != end; ++it) {
    this->InsertNode(std::get<0>(*it));
    this->InsertNode(std::get<1>(*it));
    this->InsertEdge(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const std::initializer_list<N> init_list) {
  for (const auto& it : init_list) {
    auto node_ptr = std::make_shared<N>(it);
    this->adj_list_.insert(std::make_pair(node_ptr, graph_edges()));
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E>& rhs) {
  for (const auto& it : rhs.adj_list_) {

    // auto node_ptr = std::make_shared<N>(*it.first);
    // auto ret = this->adj_list_.insert(typename graph_type::value_type(node_ptr, graph_edges()));
    this->InsertNode(*it.first);
  }
  for (const auto& it : rhs.adj_list_) {
    for (const auto& edge : it.second) {
      for (const auto& weight : edge.second) {
        this->InsertEdge(*it.first, *edge.first, weight);
      }
      // auto graph_dst_node = this->adj_list_.find(edges)
      // if (this->adj_list_.)
      // ret.second.insert(typename graph_edges::value_type(, 1));
    }
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E>&& rhs) {
  std::swap(this->adj_list_, rhs.adj_list_);
}

template <typename N, typename E>
gdwg::Graph<N, E>::~Graph() {}

template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(const gdwg::Graph<N, E>& rhs) noexcept {
  if (this == &rhs)
    return *this;
  this->Clear();

  for (const auto& it : rhs.adj_list_) {
    // auto node_ptr = std::make_shared<N>(*it.first);
    // auto ret = this->adj_list_.insert(typename graph_type::value_type(node_ptr, graph_edges()));
    this->InsertNode(*it.first);
  }
  for (const auto& it : rhs.adj_list_) {
    for (const auto& edge : it.second) {
      for (const auto& weight : edge.second) {
        this->InsertEdge(*it.first, *edge.first, weight);
      }
      // auto graph_dst_node = this->adj_list_.find(edges)
      // if (this->adj_list_.)
      // ret.second.insert(typename graph_edges::value_type(, 1));
    }
  }

  return *this;
}

template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(gdwg::Graph<N, E>&& rhs) noexcept {
  std::swap(this->adj_list_, rhs.adj_list_);
  rhs.adj_list_.clear();
  return *this;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& val) noexcept {
  auto node_ptr = std::make_shared<N>(val);
  auto ret = this->adj_list_.insert(typename graph_type::value_type(node_ptr, graph_edges()));

  return ret.second;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w) {
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
    src_node->second.insert(typename graph_edges::value_type(dst_node->first, std::set<E>({w})));

    return true;
  } else {
    auto ret = src_dst_node->second.insert(w);
    return ret.second;
  }
  // const auto ret = src_node->second.insert(typename graph_edges::value_type(dst_node->first, w));
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::Replace(const N& oldData, const N& newData) {
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

template <typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N& val) noexcept {
  bool removed = false;
  for (auto it = this->adj_list_.begin(); it != this->adj_list_.end();) {
    if (*it->first == val) {
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

template <typename N, typename E>
void gdwg::Graph<N, E>::MergeReplace(const N& oldData, const N& newData) {
  auto oldData_ptr = std::shared_ptr<N>(const_cast<N*>(&oldData), [](N*) {});
  auto newData_ptr = std::shared_ptr<N>(const_cast<N*>(&newData), [](N*) {});
  auto oldData_node = this->adj_list_.find(oldData_ptr);
  auto newData_node = this->adj_list_.find(newData_ptr);

  if (oldData_node == this->adj_list_.end() || newData_node == this->adj_list_.end()) {
    throw std::runtime_error("Cannot call Graph::MergeReplace on old or new data"
                             "if they don't exist in the graph");
  }

  newData_node->second.insert(oldData_node->second.begin(), oldData_node->second.end());
  this->adj_list_.erase(oldData_node);
}

template <typename N, typename E>
void gdwg::Graph<N, E>::Clear() noexcept {
  this->adj_list_.clear();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) const noexcept {
  const auto val_ptr = std::shared_ptr<N>(const_cast<N*>(&val), [](N*) {});
  return this->adj_list_.find(val_ptr) != this->adj_list_.end();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N& src, const N& dst) const {
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

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetNodes() const noexcept {
  std::vector<N> res(this->adj_list_.size());

  for (const auto& node : this->adj_list_) {
    res.push_back(*node.first);
  }

  return res;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetConnected(const N& src) const {
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

template <typename N, typename E>
std::vector<E> gdwg::Graph<N, E>::GetWeights(const N& src, const N& dst) const {
  auto src_ptr = std::shared_ptr<N>(const_cast<N*>(&src), [](N*) {});
  auto src_node = this->adj_list_.find(src_ptr);
  auto dst_ptr = std::shared_ptr<N>(const_cast<N*>(&dst), [](N*) {});
  auto dst_node = this->adj_list_.find(dst_ptr);
  if (src_node == this->adj_list_.cend() || dst_node == this->adj_list_.cend()) {
    throw std::out_of_range("Cannot call Graph::GetWeights if "
                            " src or dst node don't exist in the graph");
  }

  auto src_dst_node = src_node->second.find(dst_ptr);
  // std::vector<E> res(src_dst_node->second.size());
  std::vector<E> res;
  for (const auto& node : src_dst_node->second) {
    res.push_back(node);
  }

  return res;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator
gdwg::Graph<N, E>::find(const N& src, const N& dst, const E& weight) const noexcept {
  const auto src_ptr = std::shared_ptr<N>(const_cast<N*>(&src), [](N*) {});
  typename graph_type::const_iterator src_node = this->adj_list_.find(src_ptr);
  const auto dst_ptr = std::shared_ptr<N>(const_cast<N*>(&dst), [](N*) {});
  if (src_node == this->adj_list_.cend())
    return cend();
  typename graph_edges::const_iterator dst_node = src_node->second.find(dst_ptr);
  if (dst_node == src_node->second.cend())
    return cend();
  typename std::set<E>::const_iterator weight_node = dst_node->second.find(weight);
  if (weight_node == dst_node->second.cend())
    return cend();

  return {src_node, dst_node, weight_node, this->adj_list_.begin(), this->adj_list_.end()};
}

// 'std::map<std::shared_ptr<std::__cxx11::basic_string<char> >,
// std::map<std::shared_ptr<std::__cxx11::basic_string<char> >, std::set<int, std::less<int>,
// std::allocator<int> >, gdwg::Graph<std::__cxx11::basic_string<char>, int>::graph_edges_cmp,
// std::allocator<std::pair<const std::shared_ptr<std::__cxx11::basic_string<char> >, std::set<int,
// std::less<int>, std::allocator<int> > > > >, gdwg::Graph<std::__cxx11::basic_string<char>,
// int>::adj_list_cmp, std::allocator<std::pair<const
// std::shared_ptr<std::__cxx11::basic_string<char> >,
// std::map<std::shared_ptr<std::__cxx11::basic_string<char> >, std::set<int, std::less<int>,
// std::allocator<int> >, gdwg::Graph<std::__cxx11::basic_string<char>, int>::graph_edges_cmp,
// std::allocator<std::pair<const std::shared_ptr<std::__cxx11::basic_string<char> >, std::set<int,
// std::less<int>, std::allocator<int> > > > > > > >::const_iterator
// 'std::map<std::shared_ptr<std::__cxx11::basic_string<char> >,
// std::map<std::shared_ptr<std::__cxx11::basic_string<char> >, std::set<int, std::less<int>,
// std::allocator<int> >, gdwg::Graph<std::__cxx11::basic_string<char>, int>::graph_edges_cmp,
// std::allocator<std::pair<const std::shared_ptr<std::__cxx11::basic_string<char> >, std::set<int,
// std::less<int>, std::allocator<int> > > > >, gdwg::Graph<std::__cxx11::basic_string<char>,
// int>::adj_list_cmp, std::allocator<std::pair<const
// std::shared_ptr<std::__cxx11::basic_string<char> >,
// std::map<std::shared_ptr<std::__cxx11::basic_string<char> >, std::set<int, std::less<int>,
// std::allocator<int> >, gdwg::Graph<std::__cxx11::basic_string<char>, int>::graph_edges_cmp,
// std::allocator<std::pair<const std::shared_ptr<std::__cxx11::basic_string<char> >, std::set<int,
// std::less<int>, std::allocator<int> > > > > > > >::iterator

template <typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N& src, const N& dst, const E& weight) noexcept {
  if (erase(find(src, dst, weight)) == cend())
    return false;
  return true;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator
gdwg::Graph<N, E>::erase(const_iterator __position) noexcept {
  if (__position == cend())
    return __position;

  // auto it_base = __position.base();
  // auto& const_src_node = std::get<0>(it_base);
  // auto& const_dst_node = std::get<1>(it_base);
  // auto& const_weight_node = std::get<2>(it_base);

  // typename graph_type::iterator src_node;
  // std::advance(src_node,
  //   std::distance<typename graph_type::const_iterator>(src_node, const_src_node));
  // typename graph_edges::iterator dst_node;
  // std::advance(dst_node,
  //   std::distance<typename graph_edges::const_iterator>(dst_node, const_dst_node));
  // typename std::set<E>::iterator weight_node;
  // std::advance(weight_node,
  //   std::distance<typename std::set<E>::const_iterator>(weight_node, const_weight_node));

  const auto src_ptr = std::shared_ptr<N>(const_cast<N*>(&std::get<0>(*__position)), [](N*) {});
  typename graph_type::iterator src_node = this->adj_list_.find(src_ptr);
  const auto dst_ptr = std::shared_ptr<N>(const_cast<N*>(&std::get<1>(*__position)), [](N*) {});
  if (src_node == this->adj_list_.cend())
    return cend();
  typename graph_edges::iterator dst_node = src_node->second.find(dst_ptr);
  if (dst_node == src_node->second.cend())
    return cend();
  typename std::set<E>::iterator weight_node = dst_node->second.find(std::get<2>(*__position));
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
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cbegin() const noexcept {
  typename graph_type::const_iterator node_iterator = this->adj_list_.cbegin();
  typename graph_edges::const_iterator edge_iterator;
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
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cend() const noexcept {
  typename graph_type::const_iterator node_iterator = this->adj_list_.end();
  typename graph_edges::const_iterator edge_iterator;
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
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator++() {
  ++this->weight_iterator;
  if (this->weight_iterator == this->edge_iterator->second.cend()) {
    ++this->edge_iterator;
    while (this->edge_iterator == this->node_iterator->second.cend()) {
      ++this->node_iterator;
      if (this->node_iterator == this->node_iterator_end) {
        return *this;
      }
      this->edge_iterator = this->node_iterator->second.begin();
    }
    this->weight_iterator = this->edge_iterator->second.begin();
  }
  return *this;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator--() {
  // std::cout << "operator--" << std::endl;
  if (this->node_iterator == this->node_iterator_end) {
    --this->node_iterator;
    while (this->node_iterator != this->node_iterator_begin &&
           this->node_iterator->second.size() == 0) {
      --this->node_iterator;
    }

    this->edge_iterator = this->node_iterator->second.end();
    --this->edge_iterator;
    this->weight_iterator = this->edge_iterator->second.end();
    --this->weight_iterator;
  } else if (this->weight_iterator == this->edge_iterator->second.begin()) {
    if (this->edge_iterator == this->node_iterator->second.begin()) {
      --this->node_iterator;
      while (this->node_iterator != this->node_iterator_begin &&
             this->node_iterator->second.size() == 0) {
        --this->node_iterator;
      }
      this->edge_iterator = this->node_iterator->second.end();
      --this->edge_iterator;
      this->weight_iterator = this->edge_iterator->second.end();
      --this->weight_iterator;
    } else {
      --this->edge_iterator;
      this->weight_iterator = this->edge_iterator->second.end();
      --this->weight_iterator;
    }
  } else {
    --this->weight_iterator;
  }

  return *this;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator::reference gdwg::Graph<N, E>::const_iterator::
operator*() const {
  return {*this->node_iterator->first, *this->edge_iterator->first, *this->weight_iterator};
}