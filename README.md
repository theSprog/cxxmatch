# 🦀 cxxmatch: Rust-style Enums with Pattern Matching in C++11

> Like `std::variant`, but fun. Like Rust enums, but in C++. Like macros, but not a nightmare.

**cxxmatch** is a lightweight C++11 macro library that gives you:

- ✅ Algebraic Data Types (enums with fields, like Rust)  
- ✅ Pattern matching via `MATCH()` + `CASE()` macros  
- ✅ Clean syntax: no boilerplate, no inheritance, no template gymnastics  
- ✅ C++11 compatible — works even without structured bindings or `decltype(auto)`  
- ✅ Explicit lambda capture control (default `&`, or `CASE_CAPTURE([=], ...)`)  
- ✅ One header, zero dependencies (uses mpark/variant for C++11 fallback)



## ✨ Quick Example

```cpp
// define a enum with data
CXXRUST_ENUM(UserState,
    (Guest,     {}),	// without data
    (LoggedIn,  { std::string name; }),	// with a string
    (VIP,       { std::string name; int level; })	// // with a string and level
);

UserState user = VIP{"Alice", 42};

int main(const int argc, const char** argv) {
    // match-case
    int ret_code = MATCH(user,
        CASE(Guest) {	// default const reference
            std::cout << "Guest\n";
            return 1;
        },
        CASE_MUT(LoggedIn, u) {	// with mutable var `u`
            u.name += "!";	// you can change it becase it is mutable reference
            std::cout << "Hello " << u.name << "\n";
            return 2;
        },
        CASE_CLONE(VIP, v) {	// with copied var `v`
            std::cout << "VIP " << v.name << ", level " << v.level << "\n";
            return 114514;
        }
    );
    
    std::cout << ret_code << std::endl;
    return 0;
}
```



## 🧠 Why not `enum class`?

C++'s `enum`/`enum class` is stuck in the Stone Age. You can't attach data. You can't pattern match. You can't even attach behavior without writing a class.

But **Rust has the answer**: enums with data.

```rust
enum UserState {
    Guest,
    LoggedIn(String),
    VIP(String, u32),
}
```

Now you can do the same in C++:

```cpp
CXXRUST_ENUM(UserState,
    (Guest, {}),
    (LoggedIn, { std::string name; }),
    (VIP, { std::string name; int level; })
);
```

Yes, this compiles as plain C++11 with just one header. 🤯



## 🧩 Highlight Features

* ✨ **Value-carrying enums** with ergonomic macro syntax
* 🧬 Auto-generates both the structs and `std::variant` type alias
* 🧠 `MATCH()` functionally similar to `std::visit`, but human-readable
* 🛠️ Fall back to `mpark::variant` if C++11 (no `std::variant` in sight)
* 📦 `CASE_MUT`, `CASE_CLONE`, `CASE_CAPTURE([=], Type, var)` — your control, your capture
* 🔧 Extendable: you can build serialization, visit-based logic, etc.



## 🐛 Caveats

* Macro errors can be cryptic — you forget a `{}` and the compiler might scream about the void.
* Maximum of 8 enum variants supported unless you extend `FOR_EACH`.
* No reflection. Yet.



## 🛠 Installation

This lib is a header-only so all you need is just put `cxxmatch.h` and `variant.hpp` to your project，have fun !




## 💬 Inspired By

* Rust's powerful `enum` and `match`
* mpark/variant (for C++11 backport)
* Louis Dionne’s `Boost.Hana`
* Good macros that don’t make you cry



## 📄 License

MIT. Use it, fork it, hack it, break it. Just don’t forget to STAR this repo ~

