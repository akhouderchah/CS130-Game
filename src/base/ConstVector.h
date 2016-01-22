#pragma once
#include <vector>

/**
 * @brief Redefines a limited interface with the std::vector class.
 *
 * Conceptually, this defines a constant vector that may or may not
 * contain const elements. That is, the vector itself can't be changed
 * (no adding or removing elements, etc), but elements can be accessed
 * if they are not also of a const type.
 */
template <class T>
class ConstVector
{
public:
	ConstVector() : m_pVector(nullptr){}
	~ConstVector(){}
	ConstVector(std::vector<T>& vector) : m_pVector(&vector){}
	ConstVector& operator=(const ConstVector& that){ m_pVector = that.m_pVector; return *this; }

	T& at(std::size_t index){return m_pVector->at(index);}
	T& operator[](std::size_t index){return (*m_pVector)[index];}

	std::size_t size(){ return m_pVector->size(); }

private:
	template <class X>
	operator std::vector<X>(); // User cannot cast back to a regular vector

private:
	std::vector<T>* m_pVector;
};

