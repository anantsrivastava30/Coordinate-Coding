
#ifndef MATRIX_H
#define MATRIX_H

namespace dpl{

template <typename T, typename S>
class Matrix {
    
public:
    Matrix( S Column, S Row ){
        column = Column;
        row = Row;
        initializeMatrix();
    }
    
    Matrix(){
    }
    
    ~Matrix()
    {
        for( S index = 0; index < row; index ++ )
            free(values[index]);
        free(values);
    }
    
    void initializeMatrix(){
        values = new T *[row];
        for( S index = 0; index < row; index ++ )
            values[index] = new T[column];
    }
    
    T **values;
    S column;
    S row;
};

}
    
#endif