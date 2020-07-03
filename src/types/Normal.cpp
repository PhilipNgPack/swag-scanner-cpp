#include "Normal.h"

equations::Normal::Normal(float A, float B, float C) : A(A), B(B), C(C) {}

equations::Normal::Normal(std::vector<float> in) : A(in[0]), B(in[1]), C(in[2]) {
    if (in.size() < 3) {
        throw std::invalid_argument("cannot create normal, input vector size too small");
    }
}

equations::Normal::Normal(pcl::ModelCoefficients::Ptr in) : A(in->values[0]), B(in->values[1]), C(in->values[2]) {}