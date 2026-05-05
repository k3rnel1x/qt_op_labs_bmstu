#include <iostream>
#include <vector>

#include "parser.h"
ResultCode parceCSVpoints(FILE* f, PointsArr* arr, size_t* matrixSize);

using namespace std;

#define TEST_FAULT  cout << "fault" << endl
#define TEST_PASSED cout << "passed" << endl

int test01()
{
    cout << "test 01";
    FILE* f = fopen("/home/k3rnel1x/Programing/qt_op_labs_bmstu/laba4/test_files/students_exams (вариант 2).csv", "r");
    if(!f){
        cout << "open file fail" << endl;
        return 1;
    }

    PointsArr arr = getPointsArr();
    size_t matrixSize = 0;
    
    ResultCode res = parceCSVpoints(f, &arr, &matrixSize);
    if(res == SUCCEED)
    {
        TEST_PASSED;
    } else {
        TEST_FAULT;
    }

    fclose(f);
    return res == SUCCEED;
}

int main(int argc, char const *argv[])
{
    test01();
    return 0;
}
