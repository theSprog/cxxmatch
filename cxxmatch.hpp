#pragma once

#if __cplusplus >= 201703L
  #include <variant>
  namespace cxxmatch {
    template <typename... Ts>
    using variant = std::variant<Ts...>;
    using std::visit;
  }
#else
  #include "variant.hpp" // mpark::variant
  namespace cxxmatch {
    template <typename... Ts>
    using variant = mpark::variant<Ts...>;
    using mpark::visit;
  }
#endif


// —— overloaded 结构体 ——
template <typename... Fs>
struct overloaded;

template <typename F>
struct overloaded<F> : F {
    explicit overloaded(F f) : F(f) {}
    using F::operator();
};

template <typename F, typename... Fs>
struct overloaded<F, Fs...> : F, overloaded<Fs...> {
    overloaded(F f, Fs... fs) : F(f), overloaded<Fs...>(fs...) {}
    using F::operator();
    using overloaded<Fs...>::operator();
};

template <typename... Fs>
overloaded<Fs...> make_overloaded(Fs... fs) {
    return overloaded<Fs...>(fs...);
}

#define MATCH(var, ...) cxxmatch::visit(make_overloaded(__VA_ARGS__), var)

// —— CASE 宏族 ——
#define _GET_2ND(_1, _2, NAME, ...) NAME
#define _GET_3RD(_1, _2, _3, NAME, ...) NAME

#define _CAP_CASE1(CAP, T)      CAP (const T&)
#define _CAP_CASE2(CAP, T, var) CAP (const T& var)

#define _CAP_MCASE1(CAP, T)      CAP (T&)
#define _CAP_MCASE2(CAP, T, var) CAP (T& var)

#define _CAP_CCASE1(CAP, T)      CAP (T)
#define _CAP_CCASE2(CAP, T, var) CAP (T var)

// — Dispatcher —
#define CASE_CAPTURE(CAP, ...)      _GET_2ND(__VA_ARGS__, _CAP_CASE2, _CAP_CASE1)(CAP, __VA_ARGS__)
#define CASE_MUT_CAPTURE(CAP, ...)  _GET_2ND(__VA_ARGS__, _CAP_MCASE2, _CAP_MCASE1)(CAP, __VA_ARGS__)
#define CASE_CLONE_CAPTURE(CAP, ...) _GET_2ND(__VA_ARGS__, _CAP_CCASE2, _CAP_CCASE1)(CAP, __VA_ARGS__)

// — 默认 [&] 快捷写法 —
#define CASE(...)         CASE_CAPTURE([&], __VA_ARGS__)
#define CASE_MUT(...)     CASE_MUT_CAPTURE([&], __VA_ARGS__)
#define CASE_CLONE(...)   CASE_CLONE_CAPTURE([&], __VA_ARGS__)

// —— FOR_EACH 展开（最多支持 8 项）——
#define FE_1(WHAT, X) WHAT(X)
#define FE_2(WHAT, X, ...) WHAT(X) FE_1(WHAT, __VA_ARGS__)
#define FE_3(WHAT, X, ...) WHAT(X) FE_2(WHAT, __VA_ARGS__)
#define FE_4(WHAT, X, ...) WHAT(X) FE_3(WHAT, __VA_ARGS__)
#define FE_5(WHAT, X, ...) WHAT(X) FE_4(WHAT, __VA_ARGS__)
#define FE_6(WHAT, X, ...) WHAT(X) FE_5(WHAT, __VA_ARGS__)
#define FE_7(WHAT, X, ...) WHAT(X) FE_6(WHAT, __VA_ARGS__)
#define FE_8(WHAT, X, ...) WHAT(X) FE_7(WHAT, __VA_ARGS__)
#define GET_FE(_1,_2,_3,_4,_5,_6,_7,_8,NAME,...) NAME
#define FOR_EACH(WHAT, ...) \
    GET_FE(__VA_ARGS__, FE_8, FE_7, FE_6, FE_5, FE_4, FE_3, FE_2, FE_1)(WHAT, __VA_ARGS__)

#define FE_1V(WHAT, X) WHAT(X)
#define FE_2V(WHAT, X, ...) WHAT(X), FE_1V(WHAT, __VA_ARGS__)
#define FE_3V(WHAT, X, ...) WHAT(X), FE_2V(WHAT, __VA_ARGS__)
#define FE_4V(WHAT, X, ...) WHAT(X), FE_3V(WHAT, __VA_ARGS__)
#define FE_5V(WHAT, X, ...) WHAT(X), FE_4V(WHAT, __VA_ARGS__)
#define FE_6V(WHAT, X, ...) WHAT(X), FE_5V(WHAT, __VA_ARGS__)
#define FE_7V(WHAT, X, ...) WHAT(X), FE_6V(WHAT, __VA_ARGS__)
#define FE_8V(WHAT, X, ...) WHAT(X), FE_7V(WHAT, __VA_ARGS__)
#define GET_FEV(_1,_2,_3,_4,_5,_6,_7,_8,NAME,...) NAME
#define FOR_EACH_COMMA(WHAT, ...) \
    GET_FEV(__VA_ARGS__, FE_8V, FE_7V, FE_6V, FE_5V, FE_4V, FE_3V, FE_2V, FE_1V)(WHAT, __VA_ARGS__)

// —— struct 定义宏 ——
#define DECL_VARIANT(X) DECL_VARIANT_IMPL X
#define DECL_VARIANT_IMPL(name, body) \
    struct name body;

#define LIST_VARIANT(X) LIST_VARIANT_IMPL X
#define LIST_VARIANT_IMPL(name, ...) name

// —— Rust-like Enum 宏 ——
#define CXXRUST_ENUM(Name, ...) \
    FOR_EACH(DECL_VARIANT, __VA_ARGS__) \
    using Name = cxxmatch::variant<FOR_EACH_COMMA(LIST_VARIANT, __VA_ARGS__)>;
