/*
  File main.cc
 
  This file contains just an example on how to set-up the matrices for using with
  the solve_quadprog() function.
 
  The test problem is the following:
 
  Given:
  G =  4 -2   g0^T = [6 0]
  -2  4       
 
  Solve:
  min f(x) = 1/2 x G x + g0 x
  s.t.
  x_1 + x_2 = 3
  x_1 >= 0
  x_2 >= 0
v  x_1 + x_2 >= 2
 
  The solution is x^T = [1 2] and f(x) = 12
 
  Author: Luca Di Gaspero
  DIEGM - University of Udine, Italy
  l.digaspero@uniud.it
  http://www.diegm.uniud.it/digaspero/
 
  LICENSE
 
  This file is part of QuadProg++: a C++ library implementing
  the algorithm of Goldfarb and Idnani for the solution of a (convex) 
  Quadratic Programming problem by means of an active-set dual method.
  Copyright (C) 2007-2009 Luca Di Gaspero.
  Copyright (C) 2009 Eric Moyer.  
 
  QuadProg++ is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  QuadProg++ is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public License
  along with QuadProg++. If not, see <http://www.gnu.org/licenses/>.
*/
#define DNDEBUG
#include <iostream>
#include <sstream>
#include <string>
#include "uQuadProg++.hh"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <Eigen/Eigen>
#include <EigenQP.h>
#include <boost/progress.hpp>

using boost::timer;

using namespace Eigen;
namespace ublas = boost::numeric::ublas;
using std::endl;
using std::cout;

int main (int argc, char *const argv[]) {
  timer clock;



  int n, m, p;
  n = 1000;
  m = 505;
  p = 703;
  double sum = 0.0;
  char ch;

  ublas::matrix<double> G, CE, CI;
  ublas::vector<double> g0, ce0, ci0, x;

  MatrixXd eG(n,n), eCi(n, p), eCe(n, m);
  VectorXd eg0(n), ece(m), eci(p), ex(n);
  

  G.resize(n, n);
  {
    //    std::istringstream is("4, -2,"
//			  "-2, 4 ");
/*    
    for (int i = 0; i < n; i++)	
      for (int j = 0; j < n; j++){
	is >> G(i, j) >> ch;
	eG(i, j) = G(i, j);
*/

    for (int i = 0; i < n; ++i)
      for(int j = 0; j <= i; ++j){
	eG(i, j) = G(i, j) = (i== j)? i+j+1 : pow((-1), (i+j)) * (double)i/(j+1)*(rand()/RAND_MAX);
      }

    eG = (eG.transpose() * eG).eval();
    G = prod(trans(G) , G);

     
    //    cout << "G" << G << endl << endl << eG << endl;
  }
	
  g0.resize(n);
  {
    std::istringstream is("6.0, 0.0 ");

    for (int i = 0; i < n; i++){
      eg0(i)=g0(i) = (double)rand()/RAND_MAX;
    }
    //    cout << "g0" << g0 << endl << endl << eg0 << endl;
  }
  
  CE.resize(n, m);
  {
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++){
	eCe(i, j) = CE(i, j) = (double) rand()/RAND_MAX;
      }
    //    cout << "CE" << CE << endl << endl << eCe << endl;
	
  } 
  
  ce0.resize(m);
  {
    std::istringstream is("-3.0 ");
		
    for (int j = 0; j < m; j++){
      is >> ce0(j) >> ch;
      ece(j) = ce0(j);
    }
    //    cout << "ce" << ce0 << endl << endl << ece << endl;
  }
	

  CI.resize(n, p);
  {
    std::istringstream is("1.0, 0.0, 1.0, "
			  "0.0, 1.0, 1.0 ");
  
    for (int i = 0; i < n; i++)
      for (int j = 0; j < p; j++){
	eCi(i, j) = CI(i, j) = (double) (rand()) /RAND_MAX;
      }
    //    cout << "CI" << CI << endl << endl << eCi << endl;
  }
  
  ci0.resize(p);
  {
    std::istringstream is("0.0, 0.0, -2.0 ");

    for (int j = 0; j < p; j++)
      {
	eci(j) = ci0(j) = (double) rand() / RAND_MAX;
      }
    //    cout << "ci" << ci0 << endl << endl << eci << endl;
  }
  x.resize(n);

  clock.restart();
  std::cout << "f: " << uQuadProgPP::solve_quadprog(G, g0, CE, ce0, CI, ci0, x) << std::endl;
  std::cout << "Elapse Time:" << clock.elapsed() << std::endl;
 

  std::cout << "x: " << x << std::endl;
  /*  for (int i = 0; i < n; i++)
      std::cout << x[i] << ' ';
      std::cout << std::endl;	 */

  /* FOR DOUBLE CHECKING COST since in the solve_quadprog routine the matrix G is modified */
	
  {
    std::istringstream is("4, -2,"
			  "-2, 4 ");
	
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
	is >> G(i, j) >> ch;
  }
	
  std::cout << "Double checking cost: ";
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      sum += x(i) * G(i, j) * x(j);
  sum *= 0.5;	
	
  for (int i = 0; i < n; i++)
    sum += g0(i) * x(i);
  std::cout << sum << std::endl;
  clock.restart();
  std::cout << QP::solve_quadprog(eG, eg0, eCe, ece, eCi, eci, ex) << std::endl;
  std::cout << "Elapsed Time:" << clock.elapsed() << std::endl;
  std::cout <<"Solution:" << endl <<  ex << std::endl;

}
