/******************************************************************************\
* Author: Matthew Beauregard Smith                                             *
* Affiliation: The University of Texas at Austin                               *
* Department: Oden Institute and Institute for Cellular and Molecular Biology  *
* PI: Edward Marcotte                                                          *
* Project: Protein Fluorosequencing                                            *
\******************************************************************************/

#ifndef WHATPROT_SIMULATION_GENERATE_DYE_TRACK_H
#define WHATPROT_SIMULATION_GENERATE_DYE_TRACK_H

// Standard C++ library headers:
#include <random>

// Local project headers:
#include "common/dye-seq.h"
#include "common/dye-track.h"
#include "common/error-model.h"

namespace whatprot {

void generate_dye_track(const ErrorModel& error_model,
                        const DyeSeq& dye_seq,
                        int num_timesteps,
                        int num_channels,
                        std::default_random_engine* generator,
                        DyeTrack* dye_track);

}  // namespace whatprot

#endif  // WHATPROT_SIMULATION_GENERATE_DYE_TRACK_H
