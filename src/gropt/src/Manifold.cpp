
#include "Manifold.h"

Manifold::~Manifold(void)
{
};

double Manifold::Metric(Variable *x, Vector *etax, Vector *xix) const
{
	const double *v1 = etax->ObtainReadData();
	const double *v2 = xix->ObtainReadData();

	integer inc = 1, N = etax->Getlength();
	return ddot_(&N, const_cast<double *> (v1), &inc, const_cast<double *> (v2), &inc);
};

void Manifold::LinearOPEEta(Variable *x, LinearOPE *Hx, Vector *etax, Vector *result) const
{
	if (etax == result)
	{
		Rcpp::Rcout << "The arguments of etax and result should not be the same!" << std::endl;
	}
	integer ell = Hx->Getsize()[0];
	const double *v = etax->ObtainReadData();
	const double *M = Hx->ObtainReadData();
	double *resultTV = result->ObtainWriteEntireData();

	char *transn = const_cast<char *> ("n");
	double one = 1, zero = 0;
	integer inc = 1, N = ell;
	dgemv_(transn, &N, &N, &one, const_cast<double *> (M), &N, const_cast<double *> (v), &inc, &zero, resultTV, &inc);
};

void Manifold::ScaleTimesVector(Variable *x, double scaler, Vector *etax, Vector *result) const
{
	etax->CopyTo(result);
	double *resultTV = result->ObtainWritePartialData();
	integer N = etax->Getlength(), inc = 1;
	dscal_(&N, &scaler, resultTV, &inc);
};

void Manifold::VectorAddVector(Variable *x, Vector *etax, Vector *xix, Vector *result) const
{
	VectorLinearCombination(x, 1.0, etax, 1.0, xix, result);
};

void Manifold::VectorMinusVector(Variable *x, Vector *etax, Vector *xix, Vector *result) const
{
	VectorLinearCombination(x, 1.0, etax, -1.0, xix, result);
};

void Manifold::ScalerVectorAddVector(Variable *x, double scaler, Vector *etax, Vector *xix, Vector *result) const
{
	VectorLinearCombination(x, scaler, etax, 1.0, xix, result);
};

void Manifold::ScalerVectorMinusVector(Variable *x, double scaler, Vector *etax, Vector *xix, Vector *result) const
{
	VectorLinearCombination(x, scaler, etax, -1.0, xix, result);
};

void Manifold::VectorLinearCombination(Variable *x, double scaler1, Vector *etax, double scaler2, Vector *xix, Vector *result) const
{
	const double *etaxTV = etax->ObtainReadData();
	const double *xixTV = xix->ObtainReadData();
	double *resultTV = result->ObtainWriteEntireData();

	integer N1 = etax->Getlength(), N2 = xix->Getlength(), N3 = result->Getlength(), inc = 1;
	integer N = (N1 > N2) ? N2 : N1;
	N = (N > N3) ? N3 : N;
	if (resultTV == etaxTV)
	{
		dscal_(&N, &scaler1, resultTV, &inc);
		daxpy_(&N, &scaler2, const_cast<double *> (xixTV), &inc, resultTV, &inc);
	}
	else
	if (resultTV == xixTV)
	{
		dscal_(&N, &scaler2, resultTV, &inc);
		daxpy_(&N, &scaler1, const_cast<double *> (etaxTV), &inc, resultTV, &inc);
	}
	else
	{
		dcopy_(&N, const_cast<double *> (etaxTV), &inc, resultTV, &inc);
		dscal_(&N, &scaler1, resultTV, &inc);
		daxpy_(&N, &scaler2, const_cast<double *> (xixTV), &inc, resultTV, &inc);
	}
};

void Manifold::Projection(Variable *x, Vector *v, Vector *result) const
{
	v->CopyTo(result);
};

void Manifold::RandomTangentVectors(Variable *x, integer N, Vector **result_arr) const // Be careful
{
};

void Manifold::Retraction(Variable *x, Vector *etax, Variable *result) const
{
	const double *v = etax->ObtainReadData();
	const double *xM = x->ObtainReadData();
	double *resultM = result->ObtainWriteEntireData();

	integer inc = 1, N = x->Getlength();
	double one = 1;
	if (resultM != xM)
		dcopy_(&N, const_cast<double *> (xM), &inc, resultM, &inc);
	daxpy_(&N, &one, const_cast<double *> (v), &inc, resultM, &inc);
};

void Manifold::coTangentVector(Variable *x, Vector *etax, Variable *y, Vector *xiy, Vector *result) const
{
	xiy->CopyTo(result);
};

void Manifold::DiffRetraction(Variable *x, Vector *etax, Variable *y, Vector *xix, Vector *result, bool IsEtaXiSameDir) const
{
	xix->CopyTo(result);
};

double Manifold::Beta(Variable *x, Vector *etax) const
{
	return 1;
};

void Manifold::VectorTransport(Variable *x, Vector *etax, Variable *y, Vector *xix, Vector *result) const
{
	xix->CopyTo(result);
};

void Manifold::InverseVectorTransport(Variable *x, Vector *etax, Variable *y, Vector *xiy, Vector *result) const
{
	xiy->CopyTo(result);
};

void Manifold::HInvTran(Variable *x, Vector *etax, Variable *y, LinearOPE *Hx, integer start, integer end, LinearOPE *result) const
{
	Hx->CopyTo(result);
};

void Manifold::TranH(Variable *x, Vector *etax, Variable *y, LinearOPE *Hx, integer start, integer end, LinearOPE *result) const
{
	Hx->CopyTo(result);
};

void Manifold::TranHInvTran(Variable *x, Vector *etax, Variable *y, LinearOPE *Hx, LinearOPE *result) const
{
	Hx->CopyTo(result);
};

void Manifold::HaddScaledRank1OPE(Variable *x, LinearOPE *Hx, double scaler, Vector *etax, Vector *xix, LinearOPE *result) const
{
	const double *veta = etax->ObtainReadData();
	const double *vxi = xix->ObtainReadData();
	Hx->CopyTo(result);
	double *resultL = result->ObtainWritePartialData();
	integer ell = Hx->Getsize()[0], N = ell, inc = 1;
	dger_(&N, &N, &scaler, const_cast<double *> (veta), &inc, const_cast<double *> (vxi), &inc, resultL, &N);
};

void Manifold::ObtainEtaxFlat(Variable *x, Vector *etax, Vector *etaxflat) const
{
	etax->CopyTo(etaxflat);
};

void Manifold::ObtainIntr(Variable *x, Vector *etax, Vector *result) const
{
	etax->CopyTo(result);
};

void Manifold::ObtainExtr(Variable *x, Vector *intretax, Vector *result) const
{
	intretax->CopyTo(result);
};

void Manifold::Obtainnu1nu2forLC(Variable *x, Vector *etax, Variable *y) const
{
	Vector *eps1 = etax->ConstructEmpty();
	Vector *nu1 = etax->ConstructEmpty();
	Vector *nu2 = etax->ConstructEmpty();
	if (!etax->TempDataExist("beta") || !etax->TempDataExist("betaTReta"))
	{
		DiffRetraction(x, etax, y, etax, eps1, true);
	}
	HasHHR = false; VectorTransport(x, etax, y, etax, eps1); HasHHR = true;
	const SharedSpace *TReta = etax->ObtainReadTempData("betaTReta");
	Vector *TRetaVector = TReta->GetSharedElement();
	SharedSpace *Sharedtau1tau2 = new SharedSpace(1, 2);
	SharedSpace *Sharednu1 = new SharedSpace(nu1);
	SharedSpace *Sharednu2 = new SharedSpace(nu2);
	double *tau1tau2 = Sharedtau1tau2->ObtainWriteEntireData();
	ScaleTimesVector(x, 2.0, eps1, nu1);
	VectorLinearCombination(x, -1.0, eps1, -1.0, TRetaVector, nu2);
	tau1tau2[0] = 2.0 / Metric(x, nu1, nu1);
	tau1tau2[1] = 2.0 / Metric(x, nu2, nu2);

	etax->AddToTempData("tau1tau2", Sharedtau1tau2);
	etax->AddToTempData("nu1", Sharednu1);
	etax->AddToTempData("nu2", Sharednu2);
	delete eps1;
};

void Manifold::LCVectorTransport(Variable *x, Vector *etax, Variable *y, Vector *xix, Vector *result) const
{
	if (!etax->TempDataExist("nu1nu2"))
	{
		Obtainnu1nu2forLC(x, etax, y);
	}
	HasHHR = false; VectorTransport(x, etax, y, xix, result); HasHHR = true;
	const SharedSpace *Sharedtau1tau2 = etax->ObtainReadTempData("tau1tau2");
	const double *tau1tau2 = Sharedtau1tau2->ObtainReadData();
	const SharedSpace *Sharednu1 = etax->ObtainReadTempData("nu1");
	Vector *nu1 = Sharednu1->GetSharedElement();
	const SharedSpace *Sharednu2 = etax->ObtainReadTempData("nu2");
	Vector *nu2 = Sharednu2->GetSharedElement();
	double temp = 0;
	temp = - Metric(x, result, nu1);
	ScalerVectorAddVector(x, temp * tau1tau2[0], nu1, result, result);
	temp = - Metric(x, result, nu2);
	ScalerVectorAddVector(x, temp * tau1tau2[1], nu2, result, result);
};

void Manifold::LCInverseVectorTransport(Variable *x, Vector *etax, Variable *y, Vector *xiy, Vector *result) const
{
	if (!etax->TempDataExist("nu1nu2"))
	{
		Obtainnu1nu2forLC(x, etax, y);
	}
	const SharedSpace *Sharedtau1tau2 = etax->ObtainReadTempData("tau1tau2");
	const double *tau1tau2 = Sharedtau1tau2->ObtainReadData();
	const SharedSpace *Sharednu1 = etax->ObtainReadTempData("nu1");
	Vector *nu1 = Sharednu1->GetSharedElement();
	const SharedSpace *Sharednu2 = etax->ObtainReadTempData("nu2");
	Vector *nu2 = Sharednu2->GetSharedElement();
	double temp = 0;
	temp = -Metric(x, xiy, nu2);
	ScalerVectorAddVector(x, temp * tau1tau2[1], nu2, xiy, result);
	temp = -Metric(x, result, nu1);
	ScalerVectorAddVector(x, temp * tau1tau2[0], nu1, result, result);
	HasHHR = false; InverseVectorTransport(x, etax, y, result, result); HasHHR = true;
};

void Manifold::LCHInvTran(Variable *x, Vector *etax, Variable *y, LinearOPE *Hx, integer start, integer end, LinearOPE *result) const
{
	if (!etax->TempDataExist("nu1nu2"))
	{
		Obtainnu1nu2forLC(x, etax, y);
	}
	const SharedSpace *Sharedtau1tau2 = etax->ObtainReadTempData("tau1tau2");
	const double *tau1tau2 = Sharedtau1tau2->ObtainReadData();
	const SharedSpace *Sharednu1 = etax->ObtainReadTempData("nu1");
	Vector *nu1 = Sharednu1->GetSharedElement();
	const SharedSpace *Sharednu2 = etax->ObtainReadTempData("nu2");
	Vector *nu2 = Sharednu2->GetSharedElement();

	double temp = 0;
	const double *nu1TV = nu1->ObtainReadData();
	const double *nu2TV = nu2->ObtainReadData();
		
	//double *nu1TV = new double[nu1->Getlength() * 2];
	//double *nu2TV = nu1TV + nu1->Getlength();
	//nu1->CopytoArray(nu1TV);
	//nu2->CopytoArray(nu2TV);
	HasHHR = false; HInvTran(x, etax, y, Hx, start, end, result); HasHHR = true;
	double *resultTV = result->ObtainWritePartialData();
	char *sider = const_cast<char *> ("r");
	integer ell = Hx->Getsize()[0], length = etax->Getlength();
	double *work = new double[ell];

	dlarfx_(sider, &ell, &length, const_cast<double *> (nu1TV), const_cast<double *> (tau1tau2), resultTV + start * ell, &ell, work);
	dlarfx_(sider, &ell, &length, const_cast<double *> (nu2TV), const_cast<double *> (tau1tau2 + 1), resultTV + start * ell, &ell, work);
	delete[] work;
	//delete[] nu1TV;
};

void Manifold::LCTranH(Variable *x, Vector *etax, Variable *y, LinearOPE *Hx, integer start, integer end, LinearOPE *result) const
{
	if (!etax->TempDataExist("nu1nu2"))
	{
		Obtainnu1nu2forLC(x, etax, y);
	}
	const SharedSpace *Sharedtau1tau2 = etax->ObtainReadTempData("tau1tau2");
	const double *tau1tau2 = Sharedtau1tau2->ObtainReadData();
	const SharedSpace *Sharednu1 = etax->ObtainReadTempData("nu1");
	Vector *nu1 = Sharednu1->GetSharedElement();
	const SharedSpace *Sharednu2 = etax->ObtainReadTempData("nu2");
	Vector *nu2 = Sharednu2->GetSharedElement();
	double temp = 0;
	const double *nu1TV = nu1->ObtainReadData();
	const double *nu2TV = nu2->ObtainReadData();
	//double *nu1TV = new double[nu1->Getlength() * 2];
	//double *nu2TV = nu1TV + nu1->Getlength();
	//nu1->CopytoArray(nu1TV);
	//nu2->CopytoArray(nu2TV);
	HasHHR = false; TranH(x, etax, y, Hx, start, end, result); HasHHR = true;
	double *resultTV = result->ObtainWritePartialData();

	char *sidel = const_cast<char *> ("l");
	integer ell = Hx->Getsize()[0], length = etax->Getlength();
	double *work = new double[ell];
	dlarfx_(sidel, &length, &ell, const_cast<double *> (nu1TV), const_cast<double *> (tau1tau2), resultTV + start, &ell, work);
	dlarfx_(sidel, &length, &ell, const_cast<double *> (nu2TV), const_cast<double *> (tau1tau2 + 1), resultTV + start, &ell, work);
	delete[] work;
	//delete[] nu1TV;
};

void Manifold::LCTranHInvTran(Variable *x, Vector *etax, Variable *y, LinearOPE *Hx, LinearOPE *result) const
{
	if (!etax->TempDataExist("nu1nu2"))
	{
		Obtainnu1nu2forLC(x, etax, y);
	}
	const SharedSpace *Sharedtau1tau2 = etax->ObtainReadTempData("tau1tau2");
	const double *tau1tau2 = Sharedtau1tau2->ObtainReadData();
	const SharedSpace *Sharednu1 = etax->ObtainReadTempData("nu1");
	Vector *nu1 = Sharednu1->GetSharedElement();
	const SharedSpace *Sharednu2 = etax->ObtainReadTempData("nu2");
	Vector *nu2 = Sharednu2->GetSharedElement();
	double temp = 0;
	const double *nu1TV = nu1->ObtainReadData();
	const double *nu2TV = nu2->ObtainReadData();
	HasHHR = false; TranHInvTran(x, etax, y, Hx, result); HasHHR = true;
	double *resultTV = result->ObtainWritePartialData();
	char *sidel = const_cast<char *> ("l"), *sider = const_cast<char *> ("r");
	integer ell = Hx->Getsize()[0], length = etax->Getlength();
	double *work = new double[ell];
	dlarfx_(sider, &ell, &length, const_cast<double *> (nu1TV), const_cast<double *> (tau1tau2), resultTV, &ell, work);
	dlarfx_(sider, &ell, &length, const_cast<double *> (nu2TV), const_cast<double *> (tau1tau2 + 1), resultTV, &ell, work);
	dlarfx_(sidel, &length, &ell, const_cast<double *> (nu1TV), const_cast<double *> (tau1tau2), resultTV, &ell, work);
	dlarfx_(sidel, &length, &ell, const_cast<double *> (nu2TV), const_cast<double *> (tau1tau2 + 1), resultTV, &ell, work);
	delete[] work;
	//delete[] nu1TV;
};

void Manifold::IntrProjection(Variable *x, Vector *v, Vector *result) const
{
	v->CopyTo(result);
};

void Manifold::ExtrProjection(Variable *x, Vector *v, Vector *result) const
{
	v->CopyTo(result);
};

void Manifold::CheckParams(void) const
{
	Rcpp::Rcout << "GENERAL PARAMETERS:" << std::endl;
	Rcpp::Rcout << "name          :" << std::setw(15) << name << ",\t";
	Rcpp::Rcout << "IsIntrApproach:" << std::setw(15) << IsIntrApproach << std::endl;
	Rcpp::Rcout << "IntrinsicDim  :" << std::setw(15) << IntrinsicDim << ",\t";
	Rcpp::Rcout << "ExtrinsicDim  :" << std::setw(15) << ExtrinsicDim << std::endl;
	Rcpp::Rcout << "HasHHR        :" << std::setw(15) << HasHHR << ",\t";
	Rcpp::Rcout << "UpdBetaAlone  :" << std::setw(15) << UpdBetaAlone << std::endl;
	Rcpp::Rcout << "HasLockCon    :" << std::setw(15) << HasLockCon << std::endl;
};

void Manifold::CheckIntrExtr(Variable *x) const
{
	Rcpp::Rcout << "==============Check Intrinsic/Extrinsic transform=========" << std::endl;
	Vector *exetax = EMPTYEXTR->ConstructEmpty();
	Vector *inetax = EMPTYINTR->ConstructEmpty();

	exetax->RandGaussian();
	ExtrProjection(x, exetax, exetax);
	exetax->Print("exetax1");
	ObtainIntr(x, exetax, inetax);
	inetax->Print("inetax1");
	ObtainExtr(x, inetax, exetax);
	exetax->Print("exetax2");
	ObtainIntr(x, exetax, inetax);
	inetax->Print("inetax2");
	Rcpp::Rcout << "exeta1 and inetax1 should approximately equal exetax2 and inetax2 respectively!" << std::endl;

	delete exetax;
	delete inetax;
};

void Manifold::CheckRetraction(Variable *x) const
{
	Rcpp::Rcout << "==============Check Retraction=========" << std::endl;
	Vector *etax, *FDetax;
	etax = EMPTYEXTR->ConstructEmpty();
	FDetax = EMPTYEXTR->ConstructEmpty();
	etax->RandGaussian();
	ExtrProjection(x, etax, etax);
	etax->Print("etax:");
	double eps = 1e-5;
	Variable *y = x->ConstructEmpty();
	ScaleTimesVector(x, eps, etax, etax);
	if (IsIntrApproach)
	{
		Vector *inetax = EMPTYINTR->ConstructEmpty();
		ObtainIntr(x, etax, inetax);
		Retraction(x, inetax, y);
		delete inetax;
	}
	else
	{
		Retraction(x, etax, y);
	}
	VectorMinusVector(x, y, x, FDetax);
	ScaleTimesVector(x, 1.0 / eps, FDetax, FDetax);
	FDetax->Print("FDetax:");

	Rcpp::Rcout << "etax should approximately equal FDetax = (R(eps etax)-R(etax))/eps!" << std::endl;
	delete etax;
	delete FDetax;
	delete y;
};

void Manifold::CheckDiffRetraction(Variable *x, bool IsEtaXiSameDir) const
{
	Rcpp::Rcout << "==============Check Differentiated Retraction=========" << std::endl;
	Vector *etax, *xix, *zetax;
	etax = EMPTYEXTR->ConstructEmpty();
	xix = EMPTYEXTR->ConstructEmpty();
	zetax = EMPTYEXTR->ConstructEmpty();
	etax->RandGaussian();
	ExtrProjection(x, etax, etax);
	if (IsEtaXiSameDir)
	{
		etax->CopyTo(xix);
	}
	else
	{
		xix->RandGaussian();
		ExtrProjection(x, xix, xix);
	}
	Variable *y = x->ConstructEmpty();
	if (IsIntrApproach)
	{
		Vector *inetax = EMPTYINTR->ConstructEmpty();
		Vector *inxix = EMPTYINTR->ConstructEmpty();
		Vector *inzetax = EMPTYINTR->ConstructEmpty();
		ObtainIntr(x, etax, inetax);
		ObtainIntr(x, xix, inxix);
		Retraction(x, inetax, y);
		DiffRetraction(x, inetax, y, inxix, inzetax, IsEtaXiSameDir);
		ObtainExtr(y, inzetax, zetax);
		delete inetax;
		delete inxix;
		delete inzetax;
	}
	else
	{
		Retraction(x, etax, y);
		DiffRetraction(x, etax, y, xix, zetax, IsEtaXiSameDir);
	}
	y->Print("y:");
	zetax->Print("zetax:");
	Variable *yeps = x->ConstructEmpty();
	double eps = 1e-5;
	ScalerVectorAddVector(x, eps, xix, etax, etax);
	if (IsIntrApproach)
	{
		Vector *inetax = EMPTYINTR->ConstructEmpty();
		ObtainIntr(x, etax, inetax);
		Retraction(x, inetax, yeps);
		delete inetax;
	}
	else
	{
		Retraction(x, etax, yeps);
	}
	VectorMinusVector(x, yeps, y, zetax);
	ScaleTimesVector(x, 1.0 / eps, zetax, zetax);
	zetax->Print("FDzetax:");
	Rcpp::Rcout << "zetax = T_{R_etax} xix should approximately equal FDzetax = (R(etax+eps xix) - R(etax))/eps!" << std::endl;

	delete etax;
	delete xix;
	delete zetax;
	delete yeps;
	delete y;
};

void Manifold::CheckLockingCondition(Variable *x) const
{
	Rcpp::Rcout << "==============Check Locking Condition=========" << std::endl;
	Vector *etax, *xix, *zetax;
	etax = EMPTYEXTR->ConstructEmpty();
	xix = EMPTYEXTR->ConstructEmpty();
	zetax = EMPTYEXTR->ConstructEmpty();
	etax->RandGaussian();
	ExtrProjection(x, etax, etax);
	ScaleTimesVector(x, genrand_real2() + 0.5, etax, xix);
	Variable *y = x->ConstructEmpty();
	if (IsIntrApproach)
	{
		Vector *inetax = EMPTYINTR->ConstructEmpty();
		Vector *inxix = EMPTYINTR->ConstructEmpty();
		Vector *inzetax = EMPTYINTR->ConstructEmpty();
		ObtainIntr(x, etax, inetax);
		ObtainIntr(x, xix, inxix);
		Retraction(x, inetax, y);
		DiffRetraction(x, inetax, y, inxix, inzetax, true);
		if (inetax->TempDataExist("beta"))
		{
			const SharedSpace *beta = inetax->ObtainReadTempData("beta");
			const double *betav = beta->ObtainReadData();
			Rcpp::Rcout << "beta = |etax| / |T_{etax} etax|:" << betav[0] << std::endl;
		}
		else
		{
			Rcpp::Rcout << "beta:" << 1 << std::endl;
		}
		Rcpp::Rcout << "|xix| / |T_{etax} xix|:" << sqrt(Metric(x, inxix, inxix) / Metric(x, inzetax, inzetax)) << std::endl;
		ScaleTimesVector(x, sqrt(Metric(x, inxix, inxix) / Metric(x, inzetax, inzetax)),
			inzetax, inzetax);
		ObtainExtr(y, inzetax, zetax);
		zetax->Print("Beta DiffRetraction zetax:");
		VectorTransport(x, inetax, y, inxix, inzetax);
		ObtainExtr(y, inzetax, zetax);
		zetax->Print("Vector Transport zetax:");
		delete inetax;
		delete inxix;
		delete inzetax;
	}
	else
	{
		Retraction(x, etax, y);
		DiffRetraction(x, etax, y, xix, zetax, true);
		if (etax->TempDataExist("beta"))
		{
			const SharedSpace *beta = etax->ObtainReadTempData("beta");
			const double *betav = beta->ObtainReadData();
			Rcpp::Rcout << "beta = |etax| / |T_{etax} etax|:" << betav[0] << std::endl;
		}
		else
		{
			Rcpp::Rcout << "beta:" << 1 << std::endl;
		}
		Rcpp::Rcout << "|xix| / |T_{etax} xix|:" << sqrt(Metric(x, xix, xix) / Metric(x, zetax, zetax)) << std::endl;
		ScaleTimesVector(x, sqrt(Metric(x, xix, xix) / Metric(x, zetax, zetax)),
			zetax, zetax);
		zetax->Print("Beta DiffRetraction zetax:");
		VectorTransport(x, etax, y, xix, zetax);
		zetax->Print("Vector Transport zetax:");
	}
	Rcpp::Rcout << "Beta DiffRetraction zetax should approximately equal Vector Transport zetax!" << std::endl;

	delete etax;
	delete xix;
	delete zetax;
	delete y;
};

void Manifold::CheckcoTangentVector(Variable *x) const
{
	Rcpp::Rcout << "==============Check CoTangentVector=========" << std::endl;
	Vector *etax, *xix, *zetay, *xiy, *zetax;
	etax = EMPTYEXTR->ConstructEmpty();
	xix = EMPTYEXTR->ConstructEmpty();
	zetay = EMPTYEXTR->ConstructEmpty();
	zetax = EMPTYEXTR->ConstructEmpty();
	xiy = EMPTYEXTR->ConstructEmpty();
	etax->RandGaussian();
	ExtrProjection(x, etax, etax);

	xix->RandGaussian();
	ExtrProjection(x, xix, xix);

	Variable *y = x->ConstructEmpty();
	if (IsIntrApproach)
	{
		Vector *inetax = EMPTYINTR->ConstructEmpty();
		Vector *inxix = EMPTYINTR->ConstructEmpty();
		Vector *inzetay = EMPTYINTR->ConstructEmpty();
		Vector *inxiy = EMPTYINTR->ConstructEmpty();
		Vector *inzetax = EMPTYINTR->ConstructEmpty();
		ObtainIntr(x, etax, inetax);
		ObtainIntr(x, xix, inxix);
		Retraction(x, inetax, y);
		DiffRetraction(x, inetax, y, inxix, inzetay, false);
		ObtainExtr(y, inzetay, zetay);

		xiy->RandGaussian();
		ExtrProjection(y, xiy, xiy);
		ObtainIntr(y, xiy, inxiy);
		Rcpp::Rcout << "<xiy, T_{R_{eta}} xix>:" << Metric(y, inxiy, inzetay) << std::endl;

		coTangentVector(x, inetax, y, inxiy, inzetax);
		ObtainExtr(x, inzetax, zetax);
		Rcpp::Rcout << "C(x, etax, xiy) [xix]:" << Metric(x, inzetax, inxix) << std::endl;
		delete inetax;
		delete inxix;
		delete inzetay;
		delete inxiy;
		delete inzetax;
	}
	else
	{
		Retraction(x, etax, y);
		DiffRetraction(x, etax, y, xix, zetay, false);
		xiy->RandGaussian();
		ExtrProjection(y, xiy, xiy);
		ScaleTimesVector(y, sqrt(Metric(y, xiy, xiy)), xiy, xiy);
		Rcpp::Rcout << "<xiy, T_{R_{eta}} xix>:" << Metric(y, xiy, zetay) << std::endl;
		coTangentVector(x, etax, y, xiy, zetax);
		Rcpp::Rcout << "C(x, etax, xiy) [xix]:" << Metric(x, zetax, xix) << std::endl;
	}
	Rcpp::Rcout << "<xiy, T_{R_{eta}} xix> should approximately equal C(x, etax, xiy) [xix]!" << std::endl;


	delete etax;
	delete xix;
	delete zetay;
	delete zetax;
	delete xiy;
	delete y;
};

void Manifold::CheckIsometryofVectorTransport(Variable *x) const
{
	Rcpp::Rcout << "==============Check Isometry of the Vector Transport=========" << std::endl;
	Vector *etax, *xix, *zetay;
	etax = EMPTYEXTR->ConstructEmpty();
	xix = EMPTYEXTR->ConstructEmpty();
	zetay = EMPTYEXTR->ConstructEmpty();
	etax->RandGaussian();
	ExtrProjection(x, etax, etax);

	xix->RandGaussian();
	ExtrProjection(x, xix, xix);

	Variable *y = x->ConstructEmpty();
	if (IsIntrApproach)
	{
		Vector *inetax = EMPTYINTR->ConstructEmpty();
		Vector *inxix = EMPTYINTR->ConstructEmpty();
		Vector *inzetay = EMPTYINTR->ConstructEmpty();
		ObtainIntr(x, etax, inetax);
		ObtainIntr(x, xix, inxix);
		Retraction(x, inetax, y);
		VectorTransport(x, inetax, y, inxix, inzetay);
		Rcpp::Rcout << "Before vector transport:" << Metric(x, inxix, inxix)
			<< ", After vector transport:" << Metric(y, inzetay, inzetay) << std::endl;
		delete inetax;
		delete inxix;
		delete inzetay;
	}
	else
	{
		Retraction(x, etax, y);
		VectorTransport(x, etax, y, xix, zetay);
		Rcpp::Rcout << "Before vector transport:" << Metric(x, xix, xix)
			<< ", After vector transport:" << Metric(y, zetay, zetay) << std::endl;
	}
	Rcpp::Rcout << "|xix| (Before vector transport) should approximately equal |T_{R_etax} xix| (After vector transport)" << std::endl;

	delete etax;
	delete xix;
	delete zetay;
	delete y;
};

void Manifold::CheckIsometryofInvVectorTransport(Variable *x) const
{
	Rcpp::Rcout << "==============Check Isometry of the Inverse Vector Transport=========" << std::endl;
	Vector *etax, *xix, *zetay;
	etax = EMPTYEXTR->ConstructEmpty();
	xix = EMPTYEXTR->ConstructEmpty();
	zetay = EMPTYEXTR->ConstructEmpty();

	etax->RandGaussian();
	ExtrProjection(x, etax, etax);

	Variable *y = x->ConstructEmpty();
	if (IsIntrApproach)
	{
		Vector *inetax = EMPTYINTR->ConstructEmpty();
		Vector *inxix = EMPTYINTR->ConstructEmpty();
		Vector *inzetay = EMPTYINTR->ConstructEmpty();
		ObtainIntr(x, etax, inetax);
		Retraction(x, inetax, y);
		zetay->RandGaussian();
		ExtrProjection(y, zetay, zetay);
		ScaleTimesVector(y, sqrt(Metric(y, zetay, zetay)), zetay, zetay);
		ObtainIntr(y, zetay, inzetay);

		InverseVectorTransport(x, inetax, y, inzetay, inxix);
		Rcpp::Rcout << "Before inverse vector transport:" << Metric(y, inzetay, inzetay)
			<< ", After inverse vector transport:" << Metric(x, inxix, inxix) << std::endl;
		delete inetax;
		delete inxix;
		delete inzetay;
	}
	else
	{
		Retraction(x, etax, y);
		zetay->RandGaussian();
		ExtrProjection(x, zetay, zetay);
		InverseVectorTransport(x, etax, y, zetay, xix);
		Rcpp::Rcout << "Before inverse vector transport:" << Metric(y, zetay, zetay)
			<< ", After inverse vector transport:" << Metric(x, xix, xix) << std::endl;
	}
	Rcpp::Rcout << "|zetay| (Before inverse vector transport) should approximately equal |T_{R_etax}^{-1} zetay| (After inverse vector transport)" << std::endl;

	delete etax;
	delete xix;
	delete zetay;
	delete y;
};

void Manifold::CheckVecTranComposeInverseVecTran(Variable *x) const
{
	Rcpp::Rcout << "==============Check Vector Transport Compose Inverse Vector Transport=========" << std::endl;
	Vector *etax, *xix, *zetay;
	etax = EMPTYEXTR->ConstructEmpty();
	xix = EMPTYEXTR->ConstructEmpty();
	zetay = EMPTYEXTR->ConstructEmpty();

	etax->RandGaussian();
	ExtrProjection(x, etax, etax);
	xix->RandGaussian();
	ExtrProjection(x, xix, xix);

	Variable *y = x->ConstructEmpty();
	if (IsIntrApproach)
	{
		Vector *inetax = EMPTYINTR->ConstructEmpty();
		Vector *inxix = EMPTYINTR->ConstructEmpty();
		Vector *inzetay = EMPTYINTR->ConstructEmpty();
		ObtainIntr(x, etax, inetax);
		Retraction(x, inetax, y);
		ObtainIntr(x, xix, inxix);
		xix->Print("xix:");
		VectorTransport(x, inetax, y, inxix, inzetay);
		InverseVectorTransport(x, inetax, y, inzetay, inxix);
		ObtainExtr(x, inxix, xix);
		xix->Print("T^{-1} ciric T xix:");
		Rcpp::Rcout << "xix and T^{-1} ciric T xix should be similar!" << std::endl;
		delete inetax;
		delete inxix;
		delete inzetay;
	}
	else
	{
		Retraction(x, etax, y);
		xix->Print("xix:");
		VectorTransport(x, etax, y, xix, zetay);
		InverseVectorTransport(x, etax, y, zetay, xix);
		xix->Print("T^{-1} ciric T xix:");
		Rcpp::Rcout << "xix and T^{-1} ciric T xix should be similar!" << std::endl;
	}
	delete etax;
	delete xix;
	delete zetay;
	delete y;
};

void Manifold::CheckTranHInvTran(Variable *x) const
{
	Rcpp::Rcout << "==============Check Transport of a Hessian approximation=========" << std::endl;
	Vector *etax;
	Variable *y;
	LinearOPE *Hx, *result;
	
	etax = EMPTYEXTR->ConstructEmpty();
	etax->RandGaussian();
	ExtrProjection(x, etax, etax);

	y = x->ConstructEmpty();
	if (IsIntrApproach)
	{
		Vector *inetax = EMPTYINTR->ConstructEmpty();
		ObtainIntr(x, etax, inetax);
		Retraction(x, inetax, y);
		Hx = new LinearOPE(EMPTYINTR->Getlength());
		Hx->ScaledIdOPE();
		Hx->Print("Hx before:");
		result = new LinearOPE(EMPTYINTR->Getlength());
		TranHInvTran(x, inetax, y, Hx, result);

		result->Print("Hx after:");
		delete inetax;
	}
	else
	{
		Hx = new LinearOPE(EMPTYEXTR->Getlength());
		Hx->ScaledIdOPE();
		Hx->Print("Hx before:");
		result = new LinearOPE(EMPTYEXTR->Getlength());
		Retraction(x, etax, y);
		Vector *zetay1 = EMPTYEXTR->ConstructEmpty();
		Vector *zetay2 = EMPTYEXTR->ConstructEmpty();
		zetay1->RandGaussian();
		ExtrProjection(y, zetay1, zetay1);
		TranHInvTran(x, etax, y, Hx, result);
		result->Print("Hx after:");
		zetay1->Print("zetay:");
		LinearOPEEta(y, result, zetay1, zetay2);
		zetay2->Print("Hx zetay:");
		delete zetay1;
		delete zetay2;
	}

	delete etax;
	delete y;
	delete Hx;
	delete result;
};

void Manifold::CheckHaddScaledRank1OPE(Variable *x) const
{
	Rcpp::Rcout << "==============Check Rank one Update to a Hessian Approximation=========" << std::endl;
	LinearOPE *Hx, *result;
	double scaler = 1.0;
	Vector *etax, *xix;
	etax = EMPTYEXTR->ConstructEmpty();
	etax->RandGaussian();
	ExtrProjection(x, etax, etax);

	xix = EMPTYEXTR->ConstructEmpty();
	xix->RandGaussian();
	ExtrProjection(x, xix, xix);

	if (IsIntrApproach)
	{
		Vector *inetax = EMPTYINTR->ConstructEmpty();
		Vector *inxix = EMPTYINTR->ConstructEmpty();
		ObtainIntr(x, etax, inetax);
		ObtainIntr(x, xix, inxix);
		Hx = new LinearOPE(EMPTYINTR->Getlength());
		Hx->ScaledIdOPE();
		Hx->Print("Hx before:");
		result = new LinearOPE(EMPTYINTR->Getlength());
		HaddScaledRank1OPE(x, Hx, scaler, inetax, inxix, result);
		inetax->Print("etax:");
		inxix->Print("xix:");
		result->Print("Hx after:");
		delete inetax;
		delete inxix;
	}
	else
	{
		Hx = new LinearOPE(EMPTYEXTR->Getlength());
		Hx->ScaledIdOPE();
		Hx->Print("Hx before:");
		result = new LinearOPE(EMPTYEXTR->Getlength());
		HaddScaledRank1OPE(x, Hx, scaler, etax, xix, result);
		etax->Print("etax:");
		xix->Print("xix:");
		result->Print("Hx after:");
	}
	delete Hx;
	delete result;
	delete etax;
	delete xix;
};

void Manifold::SetParams(PARAMSMAP params)
{
	PARAMSMAP::iterator iter;
	for (iter = params.begin(); iter != params.end(); iter++)
	{
		if (iter->first == static_cast<std::string> ("HasHHR"))
		{
			SetHasHHR(((static_cast<integer> (iter->second)) != 0));
		}
	}
};
