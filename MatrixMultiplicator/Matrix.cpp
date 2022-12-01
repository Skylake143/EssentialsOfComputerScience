#include "Matrix.h"
#include <iostream>
#include <stdexcept>
#include <chrono>
#include "pthread.h"
#include "assert.h"

using namespace std;
using namespace std::chrono;
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

bool PRINT_MATRICES=false;

//Constructor
Matrix::Matrix()
{
}

//Constructor creating 
Matrix::Matrix(uint32_t dimX, uint32_t dimY)
{
    createRandomMatrix(dimX,dimY);
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

void Matrix::createRandomMatrix(uint32_t dimX, uint32_t dimY)
{
    this->dimX = dimX;
    this->dimY = dimY;
    this->matrix = new int32_t*[dimY];
    
    for(size_t i = 0; i<dimY; i++)
    {
        matrix[i]=new int32_t[dimX];
        for(size_t j = 0; j<dimX; j++)
        {
            matrix[i][j]=std::rand() %10;
            //matrix[i][j]=i*dimX+j+1;
        }
    }
}


//Method filling matrix of given dimension successivly with 1,2,3...
void Matrix::increasingFillMatrix(uint32_t dimX, uint32_t dimY)
{
    this->dimX = dimX;
    this->dimY = dimY;
    this->matrix = new int32_t*[dimY];
    
    for(size_t i = 0; i<dimY; i++)
    {
        matrix[i]=new int32_t[dimX];
        for(size_t j = 0; j<dimX; j++)
        {
            matrix[i][j]=i*dimX+j+1;
        }
    }
}


//Matrix-Multiplication Method single threaded
Matrix Matrix::matrixmultiplicationSingleThreaded(Matrix matrixB)
{   
    try
    {
        if(this->dimX != matrixB.dimY)
        {
            cout << "X-Dimension of Matrix A must be equal to Y-Dimentsion of Matrix B" << endl;
            exit(1);
            //TODO: does not work??
            throw runtime_error("X-Dimension of Matrix A must be equal to Y-Dimentsion of Matrix B");
        }
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

    catch(runtime_error e)
    {
        cout << "Runtime error: " << e.what();
    }
    exit(1);
}

struct thread_data{
    Matrix* output;
    int32_t** matrix;
    Matrix* matrixB; 
    size_t y;
    uint32_t dimX;

};

void* threadMatrixCalculation(void *threadarg)
{
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    int32_t** matrix = my_data->matrix;
    Matrix* matrixB = my_data->matrixB;
    Matrix* output = my_data->output;
    size_t y = my_data->y;
    uint32_t dimX = my_data->dimX;

    //Iteration through columns of new matrix
    for(size_t matX=0; matX<output->dimX;++matX)
    {
        //calculating Matrix result
        int32_t result = 0;
        //Iterating again through x to calculate 
        for(size_t x=0; x<dimX;x++)
        {
            int32_t test1 = matrix[y][x];
            int32_t test2 = matrixB->matrix[x][matX];
            int32_t multipliedValue=matrix[y][x]*matrixB->matrix[x][matX];
            result+=multipliedValue;
        }
        output->matrix[y][matX] = result;
    }

    return NULL;
}

//Matrix-Multiplication Method single threaded
Matrix Matrix::matrixmultiplicationMultiThreaded(Matrix matrixB)
{
    if(this->dimX != matrixB.dimY)
    {
        cout << "X-Dimension of Matrix A must be equal to Y-Dimentsion of Matrix B" << endl;
        exit(1);
    }
    //Define new output Matrix
    Matrix output;
    output.dimX = matrixB.dimX;
    output.dimY = dimY;
    output.matrix = new int32_t * [output.dimY];

    pthread_t threadArr[output.dimY];
    struct thread_data thread_data_array[output.dimY];
    int result_code;

    //Iteration through lines of new matrix
    for(size_t y=0;y<output.dimY;++y)
    {
        output.matrix[y] = new int32_t[output.dimX];
    }
    //Iteration through lines of new matrix
    for(size_t y=0;y<output.dimY;++y)
    {
        thread_data_array[y].matrixB = &matrixB;
        thread_data_array[y].matrix = matrix;
        thread_data_array[y].output = &output;
        thread_data_array[y].y = y;
        thread_data_array[y].dimX = dimX;

        result_code = pthread_create(&threadArr[y], NULL,threadMatrixCalculation,(void *)&thread_data_array[y]);
        assert(!result_code);
    }
    for(size_t y=0;y<output.dimY;++y)
    {
        result_code = pthread_join(threadArr[y], NULL);
        assert(!result_code);
    }
    
    return output;
}

//Matrix-Multiplication Method single threaded
Matrix Matrix::matrixmultiplicationMultiThreadedScheduler(Matrix matrixB, int scheduler, bool randPriority)
{
    if(this->dimX != matrixB.dimY)
    {
        cout << "X-Dimension of Matrix A must be equal to Y-Dimentsion of Matrix B" << endl;
        exit(1);
    }
    //Define new output Matrix
    Matrix output;
    output.dimX = matrixB.dimX;
    output.dimY = dimY;
    output.matrix = new int32_t * [output.dimY];

    pthread_t threadArr[output.dimY];
    struct thread_data thread_data_array[output.dimY];

    //Iteration through lines of new matrix
    for(size_t y=0;y<output.dimY;++y)
    {
        output.matrix[y] = new int32_t[output.dimX];
    }
    //Iteration through lines of new matrix
    for(size_t y=0;y<output.dimY;++y)
    {
        thread_data_array[y].matrixB = &matrixB;
        thread_data_array[y].matrix = matrix;
        thread_data_array[y].output = &output;
        thread_data_array[y].y = y;
        thread_data_array[y].dimX = dimX;

        //Set scheduler
        sched_param schedparam;
        if(randPriority == true)schedparam.sched_priority = std::rand() %10;
        
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy(&attr, SCHED_RR);
        pthread_attr_setschedparam(&attr, &schedparam);

        int rc = pthread_create(&threadArr[y], &attr,threadMatrixCalculation,(void *)&thread_data_array[y]);
        pthread_attr_destroy(&attr);
        //TODO: int rc
    }
    for(size_t y=0;y<output.dimY;++y)
    {
        pthread_join(threadArr[y], NULL);
    }
    //pthread_exit(NULL);
    
    return output;
}

int main()
{
    //Initialize matrixA
    Matrix matrixA(1500,1500);
    if(PRINT_MATRICES==true)
    {
        cout << "Matrix A: " << endl;
        matrixA.printMatrix();
    }
    
    //Initialize matrix B
    Matrix matrixB(1500,1500);
    if(PRINT_MATRICES==true)
    {
        cout << "Matrix B: " << endl;
        matrixB.printMatrix();
    }

    auto startTime = chrono::high_resolution_clock::now();
    Matrix output = matrixA.matrixmultiplicationSingleThreaded(matrixB);
    auto stopTime = chrono::high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stopTime - startTime);
    if(PRINT_MATRICES==true)
    {
        cout << "Matrix A times B: " << endl;
        output.printMatrix();
    }
    cout << "Duration to perform single-threaded Matrix multiplication in ms: " << duration.count() << endl;

    startTime = chrono::high_resolution_clock::now();
    Matrix output2 = matrixA.matrixmultiplicationMultiThreaded(matrixB);
    stopTime = chrono::high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stopTime - startTime);
    if(PRINT_MATRICES==true)
    {
        cout << "Matrix A times B: " << endl;
        output2.printMatrix();
    }
    cout << "Duration to perform multi-threaded Matrix multiplication in ms: " << duration.count() << endl;

    //Round Robin scheduler
    startTime = chrono::high_resolution_clock::now();
    Matrix output3 = matrixA.matrixmultiplicationMultiThreadedScheduler(matrixB, SCHED_RR, false);
    stopTime = chrono::high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stopTime - startTime);
    if(PRINT_MATRICES==true)
    {
        cout << "Matrix A times B: " << endl;
        output2.printMatrix();
    }
    cout << "Duration to perform multi-threaded Matrix multiplication with Round Robin Scheduler in ms: " << duration.count() << endl;

    //FIFO Scheduler - First in first out
    startTime = chrono::high_resolution_clock::now();
    Matrix output4 = matrixA.matrixmultiplicationMultiThreadedScheduler(matrixB, SCHED_FIFO, false);
    stopTime = chrono::high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stopTime - startTime);
    if(PRINT_MATRICES==true)
    {
        cout << "Matrix A times B: " << endl;
        output2.printMatrix();
    }
    cout << "Duration to perform multi-threaded Matrix multiplication with FirstInFirstOut Scheduler in ms: " << duration.count() << endl;

//Round Robin scheduler
    startTime = chrono::high_resolution_clock::now();
    Matrix output5 = matrixA.matrixmultiplicationMultiThreadedScheduler(matrixB, SCHED_RR, true);
    stopTime = chrono::high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stopTime - startTime);
    if(PRINT_MATRICES==true)
    {
        cout << "Matrix A times B: " << endl;
        output2.printMatrix();
    }
    cout << "Duration to perform multi-threaded Matrix multiplication with Round Robin Scheduler and random priority in ms: " << duration.count() << endl;

    //FIFO Scheduler - First in first out
    startTime = chrono::high_resolution_clock::now();
    Matrix output6 = matrixA.matrixmultiplicationMultiThreadedScheduler(matrixB, SCHED_FIFO, true);
    stopTime = chrono::high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stopTime - startTime);
    if(PRINT_MATRICES==true)
    {
        cout << "Matrix A times B: " << endl;
        output2.printMatrix();
    }
    cout << "Duration to perform multi-threaded Matrix multiplication with FirstInFirstOut Scheduler  and random priority in ms: " << duration.count() << endl;

    return 0;
}