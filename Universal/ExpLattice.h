#ifndef EXP_LATTICE_H
#define EXP_LATTICE_H

#include "Lattice.h"

class ExpLattice : public Lattice
{
    /** Simple exponential conversion between a lattice with even spacing (x),
        and a "real" space (r):
            r = rmin * exp(x)
            x = ln(r/rmin)
        beta (from Lattice class) is not used here.
     */
public:
    ExpLattice(const ExpLattice& other);
    ExpLattice(unsigned int numpoints, double r_min, double H);
    virtual ~ExpLattice(void) {}

    /** Equality does not check size of lattice. */
    virtual bool operator==(const ExpLattice& other) const;

protected:
    /** Calculate the value that r[i] should be. */
    virtual double lattice_to_real(unsigned int i) const;

    /** Calculate the lattice spacing at a point. */
    virtual double calculate_dr(double r_point) const;
};

#endif