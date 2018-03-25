#pragma once
#include <ostream>
#include <vector>

class DataClass
{
public:
	DataClass(const size_t& id);
	DataClass(const size_t& id, const std::vector< std::vector<double> >& datapoints);

	void write(std::ostream& out, const std::string& delim) const;

	std::vector<std::vector<double>> split();
	std::vector<std::vector<double>> splitEven(const size_t& nPointsToRemove);

	void normalizeFeature(const double& a, const double& b, const double& min,
		                  const double& max, const size_t& featureID);
	
	size_t getID() const;
	size_t getNPoints() const;
	bool addDataPoint(const std::vector<double>& datapoint);
	double getPoint(const size_t& pointNum, const size_t& featureID) const;

	double getFeatureMin(const size_t& featureID) const;
	double getFeatureMax(const size_t& featureID) const;
	double calculateFeatureMean(const size_t& featureID) const;
	double calculateFeatureVariance (const size_t& featureID) const;

private:
	size_t id_;
	std::vector< std::vector<double> > dataPoints_;
};