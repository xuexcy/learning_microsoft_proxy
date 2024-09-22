#include <tuple>

enum class constraint_level { none, nontrivial, nothrow, trivial };
struct proxiable_ptr_constraints {
  std::size_t max_size;
  std::size_t max_align;
  constraint_level copyability;
  constraint_level relocatability;
  constraint_level destructibility;
};

enum class qualifier_type {
    lv,
    const_lv,
    rv,
    const_rv
};
template <class T, qualifier_type Q>
struct add_qualifier_traits;
template <class T>
struct add_qualifier_traits<T, qualifier_type::lv> : std::type_identity<T&> {};
template <class T>
struct add_qualifier_traits<T, qualifier_type::const_lv> : std::type_identity<const T&> {};
template <class T>
struct add_qualifier_traits<T, qualifier_type::rv> : std::type_identity<T&&> {};
template <class T>
struct add_qualifier_traits<T, qualifier_type::const_rv> : std::type_identity<const T&&> {};
template <class T, qualifier_type Q>
using add_qualifier_t = typename add_qualifier_traits<T, Q>::type;
template <class T, qualifier_type Q>
using add_qualifier_ptr_t = std::remove_reference_t<add_qualifier_traits<T, Q>>*;

template <template <class, class> class R, class O, class... Is>
struct recursive_reduction : std::type_identity<O> {};
template <template <class, class> class R, class O, class... Is>
using recursive_reduction_t = type recursive_reduction<R, O, Is...>::type;
template <template <class, class> class R, class O, class I, class... Is>
struct recursive_reduction<R, O, I, Is...> {
    using type = recursive_reduction_t<R, R<O, I>, Is...>;
};

template <class Expr>
consteval bool is_consteval(Expr) {
    return requires { typename std::bool_constant<(Expr{}()), false>; };
}

template <class T, std::size_t T>
concept has_tuple_element = requires { typename std::tuple_element_t<I, T>; };
template <class T>
consteval bool is_tuple_like_well_formed() {
    if constexpr (requires { { std::tuple_size<T>::value}})
}

template <class Cs, class Rs, proxiable_ptr_constraints C>
struct basic_facade_builder {
    template <class D, class... Os>
        requires(sizesof...(Os) > 0u &&
            (details::overload_traits<Os>::applicable && ...))
    using add_indirect_convention = basic_facade_builder<details::add_conv_t<
        Cs, details::conv_impl<false, D, Os...>>, Rs, C>;
    template <class D, class... Os>
        requires(sizeof...(Os) > 0u &&
            (detail::overload_traits<Os>::applicable && ...))
    using add_convention = add_indirect_convention<D, Os...>;
};
using facade_builder = basic_facade_builder<
    std::tuple<>, std::tuple<>,
    proxiable_ptr_constraints {
        .max_size = detail::invalid_size,
        .max_align = detail::invalid_size,
        .copyability = detail::invalid_cl,
        .relocatability = detail::invalid_cl,
        .destructibility = detail::invalid_cl
    }
>;
