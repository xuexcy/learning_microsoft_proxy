#include <iostream>
#include <string>

#include "proxy.h"

struct Streamable : pro::facade_builder
    ::add_convention<
        pro::operator_dispatch<"<<", true>,
        std::ostream&(std::ostream& out) const>
    ::build {};

struct draw : pro::dispatch<void()> {
    template <class T>
    void operator()(T& self) { self.draw(); }
};
struct shape: pro::facade<draw> {};

struct rectangle {
    void draw() {
        std::cout << "rectangle" << std::endl;
    }
}
struct circle {
    void draw() {
        std::cout << "circle" << std::endl;
    }
}
int main() {
    std::string str = "hello world";
    pro::proxy<Streamable> p1 = &str;
    std::cout << "p1 = " << *p1 << "\n";
    pro::proxy<Streamable> p2 = std::make_unique<int>(123);
    std::cout << "p2 = " << *p2 << "\n";
    pro::proxy<Streamable> p3 = pro::make_proxy<Streamable>(3.14);
    std::cout << "p3 = " << *p3 << "\n";

    std::vector<pro::proxy<shape>> shapes;
    shapes.emplace_back(pro::make_proxy<shape>(rectangle()));
    shapes.emplace_back(pro::make_proxy<shape>(circle()));
    for (auto& p: shapes) {
        p.invoke<draw>();
    }
}
