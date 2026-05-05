#include <iostream>
#include "pointsarr.h"
#include <vector>

using namespace std;

#define TEST_FAULT  cout << "fault" << endl
#define TEST_PASSED cout << "passed" << endl

int main(int argc, char const *argv[])
{
    PointsArr arr = getPointsArr();
    cout << "test points arr" << endl;

    {
        cout << "test 01 ";
        if(arr.count == 0 && arr.capacity == INITCAPACITY){
            TEST_PASSED;
        } else {
            TEST_FAULT;
        }
    }

    {
        cout << "test 02 ";
        Point p1 = {1, 3, 3};
        Point p2 = {5, -1, 2};
        addPoint(&arr, p1);
        addPoint(&arr, p2);

        bool passed = false;
        if(arr.points[0].x == p1.x && arr.points[0].y == p1.y && arr.points[0].z == p1.z)
        {
            if(arr.points[1].x == p2.x && arr.points[1].y == p2.y && arr.points[1].z == p2.z)
            {
                passed = true;
            }
        }
        if(passed)
        {
            TEST_PASSED;
        } else {
            TEST_FAULT;
        }
    }

    {
        cout << "test 03 ";
        Point p3 = {5, 6, 7};
        addPoint(&arr, p3);

        if(arr.count == 3 && arr.capacity == INITCAPACITY*SCALECAPACITY && arr.points) {
            TEST_PASSED;
        } else {
            TEST_FAULT;
            cout << "arr.count = " << arr.count << endl;
            cout << "arr.capacity = " << arr.capacity << endl;
        }
    }

    {
        cout << "test 04 ";

        delPointsArr(&arr);
        if(arr.points == NULL){
            TEST_PASSED;
        } else {
            TEST_FAULT;
        }
    }

    return 0;
}
