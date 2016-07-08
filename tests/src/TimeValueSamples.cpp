#include "scraps/TimeValueSamples.h"

#include <gtest/gtest.h>

#include <chrono>
#include <map>

using namespace scraps;

TEST(TimeValueSamples, basicUsage) {
    TimeValueSamples<uintmax_t> timeValues{3};
    EXPECT_EQ(timeValues.size(), 0);

    timeValues.insert(std::chrono::steady_clock::time_point(1s), 1000); EXPECT_EQ(timeValues.size(), 1);
    timeValues.insert(std::chrono::steady_clock::time_point(2s), 2000); EXPECT_EQ(timeValues.size(), 2);
    timeValues.insert(std::chrono::steady_clock::time_point(3s), 3000); EXPECT_EQ(timeValues.size(), 3);
    timeValues.insert(std::chrono::steady_clock::time_point(4s), 4000); EXPECT_EQ(timeValues.size(), 3); // older sample was erased
    timeValues.insert(std::chrono::steady_clock::time_point(5s), 5000); EXPECT_EQ(timeValues.size(), 3); // older sample was erased

    EXPECT_EQ(timeValues.samples(), (std::map<std::chrono::steady_clock::time_point, uintmax_t>{
        {std::chrono::steady_clock::time_point(3s), 3000},
        {std::chrono::steady_clock::time_point(4s), 4000},
        {std::chrono::steady_clock::time_point(5s), 5000},
    }));
}

TEST(TimeValueSamples, erasesOlderSamples) {
    TimeValueSamples<uintmax_t> timeValues{3};
    EXPECT_EQ(timeValues.size(), 0);

    timeValues.insert(std::chrono::steady_clock::time_point(4s), 4000);
    timeValues.insert(std::chrono::steady_clock::time_point(5s), 5000);
    timeValues.insert(std::chrono::steady_clock::time_point(1s), 1000);
    timeValues.insert(std::chrono::steady_clock::time_point(2s), 2000); // 1s erased
    timeValues.insert(std::chrono::steady_clock::time_point(3s), 3000); // 2s erased

    EXPECT_EQ(timeValues.samples(), (std::map<std::chrono::steady_clock::time_point, uintmax_t>{
        {std::chrono::steady_clock::time_point(3s), 3000},
        {std::chrono::steady_clock::time_point(4s), 4000},
        {std::chrono::steady_clock::time_point(5s), 5000},
    }));
}

TEST(TimeValueSamples, oldestNewest) {
    TimeValueSamples<uintmax_t> timeValues{3};
    EXPECT_EQ(timeValues.size(), 0);

    timeValues.insert(std::chrono::steady_clock::time_point(1s), 1000);
    timeValues.insert(std::chrono::steady_clock::time_point(2s), 2000);
    timeValues.insert(std::chrono::steady_clock::time_point(3s), 3000);

    EXPECT_EQ(timeValues.oldest(), std::make_pair(std::chrono::steady_clock::time_point(1s), 1000ul));
    EXPECT_EQ(timeValues.newest(), std::make_pair(std::chrono::steady_clock::time_point(3s), 3000ul));
}
