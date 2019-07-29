#include "graph.h"
// #include <vector>



template <typename N, typename E>
gdwg::Graph<N, E>::Graph() {
  //   this->adj_list_ = std::map<
  //   std::shared_ptr<N>,
  //   graph_edge
  // >(this->graph_key_cmp);
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const typename std::vector<N>::const_iterator start,
                        const typename std::vector<N>::const_iterator end) {
  // this->adj_list_ = std::map<
  //   std::shared_ptr<N>,
  //   graph_edge
  // >(this->graph_key_cmp);
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const typename std::vector<std::tuple<N, N, E>>::const_iterator start,
                        const typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
  // this->adj_list_ = std::map<
  //   std::shared_ptr<N>,
  //   graph_edge
  // >(this->graph_key_cmp);
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N> init_list) {
  // this->adj_list_ = std::map<
  //   std::shared_ptr<N>,
  //   graph_edge
  // >(this->graph_key_cmp);

  for (const auto& it : init_list) {
      auto node_ptr = std::make_shared<N>(it);
      this->adj_list_.insert(std::make_pair(node_ptr, graph_edges()));
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E>& rhs) {
  // this->adj_list_ = std::map<
  //   std::shared_ptr<N>,
  //   graph_edge
  // >(this->graph_key_cmp);
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E>&& rhs) {
  // this->adj_list_ = std::map<
  //   std::shared_ptr<N>,
  //   graph_edge
  // >(this->graph_key_cmp);
}

template <typename N, typename E>
gdwg::Graph<N, E>::~Graph() {
  // this->adj_list_ = std::map<
  //   std::shared_ptr<N>,
  //   graph_edge
  // >(this->graph_key_cmp);
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
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) {
    for (const auto& it : this->adj_list_) {
        if (*it.first == val) {
            return true;
        }
    }
    return false;
}


// template <typename FN, typename FE>
// std::ostream& gdwg::operator<<(std::ostream& os, const gdwg::Graph<FN, FE>& g) {
//     os << "lmao";
//     return os;
// }