#include "Symmetry.h"
#include "Include.h"

std::string Symmetry::GetString() const
{
    std::string ret;
    std::stringstream angmom;
    angmom << twoJ;
    ret = angmom.str();
    if(P == Parity::even)
        ret = ret + ".even";
    else
        ret = ret + ".odd";

    return ret;
}

bool Symmetry::operator<(const Symmetry& other) const
{
    if(P != other.P)
        return (P == Parity::even);
    else
        return (twoJ < other.twoJ);
}

bool Symmetry::operator==(const Symmetry& other) const
{
    if((P == other.P) && (twoJ == other.twoJ))
        return true;
    else
        return false;
}

const Symmetry& Symmetry::operator=(const Symmetry& other)
{
    P = other.P;
    twoJ = other.twoJ;
    
    return *this;
}

std::set<Symmetry> ChooseSymmetries(const MultirunOptions& user_input)
{
    std::set<Symmetry> symmetries;
    int i, num_symmetries;
    int two_j;

    // Even parity
    num_symmetries = user_input.vector_variable_size("CI/EvenParityTwoJ");
    for(i = 0; i < num_symmetries; i++)
    {   two_j = user_input("CI/EvenParityTwoJ", 0, i);
        symmetries.emplace(two_j, Parity::even);
    }

    // Odd parity
    num_symmetries = user_input.vector_variable_size("CI/OddParityTwoJ");
    for(i = 0; i < num_symmetries; i++)
    {   two_j = user_input("CI/OddParityTwoJ", 0, i);
        symmetries.emplace(two_j, Parity::odd);
    }

    if(symmetries.empty())
    {   *errstream << "USAGE: No symmetries requested (EvenParityTwoJ or OddParityTwoJ)" << std::endl;
        exit(1);
    }

    return symmetries;
}
