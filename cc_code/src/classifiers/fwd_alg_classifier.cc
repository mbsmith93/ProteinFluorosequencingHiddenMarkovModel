// Author: Matthew Beauregard Smith (UT Austin)
#include "fwd_alg_classifier.h"

#include <functional>
#include <utility>
#include <vector>

#include "common/dye_seq.h"
#include "common/dye_track.h"
#include "common/error_model.h"
#include "common/scored_classification.h"
#include "fwd_alg/binomial_transition.h"
#include "fwd_alg/detach_transition.h"
#include "fwd_alg/edman_transition.h"
#include "fwd_alg/emission.h"
#include "fwd_alg/fwd_alg.h"
#include "fwd_alg/initialization.h"
#include "fwd_alg/summation.h"
#include "util/range.h"

namespace fluoroseq {

namespace {
using std::function;
using std::move;
using std::vector;
}

FwdAlgClassifier::FwdAlgClassifier(
        int num_timesteps,
        int num_channels,
        const ErrorModel& error_model,
        const vector<SourcedData<DyeSeq, SourceCount<int>>>& dye_seqs)
        : num_timesteps(num_timesteps),
          num_channels(num_channels),
          num_dye_seqs(dye_seqs.size()),
          dye_seqs(dye_seqs),
          detach_transition(error_model.p_detach),
          dud_transition(error_model.p_bleach),
          bleach_transition(error_model.p_bleach) {
    edman_transitions.reserve(num_dye_seqs);
    tensors.reserve(num_dye_seqs);
    max_num_dyes = 0;
    for (int i = 0; i < num_dye_seqs; i++) {
        DyeTrack dye_track = DyeTrack(num_timesteps,
                                      num_channels,
                                      dye_seqs[i].value);
        for (int c = 0; c < num_channels; c++) {
            if (dye_track(0, c) > max_num_dyes) {
                max_num_dyes = dye_track(0, c);
            }
        }
        edman_transitions.push_back(
                move(EdmanTransition(error_model.p_edman_failure,
                                     dye_seqs[i].value,
                                     dye_track)));
        int* tensor_shape = new int[1 + num_channels];
        tensor_shape[0] = num_timesteps + 1;
        for (int c = 0; c < num_channels; c++) {
            int num_dyes = dye_track(0, c);
            tensor_shape[1 + c] = num_dyes + 1;
        }
        tensors.push_back(move(Tensor(1 + num_channels, tensor_shape)));
        delete[] tensor_shape;
    }
    dud_transition.reserve(max_num_dyes);
    bleach_transition.reserve(max_num_dyes);
    pdf = error_model.pdf();
}

ScoredClassification FwdAlgClassifier::classify(const Radiometry& radiometry) {
    return classify_helper<Range>(radiometry, Range(num_dye_seqs));
}

ScoredClassification FwdAlgClassifier::classify(
        const Radiometry& radiometry,
        const vector<int>& candidate_indices) {
    return classify_helper<const vector<int>&>(radiometry, candidate_indices);
}

vector<ScoredClassification> FwdAlgClassifier::classify(
        const vector<Radiometry>& radiometries) {
    vector<ScoredClassification> results;
    results.reserve(radiometries.size());
    for (int i = 0; i < radiometries.size(); i++) {
        results.push_back(classify(radiometries[i]));
    }
    return results;
}

}  // namespace fluoroseq
