#include "tools.h"
#include <iostream>


using namespace std;
using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {

	VectorXd rmse(4);
	rmse << 0,0,0,0;

	if (estimations.size() == 0 || ground_truth.size() != estimations.size()) {
		return rmse;
	}

	for (double i=0; i < estimations.size(); ++i) {
		//cout << i << "-" << endl << estimations[i] << endl;
		VectorXd residuals = estimations[i] - ground_truth[i];
		residuals = pow (residuals.array(), 2);
		//cout << "Before : " << residuals << endl;
		rmse += residuals;
		//cout << "After : " << residuals << endl;
	}

	rmse = rmse/estimations.size();
	//cout << "estimation : " << rmse << endl;
	rmse = rmse.array().sqrt();
	cout << "RES : " << rmse << endl;
	return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
    
	MatrixXd Hj(3,4);
    // recover state parameters
    float px = x_state(0);
    float py = x_state(1);
    float vx = x_state(2);
    float vy = x_state(3);
    
    // pre-compute a set of terms to avoid repeated calculation
    float c1 = px*px+py*py;
    float c2 = sqrt(c1);
    float c3 = (c1*c2);
    
    // check division by zero
    if (fabs(c1) < 0.0001) {
        cout << "CalculateJacobian () - Error - Division by Zero" << endl;
        return Hj;
    }
    
    // compute the Jacobian matrix
    Hj << (px/c2), (py/c2), 0, 0,
      -(py/c1), (px/c1), 0, 0,
      py*(vx*py - vy*px)/c3, px*(px*vy - py*vx)/c3, px/c2, py/c2;
    
    return Hj;
}
