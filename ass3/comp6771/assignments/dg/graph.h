#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <memory>
#include <tuple>
#include <vector>

namespace gdwg {

template <typename N, typename E>
class Graph {
 public:
    // class const_iterator {};

    Graph<N, E>();
    Graph<N, E>(const typename std::vector<N>::const_iterator start,
                const typename std::vector<N>::const_iterator end);
    Graph<N, E>(const typename std::vector<std::tuple<N, N, E>>::const_iterator,
                const typename std::vector<std::tuple<N, N, E>>::const_iterator);
    Graph<N, E>(std::initializer_list<N>);
    Graph<N, E>(const gdwg::Graph<N, E>&);
    Graph<N, E>(gdwg::Graph<N, E>&&);
    ~Graph<N, E>();

 private:
    std::vector<std::unique_ptr<N>> nodes;
    std::vector<std::unique_ptr<E>> edges;
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
