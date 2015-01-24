///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

Copyright(c) 2014 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef __ETL_IVECTOR__
#define __ETL_IVECTOR__
#define __ETL_IN_IVECTOR_H__

#include <iterator>
#include <algorithm>
#include <functional>
#include <stddef.h>

#include "vector_base.h"
#include "type_traits.h"
#include "parameter_type.h"

#ifndef ETL_THROW_EXCEPTIONS
#include "error_handler.h"
#endif

namespace etl
{
  //***************************************************************************
  /// The base class for specifically sized vectors.
  /// Can be used as a reference type for all vectors containing a specific type.
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  class ivector : public vector_base
  {
  public:

    typedef T                                     value_type;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef T*                                    iterator;
    typedef const T*                              const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t                                size_type;
    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

  protected:

    typedef typename parameter_type<T>::type parameter_t;

  public:

    //*********************************************************************
    /// Returns an iterator to the beginning of the vector.
    ///\return An iterator to the beginning of the vector.
    //*********************************************************************
    iterator begin()
    {
      return &p_buffer[0];
    }
    
    //*********************************************************************
    /// Returns a const_iterator to the beginning of the vector.
    ///\return A const iterator to the beginning of the vector.
    //*********************************************************************
    const_iterator begin() const
    {
      return &p_buffer[0];
    }        

    //*********************************************************************
    /// Returns an iterator to the end of the vector.
    ///\return An iterator to the end of the vector.
    //*********************************************************************
    iterator end()
    {
      return &p_buffer[current_size];
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the vector.
    ///\return A const iterator to the end of the vector.
    //*********************************************************************
    const_iterator end() const
    {
      return &p_buffer[current_size];
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the vector.
    ///\return A const iterator to the beginning of the vector.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return &p_buffer[0];
    } 

    //*********************************************************************
    /// Returns a const_iterator to the end of the vector.
    ///\return A const iterator to the end of the vector.
    //*********************************************************************
    const_iterator cend() const
    {
      return &p_buffer[current_size];
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the vector.
    ///\return Iterator to the reverse beginning of the vector.
    //*********************************************************************
    reverse_iterator rbegin()
    {
	    return reverse_iterator(end());
    }
    
    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the vector.
    ///\return Const iterator to the reverse beginning of the vector.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
	    return const_reverse_iterator(end());
    }        

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the vector.
    ///\return Reverse iterator to the end + 1 of the vector.
    //*********************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(begin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the vector.
    ///\return Const reverse iterator to the end + 1 of the vector.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(begin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the vector.
    ///\return Const reverse iterator to the reverse beginning of the vector.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
	    return const_reverse_iterator(cend());
    } 

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the vector.
    ///\return Const reverse iterator to the end + 1 of the vector.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
	    return const_reverse_iterator(cbegin());
    }

    //*********************************************************************
    /// Resizes the vector.
    /// If ETL_THROW_EXCEPTIONS is defined and the new size is larger than the
    /// maximum then a vector_full is thrown.
    ///\param new_size The new size.
    //*********************************************************************
    void resize(size_t new_size)
    {
      if (new_size > MAX_SIZE)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw vector_full();
#else
        error_handler::error(vector_full());
#endif
      }

      // Size up or size down?
      if (new_size > current_size)
      {
        for (size_t i = current_size; i < new_size; ++i)
        {
          while (current_size < new_size)
          {
            create_element();
          }
        }
      }
      else if (new_size < current_size)
      {
        while (current_size > new_size)
        {
          destroy_element();
        }
      }
    }

    //*********************************************************************
    /// Resizes the vector.
    /// If ETL_THROW_EXCEPTIONS is defined and the new size is larger than the
    /// maximum then a vector_full is thrown.
    ///\param new_size The new size.
    ///\param value   The value to fill new elements with. Default = default contructed value.
    //*********************************************************************
    void resize(size_t new_size, T value)
    {
      if (new_size > MAX_SIZE)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw vector_full();
#else
        error_handler::error(vector_full());
#endif
      }

      // Size up?
	    if (new_size > current_size)
	    {
        while (current_size < new_size)
        {
          create_element(value);
        }
	    }
      // Size down?
      else if (new_size < current_size)
      {
        while (current_size > new_size)
        {
          destroy_element();
        }
      }
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference operator [](size_t i)
    {
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference operator [](size_t i) const
    {
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    /// If ETL_THROW_EXCEPTIONS is defined, emits an etl::vector_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference at(size_t i)
    {
      if (i >= current_size)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw vector_out_of_bounds();
#else
        error_handler::error(vector_out_of_bounds());
#endif
      }

      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    /// If ETL_THROW_EXCEPTIONS is defined, emits an etl::vector_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference at(size_t i) const
    {
      if (i >= current_size)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw vector_out_of_bounds();
#else
        error_handler::error(vector_out_of_bounds());
#endif
      }

      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a reference to the first element.
    ///\return A reference to the first element.
    //*********************************************************************
    reference front()
    {
      return p_buffer[0];
    }

    //*********************************************************************
    /// Returns a const reference to the first element.
    ///\return A const reference to the first element.
    //*********************************************************************
    const_reference front() const
    {
      return p_buffer[0];
    }

    //*********************************************************************
    /// Returns a reference to the last element.
    ///\return A reference to the last element.
    //*********************************************************************
    reference back()
    {
      return p_buffer[current_size - 1];
    }

    //*********************************************************************
    /// Returns a const reference to the last element.
    ///\return A const reference to the last element.
    //*********************************************************************
    const_reference back() const
    {
      return p_buffer[current_size - 1];
    }

    //*********************************************************************
    /// Returns a pointer to the beginning of the vector data.
    ///\return A pointer to the beginning of the vector data.
    //*********************************************************************
    pointer data()
    {
      return p_buffer;
    }

    //*********************************************************************
    /// Returns a const pointer to the beginning of the vector data.
    ///\return A const pointer to the beginning of the vector data.
    //*********************************************************************
    const_pointer data() const
    {
      return p_buffer;
    }

    //*********************************************************************
    /// Assigns values to the vector.
    /// If ETL_THROW_EXCEPTIONS is defined, emits vector_full if the vector does not have enough free space.
    /// If ETL_THROW_EXCEPTIONS is defined, emits vector_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
      initialise();

      difference_type count = std::distance(first, last);

      if (count < 0)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw vector_iterator();
#else
        error_handler::error((vector_iterator));
#endif
      }
      else if (static_cast<size_t>(count) > MAX_SIZE)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw vector_full();
#else
        error_handler::error(vector_full());
#endif
      }
      else
      {
        // Safe to copy.
        while (first != last)
        {
          create_element(*first);
          ++first;
        }
      }
    }

    //*********************************************************************
    /// Assigns values to the vector.
    /// If ETL_THROW_EXCEPTIONS is defined, emits vector_full if the vector does not have enough free space.
    ///\param n     The number of elements to add.
    ///\param value The value to insert for each element.
    //*********************************************************************
    void assign(size_t n, parameter_t value)
    {
      initialise();

      if (n > MAX_SIZE)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw vector_full();
#else
        error_handler::error(vector_full());
#endif
      }
      else
      {
        while (n > 0)
        {
          create_element(value);
          --n;
        }
      }
    }

    //*************************************************************************
    /// Clears the vector.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*************************************************************************
    /// Increases the size of the vector by one, but does not initialise the new element.
    /// If ETL_THROW_EXCEPTIONS is defined, throws a vector_full if the vector is already full.
    //*************************************************************************
    void push_back()
    {
#ifdef ETL_THROW_EXCEPTIONS
      if (current_size == MAX_SIZE)
      {
        throw vector_full();
      }
#else
      {
        error_handler::error(vector_full());
      }
#endif

      create_element();
    }

    //*********************************************************************
    /// Inserts a value at the end of the vector.
    /// If ETL_THROW_EXCEPTIONS is defined, emits vector_full if the vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    void push_back(parameter_t value)
    {
      if (current_size == MAX_SIZE)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw vector_full();
#else
        error_handler::error(vector_full());
#endif
      }
      else
      {
        create_element(value);
      }
    }

    //*************************************************************************
    /// Removes an element from the end of the vector.
    /// Does nothing if the vector is empty.
    //*************************************************************************
    void pop_back()
    {
      if (current_size > 0)
      {
        destroy_element();
      }
    }

    //*********************************************************************
    /// Inserts a value to the vector.
    /// If ETL_THROW_EXCEPTIONS is defined, emits vector_full if the vector is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, parameter_t value)
    {
      if (position == end())
      {
        push_back(value);
        return iterator(&back());
      }
      else
      {
        if ((current_size + 1) > MAX_SIZE)
        {
#ifdef ETL_THROW_EXCEPTIONS
          throw vector_full();
#else
          error_handler::error(vector_full());
#endif
        }
        else
        {
          resize(size() + 1);
          std::copy_backward(position, end() - 1, end());
          *position = value;
        }

        return position;
      }
    }

    //*********************************************************************
    /// Inserts 'n' values to the vector.
    /// If ETL_THROW_EXCEPTIONS is defined, emits vector_full if the vector does not have enough free space.
    ///\param position The position to insert at.
    ///\param n        The number of elements to add.
    ///\param value    The value to insert.
    //*********************************************************************
    void insert(iterator position, size_t n, parameter_t value)
    {
      if ((current_size + n) > MAX_SIZE)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw vector_full();
#else
        error_handler::error(vector_full());
#endif
      }
      else
      {
        resize(size() + n);
        std::copy_backward(position, end() - n, end());
        std::fill_n(position, n, value);
      }
    }

    //*********************************************************************
    /// Inserts a range of values to the vector.
    /// If ETL_THROW_EXCEPTIONS is defined, emits vector_full if the vector does not have enough free space.
    ///\param position The position to insert at.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <class TIterator>
    void insert(iterator position, TIterator first, TIterator last)
    {
      size_t count = std::distance(first, last);

      if ((current_size + count) > MAX_SIZE)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw vector_full();
#else
        error_handler::error(vector_full());
#endif
      }
      else
      {
        resize(size() + count);
        std::copy_backward(position, end() - count, end());
        std::copy(first, first + count, position);
      }
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator i_element)
    {
      std::copy(i_element + 1, end(), i_element);
      destroy_element();

      return i_element;
    }

    //*********************************************************************
    /// Erases a range of elements.
    /// The range includes all the elements between first and last, including the 
    /// element pointed by first, but not the one pointed by last.
    ///\param first Iterator to the first element.
    ///\param last  Iterator to the last element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator first, iterator last)
    {
      std::copy(last, end(), first);
      size_t new_size = current_size - std::distance(first, last);
      resize(new_size);

      return first;
    }

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    ivector(T* p_buffer, size_t MAX_SIZE)
      : vector_base(MAX_SIZE),
        p_buffer(p_buffer)
    {
      initialise();
    }

  private:

    //*********************************************************************
    /// Initialise the vector.
    //*********************************************************************
    void initialise()
    {
      while (current_size > 0)
      {
        destroy_element();
      }
    }

    //*********************************************************************
    /// Create a new element with a default value at the back.
    //*********************************************************************
    void create_element()
    {
      new(&p_buffer[current_size++]) T();
    }

    //*********************************************************************
    /// Create a new element with a value at the back
    //*********************************************************************
    void create_element(parameter_t value)
    {
      new(&p_buffer[current_size++]) T(value);
    }

    //*********************************************************************
    /// Destroy an element at the back.
    //*********************************************************************
    void destroy_element()
    {
      p_buffer[--current_size].~T();
    }

    T* p_buffer;
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator ==(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator !=(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Less than operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexigraphically less than the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator <(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  //***************************************************************************
  /// Greater than operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexigraphically greater than the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator >(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::greater<T>());
  }

  //***************************************************************************
  /// Less than or equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexigraphically less than or equal to the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator <=(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return !operator >(lhs, rhs);
  }

  //***************************************************************************
  /// Greater than or equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexigraphically greater than or equal to the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator >=(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return !operator <(lhs, rhs);
  }
}

#undef __ETL_IN_IVECTOR_H__
#endif
