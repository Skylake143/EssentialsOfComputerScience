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

//Struct to store data needed for threading 
struct thread_data{
    Matrix* output;
    int32_t** matrix;
    Matrix* matrixB; 
    size_t y;
    uint32_t dimX;

};

//Matrix multiplication algorithm
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
        //Variable for calculation of every single entry
        int32_t result = 0;
        //Iterating again through x to calculate partly result
        for(size_t x=0; x<dimX;x++)
        {
            int32_t multipliedValue=matrix[y][x]*matrixB->matrix[x][matX];
            result+=multipliedValue;
        }
        output->matrix[y][matX] = result;
    }

    return NULL;
}

//Matrix-Multiplication Method Multi threaded
Matrix Matrix::matrixmultiplicationMultiThreaded(Matrix matrixB)
{
    //Check if Matrices have the correct dimensions
    if(this->dimX != matrixB.dimY)
    {
        cout << "X-Dimension of Matrix A must be equal to Y-Dimentsion of Matrix B" << endl;
        exit(1);
    }
    //Define new output Matrix where calculated entries are stored
    Matrix output;
    output.dimX = matrixB.dimX;
    output.dimY = dimY;
    //In C/C++ Matrices must be initialized with dynamic array
    output.matrix = new int32_t * [output.dimY];

    //Create array of type pthread_t
    //Length of rows of the output array
    //Each row will be calculated by a own thread
    pthread_t threadArr[output.dimY];
    //Create array of type struct thread_data to store data in
    struct thread_data thread_data_array[output.dimY];
    //Set up variable for result_code
    int result_code;

    //Iteration through lines of output array to initialize array
    for(size_t y=0;y<output.dimY;++y)
    {
        output.matrix[y] = new int32_t[output.dimX];
    }
    //Iteration through lines of new matrix
    for(size_t y=0;y<output.dimY;++y)
    {
        //Prepare thread_data with provided data for matrix multiplication
        thread_data_array[y].matrixB = &matrixB;
        thread_data_array[y].matrix = matrix;
        thread_data_array[y].output = &output;
        thread_data_array[y].y = y;
        thread_data_array[y].dimX = dimX;

        //Create threads by passing pthread_t object of every entry in array, function and pointer on provided data
        result_code = pthread_create(&threadArr[y], NULL,threadMatrixCalculation,(void *)&thread_data_array[y]);
        //Check if pthread has been successful
        assert(!result_code);
    }
    //Set all threads in the mode to wait for all threads until they finished before continuing with pthread_join()
    for(size_t y=0;y<output.dimY;++y)
    {
        result_code = pthread_join(threadArr[y], NULL);
        //Check again for success
        assert(!result_code);
    }
    
    //Return calculated matrix
    return output;
}

//Matrix-Multiplication Method Multi threaded with manual definition of scheduler
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

        //Set scheduling parameters
        sched_param schedparam;
        //Set scheduling priority
        if(randPriority == true)schedparam.sched_priority = std::rand() %10;
        //Manually add p_thread_attr_t
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        //Specify how thread scheduling attributes are defined
        //PTHREAD_EXPLICIT_SCHED specifies that new thread gets scheduling attribute defined in this attribute
        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        //Set scheduling policy
        pthread_attr_setschedpolicy(&attr, SCHED_RR);
        //Link sched_param to pthread_attr_t
        pthread_attr_setschedparam(&attr, &schedparam);

        //Continues as previously explained
        result_code = pthread_create(&threadArr[y], &attr,threadMatrixCalculation,(void *)&thread_data_array[y]);
        assert(!result_code);
        pthread_attr_destroy(&attr);
    }
    for(size_t y=0;y<output.dimY;++y)
    {
        pthread_join(threadArr[y], NULL);
    }
    
    return output;
}

int main()
{
    cout << "Do you want to calculate a quadratic Matrix multiplication: (yes/no)";
    char input[]="";
    cin >> input;
    string quadratic;
    for (int x=0; x<strlen(input); x++)
        quadratic.push_back(toupper(input[x]));

    int matSizeX=0;
    int matSizeY=0;
    if(quadratic == "YES" || quadratic =="Y")
    {
        cout << "Put in size of Matrix:";
        cin >> input;
        matSizeX=stoi(input);
        matSizeY=matSizeX;
    }
    else
    {
        cout << "Put in x-Dimension:";
        cin >> input;
        matSizeX=stoi(input);
        cout << "Put in y-Dimension:";
        cin >> input;
        matSizeY=stoi(input);
    }
    if(max(matSizeX,matSizeY)<11)PRINT_MATRICES=true;

    //Initialize matrixA and print if necessary
    Matrix matrixA(matSizeX,matSizeY);
    if(PRINT_MATRICES==true)
    {
        cout << "Matrix A: " << endl;
        matrixA.printMatrix();
    }

    
    //Initialize matrix B and print if necessary
    Matrix matrixB(matSizeX,matSizeX);
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