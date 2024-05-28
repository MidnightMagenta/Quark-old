#ifndef QRK_MATRIX
#define QRK_MATRIX

#include <iostream>
#include <array>
#include <vector>

///////////////////////////////////////////////////////////////////////////
// Row major matrix library with addition, subtraction, and multiplication.
// 
// To define a new matrix type:
// typedef Matrix<type, columns, rows> name;
// 
// or directly use the matrix class:
// Matrix<type, columns, rows> name(*matrix data*)
///////////////////////////////////////////////////////////////////////////
namespace qrk
{
	//main matrix class
	template<typename mat_type, size_t columns, size_t rows>
	class Matrix
	{
	public:
		//constructors
		Matrix() {};
		Matrix(const std::array<std::array<mat_type, rows>, columns>& _matrix) : data(_matrix) {};
		Matrix(const std::vector<std::vector<mat_type>>& _matrix) 
		{
			if (_matrix.size() == this->data.size() && _matrix[0].size() == this->data[0].size())
			{
				for (int i = 0; i < this->data.size(); i++)
					for (int j = 0; j < this->data[0].size(); j++) this->data[i][j] = _matrix[i][j];
			}
			else std::cout << "could not copy vector, incompatible size. Size of this: " << this->data.size() << " " << this->data[0].size() 
				<< " Size of copied: " << _matrix.size() << " " << _matrix[0].size() << std::endl;
		};

		//operator definitions
		Matrix operator+ (mat_type& scalar)
		{
			for (int i = 0; i < this->data.size(); i++)
				for (int j = 0; j < this->data[0].size(); j++) this->data[i][j] += scalar;
			return *this;
		}
		Matrix operator+ (Matrix& other)
		{
			if (this->data.size() != other.data.size()) return *this;
			if (this->data[0].size() != other.data[0].size()) return *this;

			for (int i = 0; i < this->data.size(); i++)
				for (int j = 0; j < this->data[0].size(); j++) this->data[i][j] += other.data[i][j];
			return *this;
		}
		Matrix operator- (mat_type& scalar)
		{
			for (int i = 0; i < this->data.size(); i++)
				for (int j = 0; j < this->data[0].size(); j++) this->data[i][j] -= scalar;
			return *this;
		}
		Matrix operator- (Matrix& other)
		{
			if (this->data.size() != other.data.size()) return *this;
			if (this->data[0].size() != other.data[0].size()) return *this;

			for (int i = 0; i < this->data.size(); i++)
				for (int j = 0; j < this->data[0].size(); j++) this->data[i][j] -= other.data[i][j];
			return *this;
		}
		Matrix operator* (mat_type& scalar)
		{
			for (int i = 0; i < this->data.size(); i++)
				for (int j = 0; j < this->data[0].size(); j++) this->data[i][j] *= scalar;
		}
		Matrix operator* (Matrix& other)
		{
			if (this->data[0].size() != other.data.size()) return *this;
			std::vector<std::vector<mat_type>> result (this->data[0].size(), std::vector<mat_type>(other.data.size(), 0));
			for (int i = 0; i < this->data.size(); i++)
				for (int j = 0; j < other.data[0].size(); j++)
					for (int k = 0; k < this->data.size(); k++) result[i][j] += this->data[i][k] * other.data[k][j];
			return result;
		}
		Matrix operator= (Matrix& other)
		{
			if (this->data.size() != other.data.size()) return *this;
			if (this->data[0].size() != other.data[0].size()) return *this;

			for (int i = 0; i < this->data.size(); i++)
				for (int j = 0; j < this->data[0].size(); j++) this->data[i][j] = other.data[i][j];
			return *this;
		}

		//transpose functions
		Matrix TransposeMatrix()
		{
			std::array<std::array<mat_type, columns>, rows> result;
			for (int i = 0; i < this->data.size(); i++)
				for (int j = 0; j < this->data[0].size(); j++) result[i][j] = data[j][i];
			return Matrix(result);
		}
		
// 		matrix data storage (64 bytes for a 4x4 matrix)
//		columns ↓	rows ↓
		std::array<std::array<mat_type, columns>, rows> data;
	};

	//type definitions for common matrices
	typedef Matrix<float, 2, 2> mat2;
	typedef Matrix<float, 3, 3> mat3;
	typedef Matrix<float, 4, 4> mat4;

	typedef Matrix<float, 2, 1> mat2x1;
	typedef Matrix<float, 2, 3> mat2x2;
	typedef Matrix<float, 2, 4> mat2x4;

	typedef Matrix<float, 3, 1> mat3x1;
	typedef Matrix<float, 3, 3> mat3x2;
	typedef Matrix<float, 3, 4> mat3x4;

	typedef Matrix<float, 4, 1> mat4x1;
	typedef Matrix<float, 4, 3> mat4x2;
	typedef Matrix<float, 4, 4> mat4x4;

	/// <summary>
	/// Create a GL translation matrix
	/// </summary>
	/// <param name="x"> - X coordinate</param>
	/// <param name="y"> - Y coordinate</param>
	/// <param name="z"> - Z coordinate</param>
	/// <returns>4x4 translation matrix</returns>
	inline mat4 CreateTranslationMatrix(float x, float y, float z)
	{
		return mat4(
			{
				{1, 0, 0, x},
				{0, 1, 0, y},
				{0, 0, 1, z},
				{0, 0, 0, 1}
			});
	}
	inline mat4 CreateScaleMatrix(float x, float y, float z)
	{
		return mat4(
			{
				{x, 0, 0, 0},
				{0, y, 0, 0},
				{0, 0, z, 0},
				{0, 0, 0, 1}
			});
	}

	/// <summary>
	/// Create a GL rotation matrix
	/// </summary>
	/// <param name="x"> - X angle</param>
	/// <param name="y"> - Y angle</param>
	/// <param name="z"> - Z angle</param>
	/// <returns>4x4 rotation matrix</returns>
	inline mat4 CreateRotationMatrix(float x, float y, float z)
	{
		mat4 rotationX( {
				{1, 0, 0, 0},
				{0, std::cos(x), -std::sin(x), 0},
				{0, std::sin(x), std::cos(x), 0},
				{0, 0, 0, 1}
			});
		mat4 rotationY({
				{std::cos(y), 0, -std::sin(y), 0},
				{0, 1, 0, 0},
				{std::sin(y), 0, std::cos(y), 0},
				{0, 0, 0, 1}
			});
		mat4 rotationZ({
				{std::cos(z), -std::sin(z), 0, 0},
				{std::sin(z), std::cos(z), 0, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 1}
			});
		return rotationZ * rotationY * rotationX;
	}

	/// <summary>
	/// Returns an orthographic projection GL matrix
	/// </summary>
	/// <param name="left"> - left bounding coordinate</param>
	/// <param name="right"> - right bounding coordinate</param>
	/// <param name="top"> - top bounding coordinate</param>
	/// <param name="bottom"> - bottom bounding coordinate</param>
	/// <param name="near"> - near bounding coordinate</param>
	/// <param name="far"> - far bounding coordinate</param>
	/// <returns>4x4 orthographic projection matrix</returns>
	inline mat4 CreateOrthographicProjectionMatrix(float left, float right, float top, float bottom, float _near, float _far)
	{
		return mat4({
				{(2 / (right - left)), 0, 0, ((right + left) / (right - left))},
				{0,	(2 / (top - bottom)), 0, ((top + bottom) / (top - bottom))},
				{0,	0, (2 / (_far - _near)), ((_far + _near) / (_far - _near))},
				{0,	0, 0, 1}
			});
	}

	/// <summary>
	/// Create a perspective projection matrix
	/// </summary>
	/// <param name="fov"> - fielf of view</param>
	/// <param name="aspect"> - aspect ratio of the frustum</param>
	/// <param name="near"> - near bounding coordinate</param>
	/// <param name="far"> - far bounding coordinate</param>
	/// <returns>4x4 perspective projection matrix</returns>
	inline mat4 CreatePerspectiveProjectionMatrix(float fov, float aspect, float _near, float _far)
	{
		return mat4({
				{(2 * _near / (aspect * std::tan(fov / 2))),	0, 0, 0},
				{0,	(2 * _near / std::tan(fov / 2)),	0, 0},
				{0,	0, (-(_far + _near) / (_far - _near)), (-(2 * _far * _near) / (_far - _near))},
				{0, 0, -1, 0}
			});
	}
}
#endif // !QRK_MATRIX