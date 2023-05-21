#pragma once

#include <Eigen/Dense>
#include <Eigen/Core>
#include <Eigen/SVD>

class  LeastSquaresSolver
{
public:
	void Initialize(int nSources, int nPowers)
	{
		mnSources = nSources;
		mnPowers = nPowers;

		mLia.resize(nSources);
		mF.resize(nSources);

		mE.resize(nSources, nPowers);
		mEp.resize(nSources, nPowers);
		mPEp.resize(nPowers, nSources);

		mX.resize(nPowers);
		mZ.resize(nPowers);
		mW.resize(nPowers);

		mbZ.resize(nPowers);
		mbP.resize(nPowers);
	}

	template<class T, class P> void Solve(T& srcLIA, T& srcGamma, T& map, P& powers, int nPixels, int offset)
	{
		double tol = 8e-15;

		for (int nPixel = 0; nPixel < nPixels; ++nPixel)
		{
			for (int i = 0; i < mnSources; ++i)
			{
				mLia[i] = (double)srcLIA[i][nPixel + offset];
				mF[i] = (double)srcGamma[i][nPixel + offset];

				// Check NaN
			}

			for (int i = 0; i < mnSources; ++i)
			{
				double cosLia =cos(mLia[i]);
				for (int j = 0; j < mnPowers; ++j)
				{
					mE(i, j) = pow(cosLia, double(powers[j]));
				}
			}

			mX.setZero();

			for (int i = 0; i < mnPowers; ++i)
			{
				mbZ[i] = true;
				mbP[i] = false;
			}

			while(true)
			{
				mW = mE.transpose() * (mF - mE * mX);

				bool nextPixel = true;
				for (int j = 0; j < mnPowers; ++j)
				{
					if (mbZ[j] && mW[j] > tol)
					{
						nextPixel = false;
					}
				}
				if (nextPixel) break;

				int t = 0;
				double maxW = DBL_MIN;
				for(int j = 0; j < mnPowers; ++j)
				{
					if (mbZ[j] && mW[j] > maxW)
					{
						maxW = mW[j];
						t = j;
					}
				}
				mbZ[t] = false;
				mbP[t] = true;

				bool resolve = true;

				while (true)
				{
					if (resolve)
					{
						mEp = mE;

						for (int j = 0; j < mnPowers; ++j)
						{
							if (mbZ[j])
							{
								for (int i = 0; i < mnSources; ++i)
								{
									mEp(i, j) = 0;
								}
							}
						}

						mEp.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV).pinv(mPEp);
						mZ = mPEp * mF;

						for (int j = 0; j < mnPowers; ++j)
						{
							if (mbZ[j]) mZ[j] = 0;
						}

						bool copyAndBreak = true;
						for (int j = 0; j < mnPowers; ++j)
						{
							if (mbP[j] && mZ[j] < tol)
							{
								copyAndBreak = false;
							}
						}

						if (copyAndBreak)
						{
							mX = mZ;
							break;
						}
					}

					double alpha = DBL_MAX;
					for(int j = 0; j < mnPowers; ++j)
					{
						if (mbP[j] && mZ[j] < tol)
						{
							double tmp = mX[j] / (mX[j] - mZ[j]);
							if (tmp < alpha)
							{
								alpha = tmp;
							}
						}
					}

					for (int i = 0; i < mnPowers; ++i)
					{
						mX[i] = mX[i] + alpha * (mZ[i] - mX[i]);
					}

					resolve = false;
					for(int j = 0; j < mnPowers; ++j)
					{
						if (abs(mX[j]) < tol)
						{
							resolve = true;
							mbP[j] = false;
							mbZ[j] = true;
						}
					}
				}
			}

			for (int i = 0; i < mnSources; ++i)
			{
				map[i][nPixel + offset] = 0.1f * float(mX[i]);
			}
		}
	}

private:
	Eigen::VectorXd mLia, mF;
	Eigen::MatrixXd mE, mEp, mPEp;
	Eigen::VectorXd mX, mZ, mW;

	std::vector<bool> mbZ, mbP;

	int mnSources, mnPowers;
};