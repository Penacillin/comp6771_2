#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <memory>
#include <set>
#include <tuple>
#include <utility>
#include <vector>
#include <map>

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

  typedef std::map<std::shared_ptr<N>, std::set<E>, graph_edges_cmp> graph_edges;
  typedef std::map<std::shared_ptr<N>, graph_edges, adj_list_cmp> graph_type;

 public:
  class const_iterator {
   public:
      using iterator_category = std::bidirectional_iterator_tag;
      using value_type = std::tuple<N, N, E>;
      using reference = std::tuple<const N&, const N&, const E&>;
      using pointer = value_type*;
      using difference_type = int;

      reference operator*() const;

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
        return lhs.node_iterator == rhs.node_iterator
              && lhs.edge_iterator == rhs.edge_iterator
              && lhs.weight_iterator == rhs.weight_iterator;
      }

      friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
        return !(lhs == rhs);
      }

   private:
    typename graph_type::iterator node_iterator;
    typename graph_edges::iterator edge_iterator;
    typename std::set<E>::iterator weight_iterator;

    typename graph_type::iterator node_iterator_end;
  };
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;


  Graph<N, E>();
  Graph<N, E>(const typename std::vector<N>::const_iterator start,
              const typename std::vector<N>::const_iterator end);
  Graph<N, E>(const typename std::vector<std::tuple<N, N, E>>::const_iterator,
              const typename std::vector<std::tuple<N, N, E>>::const_iterator);
  Graph<N, E>(std::initializer_list<N>);
  Graph<N, E>(const gdwg::Graph<N, E>&);
  Graph<N, E>(gdwg::Graph<N, E>&&);
  ~Graph<N, E>();

  // Assignment operators
  Graph<N, E>& operator=(const gdwg::Graph<N, E>&);
  Graph<N, E>& operator=(gdwg::Graph<N, E>&&);

  bool InsertNode(const N& val) noexcept;
  bool InsertEdge(const N& src, const N& dst, const E& w);
  bool DeleteNode(const N&) noexcept;
  bool Replace(const N& oldData, const N& newData);
  void MergeReplace(const N& oldData, const N& newData);
  void Clear();
  bool IsNode(const N& val);
  bool IsConnected(const N& src, const N& dst);

  std::vector<N> GetNodes();
  std::vector<N> GetConnected(const N& src);
  std::vector<E> GetWeights(const N& src, const N& dst);
  const_iterator find(const N&, const N&, const E&);
  bool erase(const N& src, const N& dst, const E& w);

  const_iterator erase(const_iterator it);
  const_iterator cbegin();
  const_iterator cend();
  const_reverse_iterator crbegin();
  const_iterator crend();
  const_iterator begin() { return cbegin(); }
  const_iterator end() { return cend(); }
  const_reverse_iterator rbegin() { return crbegin(); }
  const_reverse_iterator rend() { return crend(); }


  // friend bool operator==(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&);
  // friend bool operator!=(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&);
  friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
    for (const auto& it : g.adj_list_) {
      os << *(it.first) << " (" << std::endl;
      for (const auto& edge : it.second) {
        for (const auto& weight : edge.second) {
          os << "  " << *edge.first << " | " << weight << std::endl;;
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
