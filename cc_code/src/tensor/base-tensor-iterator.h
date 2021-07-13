/******************************************************************************\
* Author: Matthew Beauregard Smith                                             *
* Affiliation: The University of Texas at Austin                               *
* Department: Oden Institute and Institute for Cellular and Molecular Biology  *
* PI: Edward Marcotte                                                          *
* Project: Protein Fluorosequencing                                            *
\******************************************************************************/

#ifndef WHATPROT_TENSOR_BASE_TENSOR_ITERATOR_H
#define WHATPROT_TENSOR_BASE_TENSOR_ITERATOR_H

// Local project headers:
#include "util/kd-range.h"

namespace whatprot {

// Templatizing constness to share code between const and non-const iterators.
template <typename P>
class BaseTensorIterator {
public:
    BaseTensorIterator(unsigned int order,
                       const KDRange& range,
                       const unsigned int* shape,
                       unsigned int size,
                       P values)
            : values(values),
              range(range),
              shape(shape),
              order(order),
              index(0),
              size(size) {
        loc = new unsigned int[order]();
        reset();
    }

    ~BaseTensorIterator() {
        delete[] loc;
    }

    void reset() {
        index = 0;
        unsigned int stride = 1;  // step size for current value of o.
        // Need to use signed int o to detect when out of entries to decrease.
        for (int o = order - 1; o >= 0; o--) {
            loc[o] = range.min[o];
            index += stride * range.min[o];
            stride *= shape[o];
        }
    }

    void advance() {
        index++;
        unsigned int stride = 1;  // step size for current value of o.
        // Need to use signed int o to detect when out of entries to decrease.
        for (int o = order - 1; o >= 0; o--) {
            loc[o]++;
            if (loc[o] < range.max[o]) {
                break;
            } else {
                // (range.min[o] + shape[o] - range.max[o]) is the number of
                // entries for this order between the max of one set of numbers
                // in range to the min of the next.
                index += stride * (range.min[o] + shape[o] - range.max[o]);
                // Stride needs to be raised to account for one more dimension.
                stride *= shape[o];
                loc[o] = range.min[o];
            }
        }
    }

    P get() {
        return &values[index];
    }

    bool done() {
        return index >= size;
    }

    P values;  // not owned
    const KDRange& range;  // not owned
    const unsigned int* shape;  // not owned
    unsigned int* loc;
    const unsigned int order;
    unsigned int index;  // current index directly into values
    const unsigned int size;  // length of values
};

}  // namespace whatprot

#endif  // WHATPROT_TENSOR_BASE_TENSOR_ITERATOR_H