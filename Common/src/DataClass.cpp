#include "DataClass.h"

/*
	DataClass::DataClass(constructor)
	- Create data class with an id = id
*/
DataClass::DataClass(size_t id)
: id_(id)
{ /* NO-OP */ }

/*
	DataClass::DataClass(constructor)
	- Create data class with an id = id and datapoints = datapoints
*/
DataClass::DataClass(size_t id, const std::vector< std::vector<double> >& datapoints)
: id_(id), dataPoints_(datapoints)
{ /* NO-OP */ }

/*
	DataClass::getID()
	- Returns class ID
*/
size_t DataClass::getID() const
{ return id_; }

/*
	DataClass::getNPoints()
	- Returns the number of points in the class
*/
size_t DataClass::getNPoints() const
{ return dataPoints_.size(); }

/*
	DataClass::getPoint()
	- Return the feature value at point pointNum
*/
double DataClass::getPoint(size_t pointNum, size_t featureID) const
{ return dataPoints_[pointNum][featureID]; }

/*
	DataClass::write()
	- Writes out the class's datapoints with a particular delimiter
*/
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

/*
	DataClass::split()
	- Removes and returns the second half of the points in the class.
*/
std::vector<std::vector<double>> DataClass::split()
{
	size_t nPointsToRemove = dataPoints_.size() / 2.0;

	std::vector<std::vector<double>> newPoints;
	for (size_t i = 0; i < nPointsToRemove; ++i)
	{
		newPoints.push_back(dataPoints_.back());
		dataPoints_.pop_back();
	}
	return newPoints;
}

/*
	DataClass::splitEven()
	- Removes and returns the last nPointsToRemove points in the class.
*/
std::vector<std::vector<double>> DataClass::splitEven(size_t nPointsToRemove)
{
	std::vector<std::vector<double>> newPoints;
	for (size_t i = 0; i < nPointsToRemove; ++i)
	{
		newPoints.push_back(dataPoints_.back());
		dataPoints_.pop_back();
	}

	size_t extraPts = getNPoints() - nPointsToRemove;
	for (size_t i = 0; i < extraPts; ++i)
	{
		dataPoints_.pop_back();
	}

	return newPoints;
}

/*
	DataClass::normalizeFeature()
	- Scales a particular feature between values a and b
*/
void DataClass::normalizeFeature(double a, double b, double min, double max, size_t featureID)
{
	for (size_t i = 0; i < dataPoints_.size(); ++i)
	{
		double x = dataPoints_[i][featureID];
		dataPoints_[i][featureID] = (b - a)*((x - min) / (max - min)) + a;
	}
}

/*
	DataClass::addDataPoint()
	- Add a datapoint to the list of datapoints.
	- Number of features in a datapoint must be equal to the number of features
	  in previous datapoints.
*/
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

/*
	DataClass::calculateFeatureMean()
	- Calculate the mean of a particular feature
*/
double DataClass::calculateFeatureMean(size_t featureID) const
{
	double sum = 0.0;

	for (auto it = dataPoints_.begin(); it != dataPoints_.end(); ++it)
	{
		sum += it->at(featureID);
	}

	return sum / dataPoints_.size();
}

/*
	DataClass::calculateFeatureVariance()
	- Calculate the variance of a particular feature
*/
double DataClass::calculateFeatureVariance(size_t featureID) const
{
	double mean = this->calculateFeatureMean(featureID);
	double sqrSum = 0.0;

	for (auto it = dataPoints_.begin(); it != dataPoints_.end(); ++it)
	{
		sqrSum += pow((it->at(featureID) - mean), 2.0);
	}

	return sqrSum / (dataPoints_.size() - 1);
}

/*
	DataClass::getFeatureMin()
	- Find the minimum value for a particular feature
*/
double DataClass::getFeatureMin(size_t featureID) const
{
	double min = DBL_MAX;
	for (auto it = dataPoints_.begin(); it != dataPoints_.end(); ++it)
	{
		if (it->at(featureID) < min) min = it->at(featureID);
	}
	return min;
}

/*
	DataClass::getFeatureMax()
	- Find the maximum value for a particular feature
*/
double DataClass::getFeatureMax(size_t featureID) const
{
	double max = -DBL_MAX;
	for (auto it = dataPoints_.begin(); it != dataPoints_.end(); ++it)
	{
		if (it->at(featureID) > max) max = it->at(featureID);
	}
	return max;
}
