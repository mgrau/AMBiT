#ifndef SOLUTION_H
#define SOLUTION_H

#include <map>
#include <string>

#include "Include.h"
#include "Atom/Transitions.h"
#include "Configuration/Configuration.h"
#include "Configuration/Symmetry.h"
#include "Universal/MathConstant.h"
#include "Universal/Enums.h"

class TransitionSet;

// Effectively a hash function for converting
// J, Pi and solution position in the list
// To a unique ID
class SolutionID
{
public:
    SolutionID(double aJ, ParityType::Enum aParity, unsigned int aID);
    SolutionID(Symmetry aSymmetry, unsigned int aID);

    bool operator<(const SolutionID& other) const;
    inline bool operator==(const SolutionID& other)
    {   return ((mJ == other.mJ) && (mParity == other.mParity) && (mID == other.mID));
    }
    
    double GetJ() const { return mJ; }
    ParityType::Enum GetParity() const { return mParity; }
    unsigned int GetID() const { return mID; }
    Symmetry GetSymmetry() const;

    std::string GetIdentifier() const;

protected:
    double mJ;
    ParityType::Enum mParity;
    unsigned int mID;
};

class Solution
{
public:
    // Creates a Solution with energy aEnergy in atomic units
    Solution(double aEnergy, double agFactor = 0.0);
    Solution(double aEnergy, std::map<Configuration, double> aPercentagesMap, double agFactor = 0.0);

    inline double GetEnergyInversecm() const
    {   return mEnergy * MathConstant::Instance()->HartreeEnergyInInvCm();
    }
    inline double GetEnergyAtomicUnits() const
    {   return mEnergy;
    }
    inline double GetgFactor() const
    {   return mgFactor;
    }
    inline ConfigurationSet* GetConfigurationSet()
    {   return mConfigurationSet;
    }
    inline Configuration GetLeadingConfiguration()
    {   return GetConfigurationSet()->GetLargestConfiguration();
    }
    inline TransitionSet* GetTransitionSet()
    {   return mTransitionSet;
    }

protected:
    double mEnergy;
    double mgFactor;
    ConfigurationSet* mConfigurationSet;
    TransitionSet* mTransitionSet;
};

class SolutionMap : public std::map<SolutionID, Solution>
{
public:
    void Print(DisplayOutputType::Enum aDisplayOutputType = DisplayOutputType::Standard);
    void PrintID();
    void PrintSolution(SolutionMap::iterator aSolutionIterator);

    SolutionMap::iterator FindByIdentifier(const std::string& aIdentifier);
protected:
};

class SolutionMapMap : public std::map<unsigned int, SolutionMap>
{
public:
    void Print();
};

#endif