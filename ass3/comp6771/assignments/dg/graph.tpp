#include "graph.h"
// #include <vector>

template <typename N, typename E>
gdwg::Graph<N, E>::Graph() {
    
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const typename std::vector<N>::const_iterator start,
                        const typename std::vector<N>::const_iterator end) {

}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const typename std::vector<std::tuple<N, N, E>>::const_iterator start,
                        const typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
    
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N> init_list) {
    for (const auto& it : init_list) {
        auto node_ptr = std::make_shared<N>(it);
        this->adj_list_.push_back(std::make_pair(node_ptr, std::vector<std::pair<std::shared_ptr<N>, E>>()));
    }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E>& rhs) {

}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E>&& rhs) {

}

template <typename N, typename E>
gdwg::Graph<N, E>::~Graph() {
    
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& val) noexcept {
    for (const auto& it : this->adj_list_) {
        if (*it.first == val) {
            return false;
        }
    }
    auto node_ptr = std::make_shared<N>(val);
    this->adj_list_.push_back(std::make_pair(node_ptr, std::vector<std::pair<std::shared_ptr<N>, E>>()));
    return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w) {
  for (auto& node : this->adj_list_) {
    if (*node.first != src) continue;
    for (const auto& edge : node.second) {
      if (*edge.first == dst && edge.second == w) return false;
    }
    for (const auto& dst_find: this->adj_list_) {
      if (*dst_find.first == dst) {
        std::shared_ptr<N> dst_ptr = dst_find.first;
        node.second.push_back(std::make_pair(dst_ptr, w));
        return true;
      }
    }
  }

  throw std::runtime_error("Cannot call Graph::InsertEdge when"
    " either src or dst node does not exist");
}

template <typename N, typename E>
void gdwg::Graph<N, E>::MergeReplace(const N& oldData, const N& newData) {
  std::pair<
      std::shared_ptr<N>,
      std::vector<std::pair<std::shared_ptr<N>, E>>
  >* old_data_pair, new_data_pair;

  for (const auto& node_pairs : this->adj_list_) {
    if (*node_pairs.first == oldData) {
      old_data_pair = &node_pairs;
    }
  }
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