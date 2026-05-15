#include "arrayd.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../prj.thirdparty/doctest/doctest.h"

TEST_CASE("[arrayd] - ctor default") {
    ArrayD a;
    CHECK(0 == a.size());
}

TEST_CASE("[arrayd] - ctor size") {
    ArrayD a(5);
    CHECK(5 == a.size());
    for (std::ptrdiff_t i = 0; i < 5; i++) CHECK(a[i] == doctest::Approx(0.0f));

    CHECK_THROWS_AS(ArrayD(0),  std::invalid_argument);
    CHECK_THROWS_AS(ArrayD(-1), std::invalid_argument);
}

TEST_CASE("[arrayd] - ctor copy") {
    ArrayD a(3);
    a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;

    ArrayD b(a);
    CHECK(3 == b.size());
    CHECK(b[0] == doctest::Approx(1.0f));
    CHECK(b[1] == doctest::Approx(2.0f));
    CHECK(b[2] == doctest::Approx(3.0f));

    // deep copy: изменение b не затрагивает a
    b[0] = 99.0f;
    CHECK(a[0] == doctest::Approx(1.0f));

    // копия пустого массива
    ArrayD empty;
    ArrayD empty_copy(empty);
    CHECK(0 == empty_copy.size());
}

TEST_CASE("[arrayd] - op= copy") {
    ArrayD a(3);
    a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;

    ArrayD b(2);
    b = a;
    CHECK(3 == b.size());
    CHECK(b[0] == doctest::Approx(1.0f));
    CHECK(b[2] == doctest::Approx(3.0f));

    // deep copy
    b[0] = 99.0f;
    CHECK(a[0] == doctest::Approx(1.0f));

    // самоприсваивание
    a = a;
    CHECK(3 == a.size());
    CHECK(a[0] == doctest::Approx(1.0f));
}

TEST_CASE("[arrayd] - resize") {
    ArrayD a(3);
    a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;

    // рост: новые элементы = 0
    a.resize(5);
    CHECK(5 == a.size());
    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK(a[2] == doctest::Approx(3.0f));
    CHECK(a[3] == doctest::Approx(0.0f));
    CHECK(a[4] == doctest::Approx(0.0f));

    // сужение: элементы в пределах нового размера сохраняются
    a.resize(2);
    CHECK(2 == a.size());
    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK(a[1] == doctest::Approx(2.0f));
    CHECK_THROWS_AS(a[2], std::out_of_range);

    // до нуля
    a.resize(0);
    CHECK(0 == a.size());

    // повторный рост после сужения до 0
    a.resize(3);
    CHECK(3 == a.size());
    for (std::ptrdiff_t i = 0; i < 3; i++) CHECK(a[i] == doctest::Approx(0.0f));

    CHECK_THROWS_AS(a.resize(-1), std::invalid_argument);
}

TEST_CASE("[arrayd] - op[]") {
    ArrayD a(3);
    a[0] = 1.5f;
    a[1] = -2.5f;
    a[2] = 3.14f;
    CHECK(a[0] == doctest::Approx(1.5f));
    CHECK(a[1] == doctest::Approx(-2.5f));
    CHECK(a[2] == doctest::Approx(3.14f));

    CHECK_THROWS_AS(a[-1], std::out_of_range);
    CHECK_THROWS_AS(a[3],  std::out_of_range);
}

TEST_CASE("[arrayd] - op[] const") {
    ArrayD tmp(2);
    tmp[0] = 1.0f; tmp[1] = 2.0f;
    const ArrayD a(tmp);

    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK(a[1] == doctest::Approx(2.0f));
    CHECK_THROWS_AS(a[-1], std::out_of_range);
    CHECK_THROWS_AS(a[2],  std::out_of_range);
}

TEST_CASE("[arrayd] - insert") {
    ArrayD a(3);
    a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;

    // вставка в середину
    a.insert(1, 99.0f);
    CHECK(4 == a.size());
    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK(a[1] == doctest::Approx(99.0f));
    CHECK(a[2] == doctest::Approx(2.0f));
    CHECK(a[3] == doctest::Approx(3.0f));

    // вставка в начало
    a.insert(0, -1.0f);
    CHECK(5 == a.size());
    CHECK(a[0] == doctest::Approx(-1.0f));
    CHECK(a[1] == doctest::Approx(1.0f));

    // вставка в конец (idx == size)
    a.insert(5, 100.0f);
    CHECK(6 == a.size());
    CHECK(a[5] == doctest::Approx(100.0f));

    CHECK_THROWS_AS(a.insert(-1, 0.0f), std::out_of_range);
    CHECK_THROWS_AS(a.insert(7, 0.0f),  std::out_of_range);
}

TEST_CASE("[arrayd] - remove") {
    ArrayD a(4);
    a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f; a[3] = 4.0f;

    // удаление из середины
    a.remove(1);
    CHECK(3 == a.size());
    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK(a[1] == doctest::Approx(3.0f));
    CHECK(a[2] == doctest::Approx(4.0f));

    // удаление последнего
    a.remove(2);
    CHECK(2 == a.size());
    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK(a[1] == doctest::Approx(3.0f));

    // удаление первого
    a.remove(0);
    CHECK(1 == a.size());
    CHECK(a[0] == doctest::Approx(3.0f));

    // удаление единственного элемента
    a.remove(0);
    CHECK(0 == a.size());

    CHECK_THROWS_AS(a.remove(-1), std::out_of_range);
    CHECK_THROWS_AS(a.remove(0),  std::out_of_range);
}
