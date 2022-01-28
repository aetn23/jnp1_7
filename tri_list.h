#ifndef TRI_LIST_H
#define TRI_LIST_H

#include <vector>
#include <variant>
#include <concepts>
#include <ranges>
#include <iostream>
#include <functional>
#include <iterator>


#include "tri_list_concepts.h"

template<typename T, typename T1, typename T2,typename T3>
concept check_type_condition = (std::same_as<T, T1> || std::same_as<T, T2> || std::same_as<T, T3>) && 
                                !(std::same_as<T, T1> && std::same_as<T, T2>) && !(std::same_as<T, T2> && std::same_as<T, T3>);
    


//todo delete default args, coz not needed
template <typename T>
T identity(T arg) {
    return arg;
}

//Oh yess, templates + concepts + lambda + PMAT
//This actually works xD
template <typename T, modifier<T> F1, modifier<T> F2> 
auto compose(F1 arg1, F2 arg2) {
    return [=](T t) mutable {return arg1(arg2(t));};
    //return F2{};
}

template <typename T1, typename T2, typename T3>
class tri_list {
    
    private:
        using variant = std::variant<T1, T2, T3>;
        using var_list_t= std::vector<variant>;

        mutable var_list_t var_list;
        std::function<T1(T1)> function_t1 = identity<T1>;
        std::function<T2(T2)> function_t2 = identity<T2>;
        std::function<T3(T3)> function_t3 = identity<T3>;

        class Iterator : public var_list_t::iterator {
            private:
                std::function<T1(T1)> function_t1;
                std::function<T2(T2)> function_t2;
                std::function<T3(T3)> function_t3;
            public:
                Iterator(std::function<T1(T1)> fun1, std::function<T2(T2)> fun2, std::function<T3(T3)> fun3, var_list_t::iterator container) :
                var_list_t::iterator(container), function_t1(fun1), function_t2(fun2), function_t3(fun3) {};
                Iterator() = default;
                inline variant operator*() const {
                    //todo modify
                    variant value = var_list_t::iterator::operator*();
                    variant result;
                    std::visit([&](variant arg) {
                       if (std::get_if<T1>(&arg)) 
                           result = std::variant<T1, T2, T3>(function_t1(std::get<T1>(arg)));
                           //std::cout << result << "\n"; }
                        else if (std::get_if<T2>(&arg) != nullptr)
                           result = std::variant<T1, T2, T3>(function_t2(std::get<T2>(arg)));
                        else if (std::get_if<T3>(&arg) != nullptr)
                           result = std::variant<T1, T2, T3>(function_t3(std::get<T3>(arg)));
                    }, value);
                    return result;
                }

                inline Iterator operator++(int) {
                    auto copy = *this;
                    var_list_t::iterator::operator++(0);
                    return copy;
                }

                inline Iterator &operator++() {
                    var_list_t::iterator::operator++();
                    return *this;
                 }

        };
    public:
        tri_list() : var_list() {}
        tri_list(std::initializer_list<std::variant<T1, T2, T3>> arg) 
        : var_list(arg) {};

        template <typename T>
        requires check_type_condition<T, T1, T2, T3>
        void push_back(const T& t) {

            var_list.push_back(t);
        }

        template <typename T, modifier<T> F>
        requires check_type_condition<T, T1, T2, T3>
        void modify_only(F m = F{}) {
            if constexpr (std::same_as<T, T1>) 
                function_t1 = compose<T1>(m, function_t1);
            else if constexpr (std::same_as<T, T2>)
                function_t2 = compose<T1>(m, function_t2);
            else if constexpr (std::same_as<T, T3>)
                function_t3 = compose<T1>(m, function_t3);

        }
        template <typename T>
        requires check_type_condition<T, T1, T2, T3>
        void reset() {
            if constexpr (std::same_as<T, T1>)
                function_t1 = identity<T1>;
            else if constexpr (std::same_as<T, T2>)
                function_t2 = identity<T2>;
            else if constexpr (std::same_as<T, T3>)
                function_t3 = identity<T3>;
        }

        template <typename T>
        requires check_type_condition<T, T1, T2, T3>
        auto range_over() const  {
            auto filtr = var_list 
            | std::views::filter([] (std::variant<T1, T2, T3> arg) {return std::get_if<T>(&arg) != nullptr; }) 
            | std::views::transform([](std::variant<T1, T2, T3> arg){return std::get<T>(arg);});
            if constexpr (std::same_as<T, T1>)
                return filtr | std::views::transform(function_t1);
           else if constexpr (std::same_as<T, T2>)
                return filtr | std::views::transform(function_t2);
            else if constexpr (std::same_as<T, T3>)
                return filtr | std::views::transform(function_t3);
        }
        //TODO find out how this works exactly 
        auto begin() const {
            //std::cout << function_t3(0) << "\n";
            return Iterator(function_t1, function_t2, function_t3, var_list.begin());
        }

        var_list_t::iterator end() const {
            return Iterator(function_t1, function_t2, function_t3, var_list.end());
        }

};


template <typename T1, typename T2, typename T3>
inline constexpr bool std::ranges::enable_borrowed_range<tri_list<T1, T2, T3>> = true;

#endif //TRI_LIST_H                   
