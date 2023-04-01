#pragma once
#include "List.h"

template <typename T>
class Node
{
private:
	T data;
	Node<T>* prev = nullptr;
	Node<T>* next = nullptr;

	Node<T>(const T& data, Node<T>* prev, Node<T>* next) : data(data), prev(prev), next(next) {}

	template <typename T>
	friend class List;
};
