#include "Dataset.h"
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>

Dataset::Dataset(const std::string& inputFile)
{
	std::ifstream in(inputFile);

	// If input file does not exist, throw error
	if (!in)
	{
		throw std::runtime_error("Unable to find input file: " + inputFile + " to create dataset.");
	}
	else
	{
		std::string line = "";
		std::getline(in, line);
		std::stringstream ss(line);

		// Try to read the first line of the file with the description
		if (!(ss >> npoints_ >> nfeatures_ >> nclasses_))
		{
			throw std::runtime_error("Unexpected file format.\nFirst line should read <number of points> <number of features> <number of classes>.");
		}
		else
		{
			// EXPECTED FORMAT: <feature1> <feature2> ... <featureN> <classID>
			std::vector<size_t> classesFound;

			// Read the rest of the file to create the dataset
			while (std::getline(in, line))
			{
				std::vector<std::string> inputline = splitLineIntoList(line);

				// Create new class if new class found in file
				size_t classID = std::stoi(inputline.back());

				auto it = std::find(classesFound.begin(), classesFound.end(), classID);
				if (it == classesFound.end())
				{
					classes_.push_back( DataClass(classID) );
					classesFound.push_back( classID );
				}

				// Add data to existing class
				for (auto c = classes_.begin(); c != classes_.end(); ++c)
				{
					if (c->getID() == classID)
					{
						std::vector<double> points(nfeatures_);
						for (size_t p = 0; p < points.size(); ++p)
						{
							points[p] = std::stod(inputline[p]);
						}

						if (!c->addDataPoint(points))
						{
							throw std::runtime_error("Invalid number of feature points");
						}
					}
				}
			}

			if (classes_.size() != nclasses_)
			{
				throw std::runtime_error("Did not find correct number of classes.");
			}
		}
	}
}

Dataset::Dataset(const size_t nfeatures, const size_t nclasses, const size_t npoints, const std::vector<DataClass> classes)
	: nfeatures_(nfeatures), nclasses_(nclasses), npoints_(npoints)
{
	for (auto it = classes.begin(); it != classes.end(); ++it)
	{
		classes_.push_back(*it);
	}
}

Dataset Dataset::split(bool removeRemaining)
{
	size_t npoints = std::ceil((double)this->getMinPointsInClass() / 2.0);
	npoints_ = (removeRemaining) ? npoints*nclasses_ : npoints_-npoints;

	std::vector<DataClass> newclasses;
	for (auto c = classes_.begin(); c != classes_.end(); ++c)
	{
		newclasses.push_back( DataClass(c->getID(), c->split(npoints, removeRemaining)) );
	}

	return Dataset(nfeatures_, nclasses_, npoints*nclasses_, newclasses);
}

void Dataset::write(std::ostream& out, const std::string& delim) const
{
	out << npoints_ << delim << nfeatures_ << delim << nclasses_ << std::endl;

	for (size_t i = 0; i < classes_.size(); ++i)
	{
		classes_[i].write(out, delim);
	}
}

void Dataset::normalize(const double& a, const double& b)
{
	std::vector<double> mins;
	std::vector<double> maxs;

	// calculate min and max of each feature
	for (size_t i = 0; i < nfeatures_; ++i)
	{
		double min = DBL_MAX;
		double max = -DBL_MAX;

		for (auto it = classes_.begin(); it != classes_.end(); ++it)
		{
			double tmpMin = it->getFeatureMin(i);
			double tmpMax = it->getFeatureMax(i);

			if (tmpMin < min) min = tmpMin;
			if (tmpMax > max) max = tmpMax;
		}

		mins.push_back(min); // add min for this feature
		maxs.push_back(max); // add max for this feature
	}

	// Normalize feature in each class
	for (size_t i = 0; i < mins.size(); ++i)
	{
		for (auto it = classes_.begin(); it != classes_.end(); ++it)
		{
			it->normalizeFeature(a, b, mins[i], maxs[i], i);
		}
	}
}

size_t Dataset::getNClasses() const
{
	return nclasses_;
}

size_t Dataset::getNFeatures() const
{
	return nfeatures_;
}

const DataClass& Dataset::getClass(const size_t& classID) const
{
	for (auto it = classes_.begin(); it != classes_.end(); ++it)
	{
		if (it->getID() == classID)
		{
			return *it;
		}
	}

	throw std::runtime_error("Unable to find class in dataset");
}

double Dataset::calculateFDRForFeature(const size_t& featureID) const
{
	double sum = 0.0;

	for (size_t i = 0; i < classes_.size(); ++i)
	{
		for (size_t j = 0; j < classes_.size(); ++j)
		{
			if (j != i)
			{
				double c1Mean = classes_[i].calculateFeatureMean(featureID);
				double c2Mean = classes_[j].calculateFeatureMean(featureID);

				double c1Var = classes_[i].calculateFeatureVariance(featureID);
				double c2Var = classes_[j].calculateFeatureVariance(featureID);

				double num = pow((c1Mean - c2Mean), 2.0);
				double den = c1Var + c2Var;

				sum += num / den;
			}
		}
	}

	return sum;
}

size_t Dataset::getMinPointsInClass() const
{
	size_t min = INT_MAX;
	for (size_t i = 0; i < classes_.size(); ++i)
	{
		size_t n = classes_[i].getNPoints();
		if (n < min) min = n;
	}
	return min;
}

std::vector<std::string> Dataset::splitLineIntoList(const std::string& line) const
{
	std::stringstream ss(line);

	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> strings(begin, end);

	return strings;
}