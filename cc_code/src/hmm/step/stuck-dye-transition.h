/******************************************************************************\
* Author: Matthew Beauregard Smith                                             *
* Affiliation: The University of Texas at Austin                               *
* Department: Oden Institute and Institute for Cellular and Molecular Biology  *
* PI: Edward Marcotte                                                          *
* Project: Protein Fluorosequencing                                            *
\******************************************************************************/

#ifndef WHATPROT_HMM_STEP_STUCK_DYE_TRANSITION_H
#define WHATPROT_HMM_STEP_STUCK_DYE_TRANSITION_H

// Local project headers:
#include "hmm/state-vector/stuck-dye-state-vector.h"
#include "hmm/step/step.h"
#include "parameterization/fit/sequencing-model-fitter.h"

namespace whatprot {

class StuckDyeTransition : public Step<StuckDyeStateVector> {
public:
    StuckDyeTransition(double loss_rate, int channel);
    virtual void forward(int* num_edmans,
                         StuckDyeStateVector* sdsv) const override;
    virtual void backward(const StuckDyeStateVector& input,
                          int* num_edmans,
                          StuckDyeStateVector* output) const override;
    virtual void improve_fit(const StuckDyeStateVector& forward_sdsv,
                             const StuckDyeStateVector& backward_sdsv,
                             const StuckDyeStateVector& next_backward_sdsv,
                             int num_edmans,
                             double probability,
                             SequencingModelFitter* fitter) const override;

    int channel;
    double loss_rate;
};

}  // namespace whatprot

#endif  // WHATPROT_HMM_STEP_STUCK_DYE_TRANSITION_H
