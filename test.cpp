#include <iostream>
#include <cassert>
#include "src.hpp"

struct Tracker {
    static int count;
    int value;
    Tracker(int v = 0) : value(v) { count++; }
    Tracker(int a, int b) : value(a + b) { count++; }
    ~Tracker() { count--; }
};

int Tracker::count = 0;

void test_basic() {
    {
        sjtu::unique_ptr<Tracker> p(new Tracker(10));
        assert(Tracker::count == 1);
        assert(p->value == 10);
        assert((*p).value == 10);
        assert(p.get() != nullptr);
    }
    assert(Tracker::count == 0);
}

void test_move() {
    {
        sjtu::unique_ptr<Tracker> p1(new Tracker(20));
        sjtu::unique_ptr<Tracker> p2(std::move(p1));
        assert(Tracker::count == 1);
        assert(p1.get() == nullptr);
        assert(p2->value == 20);

        sjtu::unique_ptr<Tracker> p3;
        p3 = std::move(p2);
        assert(Tracker::count == 1);
        assert(p2.get() == nullptr);
        assert(p3->value == 20);
    }
    assert(Tracker::count == 0);
}

void test_release_reset() {
    {
        sjtu::unique_ptr<Tracker> p(new Tracker(30));
        Tracker *raw = p.release();
        assert(Tracker::count == 1);
        assert(p.get() == nullptr);
        delete raw;
        assert(Tracker::count == 0);

        p.reset(); // should be safe
        // README says: void reset();
        // So I'll only test reset()
        sjtu::unique_ptr<Tracker> p2(new Tracker(50));
        p2.reset();
        assert(Tracker::count == 0);
        assert(p2.get() == nullptr);
    }
}

void test_make_unique() {
    {
        auto p1 = sjtu::make_unique<Tracker>(60);
        assert(Tracker::count == 1);
        assert(p1->value == 60);

        auto p2 = sjtu::make_unique<Tracker>(10, 20);
        assert(Tracker::count == 2);
        assert(p2->value == 30);
    }
    assert(Tracker::count == 0);
}

int main() {
    test_basic();
    test_move();
    test_release_reset();
    test_make_unique();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
