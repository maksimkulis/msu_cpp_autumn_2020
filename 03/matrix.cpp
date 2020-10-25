#include "matrix.h"

Row::Row(size_t size)
    : size_(size)
{
    buff_ = new int[size];
}

Row::~Row()
{
    delete[] buff_;
}

int &Row::operator[](size_t ind)
{
    if (ind >= size_)
    {
        throw std::out_of_range("");
    }
    return buff_[ind];
}

int Row::operator[](size_t ind) const
{
    if (ind >= size_)
    {
        throw std::out_of_range("");
    }
    return buff_[ind];
}

Matrix::Matrix(size_t rows, size_t columns)
    : rows_(rows), columns_(columns)
{
    data_ = static_cast<Row *>(malloc(rows * sizeof(Row)));
    for (size_t i = 0; i < rows_; ++i)
    {
        new (data_ + i) Row(columns_);
    }
}

Matrix::~Matrix()
{
    for (size_t i = 0; i < rows_; ++i)
    {
        data_[i].~Row();
    }
    free(data_);
}

Row &Matrix::operator[](size_t row)
{
    if (row >= rows_)
    {
        throw std::out_of_range("");
    }
    return data_[row];
}

Row Matrix::operator[](size_t row) const
{
    if (row >= rows_)
    {
        throw std::out_of_range("");
    }
    return data_[row];
}

size_t Matrix::getRows() const
{
    return rows_;
}

size_t Matrix::getColumns() const
{
    return columns_;
}

void Matrix::operator*=(int value)
{
    for (size_t i = 0; i < rows_; ++i)
    {
        for (size_t j = 0; j < columns_; ++j)
        {
            data_[i][j] *= value;
        }
    }
}

Matrix Matrix::operator+(const Matrix &other) const
{
    if ((columns_ != other.columns_) || (rows_ != other.rows_))
    {
        throw std::logic_error("Sum of matrix with different sizes");
    }
    Matrix matrix(rows_, columns_);
    for (size_t i = 0; i < rows_; ++i)
    {
        for (size_t j = 0; j < columns_; ++j)
        {
            matrix.data_[i][j] = data_[i][j] + other.data_[i][j];
        }
    }
    return matrix;
}

bool Matrix::operator==(const Matrix &other) const
{
    for (size_t i = 0; i < rows_; ++i)
    {
        for (size_t j = 0; j < columns_; ++j)
        {
            if (data_[i][j] != other.data_[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &other) const
{
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Matrix &matrix)
{
    os << "[";
    for (size_t i = 0; i < matrix.rows_; ++i)
    {
        os << "[";
        for (size_t j = 0; j < matrix.columns_; ++j)
        {
            os << matrix.data_[i][j];
            if (j < matrix.columns_ - 1)
            {
                os << ", ";
            }
        }
        os << "]";
        if (i < matrix.rows_ - 1)
        {
            os << std::endl;
        }
    }
    os << "]";
}