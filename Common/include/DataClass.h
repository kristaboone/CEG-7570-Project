/*
	 DataClass.h
	 Created by Krista Vanderhorst (2018)

	 Holds n-dimensional, m-point feature data.
	 All points belong to this particular class.
*/
#pragma once
#include <ostream>
#include <vector>

class DataClass
{
public:
	DataClass(size_t id);
	DataClass(size_t id, const std::vector< std::vector<double> >& datapoints);

	void write(std::ostream& out, const std::string& delim) const;

	std::vector<std::vector<double>> split();
	std::vector<std::vector<double>> splitEven(size_t nPointsToRemove);

	void normalizeFeature(double a, double b, double min,
		                    double max, size_t featureID);

	size_t getID() const;
	size_t getNPoints() const;
	bool addDataPoint(const std::vector<double>& datapoint);
	double getPoint(size_t pointNum, size_t featureID) const;

	double getFeatureMin(size_t featureID) const;
	double getFeatureMax(size_t featureID) const;
	double calculateFeatureMean(size_t featureID) const;
	double calculateFeatureVariance (size_t featureID) const;

private:
	size_t id_;
	std::vector< std::vector<double> > dataPoints_;
};
