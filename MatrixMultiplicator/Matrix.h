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

        //Constructor with given dimensions calling Random creation method
        Matrix(uint32_t dimX, uint32_t dimY);
        //Destructor
	    ~Matrix();


        //print matrix
        void printMatrix(); 

        //Method filling matrix of given dimension with random numbers
        void createRandomMatrix(uint32_t dimX, uint32_t dimY);

        //Method filling matrix of given dimension successivly with 1,2,3...
        void increasingFillMatrix(uint32_t dimX, uint32_t dimY);


        //Matrix-Multiplication Method single threaded
        Matrix* matrixmultiplicationSingleThreaded(Matrix matrixB);

        //Thread function
        //void* threadMatrixCalculation(void *threadarg);

        //Matrix-Multiplication Method multi threaded
        Matrix* matrixmultiplicationMultiThreaded(Matrix matrixB);


};

//TODO: 
// Random function always generates the same values?
// Ask lecturer why try catch does not work?
// pthread exit properly and check if calculation works correctly