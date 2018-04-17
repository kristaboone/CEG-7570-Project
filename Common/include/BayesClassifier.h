/*
	 BayesClassifier.h
	 Created by Krista Vanderhorst (2018)

	 Trains an n-class 1D classifier using naive Bayes decision theory.
	 Gaussian distributed input is assumed.
*/
#pragma once
#include "Dataset.h"

class BayesClassifier
{
public:
	BayesClassifier(const Dataset& ds, size_t feature, double featureMin, double featureMax);
	int classify(double datapoint) const;

	void writeDistributions2CSV(const std::string& filename) const;

private:
	double binStep_;
	double featureMin_;
	double featureMax_;

	std::vector<int> decisionRegions_;
	std::vector<std::vector<double>> probabilityDensities_;

	double calculateClassConditionalProbability(double mean, double var, double point) const;
};
