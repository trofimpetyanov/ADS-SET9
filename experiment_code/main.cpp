#include <iostream>
#include "StringGenerator.h"
#include "StringSortTester.h"

int main() {
    StringGenerator generator(10, 200);
    StringSortTester tester(generator);

    tester.runAllExperiments();

    return 0;
} 