/*
	 Dataset.h
	 Created by Krista Vanderhorst (2018)

	 Holds n-dimensional, m-point feature data.
	 Uses DataClass to encapsulate feature/point access.
*/
#pragma once
#include "DataClass.h"
#include <string>

class Dataset
{
public:
	Dataset(const std::string& inputFile);
	Dataset(size_t nfeatures, size_t nclasses, size_t npoints, const std::vector<DataClass>& classes);

	Dataset splitEven();
	Dataset split();

	void write(std::ostream& out, const std::string& delim) const;
	void normalize(double a, double b);

	size_t getNPoints() const;
	size_t getNClasses() const;
	size_t getNFeatures() const;
	const DataClass& getClass(size_t classID) const;

	double calculateFDRForFeature(size_t featureID) const;

private:
	std::vector<DataClass> classes_;
	size_t nfeatures_;
	size_t nclasses_;
	size_t npoints_;

	size_t getMinPointsInClass() const;
	std::vector<std::string> splitLineIntoList(const std::string& line) const;
};
