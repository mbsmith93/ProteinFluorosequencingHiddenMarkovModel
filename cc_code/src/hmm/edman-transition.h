/******************************************************************************\
* Author: Matthew Beauregard Smith                                             *
* Affiliation: The University of Texas at Austin                               *
* Department: Oden Institute and Institute for Cellular and Molecular Biology  *
* PI: Edward Marcotte                                                          *
* Project: Protein Fluorosequencing                                            *
\******************************************************************************/

#ifndef FLUOROSEQ_HMM_EDMAN_TRANSITION_H
#define FLUOROSEQ_HMM_EDMAN_TRANSITION_H

// Local project headers:
#include "common/dye-track.h"
#include "hmm/step.h"
#include "tensor/tensor.h"

namespace fluoroseq {

class EdmanTransition : public Step {
public:
    EdmanTransition(double p_edman_failure,
                    const DyeSeq& dye_seq,
                    const DyeTrack& dye_track);
    virtual void forward(const Tensor& input,
                         int* edmans,
                         Tensor* output) const override;
    virtual void backward(const Tensor& input,
                          int* edmans,
                          Tensor* output) const override;

    DyeSeq dye_seq;
    DyeTrack dye_track;
    double p_edman_failure;
};

}  // namespace fluoroseq

#endif  // FLUOROSEQ_HMM_EDMAN_TRANSITION_H