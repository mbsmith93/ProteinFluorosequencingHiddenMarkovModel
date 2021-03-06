/******************************************************************************\
* Author: Matthew Beauregard Smith                                             *
* Affiliation: The University of Texas at Austin                               *
* Department: Oden Institute and Institute for Cellular and Molecular Biology  *
* PI: Edward Marcotte                                                          *
* Project: Protein Fluorosequencing                                            *
\******************************************************************************/

// Defining symbols from header:
#include "generate-radiometry.h"

// Standard C++ library headers:
#include <cmath>
#include <random>

// Local project headers:
#include "simulation/generate-dye-track.h"

namespace whatprot {

namespace {
using std::default_random_engine;
using std::log;
using std::lognormal_distribution;
}  // namespace

void generate_radiometry(const ErrorModel& error_model,
                         const DyeSeq& dye_seq,
                         int num_timesteps,
                         int num_channels,
                         default_random_engine* generator,
                         Radiometry* radiometry) {
    DyeTrack dye_track(num_timesteps, num_channels);
    generate_dye_track(error_model,
                       dye_seq,
                       num_timesteps,
                       num_channels,
                       generator,
                       &dye_track);
    for (int t = 0; t < num_timesteps; t++) {
        for (int c = 0; c < num_channels; c++) {
            if (dye_track(t, c) > 0) {
                lognormal_distribution<double> lognormal(
                        error_model.mu + log((double)dye_track(t, c)),
                        error_model.sigma);
                (*radiometry)(t, c) = lognormal(*generator);
            } else {
                (*radiometry)(t, c) = 0.0;
            }
        }
    }
}

}  // namespace whatprot
