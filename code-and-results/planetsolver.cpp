#include "particlesolver.hpp"
#include "planets.hpp"
#include <vector>
#include <string>

void PlanetSolver::init(vector<string> m_names, double beta, int N, int k, double T){
  initialize(beta, N, k, T);

  Planets Planet;
  Planet.read_pos_vel();
  vec params = vec(7);

  m_masses = zeros<vec>(m_N);
  M = 0;
  double posMx = 0;
  double posMy = 0;
  double posMz = 0;
  double velMx = 0;
  double velMy = 0;
  double velMz = 0;

  //double velx_sun = 0;
  //double vely_sun = 0;
  //double velz_sun = 0;

  for (int i = 0; i < m_N; i++){
    params = Planet.initialize(m_names[i]);
    m_masses(i) = params(0);
    m_X(i*m_k) = params(1); m_Y(i*m_k) = params(2); m_Z(i*m_k) = params(3);
    m_Vx(i*m_k) = 365*params(4); m_Vy(i*m_k) = 365*params(5); m_Vz(i*m_k) = 365*params(6);
    m_ax(i*m_k) = m_ay(i) = m_az(i*m_k) = 0.0;
    M += m_masses[i];
    posMx += m_masses[i]*m_X(i*m_k);
    posMy += m_masses[i]*m_Y(i*m_k);
    posMz += m_masses[i]*m_Z(i*m_k);
    velMx += m_masses[i]*m_Vx(i*m_k);
    velMy += m_masses[i]*m_Vy(i*m_k);
    velMz += m_masses[i]*m_Vz(i*m_k);
    //velx_sun -= m_masses[i]*m_Vx(i*m_k);
    //vely_sun -= m_masses[i]*m_Vy(i*m_k);
    //velz_sun -= m_masses[i]*m_Vz(i*m_k);
  }
  //m_Vx(0) = velx_sun;
  //m_Vy(0) = vely_sun;
  //m_Vz(0) = velz_sun;

  for (int i = 0; i < m_N; i++){
    m_X(i*m_k) -= posMx/M;
    m_Y(i*m_k) -= posMy/M;
    m_Z(i*m_k) -= posMz/M;
    m_Vx(i*m_k) -= velMx/M;
    m_Vy(i*m_k) -= velMy/M;
    m_Vz(i*m_k) -= velMz/M;
  }
};

double PlanetSolver::force_a(vec pos, int l, int j){
  double G = 4*M_PI*M_PI; //AU^(3)*yr^(-2)*M(sol)^(-1);
  double a = 0;
  double diffx,diffy,diffz,diffr,r;
  for (int i = 0; i < m_N; i++){ //for planets
    if (i != l){                 //l is the index of the specific planet we are looking at
      diffx = m_X(l*m_k+j) - m_X(i*m_k+j);   //j is the given timestep
      diffy = m_Y(l*m_k+j) - m_Y(i*m_k+j);
      diffz = m_Z(l*m_k+j) - m_Z(i*m_k+j);
      diffr = diffx*diffx + diffy*diffy + diffz*diffz;
      r = pow(diffr,(m_beta+1)/2);
      a += ((pos(l*m_k+j)-pos(i*m_k+j))*G*m_masses(i))/r;
    }
  }
  return -a;
}

void PlanetSolver::solvesystem(){
  for (int j = 0; j < m_k-1; j++){ // for time
    for (int i = 0; i < m_N; i++){ //for planets
      verlet_position(i,j);
    }
    for (int i = 0; i < m_N; i++){ //for planets
      m_ax(i*m_k+j+1) = force_a(m_X,i,j+1);
      m_ay(i*m_k+j+1) = force_a(m_Y,i,j+1);
      m_az(i*m_k+j+1) = force_a(m_Z,i,j+1);
      verlet_velocity(i,j);
    }
  }
};

void PlanetSolver::write_pos_to_file(){
  ofstream x;
  ofstream y;
  ofstream z;

  string filename_1("./results/position_x.txt");
  string filename_2("./results/position_y.txt");
  string filename_3("./results/position_z.txt");
  x.open(filename_1);
  y.open(filename_2);
  z.open(filename_3);
  for (int j = 0; j < m_k; j++){
    for (int i = 0; i < m_N; i++){
      x << m_X(i*m_k+j) << " ";
      y << m_Y(i*m_k+j) << " ";
      z << m_Z(i*m_k+j) << " ";
    }
    x << "\n";
    y << "\n";
    z << "\n";
  }
  x.close();
  y.close();
  z.close();
}
