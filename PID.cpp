#include "PID.h"

using namespace std;


PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd)
{
	Kp_ = Kp;
	Ki_ = Ki;
	Kd_ = Kd;

	i_error_ = 0.0;		// initialize integral
	cte_old_ = 0.0;     // assume previous cte=0
}

void PID::UpdateError(double cte)
{
	 p_error_  = Kp_ * cte;
	 i_error_ += (Ki_ * cte);
	 d_error_  = Kd_ * (cte - cte_old_);

	cte_old_  = cte;
}

double PID::TotalError(void)
{
	double error = -p_error_ - i_error_ - d_error_;
	if (error > 1.0)
		error = 1.0;
	if (error < -1)
		error = -1.0;
	return(error);
}

