#include "graph.h"
// #include <vector>



template <typename N, typename E>
gdwg::Graph<N, E>::Graph() {
}

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
gdwg::Graph<N, E>::Graph(std::initializer_list<N> init_list) {
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
      this->InsertEdge(*it.first, *edge.first, edge.second);
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
gdwg::Graph<N, E>::~Graph() {
}

template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(const gdwg::Graph<N, E>& rhs) {
  if (this == &rhs) return *this;
  this->Clear();

  for (const auto& it : rhs.adj_list_) {    
    // auto node_ptr = std::make_shared<N>(*it.first);
    // auto ret = this->adj_list_.insert(typename graph_type::value_type(node_ptr, graph_edges()));
    this->InsertNode(*it.first);
  }
  for (const auto& it : rhs.adj_list_) {
    for (const auto& edge : it.second) {
      this->InsertEdge(*it.first, *edge.first, edge.second);
      // auto graph_dst_node = this->adj_list_.find(edges)
      // if (this->adj_list_.)
      // ret.second.insert(typename graph_edges::value_type(, 1));
    }
  }

  return *this;
}
template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(gdwg::Graph<N, E>&& rhs) {
  std::swap(this->adj_list_, rhs.adj_list_);
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
  auto src_ptr = std::shared_ptr<N>(const_cast<N*>(&src), [](N*){});
  auto dst_ptr = std::shared_ptr<N>(const_cast<N*>(&dst), [](N*){});
  auto src_node = this->adj_list_.find(src_ptr);
  auto dst_node = this->adj_list_.find(dst_ptr);
  
  if (src_node == this->adj_list_.end()
      || dst_node == this->adj_list_.end()) {
    throw std::runtime_error("Cannot call Graph::InsertEdge when"
      " either src or dst node does not exist");
  }

  const auto ret = src_node->second.insert(typename graph_edges::value_type(dst_node->first, w));
  return ret.second;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::Replace(const N& oldData, const N& newData) {
  auto oldData_ptr = std::shared_ptr<N>(const_cast<N*>(&oldData), [](N*){});
  auto newData_ptr = std::shared_ptr<N>(const_cast<N*>(&newData), [](N*){});
  auto oldData_node = this->adj_list_.find(oldData_ptr);
  auto newData_node = this->adj_list_.find(newData_ptr);

  if (oldData_node == this->adj_list_.end()) {
    throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
  }
  if (newData_node != this->adj_list_.end()) {
    return false;
  }
  
  *oldData_node->first = newData;
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
  auto oldData_ptr = std::shared_ptr<N>(const_cast<N*>(&oldData), [](N*){});
  auto newData_ptr = std::shared_ptr<N>(const_cast<N*>(&newData), [](N*){});
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
void gdwg::Graph<N ,E>::Clear() {
  this->adj_list_.clear();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) {
  const auto val_ptr = std::shared_ptr<N>(const_cast<N*>(&val), [](N*){});
  return this->adj_list_.find(val_ptr) != this->adj_list_.end();
}

