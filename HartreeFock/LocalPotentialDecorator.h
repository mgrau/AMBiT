#ifndef LOCAL_POTENTIAL_DECORATOR
#define LOCAL_POTENTIAL_DECORATOR

#include "HFOperator.h"

/** Generic (abstract-ish) class to add an extra local potential to a HF operator.
    Extra local potential should be stored in directPotential (inherited from HF operator) and
    must be set by subclasses via SetCore() and/or SetODEParameters().
    Note sign is for normal electrostatic potential: V_nucleus(r) > 0.
 */
class LocalPotentialDecorator : public HFOperatorDecorator
{
public:
    LocalPotentialDecorator(pHFOperator wrapped_hf, pOPIntegrator integration_strategy = pOPIntegrator());

public:
    virtual RadialFunction GetDirectPotential() const override;
    virtual void GetODEFunction(unsigned int latticepoint, const SpinorFunction& fg, double* w) const override;
    virtual void GetODECoefficients(unsigned int latticepoint, const SpinorFunction& fg, double* w_f, double* w_g, double* w_const) const override;
    virtual void GetODEJacobian(unsigned int latticepoint, const SpinorFunction& fg, double** jacobian, double* dwdr) const override;

public:
    virtual SpinorFunction ApplyTo(const SpinorFunction& a) const override;
};

/** Decorate HF operator with a local approximation to the exchange potential. Good for first approximations. */
class LocalExchangeApproximation : public LocalPotentialDecorator
{
public:
    LocalExchangeApproximation(pHFOperator wrapped_hf, pOPIntegrator integration_strategy = pOPIntegrator());

    virtual void SetCore(pCoreConst hf_core) override;
    virtual void Alert() override;
};

#endif
