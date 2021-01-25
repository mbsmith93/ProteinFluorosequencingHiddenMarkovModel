/******************************************************************************\
* Author: Matthew Beauregard Smith                                             *
* Affiliation: The University of Texas at Austin                               *
* Department: Oden Institute and Institute for Cellular and Molecular Biology  *
* PI: Edward Marcotte                                                          *
* Project: Protein Fluorosequencing                                            *
\******************************************************************************/

#ifndef FLUOROSEQ_HMM_DETACH_TRANSITION_H
#define FLUOROSEQ_HMM_DETACH_TRANSITION_H

// Local project headers:
#include "hmm/step.h"
#include "tensor/tensor.h"

namespace fluoroseq {

class DetachTransition : public Step {
public:
    DetachTransition(double p_detach);
    virtual void forward(const Tensor& input,
                         int* edmans,
                         Tensor* output) const override;
    virtual void backward(const Tensor& input,
                          int* edmans,
                          Tensor* output) const override;

    double p_detach;
};

}  // namespace fluoroseq

#endif  // FLUOROSEQ_HMM_DETACH_TRANSITION_H
