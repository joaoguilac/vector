#include <iostream>

#include "../include/vector.h"

int main()
{
    sc::vector<int> A( 10 ); // Construtor.
    sc::vector<int> B; // Construtor.
    sc::vector<int> C{ 1, 2, 3, 4, 5 }; // Construtor a partir de uma lista inicializadora.

    // int *v{nullptr};
    // try { v = new int[100]; }
    // catch( const std::bad_alloc & e ) {

    // }

    std::cout << ">>> The vector: " << A.to_string() << "\n";
    for ( size_t i{0} ; i < A.capacity()/2 ; ++i )
        A.push_back( i+1 );

    std::cout << ">>> The vector after several pushbacks: " << A.to_string() << "\n";

    for ( size_t i{2} ; i < 10; ++i )
    {
        try {
            A.push_back( i+1 );
        }
        catch( const std::length_error& e ) {
            std::cout << ">>> Length error exception successfuly caught!\n";
        }
    }
    
    std::cout << ">>> The vector after several pushbacks: " << A.to_string() << "\n";

    // testando atribuicao
    std::cout << "C: " << C.to_string() << '\n';
    std::cout << "B antes da atribuicao: " << B.to_string() << '\n';
    B = C; // B.operator=( C );
    std::cout << "B depois de B=C: " << B.to_string() << '\n';

    // B.at(20) = 100;
    B[1] = 100;
    std::cout << "B depois de B[1]=100: " << B.to_string() << '\n';

#ifdef NOT_IMPLEMENTED
    A.insert( 2, 500 );
    std::cout << ">>> The vector after insert(2,500): " << A.to_string() << "\n";

    A.insert( 8, 100 );
    std::cout << ">>> The vector after insert(8,100): " << A.to_string() << "\n";

    A.insert( 0, -50 );
    std::cout << ">>> The vector after insert(0,-50): " << A.to_string() << "\n";

    try {
    A.push_back( 200 );
    }
    catch ( const std::length_error & e )
    {
        std::cout << ">>> Excecao capturada com sucesso!\n";
    }
    
    {
        sc::vector<int> A( 10 ); // Construtor.
        for ( size_t i{0} ; i < A.capacity() ; ++i )
            A.push_back( i+1 );

        std::cout << ">>> The vector: " << A.to_string() << "\n";
        A.remove( 0 );
        std::cout << ">>> The vector after removing A[0]: " << A.to_string() << "\n";
        A.remove( 4 );
        std::cout << ">>> The vector after removing A[4]: " << A.to_string() << "\n";
        A.remove( 7 );
        std::cout << ">>> The vector after removing A[7]: " << A.to_string() << "\n";
        while( not A.empty() )
        {
            A.remove(0);
            std::cout << ">>> The vector after removing the first element: " << A.to_string() << "\n";
        }
    }
    {
        sc::vector<int> A( 10 ); // Construtor.
        for ( size_t i{0} ; i < A.capacity() ; ++i )
            A.push_back( i+1 );

        sc::vector<int> B( 2 );

        std::cout << ">>> The vector A: " << A.to_string() << "\n";
        std::cout << ">>> The vector B: " << B.to_string() << "\n";
        B = A;
        std::cout << ">>> The vector B after B = A: " << B.to_string() << "\n";
        B[0] = 100;
        std::cout << ">>> Changing the first element of B.\n";
        std::cout << ">>> The vector A: " << A.to_string() << "\n";
        std::cout << ">>> The vector B: " << B.to_string() << "\n";
    }

    {
        sc::vector<int> A{ 1, 2, 3, 4, 5 };
        std::cout << ">>> The vector A: " << A.to_string() << "\n";

        sc::vector<int> B = {1, 2, 3 };
        std::cout << ">>> The vector B: " << B.to_string() << "\n";
    }

    {
        std::cout << "\n>>> Testin the Copy Ctro.\n";
        sc::vector<int> A{ 1, 2, 3, 4, 5 };
        std::cout << "    The vector A: " << A.to_string() << "\n";

        sc::vector<int> B{A};
        std::cout << "    The vector B created with copy ctro. based on A: " << B.to_string() << "\n";
    }

    {
        std::cout << "\n>>> Testing iterators.\n";
        sc::vector<int> A{ 1, 2, 3, 4, 5 };
        std::cout << "    The vector A: " << A.to_string() << "\n";

        std::cout << "    Iterating over A: [ ";
        auto it = A.begin();
        // sc::vector<int>::iterator it = A.begin();
        // delete [] it;
        while ( it != A.end() )
        {
            std::cout << *it << " ";
            it++; // pointer arithmetics.
        }
        std::cout << "]\n";
    }
#endif
    std::cout << ">>> Normal exiting...\n\n";
    return 0;
}