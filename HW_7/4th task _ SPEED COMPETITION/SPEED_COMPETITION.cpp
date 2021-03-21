#include <algorithm>
#include <cmath>
#include <execution>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#include "../../Timer/TIMER.h"

void testForEach(std::ofstream& ofstream) {
    for (auto i = 100u; i <= 1000000; i += ((i < 1000) ? 100 : (i < 10000) ? 1000 : (i < 100000) ? 10000 : 100000)) {
        std::vector <double> containerPar(i);
        std::iota(std::begin(containerPar), std::end(containerPar), 1.);
        std::vector <double> containerSeq(containerPar);

        Timer tPar;
        std::for_each(std::execution::par, std::begin(containerPar), std::end(containerPar),
                      [](auto &x) {x = std::sin(x);});
        tPar.stop();

        ofstream << i << ' ' << tPar.getTime() << ' ';

        Timer tSeq;
        std::for_each(std::begin(containerSeq), std::end(containerSeq),
                      [](auto &x) {x = std::sin(x);});
        tSeq.stop();

        ofstream << tSeq.getTime() << std::endl;
    }
}

void testSumVsScan(std::ofstream& ofstream) {
    for (auto i = 100u; i <= 1000000; i += ((i < 1000) ? 100 : (i < 10000) ? 1000 : (i < 100000) ? 10000 : 100000)) {
        std::vector<double> containerSum(i);
        std::iota(std::begin(containerSum), std::end(containerSum), 1.);
        std::vector<double> containerScan(containerSum);

        Timer tScan;
        std::inclusive_scan(std::execution::par, std::begin(containerScan), std::end(containerScan), std::begin(containerScan),
                            [](auto x, auto y) { return std::cos(x) + std::cos(y); });
        tScan.stop();

        ofstream << i << ' ' << tScan.getTime() << ' ';

        Timer tSum;
        std::vector<double> vec(i);
        std::partial_sum(std::begin(containerSum), std::end(containerSum), std::begin(containerSum),
                         [](auto x, auto y) { return std::cos(x) + std::cos(y); });
        tSum.stop();

        ofstream << tSum.getTime() << std::endl;
    }
}

void testReduceVsProduct(std::ofstream& ofstream) {
    for (auto i = 100u; i <= 1000000; i += ((i < 1000) ? 100 : (i < 10000) ? 1000 : (i < 100000) ? 10000 : 100000)) {
        std::vector <double> containerRed1(i);
        std::iota(std::begin(containerRed1), std::end(containerRed1), 1.);
        std::vector <double> containerRed2(i);
        std::iota(std::begin(containerRed1), std::end(containerRed1), 3.);
        std::vector <double> containerPr1(containerRed1);
        std::vector <double> containerPr2(containerRed2);

        Timer tPar;
        auto res1 = std::transform_reduce(std::execution::par, std::begin(containerRed1), std::end(containerRed1), std::begin(containerRed2), 0.,
                              std::multiplies<>(), [](auto x, auto y) {return std::tan(x) + std::tan(y);});
        tPar.stop();

        ofstream << i << ' ' << tPar.getTime() << ' ';

        Timer tSeq;
        auto res2 = std::inner_product(std::begin(containerPr1), std::end(containerPr1), std::begin(containerPr2), 0.,
                           std::multiplies<>(), [](auto x, auto y) {return std::tan(x) + std::tan(y);});
        tSeq.stop();

        ofstream << tSeq.getTime() << std::endl;
        std::cout << res1 << ' ' << res2;
    }
}

int main() {
    std::ofstream ofstream("../HW_7/4th task _ SPEED COMPETITION/for_each test.txt", std::ios::trunc);
    testForEach(ofstream);
    ofstream.close();

    ofstream.open("../HW_7/4th task _ SPEED COMPETITION/partial_sum VS inclusive_scan.txt", std::ios::trunc);
    testSumVsScan(ofstream);
    ofstream.close();

    ofstream.open("../HW_7/4th task _ SPEED COMPETITION/transform_reduce VS inner_product.txt", std::ios::trunc);
    testReduceVsProduct(ofstream);

    return 0;
}