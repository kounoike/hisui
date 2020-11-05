#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>

namespace progresscpp {
class ProgressBar {
private:
    std::uint64_t ticks = 0;

    const std::uint64_t total_ticks;
    const std::uint64_t bar_width;
    const char complete_char = '=';
    const char incomplete_char = ' ';
    const std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

public:
    ProgressBar(std::uint64_t total, std::uint64_t width, char complete, char incomplete) :
            total_ticks{total}, bar_width{width}, complete_char{complete}, incomplete_char{incomplete} {}

    ProgressBar(std::uint64_t total, std::uint64_t width) : total_ticks{total}, bar_width{width} {}

    std::uint64_t operator++() { return ++ticks; }

    void setTicks(const std::uint64_t new_ticks) { ticks = new_ticks; }

    void display() const {
        float progress = static_cast<float>(ticks) / static_cast<float>(total_ticks);
        std::uint64_t pos = static_cast<std::uint64_t>(static_cast<float>(bar_width) * progress);

        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();

        std::cout << "[";

        for (std::uint64_t i = 0; i < bar_width; ++i) {
            if (i < pos) std::cout << complete_char;
            else if (i == pos) std::cout << ">";
            else std::cout << incomplete_char;
        }
        std::cout << "] " << int(progress * 100.0f) << "% "
                  << float(time_elapsed) / 1000.0f << "s\r";
        std::cout.flush();
    }

    void done() const {
        display();
        std::cout << std::endl;
    }
};
}
