/******************************************************************************\
* Author: Matthew Beauregard Smith                                             *
* Affiliation: The University of Texas at Austin                               *
* Department: Oden Institute and Institute for Cellular and Molecular Biology  *
* PI: Edward Marcotte                                                          *
* Project: Protein Fluorosequencing                                            *
\******************************************************************************/

#ifndef FLUOROSEQ_HMM_PRECOMPUTATIONS_UNIVERSAL_PRECOMPUTATIONS_H
#define FLUOROSEQ_HMM_PRECOMPUTATIONS_UNIVERSAL_PRECOMPUTATIONS_H

// Standard C++ library headers:
#include <vector>

// Local project headers:
#include "common/error-model.h"
#include "hmm/step/bleach-transition.h"
#include "hmm/step/detach-transition.h"
#include "hmm/step/dud-transition.h"
#include "hmm/step/start.h"

namespace fluoroseq {

class UniversalPrecomputations {
public:
    UniversalPrecomputations(const ErrorModel& error_model, int num_channels);
    void set_max_num_dyes(int max_num_dyes);
    Start start;
    DetachTransition detach_transition;
    std::vector<DudTransition> dud_transitions;
    std::vector<BleachTransition> bleach_transitions;
    int num_channels;
};

}  // namespace fluoroseq

#endif  // FLUOROSEQ_HMM_PRECOMPUTATIONS_UNIVERSAL_PRECOMPUTATIONS_H
