#pragma once

#include <vector>

#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/vector.hpp"

struct Data {
    std::vector<std::vector<float>> x;
    std::vector<int> y;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(x, y);
    }
};
