#ifndef LEARNING_RATE_H
#define LEARNING_RATE_H

namespace dpl{

/* adaptive learning rate */
double learningRate( double*A, int index ){
	return 1.0/A[index];
}


}

#endif /*  learning rate */

