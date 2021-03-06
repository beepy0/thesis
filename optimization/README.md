**Original README from source: Sketches for Size of Join Estimation. Not to be confused with this thesis's current implementation.**
------------------------------------
This package contains the implementation in C++ of the sketching algorithms for size of join estimation. The content of the package can be divided into the following main categories:

A) Schemes for generating pseudo-random numbers
-----------------------------------------------
+/-1 random variables and b-valued hashes are at the core of all the sketching techniques. The package contains efficient implementations of the generating schemes based on BCH codes, Reed-Muller codes, and Carter-Wegman hashes. Implementations for fast range-summation and dyadic mappings of the +/-1 random variables are also provided.
For details see the papers:<br/>
	1) "Fast Range-Summable Random Variables for Efficient Aggregate Estimation" by F. Rusu and A. Dobra<br/>
	2) "Pseudo-Random Number Generation for Sketch-Based Estimations" by F. Rusu and A. Dobra<br/>
Files:<br/>
	gen_scheme.h<br/>
	range_sum.h<br/>
	RM7_range_sum.h<br/>
	xis.h<br/>
	xis.cpp<br/>

B) Sketches for size of join estimation
---------------------------------------
The package contains the implementation of the known sketches for size of join and second frequency moment estimation. It includes the original AGMS sketches and the improved versions Fast-AGMS sketches, Fast-Count sketches, and Count-Min sketches.
For details consult the paper:<br/>
	1) "Statistical Analysis of Sketch Estimators" by F. Rusu and A. Dobra<br/>
Files:<br/>
	sketches.h<br/>
	sketches.cpp<br/>

C) Sketches over sampled data
-----------------------------
The update performance of sketches can be significantly improved if only a sample of the data is sketched, without significant degradation in the accuracy. Simple sampling algorithms like Bernoulli sampling, sampling without replacement, and sampling with replacement, are also included in the package.<br/>
Files:<br/>
	sampling.h<br/>
	sampling.cpp<br/>

D) Synthetic data generator
---------------------------
The package contains a simple data generator for different distributions with customizable parameters. This allows the testing of the implemented algorithms.<br/>
Files:<br/>
	data_generator.h<br/>
	relation.h<br/>
	relation.cpp<br/>

E) Examples
-----------
Two usage examples are included in the package. The first example generates size of join estimations with all the implemented sketching techniques. The second example is for sketching Bernoulli samples.<br/>
Files:<br/>
	sketch_compare.cpp<br/>
	sketch_sampling.cpp<br/>


Requirements
------------
In order to compile the package, the GSL development library has to be installed.<br/>


Download
--------
The package can be found in the file sketches.tar.gz.<br/>
