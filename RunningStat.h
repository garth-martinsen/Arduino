/*
RunningStat.h - Library for capturing data samples and computing the running values of mean, standard deviation, variance. 
     Created by Garth J Martinsen  January 5, 2019. Code was from URL: https://www.johndcook.com/blog/standard_deviation/.
     Released into the public domain.
 */

#ifndef RunningStat_h
   #define RunningStat_h

   #include "Arduino.h"
class RunningStat {
  public:
    RunningStat();
    void Clear();
    void Push(double x);
    int NumDataValues() const;
    double Mean() const;
    double Variance() const;
    double StandardDeviation() const;
  private:
    int m_n;
    double m_oldM, m_newM, m_oldS, m_newS;
};

 #endif
