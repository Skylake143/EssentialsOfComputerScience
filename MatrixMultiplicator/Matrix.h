#pragma once

#include <vector>
#include <pthread.h>


using namespace std;

// struct Matrix{
//         uint32_t dimX;
//         uint32_t dimY;
//         uint32_t** matrix;

//         Matrix(uint32_t dimX, uint32_t dimY);

//         uint32_t** getMatrix();

// };

//MatrixMultiplication class
class Matrix
{
    private:
        

    public:
        //Member variables
        uint32_t dimX = 0;
        uint32_t dimY = 0;
        int32_t** matrix = nullptr;
        
        //Constructor
        Matrix();
        //Destructor
	    ~Matrix();


        //print matrix
        void printMatrix();      

        //Matrix-Multiplication Method single threaded
        Matrix matrixmultiplicationSingleThreaded(Matrix matrixB);

        //Matrix-Multiplication Method multi threaded
        Matrix matrixmultiplicationMultiThreaded(Matrix matrixB);


};