// Author: Matthew Beauregard Smith (UT Austin)
#include "detach_transition.h"

#include "tensor/tensor.h"

namespace fluoroseq {

DetachTransition::DetachTransition(double p_detach) : p_detach(p_detach) {}

void DetachTransition::operator()(Tensor* tensor, int edmans) const {
    int i_max = (edmans + 1) * tensor->strides[0];
    double sum = 0.0;
    for (int i = 0; i < i_max; i++) {
        double value = tensor->values[i];
        tensor->values[i] = value * (1 - p_detach);
        sum += value;
    }
    tensor->values[edmans * tensor->strides[0]] += p_detach * sum;
}

}  // namespace fluoroseq
