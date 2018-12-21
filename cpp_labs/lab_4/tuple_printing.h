#pragma once

#include <tuple>
#include <iostream>

template<typename Ostream, typename Tuple, std::size_t... Is>
Ostream &print_tuple(Ostream &os, const Tuple &t, const std::index_sequence<Is...>&) {
    return ( (os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), ... );
}

template<typename Ch, typename Tr, typename ...Ts>
auto &operator<<(std::basic_ostream<Ch, Tr> &os, const std::tuple<Ts...> &t) {
    return print_tuple(os, t, std::index_sequence_for<Ts...>{});
}
