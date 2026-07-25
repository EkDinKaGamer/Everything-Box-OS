#pragma once

class EBOS_Calculator {
public:
 long add(long a,long b){return a+b;}
 long sub(long a,long b){return a-b;}
 long mul(long a,long b){return a*b;}
 long divi(long a,long b){return b?a/b:0;}
};
EBOS_Calculator Calculator;
