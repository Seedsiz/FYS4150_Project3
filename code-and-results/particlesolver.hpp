#ifndef ParticleSolver_HPP
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

protected:
  vec m_X,m_Y,m_Z;
  vec m_Vx,m_Vy, m_Vz;
  vec m_ax,m_ay,m_az;
  vec m_xupdate, m_yupdate; // params for RK4
  double m_beta;
  vec m_masses;
  vec m_x0,m_y0,mz0;
  vec m_vx0,m_vy0,m_vz0;
  int m_N;          // number of planets
  int m_k;          // number of time steps
  double m_T, m_h, m_T0;
  double M;
  vec m_Etot, m_Lx, m_Ly, m_Lz;
  vector<string> m_names;

public:                          // general solver
  void initialize(double beta, int N, int k, double T);      // Use keys for each planet
  double force_a(vec pos, int l, int j);
  void verlet_pos(int l, int j);                // Verlet solver
  void verlet_vel_and_a(int l, int j);                // Verlet solver

  void eulerchromer();          // EulerChromer solver
  void forwardeuler();          // Forward euler solver
  double kinetic_energy(int i, int j);
  double potential_energy(double r, int l, int i, int j);
  double angular_momentum(double pos1, double v1, double pos2, double v2);
  void get_angular_momentum();

};

//subclass to solve planet case
class PlanetSolver : public ParticleSolver {
private:

public:
  void init(double beta, int N, int k, double T, vector<string> names);           //init special solver for planet case
  void init(double beta, int N, int k, double T, vector<string> names, vector<double> x, vector<double> y, vector<double> z, vector<double> vx, vector<double> vy, vector<double> vz, vector<double> masses);
  void solvesystem(bool check);      //  solve for planet system
  void write_pos_to_file();
  void write_vel_to_file();
};

#endif
