# CEG-7570-Project
Pattern Recognition Project at Wright State University (Spring 2018)

# Build
This project uses CMake to build each part of the project. To build, ensure you have
CMake installed, then follow these steps to build:

1. Open `cmd` or terminal applicationCreate
2. Create a directory in the Part<n> folder named `build` or similar
3. Go into directory
4. Type `cmake ../` to build the Part<n> source

Use your default compiler to compile the source into the build directory
you created.

# Running the Projects
To run the projects, ensure the data file is located in a path without spaces.

# Content
## Folders
Common
> Contains source code that is common to one or more parts of the project

Data
> Contains the provided datasets used with the software

Part 1
> Contains the source code for part 1 of the project

Part 2
> Contains the source code for part 2 of the project

Part 3
> Contains the source code for part 3 of the project

## Common Classes
BayesClassifier
> Builds a 1-D Bayesian classifier, which assumes a Gaussian distribution for the feature

DataClass
> Contains n-feature datapoints for a particular class and useful functions to access or edit the data

Dataset
> Contains m-class, n-feature dataset and useful functions to access or edit the dataset
