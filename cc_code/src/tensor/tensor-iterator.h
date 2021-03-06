/******************************************************************************\
* Author: Matthew Beauregard Smith                                             *
* Affiliation: The University of Texas at Austin                               *
* Department: Oden Institute and Institute for Cellular and Molecular Biology  *
* PI: Edward Marcotte                                                          *
* Project: Protein Fluorosequencing                                            *
\******************************************************************************/

#ifndef WHATPROT_TENSOR_TENSOR_ITERATOR_H
#define WHATPROT_TENSOR_TENSOR_ITERATOR_H

namespace whatprot {

class TensorIterator {
public:
    TensorIterator(int order, int* shape, int size, double* values);
    ~TensorIterator();
    void reset();
    void advance();
    double* get();
    bool done();

    double* values;  // not owned
    int* shape;  // not owned
    int* loc;
    int order;
    int index;  // current index directly into values
    int size;  // length of values
};

}  // namespace whatprot

#endif  // WHATPROT_TENSOR_TENSOR_ITERATOR_H