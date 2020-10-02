#ifndef Particlesolver_HPP
#define ParticleSolver_HPP

#include <fstream>
#include <armadillo>
#include <chrono>

using namespace std;
using namespace arma;
using namespace chrono;

// Setting up solarsystem as a superclass
// Work in units: time [years], distance [AU], mass [solar masses]

class ParticleSolver {

private:

protected:
  string m_name;
  vec m_x,_y,m_m_z;
  double m_force;
  int m_N;         // number of planets
  int m_k;          // number of time steps
  double m_G =      //




public:
  void initialize(m_N,m_k);      // Use keys for each planet
  void force();        // calculate force between stellar objects
  void Verlet();            // Verlet solver
  void EulerChromer();       // EulerChromer solver
  void RungeKutta4();        // RungeKutta4 solver

};



#endif
