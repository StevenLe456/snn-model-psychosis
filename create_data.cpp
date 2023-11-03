#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "Data.hpp"

std::vector<float> parseLine(std::string);
void rando(std::vector<float>&, int);
std::vector<float> createRandVec();
void add(std::vector<float>, std::vector<float>, std::vector<float>&, int);
std::vector<float> addVec(std::vector<float>, std::vector<float>);
void divide(std::vector<float>, float, std::vector<float>&, int);
std::vector<float> normalize(std::vector<float>);

std::mt19937 generator(std::random_device{}()); 
std::normal_distribution<float> dist(0.0, 1.0);

int main(int argc, char* argv[]) {
    std::vector<std::vector<float>> train_x;
    std::vector<std::vector<float>> test_x;
    std::vector<int> train_y;
    std::vector<int> test_y;
    std::ifstream train(argv[1]);
    std::string line;
    int counter = 0;
    while (std::getline(train, line)) {
        std::vector<float> vec = parseLine(line);
        if (line == "\n") {
            break;
        }
        if (counter / 3000 == 0) {
            train_x.push_back(vec);
            train_y.push_back(0);
        }
        else {
            std::vector<float> randVec = createRandVec();
            std::vector<float> res = addVec(vec, randVec);
            std::vector<float> norm = normalize(res);
            train_x.push_back(norm);
            train_y.push_back(1);
        }
        counter++;
    }
    train.close();
    for (int i = 0; i < 3000; i++) {
        std::vector<float> rand = createRandVec();
        std::vector<float> norm = normalize(rand);
        train_x.push_back(norm);
        train_y.push_back(2);
    }
    {
        std::ofstream file(argv[2], std::ios::binary);
        cereal::PortableBinaryOutputArchive oarchive(file);
        Data data = {train_x, train_y};
        oarchive(data);
    }
    return 0;
}

std::vector<float> parseLine(std::string s) {
    std::stringstream ss(s);
    std::vector<float> vec;
    while (ss.good()) {
        std::string substr;
        std::getline(ss, substr, ',');
        vec.push_back(std::stof(substr) / 255.0);
    }
    return vec;
}

void rando(std::vector<float>& v, int i) {
    for (int j = i * 28; j < (i * 28) + 28; j++) {
        v.at(j) = dist(generator);
    }
}

std::vector<float> createRandVec() {
    std::vector<std::thread> threads;
    std::vector<float> v(28 * 28);
    for (int i = 0; i < 28; i++) {
        threads.push_back(std::thread(rando, std::ref(v), i));
    }
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    return v;
}

void add(std::vector<float> v1, std::vector<float> v2, std::vector<float>& res, int i) {
    for (int j = i * 28; j < (i * 28) + 28; j++) {
        res.at(j) = v1.at(j) + v2.at(j);
    }
}

std::vector<float> addVec(std::vector<float> v1, std::vector<float> v2) {
    std::vector<std::thread> threads;
    std::vector<float> v(28 * 28);
    for (int i = 0; i < 28; i++) {
        threads.push_back(std::thread(add, v1, v2, std::ref(v), i));
    }
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    return v;
}

void divide(std::vector<float> v, float num, std::vector<float>& res, int i) {
    for (int j = i * 28; j < (i * 28) + 28; j++) {
        res.at(j) = v.at(j) / (float)num;
    }
}

std::vector<float> normalize(std::vector<float> vect) {
    float big = *std::max_element(vect.begin(), vect.end());
    std::vector<std::thread> threads;
    std::vector<float> v(28 * 28);
    for (int i = 0; i < 28; i++) {
        threads.push_back(std::thread(divide, vect, big, std::ref(v), i));
    }
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    return v;
}