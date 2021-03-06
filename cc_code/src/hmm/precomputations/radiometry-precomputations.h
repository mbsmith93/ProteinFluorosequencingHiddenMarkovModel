/******************************************************************************\
* Author: Matthew Beauregard Smith                                             *
* Affiliation: The University of Texas at Austin                               *
* Department: Oden Institute and Institute for Cellular and Molecular Biology  *
* PI: Edward Marcotte                                                          *
* Project: Protein Fluorosequencing                                            *
\******************************************************************************/

#ifndef WHATPROT_HMM_PRECOMPUTATIONS_RADIOMETRY_PRECOMPUTATIONS_H
#define WHATPROT_HMM_PRECOMPUTATIONS_RADIOMETRY_PRECOMPUTATIONS_H

// Standard C++ library headers:
#include <vector>

// Local project headers:
#include "common/error-model.h"
#include "common/radiometry.h"
#include "hmm/step/peptide-emission.h"
#include "hmm/step/stuck-dye-emission.h"

namespace whatprot {

class RadiometryPrecomputations {
public:
    RadiometryPrecomputations(const Radiometry& radiometry,
                              const ErrorModel& error_model,
                              int max_num_dyes);
    PeptideEmission peptide_emission;
    std::vector<StuckDyeEmission> stuck_dye_emissions;
};

}  // namespace whatprot

#endif  // WHATPROT_HMM_PRECOMPUTATIONS_RADIOMETRY_PRECOMPUTATIONS_H
