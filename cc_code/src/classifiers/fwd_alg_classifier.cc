// Author: Matthew Beauregard Smith (UT Austin)
#include "fwd_alg_classifier.h"

#include <functional>

#include <math.h>

#include "classifiers/scored_classification.h"
#include "common/dye_seq.h"
#include "common/dye_track.h"
#include "common/error_model.h"
#include "fwd_alg/binomial_transition.h"
#include "fwd_alg/detach_transition.h"
#include "fwd_alg/edman_transition.h"
#include "fwd_alg/emission.h"
#include "fwd_alg/fwd_alg.h"
#include "fwd_alg/initialization.h"
#include "fwd_alg/summation.h"

namespace fluoroseq {

namespace {
using std::exp;
using std::log;
using std::sqrt;

double PI = 3.141592653589793238;
}  // namespace

FwdAlgClassifier::FwdAlgClassifier(
        int num_timesteps,
        int num_channels,
        const ErrorModel& error_model,
        const ApproximationModel& approximation_model,
        int num_dye_seqs,
        DyeSeq** dye_seqs,
        int* dye_seqs_num_peptides,
        int* dye_seqs_ids)
        : num_timesteps(num_timesteps),
          num_channels(num_channels),
          num_dye_seqs(num_dye_seqs),
          dye_seqs(dye_seqs),
          max_failed_edmans(approximation_model.max_failed_edmans) {
    detach_transition = new DetachTransition(error_model.p_detach,
                                             max_failed_edmans);
    edman_transitions = new EdmanTransition*[num_dye_seqs]();
    tensors = new Tensor*[num_dye_seqs]();
    max_num_dyes = 0;
    for (int i = 0; i < num_dye_seqs; i++) {
        DyeTrack dye_track = DyeTrack(num_timesteps,
                                      num_channels,
                                      *dye_seqs[i]);
        for (int c = 0; c < num_channels; c++) {
            if (dye_track(0, c) > max_num_dyes) {
                max_num_dyes = dye_track(0, c);
            }
        }
        edman_transitions[i] = new EdmanTransition(error_model.p_edman_failure,
                                                   *dye_seqs[i],
                                                   dye_track,
                                                   max_failed_edmans);
        int* tensor_shape = new int[1 + num_channels];
        tensor_shape[0] = num_timesteps + 1;
        for (int c = 0; c < num_channels; c++) {
            int num_dyes = dye_track(0, c);
            tensor_shape[1 + c] = num_dyes + 1;
        }
        tensors[i] = new Tensor(1 + num_channels, tensor_shape);
        delete[] tensor_shape;
    }
    dud_transition = new BinomialTransition(max_num_dyes,
                                            error_model.p_dud,
                                            max_failed_edmans);
    bleach_transition = new BinomialTransition(max_num_dyes,
                                               error_model.p_bleach,
                                               max_failed_edmans);
    switch(error_model.distribution_type) {
    case LOGNORMAL:
        double scale = error_model.mu;
        double sigma = error_model.sigma;
        double multiplier = 1.0 / (sigma * sqrt(2.0 * PI));
        pdf = [scale, sigma, multiplier](double observed, int state) -> double {
            if (state > 0) {
                if (observed == 0.0) {
                    return 0.0;
                } else {
                    double unit_obs = observed / scale;
                    double offset = log(unit_obs) - log((double) state);
                    return (multiplier / unit_obs)
                           * exp(-(offset * offset) / (2.0 * sigma * sigma));
                }
            } else {
                if (observed == 0.0) {
                    return 1.0;
                } else {
                    return 0.0;
                }
            }
        };
        break;
    }
}

FwdAlgClassifier::~FwdAlgClassifier() {
    delete detach_transition;
    delete dud_transition;
    delete bleach_transition;
    for (int i = 0; i < num_dye_seqs; i++) {
        delete edman_transitions[i];
    }
    delete[] edman_transitions;
    for (int i = 0; i < num_dye_seqs; i++) {
        delete tensors[i];
    }
    delete[] tensors;
}

ScoredClassification FwdAlgClassifier::classify(const Radiometry& radiometry) {
    Emission emission(radiometry, max_num_dyes, pdf, max_failed_edmans);
    int best_i = -1;
    double best_score = -1.0;
    double total_score = 0.0;
    for (int i = 0; i < num_dye_seqs; i++) {
        Initialization initialization;
        Summation summation(max_failed_edmans);
        double score = fwd_alg(tensors[i],
                               num_timesteps,
                               num_channels,
                               initialization,
                               emission,
                               *detach_transition,
                               *dud_transition,
                               *bleach_transition,
                               *edman_transitions[i],
                               summation);
        total_score += score * dye_seqs_num_peptides[i];
        if (score > best_score) {
            best_score = score;
            best_i = i;
        }
    }
    return ScoredClassification(dye_seqs_ids[best_i], best_score, total_score);
}

ScoredClassification* FwdAlgClassifier::classify(int num_radiometries,
                                                 Radiometry** radiometries) {
    ScoredClassification* result = new ScoredClassification[num_radiometries];
    for (int i = 0; i < num_radiometries; i++) {
        result[i] = classify(*radiometries[i]);
    }
    return result;
}

}  // namespace fluoroseq
