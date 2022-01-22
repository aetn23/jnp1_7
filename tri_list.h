#ifndef TRI_LIST_H
#define TRI_LIST_H

#include <vector>
#include <variant>
#include <concepts>
#include <ranges>
#include <iostream>
#include <functional>

#include "tri_list_concepts.h"

template <typename T1, typename T2, typename T3>
class tri_list {
    private:
        std::vector<std::variant<T1, T2, T3>> var_list;
        std::vector<std::ranges::join_view()> test;
    public:
        tri_list() : var_list() {}
        tri_list(std::initializer_list<std::variant<T1, T2, T3>> arg) : var_list(arg) {};

        template <typename T>
        void push_back(const T& t) {

            var_list.push_back(t);
        }

        template <typename T, modifier<T> F> 
        void modify_only(F m = F{}) {

            auto huj = std::views::filter([] (std::variant<T1, T2, T3> arg ) {return std::get_if<T>(&arg) != nullptr; }) 
            | std::views::transform(m);
            
            //std::cout << var_list.size() << "\n";
           /* for(auto arg : result)
                std::cout << std::get<0>(arg) << "\n";*/
            
        }
};

#endif //TRI_LIST_H