#ifndef STATE_INTEGRATOR_H
#define STATE_INTEGRATOR_H

#include "Universal/Integrator.h"
#include "SingleParticleWavefunction.h"
#include "Core.h"
#include <complex>

class StateIntegrator : public Integrator
{
    /** A class to integrate the coupled Dirac equations to get a state in a given potential.
        Solves the equation
            dF/dr = -(Kappa/r)F + [2 + alpha^2(E+V)]G + alpha^2 * exchange.g
            dG/dr =   -(E+V)F   +     (Kappa/r)G      -     exchange.f
        where
            F(r) = s.f[]            G(r) = s.g[]
               E = s.Energy()      Kappa = s.Kappa()
            V(r) = HFPotential[]
     */
public:
    StateIntegrator(pLattice lat): Integrator(lat) {}
    virtual ~StateIntegrator(void) {}

    /** Set up the wavefunction from r->0 (points 0 to (adams_N-2))
        and integrate until (not including) end_point.
        The nuclear charge is only needed if there is no previous approximation to work from,
        it is only used to provide an appropriate norm.
        PRE:
          N-2 < end_point <= s.size()
                end_point <= HFPotential.size()
                end_point <= exchange.size()
          exchange may be NULL
     */
    void IntegrateForwards(SingleParticleWavefunction& s, const std::vector<double>& HFPotential, const SpinorFunction* exchange, int end_point, double nuclear_charge = 1.);

    /** Set up the wavefunction at r->infinity (points s.size()-adams_N+1 to s.size()-1)
        and integrate backwards until (not including) end_point.
        PRE:
            -1 <= end_point < s.size()-(adams_N-1)
            s.size() <= HFPotential.size()
            s.size()-(adams_N-1) <= exchange.size()
            exchange may be NULL
        POST:
            SingleParticleWavefunction s may be enlarged if necessary, up to a maximum of HFPotential.size().
     */
    void IntegrateBackwards(SingleParticleWavefunction& s, const std::vector<double>& HFPotential, const SpinorFunction* exchange, int end_point);

    /** Set up the wavefunction at r->infinity and integrate backwards until a peak is reached
        (s.dfdr[] changes sign between two points or equals zero), or end_point is reached.
        PRE: s.size() <= HFPotential.size()
        POST:
            Returns the lattice point of the peak.
            If no peak is reached, returns end_point of integration.
                Otherwise, s.dfdr[value]/s.dfdr[value+1] <= 0.
            SingleParticleWavefunction s may be enlarged if necessary, up to a maximum of HFPotential.size().
     */
    unsigned int IntegrateBackwardsUntilPeak(SingleParticleWavefunction& s, const std::vector<double>& HFPotential, int end_point = -1);

    /** Set up the wavefunction at r->0 and integrate until the wavefunction
        begins to oscillate sinusoidally (outside range of potential).
        Return the point that this occurs. Also return the final amplitude and the phase.
        A return value of zero indicates that it never got to sinusoidal oscillations,
        most likely because the lattice isn't big enough.
        POST: actual amplitude as r->Infinity, A = final_amplitude/(2E)^(1/4)
     */
    unsigned int IntegrateContinuum(ContinuumWave& s, const std::vector<double>& HFPotential, const SpinorFunction& exchange, double nuclear_charge, double accuracy, double& final_amplitude, double& final_phase);

    /** Calculate the matrix element of the Hamiltonian, <s1|H|s2>. */
    double HamiltonianMatrixElement(const SingleParticleWavefunction& s1, const SingleParticleWavefunction& s2, const Core& core);

    /** Get isotope shift between two states. In the first of these functions,
            f = s1.f
            L = s1.L
     */
    double IsotopeShiftIntegral(const std::vector<double> f, unsigned int L, const SpinorFunction& s2, std::vector<double>* P = NULL);
    double IsotopeShiftIntegral(const SpinorFunction& s1, const SpinorFunction& s2, std::vector<double>* P = NULL);
    void IsotopeShiftIntegral(unsigned int L, const SpinorFunction& s2, std::vector<double>* P);

protected:
    class StateFunction;

    /** Set up first points of forward integration (near r=0) using semiclassical approximation. */
    virtual void SetUpForwardsIntegral(SingleParticleWavefunction& s, const std::vector<double>& HFPotential, double nuclear_charge);

    /** Checks that s is large enough to accomodate wavefunction, otherwise enlarges it to a
        maximum of HFPotential.size().
        Initialise last four values of state s from s.size()-(adams_N-1) to s.size()-1.
     */
    virtual void SetUpBackwardsIntegral(SingleParticleWavefunction& s, const std::vector<double>& HFPotential);

    /** Checks that s is large enough to accomodate wavefunction, otherwise enlarges it to a
        maximum of HFPotential.size().
        Initialise four values of state s from start_point to start_point+(adams_N-2).
     */
    void SetUpContinuum(ContinuumWave& s, const std::vector<double>& HFPotential, const StateFunction& state_function, double nuclear_charge, unsigned int start_point);

    /** Create interpolation using 4 points in function, from zero_point-1 to zero_point+2, inclusive.
        if(calculate_x)
            Find maximum or minimum of function using Lagrange cubic interpolation.
            Extremum must lie between zero_point and zero_point+1.
            Returns function value at the extremum.
        Return value of function at x
      */
    double FindExtremum(const std::vector<double>& function, unsigned int zero_point, double& x, bool calculate_x = true);

    std::complex<double> Gip(std::complex<double> A, std::complex<double> B, std::complex<double> Z);
    double GipReal(double A, double B, double Z);

    class StateFunction : public Function6
    {
    public:
        StateFunction(pLattice lat): Function6(), lattice(lat), exchange(NULL) {}
        virtual ~StateFunction(void) {}

        virtual double Coeff1(int point) const;
        virtual double Coeff2(int point) const;
        virtual double Coeff3(int point) const;
        virtual double Coeff4(int point) const;
        virtual double Coeff5(int point) const;
        virtual double Coeff6(int point) const;

        void SetState(const SingleParticleWavefunction& state)
        {   kappa = double(state.Kappa());
            energy = state.Energy();
        }

        void SetHFPotential(const std::vector<double>& potential) { HFPotential = &potential; }
        void SetExchange(const SpinorFunction* exchange) { this->exchange = exchange; }
    private:
        double kappa;
        double energy;
        const SpinorFunction* exchange;
        const std::vector<double>* HFPotential;
        pLattice lattice;
    };
};

#endif
