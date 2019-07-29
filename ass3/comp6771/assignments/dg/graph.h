#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <memory>
#include <set>
#include <tuple>
#include <utility>
#include <vector>
#include <map>

namespace gdwg {

template <typename N, typename E>
class Graph {
 public:
  class const_iterator {};
  class const_reverse_iterator {};

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
  const_iterator begin();
  const_iterator end();
  const_reverse_iterator rbegin();
  const_reverse_iterator rend();


  // friend bool operator==(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&);
  // friend bool operator!=(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&);
  friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
    for (const auto& it : g.adj_list_) {
      os << *(it.first) << " (" << std::endl;
      for (const auto& edge : it.second) {
        os << "  " << *edge.first << " | " << edge.second << std::endl;;
      }
      os << ")" << std::endl;
    }
    return os;
  }

 private:
  std::vector<std::shared_ptr<N>> nodes;

  struct adj_list_cmp {
    bool operator()(std::shared_ptr<N> lhs, std::shared_ptr<N> rhs) const {
        return *lhs < *rhs;
    }
  };

  struct graph_edges_cmp {
    bool operator()(std::shared_ptr<N> lhs, std::shared_ptr<N> rhs) const {
        return *lhs < *rhs;
    }
  };

  typedef std::set<std::pair<std::shared_ptr<N>, E>> graph_edges;
  typedef std::map<
    std::shared_ptr<N>,
    graph_edges,
    adj_list_cmp
  > graph_type;
  graph_type adj_list_;
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
