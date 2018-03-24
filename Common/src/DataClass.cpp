#include "DataClass.h"

// Initialize DataClass with ID
DataClass::DataClass(const size_t& id) : id_(id)
{ /* NO-OP */ }

// Initialize DataClass with ID and data points
DataClass::DataClass(const size_t& id, const std::vector< std::vector<double> >& datapoints) : id_(id), dataPoints_(datapoints)
{ /* NO-OP */ }

void DataClass::write(std::ostream& out, const std::string& delim) const
{
	for (size_t i = 0; i < dataPoints_.size(); ++i)
	{
		for (size_t j = 0; j < dataPoints_[i].size(); ++j)
		{
			out << std::fixed << dataPoints_[i][j] << delim.c_str();
		}
		out << id_ << std::endl;
	}
}

std::vector<std::vector<double>> DataClass::split(const size_t& nPointsToRemove)
{
	std::vector<std::vector<double>> newPoints;
	for (size_t i = 0; i < nPointsToRemove; ++i)
	{
		newPoints.push_back(dataPoints_.back());
		dataPoints_.pop_back();
	}
	return newPoints;
}

void DataClass::normalizeFeature(const double& a, const double& b, const double& min,
	const double& max, const size_t& featureID)
{
	for (size_t i = 0; i < dataPoints_.size(); ++i)
	{
		double x = dataPoints_[i][featureID];
		dataPoints_[i][featureID] = (b - a)*((x - min) / (max - min)) + a;
	}
}

// Return ID of DataClass
size_t DataClass::getID() const
{
	return id_;
}

size_t DataClass::getNPoints() const
{
	return dataPoints_.size();
}

// Add a datapoint to the datapoints.
// Length of datapoint must be equal to the
// number of features provided in previous datapoints.
bool DataClass::addDataPoint(const std::vector<double>& datapoint)
{
	size_t nPoints = dataPoints_.size();

	if (nPoints != 0 && datapoint.size() != dataPoints_[0].size())
	{
		return false;
	}
	else
	{
		dataPoints_.push_back(datapoint);
		return true;
	}
}

// Calculates mean for feature = featureID
double DataClass::calculateFeatureMean(const size_t& featureID) const
{
	double sum = 0.0;

	for (auto it = dataPoints_.begin(); it != dataPoints_.end(); ++it)
	{
		sum += it->at(featureID);
	}

	return sum / dataPoints_.size();
}

// Calculates variance for feature = featureID
double DataClass::calculateFeatureVariance(const size_t& featureID) const
{
	double mean = this->calculateFeatureMean(featureID);
	double sqrSum = 0.0;

	for (auto it = dataPoints_.begin(); it != dataPoints_.end(); ++it)
	{
		sqrSum += pow((it->at(featureID) - mean), 2.0);
	}

	return sqrSum / (dataPoints_.size() - 1);
}

double DataClass::getFeatureMin(const size_t& featureID) const
{
	double min = DBL_MAX;
	for (auto it = dataPoints_.begin(); it != dataPoints_.end(); ++it)
	{
		if (it->at(featureID) < min) min = it->at(featureID);
	}
	return min;
}

double DataClass::getFeatureMax(const size_t& featureID) const
{
	double max = -DBL_MAX;
	for (auto it = dataPoints_.begin(); it != dataPoints_.end(); ++it)
	{
		if (it->at(featureID) > max) max = it->at(featureID);
	}
	return max;
}