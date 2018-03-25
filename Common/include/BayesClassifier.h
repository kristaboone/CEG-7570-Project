#pragma once
#include "Dataset.h"

class BayesClassifier
{
public:
	BayesClassifier(const Dataset& ds, const size_t& feature, const double& featureMin, const double& featureMax);
	int classify(const double& datapoint) const;

	void writeDistributions2CSV(const std::string& filename) const;

private:
	double binStep_;
	double featureMin_;
	double featureMax_;

	std::vector<int> decisionRegions_;
	std::vector<std::vector<double>> probabilityDensities_;

	double calculateClassConditionalProbability(double mean, double var, double point) const;
};