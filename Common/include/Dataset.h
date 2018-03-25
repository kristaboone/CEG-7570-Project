#pragma once
#include "DataClass.h"
#include <string>

class Dataset
{
public:
	Dataset(const std::string& inputFile);
	Dataset(const size_t nfeatures, const size_t nclasses, const size_t npoints, const std::vector<DataClass> classes);

	Dataset split(bool removeRemaining);

	void write(std::ostream& out, const std::string& delim) const;
	void normalize(const double& a, const double& b);

	size_t getNPoints() const;
	size_t getNClasses() const;
	size_t getNFeatures() const;
	const DataClass& getClass(const size_t& classID) const;

	double calculateFDRForFeature(const size_t& featureID) const;	

private:
	std::vector<DataClass> classes_;
	size_t nfeatures_;
	size_t nclasses_;
	size_t npoints_;

	size_t getMinPointsInClass() const;
	std::vector<std::string> splitLineIntoList(const std::string& line) const;
};