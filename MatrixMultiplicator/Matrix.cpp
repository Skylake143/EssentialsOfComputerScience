#include "Matrix.h"
#include <iostream>

using namespace std;
// struct Matrix{
//     uint32_t dimX = 0;
//     uint32_t dimY = 0;
//     uint32_t** matrix = nullptr;

//     Matrix(uint32_t dimX, uint32_t dimY) 
//     {
        
//     };

//     uint32_t** Matrix::getMatrix()
//     {
//         return matrix;
//     }

//     uint32_t** Matrix::getMatrix()
//     {
//         return matrix;
//     }
// };

//Constructor
Matrix::Matrix()
{
}
Matrix::~Matrix()
{
}

//Print matrix 
void Matrix::printMatrix()
{
    for(size_t a = 0; a < dimY; ++a)
    {
        for(size_t b = 0; b < dimX; ++b)
        {
            int32_t test = matrix[a][b] ;
            std::cout << matrix[a][b] << " ";
        }
        cout << endl;
    } 
}

//Matrix-Multiplication Method single threaded
Matrix Matrix::matrixmultiplicationSingleThreaded(Matrix matrixB)
{
    //Define new output Matrix
    Matrix output;
    output.dimX = matrixB.dimX;
    output.dimY = dimY;
    output.matrix = new int32_t * [output.dimY];

    //Iteration through lines of new matrix
    for(size_t y=0;y<output.dimY;++y)
    {
        output.matrix[y] = new int32_t[output.dimX];
        //Iteration through columns of new matrix
        for(size_t matX=0; matX<output.dimX;++matX)
        {
            //calculating Matrix result
            int32_t result = 0;
            //Iterating again through x to calculate 
            for(size_t x=0; x<dimX;x++)
            {
                int32_t test1 = matrix[y][x];
                int32_t test2 = matrixB.matrix[x][matX];
                int32_t multipliedValue=matrix[y][x]*matrixB.matrix[x][matX];
                result+=multipliedValue;
            }
        
            output.matrix[y][matX] = result;
        }
    }
    return output;
}

//Matrix-Multiplication Method single threaded
Matrix Matrix::matrixmultiplicationMultiThreaded(Matrix matrixB)
{

}

//TODO: make filling code a function and add it to constructor
int main()
{
    //Initialize matrixA
    Matrix matrixA;
    matrixA.dimX=3;
    matrixA.dimY=2;
    int32_t** array = new int32_t*[matrixA.dimY];
    
    for(size_t i = 0; i<matrixA.dimY; i++)
    {
        array[i]=new int32_t[matrixA.dimX];
        for(size_t j = 0; j<matrixA.dimX; j++)
        {
            array[i][j]=i*matrixA.dimX+j+1;
        }
    }
    matrixA.matrix = array;
    cout << "Matrix A: " << endl;
    matrixA.printMatrix();
    
    //Initialize matrix B
    Matrix matrixB;
    matrixB.dimX=4;
    matrixB.dimY=3;
    int32_t** array2 = new int32_t*[matrixB.dimY];
    
    for(size_t i = 0; i<matrixB.dimY; i++)
    {
        array2[i]=new int32_t[matrixB.dimX];
        for(size_t j = 0; j<matrixB.dimX; j++)
        {   
            array2[i][j]=(int32_t)i*matrixB.dimX+j+1;
        }
    }
    matrixB.matrix = array2;
    cout << "Matrix B: " << endl;
    matrixB.printMatrix();

    Matrix output = matrixA.matrixmultiplicationSingleThreaded(matrixB);
    cout << "Output Matrix: " << endl;
    output.printMatrix();
    return 0;
}