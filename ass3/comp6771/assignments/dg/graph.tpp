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


