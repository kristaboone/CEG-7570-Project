#include "BayesClassifier.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>

/*
	BayesClassifier::BayesClassifier(constructor)
	- Builds classifier by extracting information about a particular feature.
	- Creates 1D binned array, and the class with the highest class conditional
	  probability at a particular bin will be assigned to that bin.
	- Decision regions are represented by groups of similar class IDs in the
	  array, and decision boundaries are implicitely created by a change in the
		class ID in the array.
	- Assumes 1/10000 of feature range is appropriate enough granularity for bin sizes.
*/
BayesClassifier::BayesClassifier(const Dataset& ds, size_t feature, double featureMin, double featureMax)
: featureMin_(featureMin), featureMax_(featureMax)
{
	// Calculate a-priori probabilities and probability densities for each class
	// Create bin size that is 0.0001 * the overall feature range
	binStep_ = 0.001 * (featureMax_ - featureMin_);
	size_t nBins = (featureMax_ - featureMin_) / binStep_;

	std::vector<double> posteriorProbabilityMaxes(nBins, -DBL_MAX);
	decisionRegions_ = std::vector<int>(nBins, -1);

	for (size_t i = 0; i < ds.getNClasses(); ++i)
	{
		double apriori = (double)ds.getClass(i).getNPoints() / ds.getNPoints();
		double mean = ds.getClass(i).calculateFeatureMean(feature);
		double var = ds.getClass(i).calculateFeatureVariance(feature);

		std::vector<double> probabilityDensity(nBins);
		for (size_t j = 0; j < probabilityDensity.size(); ++j)
		{
			double point = featureMin_ + (j*binStep_);
			probabilityDensity[j] = calculateClassConditionalProbability(mean, var, point);

			// Update which class has the highest probability in a particular bin
			if (probabilityDensity[j] > posteriorProbabilityMaxes[j])
			{
				posteriorProbabilityMaxes[j] = probabilityDensity[j];
				decisionRegions_[j] = i;
			}
		}

		probabilityDensities_.push_back(probabilityDensity);
	}

	// When we reach here, we should have a vector that describes the decision regions,
	// and therefore the decision boundaries between each class.
}

/*
	BayesClassifier::classify()
	- Determines the class of a datapoint by assigning it to a particular decision region
	- If the datapoint lies below the feature min or above the feature max, it is assigned
	  to the class that is closest to that value.
	- If the datapoint lies within the bounds of the training feature values, the
	  correct bin is calculated, and the class ID in that bin is returned.
*/
int BayesClassifier::classify(double datapoint) const
{
	if (datapoint <= featureMin_)
	{
		return decisionRegions_[0];
	}
	else if (datapoint >= featureMax_)
	{
		return decisionRegions_.back();
	}
	else
	{
		size_t bin = (datapoint - featureMin_) / binStep_;
		return decisionRegions_[bin];
	}
}

/*
	BayesClassifier::calculateClassConditionalProbability()
	- Calculates class conditional probability from feature mean, variance, and value.
*/
double BayesClassifier::calculateClassConditionalProbability(double mean, double var, double point) const
{
	double var1 = 1.0 / (sqrt(2.0 * M_PI)* sqrt(var));
	double var2 = exp(-pow((point - mean), 2.0) / (2.0 * var));
	return var1 * var2;
}

/*
	BayesClassifier::writeDistributions2CSV()
	- Writes probability densitities for each class to a CSV file. Used to create
	  plots used in the final report.
*/
void BayesClassifier::writeDistributions2CSV(const std::string& filename) const
{
	std::ofstream out(filename);

	for (size_t i = 0; i < probabilityDensities_[0].size(); ++i)
	{
		out << i;
		for (size_t j = 0; j < probabilityDensities_.size(); ++j)
		{
			out << "," << probabilityDensities_[j][i];
		}
		out << std::endl;
	}
}
