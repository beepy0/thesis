#ifndef _XIS
#define _XIS

/*
  Generating schemes for different types of random variables
    -> +/-1 random variables
    -> k-valued random variables
  Fast range-summable random variables
  For details see the papers:
	1) "Fast Range-Summable Random Variables for Efficient Aggregate Estimation" by F. Rusu and A. Dobra
	2) "Pseudo-Random Number Generation for Sketch-Based Estimations" by F. Rusu and A. Dobra
*/


using namespace std;



class Xi
{
  public:
    virtual double element(unsigned int j) = 0;
    virtual double interval_sum(unsigned int alpha, unsigned int beta) = 0;

    virtual ~Xi();
};


/*
+/-1 random variables that are 3-wise independent
*/

class Xi_EH3 : public Xi
{
  protected:
    unsigned int seeds[2];

  public:
    Xi_EH3(unsigned int I1, unsigned int I2);
    ~Xi_EH3() override;

    double element(unsigned int j) override;
    double interval_sum(unsigned int alpha, unsigned int beta) override;
};



class Xi_H3 : public  Xi
{
  protected:
    unsigned int no_bits;
    unsigned int *q_matrix;
    unsigned int truncation_mask;
    unsigned int floor_offset;
    unsigned int floor_value;

  public:
    Xi_H3(unsigned int seed_val, unsigned int no_bits,
          unsigned int truncation_mask, unsigned int floor_offset,
          unsigned int floor_value);
    ~Xi_H3() override;

    double element(unsigned int key) override;
    double interval_sum(unsigned int alpha, unsigned int beta) override;
};



#endif
