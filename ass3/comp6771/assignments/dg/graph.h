#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <map>
#include <memory>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

#include <iostream>

namespace gdwg {

template <typename N, typename E>
class Graph {
 private:
  struct AdjListComparator {
    bool operator()(const std::shared_ptr<N>& lhs, const std::shared_ptr<N>& rhs) const {
      return *lhs < *rhs;
    }
  };

  struct GraphEdgesComparator {
    bool operator()(const std::shared_ptr<N>& lhs, const std::shared_ptr<N>& rhs) const {
      return *lhs < *rhs;
    }
  };

  using GraphEdges = std::map<std::shared_ptr<N>, std::set<E>, GraphEdgesComparator>;
  using GraphRepresentation = std::map<std::shared_ptr<N>, GraphEdges, AdjListComparator>;

 public:
  class const_iterator {
   public:
    const_iterator(const typename GraphRepresentation::const_iterator node_iter,
                   const typename GraphEdges::const_iterator edge_iter,
                   const typename std::set<E>::const_iterator weight_iter,
                   const typename GraphRepresentation::const_iterator gt_begin,
                   const typename GraphRepresentation::const_iterator gt_end)
      : node_iterator_(node_iter), edge_iterator_(edge_iter), weight_iterator_(weight_iter),
        node_iterator_begin_(gt_begin), node_iterator_end_(gt_end) {}

    const_iterator& operator=(const const_iterator& rhs) {
      this->node_iterator_ = rhs.node_iterator_;
      this->edge_iterator_ = rhs.edge_iterator_;
      this->weight_iterator_ = rhs.weight_iterator_;
      return *this;
    }

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    using pointer = value_type*;
    using difference_type = int;

    reference operator*() const;
    pointer operator->() const { return &(operator*()); }

    const_iterator& operator++();
    const_iterator operator++(int) {
      auto copy{*this};
      ++(*this);
      return copy;
    }
    const_iterator& operator--();
    const_iterator operator--(int) {
      auto copy{*this};
      --(*this);
      return copy;
    }

    friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
      if (lhs.node_iterator_ == lhs.node_iterator_end_ &&
          rhs.node_iterator_ == rhs.node_iterator_end_)
        return true;
      return lhs.node_iterator_ == rhs.node_iterator_ && lhs.edge_iterator_ == rhs.edge_iterator_ &&
             lhs.weight_iterator_ == rhs.weight_iterator_;
    }

    friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
      return !(lhs == rhs);
    }

    std::tuple<typename GraphRepresentation::const_iterator,
               typename GraphEdges::const_iterator,
               typename std::set<E>::const_iterator,
               typename GraphRepresentation::const_iterator,
               typename GraphRepresentation::const_iterator>
    base() {
      return {node_iterator_, edge_iterator_, weight_iterator_, node_iterator_begin_,
              node_iterator_end_};
    }

   protected:
    typename GraphRepresentation::const_iterator node_iterator_;
    typename GraphEdges::const_iterator edge_iterator_;
    typename std::set<E>::const_iterator weight_iterator_;

    const typename GraphRepresentation::const_iterator node_iterator_begin_;
    const typename GraphRepresentation::const_iterator node_iterator_end_;
  };
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  Graph<N, E>();
  Graph<N, E>(const typename std::vector<N>::const_iterator start,
              const typename std::vector<N>::const_iterator end);
  Graph<N, E>(const typename std::vector<std::tuple<N, N, E>>::const_iterator,
              const typename std::vector<std::tuple<N, N, E>>::const_iterator);
  Graph<N, E>(const std::initializer_list<N>);
  Graph<N, E>(const gdwg::Graph<N, E>&);
  Graph<N, E>(gdwg::Graph<N, E>&&);
  ~Graph<N, E>();

  // Assignment operators
  Graph<N, E>& operator=(const Graph<N, E>&) noexcept;
  Graph<N, E>& operator=(gdwg::Graph<N, E>&&) noexcept;

  // Insert Node into graph
  // Returns true if successfully inserted
  bool InsertNode(const N& val) noexcept;
  // Insert Edge into graph
  // Returns true if successfully inserted
  bool InsertEdge(const N& src, const N& dst, const E& w);
  // Deletes a node and all references to it
  // Returns true if successfully inserted
  bool DeleteNode(const N&) noexcept;
  // Replaces a nodes value with a new one
  // Returns true if successfully changed, otherwise false if newData already
  // exists.
  bool Replace(const N& oldData, const N& newData);
  // Merges an existing Node into another node which also exists
  // And Deletes old node.
  void MergeReplace(const N& oldData, const N& newData);
  // Clears the graph
  void Clear() noexcept;
  // Check if a node exists in a graph
  bool IsNode(const N& val) const noexcept;
  // Check if two nodes are connected by at least 1 edge
  bool IsConnected(const N& src, const N& dst) const;

  std::vector<N> GetNodes() const noexcept;
  // Gets all destination nodes for a source node
  std::vector<N> GetConnected(const N& src) const;
  // Gets all weights of edges between two nodes
  std::vector<E> GetWeights(const N& src, const N& dst) const;
  // Gets a iterator to an edge which matches parameters
  // Otherwise returns end
  const_iterator find(const N&, const N&, const E&) const noexcept;
  // Erases an edge as described by params
  // Returns true if successfully removed, false otherwise
  bool erase(const N& src, const N& dst, const E& w) noexcept;

  const_iterator erase(const_iterator it) noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;
  const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{cend()}; }
  const_reverse_iterator crend() const noexcept { return const_reverse_iterator{cbegin()}; }
  const_iterator begin() const noexcept { return cbegin(); }
  const_iterator end() const noexcept { return cend(); }
  const_reverse_iterator rbegin() const noexcept { return crbegin(); }
  const_reverse_iterator rend() const noexcept { return crend(); }

  friend bool operator==(const gdwg::Graph<N, E>& lhs, const gdwg::Graph<N, E>& rhs) {
    auto lhs_node_it = lhs.adj_list_.begin();
    auto rhs_node_it = rhs.adj_list_.begin();
    if (lhs.adj_list_.size() != rhs.adj_list_.size())
      return false;
    while (lhs_node_it != lhs.adj_list_.end() && rhs_node_it != rhs.adj_list_.end()) {
      if (lhs_node_it->second.size() != rhs_node_it->second.size() ||
          *lhs_node_it->first != *rhs_node_it->first)
        return false;
      auto lhs_dst_it = lhs_node_it->second.begin();
      auto rhs_dst_it = rhs_node_it->second.begin();
      while (lhs_dst_it != lhs_node_it->second.end() && rhs_dst_it != rhs_node_it->second.end()) {
        if (lhs_dst_it->second.size() != rhs_dst_it->second.size() ||
            *lhs_dst_it->first != *rhs_dst_it->first)
          return false;
        auto lhs_w_it = lhs_dst_it->second.begin();
        auto rhs_w_it = rhs_dst_it->second.begin();

        while (lhs_w_it != lhs_dst_it->second.end() && rhs_w_it != rhs_dst_it->second.end()) {
          if (lhs_w_it != rhs_w_it)
            return false;
          ++lhs_w_it;
          ++rhs_w_it;
        }
        ++lhs_dst_it;
        ++rhs_dst_it;
      }
      ++lhs_node_it;
      ++rhs_node_it;
    }

    return true;
  }

  friend bool operator!=(const gdwg::Graph<N, E>& lhs, const gdwg::Graph<N, E>& rhs) {
    return !(lhs == rhs);
  }

  friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
    for (const auto& it : g.adj_list_) {
      os << *(it.first) << " (" << std::endl;
      for (const auto& edge : it.second) {
        for (const auto& weight : edge.second) {
          os << "  " << *edge.first << " | " << weight << std::endl;
        }
      }
      os << ")" << std::endl;
    }
    return os;
  }

 private:
  GraphRepresentation adj_list_;

  Graph<N, E>& _copy_graph(const Graph<N, E>& rhs) noexcept;
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
