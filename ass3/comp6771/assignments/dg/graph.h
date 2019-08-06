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
  struct adj_list_cmp {
    bool operator()(const std::shared_ptr<N>& lhs, const std::shared_ptr<N>& rhs) const {
      return *lhs < *rhs;
    }
  };

  struct graph_edges_cmp {
    bool operator()(const std::shared_ptr<N>& lhs, const std::shared_ptr<N>& rhs) const {
      return *lhs < *rhs;
    }
  };

  using graph_edges = std::map<std::shared_ptr<N>, std::set<E>, graph_edges_cmp>;
  using graph_type = std::map<std::shared_ptr<N>, graph_edges, adj_list_cmp>;

 public:
  class const_iterator {
   public:
    const_iterator(const typename graph_type::const_iterator node_iter,
                   const typename graph_edges::const_iterator edge_iter,
                   const typename std::set<E>::const_iterator weight_iter,
                   const typename graph_type::const_iterator gt_begin,
                   const typename graph_type::const_iterator gt_end)
      : node_iterator(node_iter), edge_iterator(edge_iter), weight_iterator(weight_iter),
        node_iterator_begin(gt_begin), node_iterator_end(gt_end) {}

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
      // std::cout << (lhs.node_iterator == rhs.node_iterator)
      //           << (lhs.edge_iterator == rhs.edge_iterator)
      //           << (lhs.weight_iterator == rhs.weight_iterator)
      //           << (lhs.node_iterator == lhs.node_iterator_end)<< std::endl;
      if (lhs.node_iterator == lhs.node_iterator_end && rhs.node_iterator == rhs.node_iterator_end)
        return true;
      return lhs.node_iterator == rhs.node_iterator && lhs.edge_iterator == rhs.edge_iterator &&
             lhs.weight_iterator == rhs.weight_iterator;
    }

    friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
      return !(lhs == rhs);
    }

    std::tuple<typename graph_type::const_iterator,
               typename graph_edges::const_iterator,
               typename std::set<E>::const_iterator,
               typename graph_type::const_iterator,
               typename graph_type::const_iterator>
    base() {
      return {node_iterator, edge_iterator, weight_iterator, node_iterator_begin,
              node_iterator_end};
    }

   protected:
    typename graph_type::const_iterator node_iterator;
    typename graph_edges::const_iterator edge_iterator;
    typename std::set<E>::const_iterator weight_iterator;

    const typename graph_type::const_iterator node_iterator_begin;
    const typename graph_type::const_iterator node_iterator_end;
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
  Graph<N, E>& operator=(const gdwg::Graph<N, E>&) noexcept;
  Graph<N, E>& operator=(gdwg::Graph<N, E>&&) noexcept;

  bool InsertNode(const N& val) noexcept;
  bool InsertEdge(const N& src, const N& dst, const E& w);
  bool DeleteNode(const N&) noexcept;
  bool Replace(const N& oldData, const N& newData);
  void MergeReplace(const N& oldData, const N& newData);
  void Clear() noexcept;
  bool IsNode(const N& val) const noexcept;
  bool IsConnected(const N& src, const N& dst) const;

  std::vector<N> GetNodes() const noexcept;
  std::vector<N> GetConnected(const N& src) const;
  std::vector<E> GetWeights(const N& src, const N& dst) const;
  const_iterator find(const N&, const N&, const E&) const noexcept;
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
    auto lhs_iter = lhs.begin();
    auto rhs_iter = rhs.begin();

    while (lhs_iter != lhs.end() && rhs_iter != rhs.end()) {
      if (*lhs == *rhs) return false;
    }
    if (lhs_iter != lhs.end() || rhs_iter != rhs.end()) return false;
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
  graph_type adj_list_;
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
