#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <exception>    // std::out_of_range
#include <iostream>     // std::cout, std::endl
#include <memory>       // std::unique_ptr
#include <iterator>     // std::advance, std::begin(), std::end(), std::ostream_iterator
#include <algorithm>    // std::copy, std::equal, std::fill
#include <initializer_list> // std::initializer_list
#include <cassert>      // assert()
#include <limits>       // std::numeric_limits<T>
#include <cstddef>      // std::size_t
#include <sstream>      // std::ostringstream

/// Sequence container namespace.
namespace sc {
    /// Implements tha infrastructure to support a bidirectional iterator.
    template <class T>
    class MyForwardIterator : public std::iterator<std::bidirectional_iterator_tag, T>
    {
        public:
            typedef MyForwardIterator self_type;   //!< Alias to iterator.
            // Below we have the iterator_traits common interface
            typedef std::ptrdiff_t difference_type; //!< Difference type used to calculated distance between iterators.
            typedef T value_type;           //!< Value type the iterator points to.
            typedef T* pointer;             //!< Pointer to the value type.
            typedef T& reference;           //!< Reference to the value type.
            typedef const T& const_reference;           //!< Reference to the value type.
            typedef std::bidirectional_iterator_tag iterator_category; //!< Iterator category.

            // Constructors.
            MyForwardIterator(pointer pt_ = nullptr) : m_ptr{pt_} {} // Regular
            MyForwardIterator(const self_type& other) : m_ptr{other.m_ptr} {} // Copy
            self_type& operator=(const self_type& other) { // Assignment Operator
                m_ptr = other.m_ptr;
                return *this;
            }
            // Destructor.
            ~MyForwardIterator(void) = default;
            
            //== Sobrecarga de operadores
            // Necessário para oferecer uma interface pública
            // parecida com a que utilizamos com ponteiro regular.

            //* Increment operator.
            // ++i will increment the value of i, and then return the incremented value.
            // i = 1;
            // j = ++i;
            // (i is 2, j is 2)
            self_type& operator++(void) { // ++it;
                m_ptr++;
                return *this;
            }
            // i++ will increment the value of i, but return the original value that i held before being incremented.
            // i = 1;
            // j = i++;
            // (i is 2, j is 1)
            self_type operator++(int) { // it++;
                self_type retval{*this};
                m_ptr++;
                return retval;
            }
            //* Decrement operator.
            self_type& operator--(void) { // --it;
                m_ptr--;
                return *this;
            }
            self_type operator--(int) { // it--;
                self_type retval{*this};
                m_ptr--;
                return retval;
            }
            //* Dereference operator.
            reference operator*(void) const { // (*it);
                return *m_ptr;
            }
            pointer operator&(void) const { // &it
                return m_ptr;
            }
            difference_type operator-( self_type it ) {
                return m_ptr - it.m_ptr;
            }
            //* Jump operators.
            friend self_type operator+( difference_type n, self_type it ) { // 2+it
                return self_type{n + it.m_ptr};
            }
            friend self_type operator+( self_type it, difference_type n ) { // it+2
                return self_type{it.m_ptr + n};
            }
            friend self_type operator-( self_type it, difference_type n ) { // it-2
                return self_type{it.m_ptr - n};
            }
            friend self_type operator-( difference_type n, self_type it ) { // 2-it
                return self_type{n - it.m_ptr};
            }
            //* Equality/difference operators.
            bool operator==(const self_type& other) const  { // it1 == it2
                return m_ptr==other.m_ptr;
            }
            bool operator!=(const self_type& other) const { // it1 != it2
                return !(m_ptr==other.m_ptr);
            }



        private:
            pointer m_ptr; //!< The raw pointer.
    };

    /// This class implements the ADT list with dynamic array.
    /*!
     * sc::vector is a sequence container that encapsulates dynamic size arrays.
     *
     * The elements are stored contiguously, which means that elements can
     * be accessed not only through iterators, but also using offsets to
     * regular pointers to elements.
     * This means that a pointer to an element of a vector may be passed to
     * any function that expects a pointer to an element of an array.
     *
     * \tparam T The type of the elements.
     */
    template <typename T>
    class vector
    {
        //=== Aliases
        public:
            using size_type = unsigned long; //!< The size type.
            using value_type = T;            //!< The value type.
            using pointer = value_type*;     //!< Pointer to a value stored in the container.
            using reference = value_type&;   //!< Reference to a value stored in the container.
            using const_reference = const value_type&; //!< Const reference to a value stored in the container.

            using iterator = MyForwardIterator<value_type>; //!< The iterator, instantiated from a template class.
            using const_iterator = MyForwardIterator<const value_type>; //!< The const_iterator, instantiated from a template class.

        public:
            //!=== [I] Special members
			//* (1)/(2) Main constructor that initializes the vector with the requested capacity.
            explicit vector(size_type new_cap = 0)
				: m_capacity{new_cap},
				  m_end{new_cap},
				  m_storage{new T[m_capacity]}
			{
                // Let us fill the vector with instances of default-initialized objects.
                std::fill(m_storage, m_storage+m_capacity, T{});
            }

            //* (3) Main constructor that initializes the vector with the contents of a range [first, last).
            template <typename InputItr>
            vector(InputItr first, InputItr last) {
                size_type sz = last - first;
                m_capacity = sz;
                m_end = sz;
                m_storage = new T[m_capacity];

                // Copy all elements from the range to the vector.
                std::copy(first, last, this->begin());
            }

            //* (4) Copy constructor. Construct the vector from another vector by copying the elements.
            vector(const vector &other)
                : m_capacity{other.m_capacity},
                  m_end{other.m_end},
                  m_storage{new T[m_capacity]}
            {
                // Let us fill the array with instances of default-initialized objects.
                std::copy(other.cbegin(), other.cend(), this->begin());
            }

            //* (5) Main constructor that initializes the vector from an initializer list.
            vector(const std::initializer_list<T> &il)
                : m_capacity{ il.size() },
                  m_end { il.size() },
                  m_storage{new T[m_capacity]}
            {
                // Copy all elements from the initializer list into the vector storage area.
                std::copy(il.begin(), il.end(), m_storage);
            }
            
            //* (6) Destructor of the vector.
            virtual ~vector(void) { delete[] m_storage; }

            //* (7) Copy assignment operator. Replaces the contents with a copy of the contents of other.
            vector &operator=(const vector &other)
            {
                if (this != &other) {
                    if (m_capacity < other.m_end) {
                        delete[] m_storage;
                        m_storage = new T[other.m_end];
                    }
                    // Copy all elements from the initializer list into the vector storage area.
                    std::copy(other.m_storage, other.m_storage + other.m_end, m_storage);
                }
                m_capacity = m_end = other.m_end;

                return *this;
            }

            //* (8) Replaces the contents with those identified by initializer list ilist.
            vector &operator=(std::initializer_list<T> il)
            {
                if (m_capacity < il.size()) {
                    delete[] m_storage;
                    m_storage = new T[il.size()];
                }
                // Copy all elements from the initializer list into the vector storage area.
                std::copy(il.begin(), il.end(), m_storage);
                m_capacity = m_end = il.size();

                return *this;
            }

            //!=== [II] Iterators
            //? Conferir se o elemento existe.
            //* An iterator pointing to the first item in the list.
            iterator begin(void) { return iterator(&m_storage[0]); }

            //* A constant iterator pointing to the first item in the list.
            const_iterator cbegin(void) const { return const_iterator(&m_storage[0]); }

            //* An iterator pointing to the position just after the last element of the list.
            iterator end(void) { return iterator(&m_storage[m_end]); }

            //* A constant iterator pointing to the position just after the last element of the list.
            const_iterator cend(void) const { return const_iterator(&m_storage[m_end]); }

            //!=== [III] Capacity
            //* Check the size of the vector.
            size_type size(void) const { return m_end; }

            //* Check the capacity of the vector.
            size_type capacity(void) const { return m_capacity; }

            //* Check if the vector is empty, that is, there are no elements.
            bool empty(void) const { return m_end == 0; }
            
            //!=== [IV] Modifiers
            //* Removes all elements from the container.
            void clear(void) { m_end = 0; }

            //* Adds value to the end of the list.
            void push_back(const_reference value)
            {
                // Verify if has space for a new element.
                if (full())  {
                    size_type new_capacity{m_capacity};
                    if (m_capacity == 0) new_capacity++;
                    else new_capacity *= 2;
                    reserve(new_capacity);
                }
                // Realize the insertion.
                m_storage[m_end] = value;
                m_end++;
            }
            
            //* Removes the object at the end of the list.
            void pop_back(void) 
            { 
                // Verify if has space for a new element.
                if (empty())
                    throw std::length_error("[vector::pop_back()]: Can not remove an element from an empty vector.");
                // Remove the element of the range.
                m_end--;
            }

            iterator insert( iterator pos_ , const_reference value_ ) {
                auto position = pos_ - begin();
                // Verify if has space for a new element.
                if (full())  {
                    size_type new_capacity{m_capacity};
                    if (m_capacity == 0) new_capacity++;
                    else new_capacity *= 2;
                    reserve(new_capacity);
                }
                auto old_m_end = m_end;
                m_end++;

                auto old_element = old_m_end - 1;
                auto new_element = m_end - 1;
                while (old_element >= position) {
                    m_storage[new_element] = m_storage[old_element];
                    if (old_element != 0) {
                        new_element--;
                        old_element--;
                    }
                    else {
                        break;
                    }
                }

                m_storage[position] = value_;
                
                return iterator(this->begin() + position);
            }

            iterator insert( const_iterator pos_ , const_reference value_ ) {
                auto position = pos_ - begin();
                // Verify if has space for a new element.
                if (full())  {
                    size_type new_capacity{m_capacity};
                    if (m_capacity == 0) new_capacity++;
                    else new_capacity *= 2;
                    reserve(new_capacity);
                }
                auto old_m_end = m_end;
                m_end++;

                auto old_element = old_m_end - 1;
                auto new_element = m_end - 1;
                while (old_element >= position) {
                    m_storage[new_element] = m_storage[old_element];
                    if (old_element != 0) {
                        new_element--;
                        old_element--;
                    }
                    else {
                        break;
                    }
                }

                m_storage[position] = value_;
                
                return iterator(this->begin() + position);
            }

            template <typename InputItr>
            iterator insert( iterator pos_ , InputItr first_, InputItr last_ ) {
                auto size_range = last_ - first_;
                auto position = pos_ - begin();
                if (capacity() + size_range > m_capacity) {
                    size_type new_capacity{m_capacity + size_range};
                    new_capacity *= 2;
                    reserve(new_capacity);
                }
                auto old_m_end = m_end;
                m_end += size_range;

                auto old_element = old_m_end - 1;
                auto new_element = m_end - 1;
                while (old_element >= position) {
                    m_storage[new_element] = m_storage[old_element];
                    if (old_element != 0) {
                        new_element--;
                        old_element--;
                    }
                    else {
                        break;
                    }
                }

                for (auto i{position}; i < (position + size_range); i++) {
                    m_storage[i] = *first_;
                    first_++;
                }
                
                return iterator(this->begin() + position);
            }

            template <typename InputItr>
            iterator insert( const_iterator pos_ , InputItr first_, InputItr last_ ) {
                auto size_range = last_ - first_;
                auto position = pos_ - begin();
                if (capacity() + size_range > m_capacity) {
                    size_type new_capacity{m_capacity + size_range};
                    new_capacity *= 2;
                    reserve(new_capacity);
                }
                auto old_m_end = m_end;
                m_end += size_range;

                auto old_element = old_m_end - 1;
                auto new_element = m_end - 1;
                while (old_element >= position) {
                    m_storage[new_element] = m_storage[old_element];
                    if (old_element != 0) {
                        new_element--;
                        old_element--;
                    }
                    else {
                        break;
                    }
                }

                for (auto i{position}; i < (position + size_range); i++) {
                    m_storage[i] = *first_;
                    first_++;
                }
                
                return iterator(this->begin() + position);
            }

            iterator insert( iterator pos_, const std::initializer_list< value_type >& ilist_ ) {
                auto size_list = ilist_.size();
                auto position = pos_ - begin();
                if (capacity() + size_list > m_capacity) {
                    size_type new_capacity{m_capacity + size_list};
                    new_capacity *= 2;
                    reserve(new_capacity);
                }
                auto old_m_end = m_end;
                m_end += size_list;

                auto old_element = old_m_end - 1;
                auto new_element = m_end - 1;
                while (old_element >= position) {
                    m_storage[new_element] = m_storage[old_element];
                    if (old_element != 0) {
                        new_element--;
                        old_element--;
                    }
                    else {
                        break;
                    }
                }

                auto list_element = ilist_.begin();
                for (auto i{position}; i < (position + size_list); i++) {
                    m_storage[i] = *list_element;
                    list_element++;
                }
                
                return iterator(this->begin() + position);
            }

            iterator insert( const_iterator pos_, const std::initializer_list< value_type >& ilist_ ) {
                auto size_list = ilist_.size();
                auto position = pos_ - begin();
                if (capacity() + size_list > m_capacity) {
                    size_type new_capacity{m_capacity + size_list};
                    new_capacity *= 2;
                    reserve(new_capacity);
                }
                auto old_m_end = m_end;
                m_end += size_list;

                auto old_element = old_m_end - 1;
                auto new_element = m_end - 1;
                while (old_element >= position) {
                    m_storage[new_element] = m_storage[old_element];
                    if (old_element != 0) {
                        new_element--;
                        old_element--;
                    }
                    else {
                        break;
                    }
                }

                auto list_element = ilist_.begin();
                for (auto i{position}; i < (position + size_list); i++) {
                    m_storage[i] = *list_element;
                    list_element++;
                }
                
                return iterator(this->begin() + position);
            }

            //* The storage will have a capacity equal to cap_ if cap_ > m_capacity.
            void reserve(size_type cap_)
            {
                if (cap_ > m_capacity) {
                    // Realloc the storage.
                    T *newVec{new T[cap_]};
                    std:copy(this->begin(), this->end(), newVec);
                    // Update storage attributes.
                    delete[] m_storage;
                    m_storage = newVec;
                    m_capacity = cap_;
                }
            }

            //* Requests the removal of unused capacity. Reduce capacity() to size().
            void shrink_to_fit(void)
            {
                if (m_end < m_capacity) {
                    T *newVec{new T[m_end]};
                    std:copy(this->begin(), this->end(), newVec);
                    // Update storage.
                    delete[] m_storage;
                    m_storage = newVec;
                    m_capacity = m_end;
                }
            }

            //* Assigns new contents to the vector, replacing its current contents, and modifying its size accordingly.
            //* Replaces the content of the vector with count copies of value.
            void assign(size_type count_, const_reference value_)
            {
                if (m_capacity < count_) {
                    T *newVec{new T[count_]};
                    // Update storage.
                    delete[] m_storage;
                    m_storage = newVec;
                    m_capacity = count_;
                }
                // Update size.
                m_end = count_;
                // Set elements into the vector.
                std::fill(this->begin(), this->end(), value_);
            }

            //* Replaces the content of the vector with copy of the initializer list.
            void assign(const std::initializer_list<T>& il)
            {
                if (m_capacity < il.size()) {
                    T *newVec{new T[il.size()]};
                    // Update storage.
                    delete[] m_storage;
                    m_storage = newVec;
                    m_capacity = il.size();
                }
                // Copy all elements from the initializer list into the vector storage area.
                std::copy(il.begin(), il.end(), m_storage);
                // Update size.
                m_end = il.size();
            }

            //* Replaces the content of the vector with copy of a range.
            template <typename InputItr>
            void assign(InputItr first, InputItr last)
            {
                size_type sz = last - first;
                if (m_capacity < sz) {
                    T *newVec{new T[sz]};
                    // Update storage.
                    delete[] m_storage;
                    m_storage = newVec;
                    m_capacity = sz;
                }
                // Copy all elements from the range into the vector storage area.
                std::copy(first, last, m_storage);
                // Update size.
                m_end = sz;
            }


            iterator erase(const_iterator first, const_iterator last) {
                const_iterator old_pos{last};
                const_iterator new_pos{first};
                while (old_pos != end()) {
                    std::swap(*new_pos, *old_pos);
                    old_pos++;
                    new_pos++;
                }
                m_end -= std::distance( first, last );
                return first;
            };

            iterator erase(iterator first, iterator last) {
                iterator old_pos{last};
                iterator new_pos{first};
                while (old_pos != end()) {
                    std::swap(*new_pos, *old_pos);
                    old_pos++;
                    new_pos++;
                }
                m_end -= std::distance( first, last );
                return first;
            };

            iterator erase(const_iterator pos) {
                const_iterator i{pos};
                while (i != end()-1) {
                    std::swap(*i, *(i+1));
                    i++;
                }
                m_end--;
                return pos;
            }

            iterator erase(iterator pos) {
                iterator i{pos};
                while (i != end()-1) {
                    std::swap(*i, *(i+1));
                    i++;
                }
                m_end--;
                return pos;
            };

            //!=== [V] Element access
            //* Returns the element at the end of the list, just to read.
            const_reference back(void) const 
            {
                // I can not return an element of an empty vector.
                if (empty())
                    throw std::length_error("[vector::back()]: empty vector.");
                // There is at least one element in the vector.
                return m_storage[m_end - 1];
            }

            //* Returns the element at the beginning of the list, just to read.
            const_reference front(void) const 
            {
                // I can not return an element of an empty vector.
                if (empty())
                    throw std::length_error("[vector::front()]: empty vector.");
                // There is at least one element in the vector.
                return m_storage[0];
            }

            //* Returns a reference of the element at the end of the list.
            reference back(void) { return m_storage[m_end - 1]; }

            //* Returns a reference of the element at the beginning of the list.
            reference front(void) { return m_storage[0]; }

            //* Access the element in the position pos, just to read.
            const_reference operator[](size_type pos) const { return m_storage[pos]; }

            //* Access the element in the position pos, can change the value.
            // A[i] = x; // A.operator[](i);
            reference operator[](size_type pos) { return m_storage[pos]; }

            //* Returns the value at the index pos_ in the vector, with bounds-checking.
            //* Just to read the value.
            const_reference at(size_type pos) const
            {
                if (pos < 0 or pos >= m_end)
                    throw std::out_of_range(
                        "[T array::at(pos)]: position provided is out of vector range");
                return m_storage[pos];
            }

            //* Returns the element of the index pos_ in the vector, with bounds-checking.
            //* Can change the value of the element.
            reference at(size_type pos)
            {
                if (pos < 0 or pos >= m_end)
                    throw std::out_of_range(
                        "[T array::at(pos)]: position provided is out of vector range");
                return m_storage[pos];
            }

            //!=== [VII] Friend functions
            friend std::ostream & operator<<(std::ostream & os_, const vector<T> & v_)
            {
                // What do I want to print???
                os_ << "{ ";
                for( auto i{0u} ; i < v_.m_capacity ; ++i )
                {
                    if ( i == v_.m_end ) os_ << "| ";
                    os_ << v_.m_storage[ i ] << " ";
                }
                os_ << "}, m_end=" << v_.m_end << ", m_capacity=" << v_.m_capacity;

                return os_;
            }
            friend void swap( vector<T> & first_, vector<T> & second_ )
            {
                // enable ADL
                using std::swap;

                // Swap each member of the class.
                swap( first_.m_end,      second_.m_end      );
                swap( first_.m_capacity, second_.m_capacity );
                swap( first_.m_storage,  second_.m_storage  );
            }

            //======================================================================
            //== Métodos para ajudar na depuração
            //----------------------------------------------------------------------
            //* This method builds and returns a string representation for an array.
            std::string to_string(void) const{
                std::ostringstream oss; // String-fluxo de saída (escrita).
                size_t i{0};

                oss << "[ ";
                for ( /*empty*/ ; i < m_end ; ++i )
                    oss << m_storage[i] << " ";
                oss << "| ";
                for ( /*empty*/ ; i < m_capacity ; ++i )
                    oss << m_storage[i] << " ";
                oss << "], end = " << m_end << ", capacity = " << m_capacity;

                // [ 1 2 3 ]
                return oss.str();
            }
            
            //* For debugging purposes, if you are using std::unique_ptr.
            pointer data(void) { return m_storage; };
            const_reference data(void) const { return m_storage; };

        private:
            //* Check if the maximum capacity has been reached.
            bool full(void) const { return m_end == m_capacity; }

            size_type m_end;                //!< The list's current size (or index past-last valid element).
            size_type m_capacity;           //!< The list's storage capacity.
            // std::unique_ptr<T[]> m_storage; //!< The list's data storage area.
            T *m_storage;                   //!< The list's data storage area.
    };

    //!=== [VI] Operators
    //* Checks if the contents of lhs and rhs are equal.
    //* Same size and equal values in the same positions.
    template <typename T>
    bool operator==(const vector<T> &lhs, const vector<T> &rhs)
	{
		if (lhs.size() != rhs.size())
			return false;
		for (int i{0}; i < lhs.size(); i++)
			if (lhs[i] != rhs[i])
				return false;
		return true;
	}

    //* The negation of the above operation, the opposite result.
    template <typename T>
    bool operator!=(const vector<T> &lhs, const vector<T> &rhs)
	{
		if (not (lhs == rhs))
			return true;
		return false;
	}

} // namespace sc.
#endif
