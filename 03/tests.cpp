#include <iostream>
#include <cstdio>
#include <cassert>
#include <string>
#include "matrix.h"

void fillMatrix(Matrix &matr)
{
    for (size_t i = 0; i < matr.getRows(); ++i)
    {
        for (size_t j = 0; j < matr.getColumns(); ++j)
        {
            matr[i][j] = i + j;
        }
    }
}

void fillMatrixWithValue(Matrix &matr, int value)
{
    for (size_t i = 0; i < matr.getRows(); ++i)
    {
        for (size_t j = 0; j < matr.getColumns(); ++j)
        {
            matr[i][j] = value;
        }
    }
}

// Some tests for Matrix class
int main()
{
    {
        Matrix matr(2, 2);
        bool catched = false;
        try
        {
            matr[0][2] = 5;
        }
        catch (const std::out_of_range &e)
        {
            catched = true;
        }
        assert(catched);

        catched = false;
        try
        {
            matr[10][1] = 5;
        }
        catch (const std::out_of_range &e)
        {
            catched = true;
        }
        assert(catched);

        catched = false;
        try
        {
            matr[1][1] = 5;
        }
        catch (const std::out_of_range &e)
        {
            catched = true;
        }
        assert(!catched);
    }

    {
        Matrix matrix1(2, 3);
        Matrix matrix2(2, 2);
        bool catched = false;
        try
        {
            matrix1 + matrix2;
        }
        catch (const std::logic_error &e)
        {
            catched = true;
        }
        assert(catched);
    }

    {
        Matrix matrix(123, 321);
        assert((matrix.getRows() == 123) && (matrix.getColumns() == 321));
    }

    {
        Matrix matrix(2, 2);
        matrix[0][0] = 0;
        matrix[0][1] = 1;
        matrix[1][0] = 10;
        matrix[1][1] = 11;

        assert(matrix[0][0] == 0);
        assert(matrix[0][1] == 1);
        assert(matrix[1][0] == 10);
        assert(matrix[1][1] == 11);
    }

    {
        Matrix matr1(10, 10);
        Matrix matr2(10, 10);
        fillMatrix(matr1);
        fillMatrix(matr2);
        assert(matr1 == matr2);
        matr1[4][5] = 45;
        assert(matr1 != matr2);
        matr2[4][5] = 45;
        assert(matr1 == matr2);
    }

    {
        Matrix matr1(5, 5);
        Matrix matr2(5, 5);
        fillMatrixWithValue(matr1, 7);
        fillMatrixWithValue(matr2, 21);
        matr1 *= 3;
        assert(matr1 == matr2);
    }

    {
        Matrix matr1(5, 5);
        Matrix matr2(5, 5);
        Matrix matr3(5, 5);

        fillMatrixWithValue(matr1, 10);
        fillMatrixWithValue(matr2, 15);
        fillMatrixWithValue(matr3, 25);
        assert(matr3 == matr1 + matr2);

        matr2 *= -1;
        assert(matr1 == matr3 + matr2);
    }

    std::cout << "TESTS OK";
    return 0;
}