#ifndef NONRELATIVISTIC_SMS_OPERATOR_H
#define NONRELATIVISTIC_SMS_OPERATOR_H

#include "HartreeY.h"

/** Non-relativistic specific mass shift operator [Berengut et al. PRA 68, 022502 (2003)].
    \f[
        P^1 (ac, bd) = \lambda p_{ab} p_{cd}
    \f]
    where
    \f[
        p_{ab} = \int f_a \left( \frac{d}{dr} - \frac{l_a}{r} \right) f_b dr . \,\delta_{l_a, l_b+1}
                + \int f_a \left( \frac{d}{dr} + \frac{l_b}{r} \right) f_b dr . \,\delta_{l_a, l_b-1}
               = - p_{ba}
    \f]
    Multipolarity K == 1.

    One can also use it as a normal OneBodyOperator by not using SetInverseMass or SetParameters, which
    default to 1 and hence \f$ P^1 = p_{ab} \f$.
 */
class NonRelativisticSMSOperator : public HartreeYDecorator
{
public:
    NonRelativisticSMSOperator(pHartreeY wrapped, pOPIntegrator integration_strategy = nullptr):
        HartreeYDecorator(wrapped, integration_strategy), lambda(1.0), p_cd(1.0)
    {   K = 1;
    }

    /** Set the inverse nuclear mass: 1/M. */
    void SetInverseMass(double InverseNuclearMass) { lambda = InverseNuclearMass; }
    double GetInverseMass() const { return lambda; }

    virtual bool SetParameters(int K, const SpinorFunction& c, const SpinorFunction& d) override;

    virtual bool isZero() const override;

    /** < b | t | a > for an operator t. */
    virtual double GetMatrixElement(const SpinorFunction& b, const SpinorFunction& a, bool reverse) const override;

    /** Potential = t | a > for an operator t such that the resulting Potential.Kappa() == kappa_b.
     i.e. t | a > has kappa == kappa_b.
     */
    virtual SpinorFunction ApplyTo(const SpinorFunction& a, int kappa_b, bool reverse) const override;

protected:
    /** Returns \f$ \hat p f_a \f$. */
    SpinorFunction ApplyOperator(const SpinorFunction& a, int kappa_b) const;

protected:
    double lambda;
    double p_cd;
};

#endif