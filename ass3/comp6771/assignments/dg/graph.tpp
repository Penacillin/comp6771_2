#include "graph.h"
// #include <vector>

template <typename N, typename E>
gdwg::Graph<N, E>::Graph() {

}

template <typename N, typename E>
gdwg::Graph<N, E>(const typename std::vector<N>::const_iterator start,
                    const typename std::vector<N>::const_iterator end) {

}

template <typename N, typename E>
gdwg::Graph<N, E>(std::vector<std::tuple<N, N, E>>::const_iterator,
             std::vector<std::tuple<N, N, E>>::const_iterator) {
                 

}

template <typename N, typename E>
gdwg::Graph<N, E>(std::initializer_list<N>) {

}

template <typename N, typename E>
gdwg::Graph<N, E>(const gdwg::Graph<N, E>&) {

}

template <typename N, typename E>
gdwg::Graph<N, E>(gdwg::Graph<N, E>&&) {

}

template <typename N, typename E>
gdwg::~Graph<N, E>() {
    
}


