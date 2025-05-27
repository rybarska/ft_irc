/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RingBuffer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:42:59 by arybarsk          #+#    #+#             */
/*   Updated: 2025/05/21 16:43:01 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <iostream>
#include <string>

template <typename T, int N>
class RingBuffer
{
	public:
		RingBuffer() :  _tail(0), _head(0), _full(false)
		{};
		
		~RingBuffer()
		{};
		
		bool addElem(const T &elem)
		{
			_buffer[_head] = elem;
			if (_full)
				_tail = (_tail + 1) % N;
			_head = (_head + 1) % N;
			_full = (_head == _tail);
			return true;
		}
		
		bool getElem(T &elem)
		{
			if (isEmpty())
				return false;
			elem = _buffer[_tail];
			_tail = (_tail + 1) % N;
			_full = false;
			return true;
		}
		
		void clearBuffer()
		{
			_head = _tail = 0;
			_full = false;
		};
		
		bool isEmpty() const
		{
			if (!_full && _head == _tail)
				return true;
			return false;
		}
		
		std::size_t getSize() const
		{
			if (_full)
				return N;
			if (_head >= _tail)
				return (_head - _tail);
			return (N + _head - _tail);
		}
		
	private:
		T _buffer[N + 1];
		std::size_t _tail;
		std::size_t _head;
		bool _full;
		
		RingBuffer(const RingBuffer &source);
		RingBuffer & operator = (const RingBuffer &source);
};

#endif
