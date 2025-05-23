#include "cxxmatch.hpp"
#include <iostream>
#include <string>

// 定义一个 Rust 风格的枚举：表示用户状态
CXXRUST_ENUM(UserState,
             (Guest, {}),                           // 无字段变体
             (LoggedIn, { std::string name; }),     // 单字段
             (VIP, {std::string name; int level; }),// 多字段
             (Banned, { std::string reason; })      // 错误状态
)

void print_user_status(const UserState& state) {
    MATCH(
        state,
        CASE(Guest) { std::cout << "👤 Guest user.\n"; },
        CASE(LoggedIn, user) { std::cout << "👋 Welcome back, " << user.name << "!\n"; },
        CASE(VIP, v) { std::cout << "🌟 VIP " << v.name << " [level " << v.level << "]\n"; },
        CASE(Banned, b) { std::cout << "❌ Banned: " << b.reason << "\n"; });
}

void promote_user(UserState& state) {
    MATCH(
        state,
        CASE_MUT(LoggedIn, user) {
            state = VIP{user.name, 1}; // 提升为 VIP
        },
        CASE_MUT(VIP, v) {
            ++v.level; // 提升 VIP 等级
        },
        CASE(Guest) { std::cout << "Can't promote guest directly.\n"; },
        CASE(Banned) { std::cout << "Can't promote banned user.\n"; });
}

int get_reward_multiplier(const UserState& state) {
    return MATCH(
        state,
        CASE_CLONE(Guest, g) { return 1; },
        CASE_CLONE(LoggedIn, u) { return 2; },
        CASE_CLONE(VIP, v) { return 5 + v.level; },
        CASE_CLONE(Banned, b) { return 0; });
}

int main() {
    UserState a = Guest{};
    UserState b = LoggedIn{"Alice"};
    UserState c = VIP{"Bob", 3};
    UserState d = Banned{"cheating detected"};

    print_user_status(a);
    print_user_status(b);
    print_user_status(c);
    print_user_status(d);

    std::cout << "--- Promote ---\n";
    promote_user(a);
    promote_user(b);
    promote_user(c);
    promote_user(d);

    print_user_status(a);
    print_user_status(b);
    print_user_status(c);
    print_user_status(d);

    std::cout << "--- Multiplier ---\n";
    std::cout << "a: x" << get_reward_multiplier(a) << "\n";
    std::cout << "b: x" << get_reward_multiplier(b) << "\n";
    std::cout << "c: x" << get_reward_multiplier(c) << "\n";
    std::cout << "d: x" << get_reward_multiplier(d) << "\n";

    return 0;
}

