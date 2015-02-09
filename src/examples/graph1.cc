#include "cnn/cnn-edges.h"
#include "cnn/cnn.h"

#include <iostream>

using namespace std;
using namespace cnn;

int main() {
  sranddev();
  cnn::Hypergraph hg;
  unsigned i_x = hg.add_input(Dim(2), "x");
  InputEdge& x = reinterpret_cast<InputEdge&>(*hg.edges.back());
  x(0,0) = 1; x(1,0) = -1;
  unsigned i_y = hg.add_input(Dim(1), "y");
  InputEdge& y = reinterpret_cast<InputEdge&>(*hg.edges.back());
  y(0,0) = 1;
  unsigned i_b = hg.add_parameter(Dim(3), "b");
  ParameterEdge& b = reinterpret_cast<ParameterEdge&>(*hg.edges.back());
  b(0,0) = 0.3; b(1,0) = -0.02; b(2,0) = 0.1;
  unsigned i_a = hg.add_parameter(Dim(1), "a");
  ParameterEdge& a = reinterpret_cast<ParameterEdge&>(*hg.edges.back());
  a(0,0) = 0.1;
  unsigned i_W = hg.add_parameter(Dim(3,2), "W");
  ParameterEdge& W = reinterpret_cast<ParameterEdge&>(*hg.edges.back());
  W(0,0) = -0.1; W(0,1) = 0.123;
  W(1,0) = 0.1; W(1,1) = 0.0123;
  W(2,0) = -0.15; W(2,1) = -1;
  unsigned i_V = hg.add_parameter(Dim(1,3), "V");
  ParameterEdge& V = reinterpret_cast<ParameterEdge&>(*hg.edges.back());
  V(0,0) = -0.08; V(0,1) = 0.22; V(0,2) = 0.5;
  unsigned i_W2 = hg.add_function<Square>({i_W}, "W^2");
  unsigned i_V2 = hg.add_function<Square>({i_V}, "V^2");
  unsigned i_t1 = hg.add_function<MatrixMultiply>({i_W2, i_x}, "t1");
  unsigned i_t2 = hg.add_function<MatrixMultiply>({i_V2, i_t1}, "t2");
  unsigned i_f = hg.add_function<MatrixMultiply>({i_W, i_x}, "f");
  unsigned i_g = hg.add_function<Sum>({i_f, i_b}, "g");
  unsigned i_h = hg.add_function<Tanh>({i_g}, "h");
  unsigned i_p = hg.add_function<MatrixMultiply>({i_V, i_h}, "p");
  unsigned i_y_pred = hg.add_function<Sum>({i_p, i_a, i_t2}, "y_pred");
  unsigned i_err = hg.add_function<EuclideanDistance>({i_y_pred, i_y}, "err");
  hg.PrintGraphviz();
  cerr << "E = " << hg.forward() << endl;
  hg.backward();
  cerr << "dE/db = " << hg.nodes[2]->dEdf.transpose() << endl;
  cerr << "dE/da = " << hg.nodes[3]->dEdf.transpose() << endl;
  cerr << "dE/dW =\n" << hg.nodes[4]->dEdf << endl;
  cerr << "dE/dV =\n" << hg.nodes[5]->dEdf << endl;
}
