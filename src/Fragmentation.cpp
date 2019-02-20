// Fragmentation.cpp is part of the IP-Glasma solver.
// Copyright (C) 2013 Bjoern Schenke.
#include "Fragmentation.h"

//**************************************************************************
// Fragmentation functions

// using KKP from http://www.desy.de/~poetter/kkp.html
// rewritten into C++ by Bjoern Schenke 2013
//**************************************************************************

//=====================================================================
//
//     ------------------------------------------------------------
//     Fragmentation functions for: Pions, Kaons, Protons, Neutrons
//            (includes mass-threshholds for c and b quarks)
//
//     Reference: B.A.Kniehl, G.Kramer, B.Potter, NPB582 (2000) 514
//     ------------------------------------------------------------
//
//     ih, iset, x, qs are input; dh is output.
//     ih   = 1 : (pi^+ + pi^-)  /2
//     ih   = 2 : (K^+ + K^-)    /2   
//     ih   = 3 : (K^0 + K^0_bar)/2   
//     ih   = 4 : (p + p_bar)    /2
//     ih   = 5 : (pi^0)
//     ih   = 6 : (n + n_bar)    /2
//     ih   = 7 : (h^+ + h^-)         [as sum of pions, kaons and protons]
//
//     iset = 0 : LO         
//     iset = 1 : NLO
//
//     x    = longitudinal-momentum fraction
//     qs   = fragmentation scale (in GeV)
//
//     Parton label:
//     0    1    2    3    4    5    6    7    8     9    10
//     g    u   ubar  d   dbar  s   sbar  c   cbar   b   bbar
//
//     Lambda_QCD (in GeV):
//     0.088 in LO
//     0.213 in NLO
//
//=====================================================================

double Fragmentation::kkp(int ih, int iset, double x, double qs)
{
  //(a-h,o-z)
  
  double rlam, s, sc, sb;
  // --- Mass-thresholds:
  double rmcc = 2.9788;
  double rmbb = 9.46037;
  // --- Q_0 (in GeV):
  double q0= sqrt(2.);
  // --- BAK
  if(qs < q0) 
    qs = q0;

  double b[4];
  double a[12];
  double dh[11];

  double dpg, dpu, dps, dpc, dpb;
  double dkg, dku, dks, dkc, dkb;
  double dprg, dpru, dprs, dprc, dprb;
  double dkd;

  // --- LO FFs
  if (iset == 0) 
    {
      rlam= 0.088;
      s=  log(log(qs*qs/(rlam*rlam))/log(q0*q0/(rlam*rlam)));
      sc= log(log(qs*qs/(rlam*rlam))/log(rmcc*rmcc/(rlam*rlam)));
      sb= log(log(qs*qs/(rlam*rlam))/log(rmbb*rmbb/(rlam*rlam)));

 //      cout << "s=" << s << endl;
//       cout << "sc=" << sc << endl;
//       cout << "sb=" << sb << endl;
      

// ---------------------- LO PION ------------------------------
      b[1]=                            6.04510;
      b[2]=                           -0.71378;
      b[3]=                            2.92133;
      a[1]=                           -6.61523;
      a[2]=                           -1.64978;
      a[3]=                            2.68223;
      a[4]=                            0.14705;
      a[5]=                           -1.08423;
      a[6]=                           -0.43182;
      a[7]=                            1.48429;
      a[8]=                            1.32887;
      a[9]=                           -1.78696;
      a[10]=                            0.23086;
      a[11]=                           -0.29182;
      dpg = (b[1] +a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
      b[1]=                           0.54610;
      b[2]=                          -1.46616;
      b[3]=                           1.01864;
      a[1]=                           -0.22946;
      a[2]=                           -0.22594;
      a[3]=                            0.21119;
      a[4]=                           -0.45404;
      a[5]=                           -0.12684;
      a[6]=                            0.27646;
      a[7]=                            0.95367;
      a[8]=                           -1.09835;
      a[9]=                            0.74657;
      a[10]=                           -0.01877;
      a[11]=                            0.02949;
      dpu = (b[1] +a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
 //      dpu= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
// 	.        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
// 	.        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)*
// 	.        (1d0+(a[10]*s +a[11]*s**2)/x)                  
      b[1]=                            22.2815;
      b[2]=                            0.12732;
      b[3]=                            6.13697;
      a[1]=                           -20.8125;
      a[2]=                           -11.5725;
      a[3]=                            15.5372;
      a[4]=                            0.23075;
      a[5]=                           -2.71424;
      a[6]=                            1.72456;
      a[7]=                            2.18849;
      a[8]=                           -5.04475;
      a[9]=                            3.29117;
      a[10]=                            0.09044;
      a[11]=                           -0.07589;
      dps = (b[1] +a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//          dps= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)*
//      .        (1d0+(a[10]*s +a[11]*s**2)/x)         
      b[1]=                            8.75500;
      b[2]=                           -0.38611;
      b[3]=                            5.61846;
      a[1]=                           -9.32277;
      a[2]=                            1.80600;
      a[3]=                            2.02179;
      a[4]=                           -0.41190;
      a[5]=                           -0.48496;
      a[6]=                            0.42525;
      a[7]=                            0.74035;
      a[8]=                           -0.64929;
      a[9]=                            0.66788;
      a[10]=                            0.06652;
      a[11]=                           -0.05531;
      dpc = (b[1] +a[1]*sc + a[2]*sc*sc +a[3]*sc*sc*sc)*pow(x,(b[2] +a[4]*sc + a[5]*sc*sc +a[6]*sc*sc*sc))
	*pow((1.-x),(b[3] +a[7]*sc + a[8]*sc*sc +a[9]*sc*sc*sc))*(1.+(a[10]*sc +a[11]*sc*sc)/x);                  
//       dpc= (b[1] +a[1]*sc + a[2]*sc**2 +a[3]*sc**3)*x**
//      .        (b[2] +a[4]*sc + a[5]*sc**2 +a[6]*sc**3)*(1d0-x)**
//      .        (b[3] +a[7]*sc + a[8]*sc**2 +a[9]*sc**3)*
//      .        (1d0+(a[10]*sc +a[11]*sc**2)/x)         
      b[1]=                            0.31147;
      b[2]=                           -1.92993;
      b[3]=                            3.47086;
      a[1]=                           -0.19319;
      a[2]=                           -0.10487;
      a[3]=                            0.18824;
      a[4]=                           -0.44692;
      a[5]=                           -0.08271;
      a[6]=                            0.30441;
      a[7]=                            0.79775;
      a[8]=                           -0.28091;
      a[9]=                            0.39504;
      a[10]=                           -0.04887;
      a[11]=                            0.03212;
      dpb = (b[1] +a[1]*sb + a[2]*sb*sb +a[3]*sb*sb*sb)*pow(x,(b[2] +a[4]*sb + a[5]*sb*sb +a[6]*sb*sb*sb))
	*pow((1.-x),(b[3] +a[7]*sb + a[8]*sb*sb +a[9]*sb*sb*sb))*(1.+(a[10]*sb +a[11]*sb*sb)/x);                  
//          dpb= (b[1] +a[1]*sb + a[2]*sb**2 +a[3]*sb**3)*x**
//      .        (b[2] +a[4]*sb + a[5]*sb**2 +a[6]*sb**3)*(1d0-x)**
//      .        (b[3] +a[7]*sb + a[8]*sb**2 +a[9]*sb**3)
//      .        *(1d0+(a[10]*sb +a[11]*sb**2)/x)

// ---------------------- LO KAON ------------------------------
      b[1]=                            0.02862;
      b[2]=                           -2.94091;
      b[3]=                            2.73474;
      a[1]=                           -0.02113;
      a[2]=                            0.00389;
      a[3]=                            0.00901;
      a[4]=                            0.66881;
      a[5]=                           -0.29670;
      a[6]=                            0.20574;
      a[7]=                           -0.58222;
      a[8]=                            0.04329;
      a[9]=                            0.78033;
      a[10]=                            0.03586;
      a[11]=                           -0.01220;
      dkg = (b[1] +a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//       dkg= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)
//      .        *(1d0+(a[10]*s +a[11]*s**2)/x)
      b[1]=                           0.25937;
      b[2]=                          -0.61925;
      b[3]=                           0.85946;
      a[1]=                           -0.10502;
      a[2]=                            0.00572;
      a[3]=                           -0.00269;
      a[4]=                            0.09956;
      a[5]=                            0.07389;
      a[6]=                           -0.00070;
      a[7]=                            0.57965;
      a[8]=                            0.26397;
      a[9]=                           -0.12764;
      a[10]=                            0.15303;
      a[11]=                            0.14807;
      dku = (b[1] +a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
 //      dku= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
// 	.        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
// 	.        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)
// 	.        *(1d0+(a[10]*s +a[11]*s**2)/x)
      b[1]=                            5.38115;
      b[2]=                           -0.00321;
      b[3]=                            3.07632;
      a[1]=                           -3.05084;
      a[2]=                           -1.10056;
      a[3]=                            1.31207;
      a[4]=                           -0.25889;
      a[5]=                           -0.18494;
      a[6]=                            0.13994;
      a[7]=                            1.13745;
      a[8]=                           -0.90413;
      a[9]=                            0.56581;
      a[10]=                            0.05141;
      a[11]=                           -0.00697;
      dkd = (b[1] +a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//       dkd= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
// 	.        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
// 	.        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)
// 	.        *(1d0+(a[10]*s +a[11]*s**2)/x)
      b[1]=                            5.18266;
      b[2]=                           -0.17751;
      b[3]=                            4.30306;
      a[1]=                           -3.48519;
      a[2]=                           -1.00982;
      a[3]=                            1.17996;
      a[4]=                            0.02309;
      a[5]=                           -0.61327;
      a[6]=                           -0.03532;
      a[7]=                            1.00547;
      a[8]=                           -0.51779;
      a[9]=                            0.20683;
      a[10]=                            0.13514;
      a[11]=                           -0.17778;
      dkc = (b[1] +a[1]*sc + a[2]*sc*sc +a[3]*sc*sc*sc)*pow(x,(b[2] +a[4]*sc + a[5]*sc*sc +a[6]*sc*sc*sc))
	*pow((1.-x),(b[3] +a[7]*sc + a[8]*sc*sc +a[9]*sc*sc*sc))*(1.+(a[10]*sc +a[11]*sc*sc)/x);                  
//       dkc= (b[1] +a[1]*sc + a[2]*sc**2 +a[3]*sc**3)*x**
//      .        (b[2] +a[4]*sc + a[5]*sc**2 +a[6]*sc**3)*(1d0-x)**
//      .        (b[3] +a[7]*sc + a[8]*sc**2 +a[9]*sc**3)
//      .        *(1d0+(a[10]*sc +a[11]*sc**2)/x)
      b[1]=                            1.57044;
      b[2]=                           -0.84143;
      b[3]=                            6.01488;
      a[1]=                           -1.78340;
      a[2]=                            0.57100;
      a[3]=                            0.15469;
      a[4]=                           -0.43448;
      a[5]=                           -0.05314;
      a[6]=                           -0.36621;
      a[7]=                            0.72953;
      a[8]=                           -0.64433;
      a[9]=                            0.92351;
      a[10]=                            0.01024;
      a[11]=                           -0.06160;
      dkb = (b[1] +a[1]*sb + a[2]*sb*sb +a[3]*sb*sb*sb)*pow(x,(b[2] +a[4]*sb + a[5]*sb*sb +a[6]*sb*sb*sb))
	*pow((1.-x),(b[3] +a[7]*sb + a[8]*sb*sb +a[9]*sb*sb*sb))*(1.+(a[10]*sb +a[11]*sb*sb)/x);                  
//       dkb= (b[1] +a[1]*sb + a[2]*sb**2 +a[3]*sb*sb*sb)*x**
//      .        (b[2] +a[4]*sb + a[5]*sb**2 +a[6]*sb**3)*(1d0-x)**
//      .        (b[3] +a[7]*sb + a[8]*sb**2 +a[9]*sb**3)
//      .        *(1d0+(a[10]*sb +a[11]*sb**2)/x)

// ---------------------- LO PROTON -----------------------------
      b[1]=                            0.73953;
      b[2]=                           -0.76986;
      b[3]=                            7.69079;
      a[1]=                           -1.64519;
      a[2]=                            1.01189;
      a[3]=                           -0.10175;
      a[4]=                           -3.58787;
      a[5]=                            13.8025;
      a[6]=                           -13.8902;
      a[7]=                           -2.84470;
      a[8]=                           -0.36719;
      a[9]=                           -2.21825;
      a[10]=                            1.26515;
      a[11]=                           -1.96117;
      dprg = (b[1] +a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s + 0.54769*s*s*s)/x);                  
//          dprg= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)
//      .        *(1d0+(a[10]*s +a[11]*s**2 + 0.54769*s**3)/x)
      b[1]=                           0.40211;
      b[2]=                          -0.85973;
      b[3]=                           2.80160;
      a[1]=                           -0.21633;
      a[2]=                           -0.07045;
      a[3]=                            0.07831;
      a[4]=                            0.13987;
      a[5]=                           -0.82412;
      a[6]=                            0.43114;
      a[7]=                            0.78923;
      a[8]=                           -0.05344;
      a[9]=                            0.01460;
      a[10]=                            0.05198;
      a[11]=                           -0.04623;
      dpru = (b[1] +a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//          dpru= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)
//      .        *(1d0+(a[10]*s +a[11]*s**2]/x)
      b[1]=                            4.07885;
      b[2]=                           -0.09735;
      b[3]=                            4.99191;
      a[1]=                           -2.97392;
      a[2]=                           -0.92973;
      a[3]=                            1.23517;
      a[4]=                            0.25834;
      a[5]=                           -1.52246;
      a[6]=                            0.77060;
      a[7]=                            1.14379;
      a[8]=                           -0.85320;
      a[9]=                            0.45607;
      a[10]=                            0.07174;
      a[11]=                           -0.08321;
      dprs = (b[1] +a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//          dprs= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)
//      .        *(1d0+(a[10]*s +a[11]*s**2]/x)
      b[1]=                            0.11061;
      b[2]=                           -1.54340;
      b[3]=                            2.20681;
      a[1]=                           -0.07726;
      a[2]=                            0.05422;
      a[3]=                           -0.03364;
      a[4]=                           -0.20804;
      a[5]=                            0.29038;
      a[6]=                           -0.23662;
      a[7]=                            0.62274;
      a[8]=                            0.29713;
      a[9]=                           -0.21861;
      a[10]=                            0.00831;
      a[11]=                            0.00065;
      dprc = (b[1] +a[1]*sc + a[2]*sc*sc +a[3]*sc*sc*sc)*pow(x,(b[2] +a[4]*sc + a[5]*sc*sc +a[6]*sc*sc*sc))
	*pow((1.-x),(b[3] +a[7]*sc + a[8]*sc*sc +a[9]*sc*sc*sc))*(1.+(a[10]*sc +a[11]*sc*sc)/x);                  
//       dprc= (b[1] +a[1]*sc + a[2]*sc**2 +a[3]*sc**3)*x**
//      .        (b[2] +a[4]*sc + a[5]*sc**2 +a[6]*sc**3)*(1d0-x)**
//      .        (b[3] +a[7]*sc + a[8]*sc**2 +a[9]*sc**3)
//      .        *(1d0+(a[10]*sc +a[11]*sc**2]/x)
      b[1]=                            40.0971;
      b[2]=                            0.74249;
      b[3]=                            12.3729;
      a[1]=                           -123.531;
      a[2]=                            128.666;
      a[3]=                           -29.1808;
      a[4]=                           -1.29639;
      a[5]=                           -3.65003;
      a[6]=                            3.05340;
      a[7]=                           -1.04932;
      a[8]=                            0.34662;
      a[9]=                           -1.34412;
      a[10]=                           -0.04290;
      a[11]=                           -0.30359;
      dprb = (b[1] +a[1]*sb + a[2]*sb*sb +a[3]*sb*sb*sb)*pow(x,(b[2] +a[4]*sb + a[5]*sb*sb +a[6]*sb*sb*sb))
	*pow((1.-x),(b[3] +a[7]*sb + a[8]*sb*sb +a[9]*sb*sb*sb))*(1.+(a[10]*sb +a[11]*sb*sb)/x);                  
//          dprb= (b[1] +a[1]*sb + a[2]*sb**2 +a[3]*sb**3)*x**
//      .        (b[2] +a[4]*sb + a[5]*sb**2 +a[6]*sb**3)*(1d0-x)**
//      .        (b[3] +a[7]*sb + a[8]*sb**2 +a[9]*sb**3)
//      .        *(1d0+(a[10]*sb +a[11]*sb**2)/x)
    }
  else 
    {
// --- NLO FFs
      if(iset != 1) cerr << "ERROR [Fragmentation::kkp]: iset must be 0 (LO) or 1 (NLO)" << endl;
      rlam= 0.213;
      s=  log(log(qs*qs/(rlam*rlam))/log(q0*q0/(rlam*rlam)));
      sc= log(log(qs*qs/(rlam*rlam))/log(rmcc*rmcc/(rlam*rlam)));
      sb= log(log(qs*qs/(rlam*rlam))/log(rmbb*rmbb/(rlam*rlam)));

// ---------------------- NLO PION ------------------------------
      b[1]=                            3.73331;
      b[2]=                           -0.74159;
      b[3]=                            2.33092;
      a[1]=                           -3.16946;
      a[2]=                           -0.47683;
      a[3]=                            0.70270;
      a[4]=                           -0.51377;
      a[5]=                           -0.19705;
      a[6]=                           -0.17917;
      a[7]=                            2.03394;
      a[8]=                           -0.50764;
      a[9]=                           -0.08565;
      a[10]=                            0.09466;
      a[11]=                           -0.10222;
      dpg= (b[1] + a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
	     //       dpg= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)*
//      .        (1d0+(a[10]*s +a[11]*s**2)/x)                  
      b[1]=                           0.44809;
      b[2]=                          -1.47598;
      b[3]=                           0.91338;
      a[1]=                           -0.13828;
      a[2]=                           -0.06951;
      a[3]=                            0.01354;
      a[4]=                           -0.30498;
      a[5]=                           -0.01863;
      a[6]=                           -0.12529;
      a[7]=                            0.64145;
      a[8]=                            0.07270;
      a[9]=                           -0.16989;
      a[10]=                            0.07396;
      a[11]=                           -0.07757;
      dpu= (b[1] + a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//       dpu= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)*
//      .        (1d0+(a[10]*s +a[11]*s**2)/x)                  
      b[1]=                            16.5987;
      b[2]=                            0.13345;
      b[3]=                            5.89903;
      a[1]=                           -18.3856;
      a[2]=                            2.44225;
      a[3]=                            2.13225;
      a[4]=                            0.22712;
      a[5]=                           -0.83625;
      a[6]=                            0.38526;
      a[7]=                           -0.16911;
      a[8]=                            0.59886;
      a[9]=                           -0.25630;
      a[10]=                          -0.18619;
      a[11]=                           0.87362;
      dps= (b[1] + a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//          dps= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)*
//      .        (1d0+(a[10]*s +a[11]*s**2)/x)
      b[1]=                            6.17173;
      b[2]=                           -0.53618;
      b[3]=                            5.60108;
      a[1]=                           -4.82450;
      a[2]=                           -1.30844;
      a[3]=                            1.95527;
      a[4]=                           -0.27879;
      a[5]=                           -0.51337;
      a[6]=                            0.10900;
      a[7]=                            0.83571;
      a[8]=                           -1.15141;
      a[9]=                            0.77027;
      a[10]=                            0.09268;
      a[11]=                           -0.11267;
      dpc= (b[1] + a[1]*sc + a[2]*sc*sc +a[3]*sc*sc*sc)*pow(x,(b[2] +a[4]*sc + a[5]*sc*sc +a[6]*sc*sc*sc))
	*pow((1.-x),(b[3] +a[7]*sc + a[8]*sc*sc +a[9]*sc*sc*sc))*(1.+(a[10]*sc +a[11]*sc*sc)/x);                  
//          dpc= (b[1] +a[1]*sc + a[2]*sc**2 +a[3]*sc**3)*x**
//      .        (b[2] +a[4]*sc + a[5]*sc**2 +a[6]*sc**3)*(1d0-x)**
//      .        (b[3] +a[7]*sc + a[8]*sc**2 +a[9]*sc**3)*
//      .        (1d0+(a[10]*sc +a[11]*sc**2)/x)
      b[1]=                            0.25944;
      b[2]=                           -1.98713;
      b[3]=                            3.52857;
      a[1]=                           -0.11449;
      a[2]=                            0.03733;
      a[3]=                           -0.18028;
      a[4]=                           -0.35858;
      a[5]=                            0.22277;
      a[6]=                           -0.66413;
      a[7]=                            0.72303;
      a[8]=                            0.46260;
      a[9]=                           -0.99235;
      a[10]=                           -0.02701;
      a[11]=                           -0.02089;
      dpb= (b[1] + a[1]*sb + a[2]*sb*sb +a[3]*sb*sb*sb)*pow(x,(b[2] +a[4]*sb + a[5]*sb*sb +a[6]*sb*sb*sb))
	*pow((1.-x),(b[3] +a[7]*sb + a[8]*sb*sb +a[9]*sb*sb*sb))*(1.+(a[10]*sb +a[11]*sb*sb)/x);                  
//          dpb= (b[1] +a[1]*sb + a[2]*sb**2 +a[3]*sb**3)*x**
//      .        (b[2] +a[4]*sb + a[5]*sb**2 +a[6]*sb**3)*(1d0-x)**
//      .        (b[3] +a[7]*sb + a[8]*sb**2 +a[9]*sb**3)*
//      .        (1d0+(a[10]*sb +a[11]*sb**2)/x)

// ---------------------- NLO KAON ------------------------------
      b[1]=                            0.23140;
      b[2]=                           -1.36400;
      b[3]=                            1.79761;
      a[1]=                           -0.33644;
      a[2]=                            0.16204;
      a[3]=                           -0.02598;
      a[4]=                            0.97182;
      a[5]=                           -0.02908;
      a[6]=                           -0.43195;
      a[7]=                            1.57116;
      a[8]=                            0.71847;
      a[9]=                           -0.68331;
      a[10]=                            0.36906;
      a[11]=                            2.39060;
      dkg= (b[1] + a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//          dkg= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)
//      .        *(1d0+(a[10]*s +a[11]*s**2)/x)
      b[1]=                            0.17806;
      b[2]=                           -0.53733;
      b[3]=                            0.75940;
      a[1]=                           -0.10988;
      a[2]=                           -0.02524;
      a[3]=                            0.03142;
      a[4]=                           -0.60058;
      a[5]=                            0.07863;
      a[6]=                            0.13276;
      a[7]=                            0.61356;
      a[8]=                           -0.43886;
      a[9]=                            0.23942;
      a[10]=                            0.10742;
      a[11]=                            0.12800;
      dku= (b[1] + a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//          dku= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)
//      .        *(1d0+(a[10]*s +a[11]*s**2)/x)
      b[1]=                            4.96269;
      b[2]=                            0.05562;
      b[3]=                            2.79926;
      a[1]=                            1.54098;
      a[2]=                           -9.06376;
      a[3]=                            4.94791;
      a[4]=                            1.88660;
      a[5]=                           -2.94350;
      a[6]=                            1.04227;
      a[7]=                            3.02991;
      a[8]=                           -4.14807;
      a[9]=                            1.91494;
      a[10]=                            0.85450;
      a[11]=                           -0.61016;
      dkd= (b[1] + a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//          dkd= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)
//      .        *(1d0+(a[10]*s +a[11]*s**2)/x)
      b[1]=                            4.25954;
      b[2]=                           -0.24144;
      b[3]=                            4.21265;
      a[1]=                           -5.44309;
      a[2]=                            6.11031;
      a[3]=                           -3.13973;
      a[4]=                           -1.07757;
      a[5]=                            1.52364;
      a[6]=                           -0.74308;
      a[7]=                            0.25590;
      a[8]=                            0.98423;
      a[9]=                           -0.52839;
      a[10]=                           -0.04000;
      a[11]=                            0.08695;
      dkc= (b[1] + a[1]*sc + a[2]*sc*sc +a[3]*sc*sc*sc)*pow(x,(b[2] +a[4]*sc + a[5]*sc*sc +a[6]*sc*sc*sc))
	*pow((1.-x),(b[3] +a[7]*sc + a[8]*sc*sc +a[9]*sc*sc*sc))*(1.+(a[10]*sc +a[11]*sc*sc)/x);                  
//       dkc= (b[1] +a[1]*sc + a[2]*sc**2 +a[3]*sc**3)*x**
//      .        (b[2] +a[4]*sc + a[5]*sc**2 +a[6]*sc**3)*(1d0-x)**
//      .        (b[3] +a[7]*sc + a[8]*sc**2 +a[9]*sc**3)
//      .        *(1d0+(a[10]*sc +a[11]*sc**2)/x)
      b[1]=                            1.32443;
      b[2]=                           -0.88351;
      b[3]=                            6.15221;
      a[1]=                           -1.41156;
      a[2]=                           -0.04809;
      a[3]=                            0.79066;
      a[4]=                           -0.44818;
      a[5]=                           -0.60073;
      a[6]=                            0.45526;
      a[7]=                            0.46679;
      a[8]=                           -0.50792;
      a[9]=                            0.67006;
      a[10]=                           -0.00477;
      a[11]=                           -0.05503;         
      dkb= (b[1] + a[1]*sb + a[2]*sb*sb +a[3]*sb*sb*sb)*pow(x,(b[2] +a[4]*sb + a[5]*sb*sb +a[6]*sb*sb*sb))
	*pow((1.-x),(b[3] +a[7]*sb + a[8]*sb*sb +a[9]*sb*sb*sb))*(1.+(a[10]*sb +a[11]*sb*sb)/x);                  
//          dkb= (b[1] +a[1]*sb + a[2]*sb**2 +a[3]*sb**3)*x**
//      .        (b[2] +a[4]*sb + a[5]*sb**2 +a[6]*sb**3)*(1d0-x)**
//      .        (b[3] +a[7]*sb + a[8]*sb**2 +a[9]*sb**3)
//      .        *(1d0+(a[10]*sb +a[11]*sb**2)/x)

// ---------------------- NLO PROTON -----------------------------
      b[1]=                            1.56255;
      b[2]=                            0.01567;
      b[3]=                            3.57583;
      a[1]=                           -1.48158;
      a[2]=                           -0.39439;
      a[3]=                            0.51249;
      a[4]=                           -2.16232;
      a[5]=                            2.47127;
      a[6]=                           -0.93259;
      a[7]=                            3.33958;
      a[8]=                           -3.05265;
      a[9]=                            1.21042;
      a[10]=                          -0.84816;
      a[11]=                           1.23583;
      dprg= (b[1] + a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//          dprg= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)
//      .        *(1d0+(a[10]*s +a[11]*s**2)/x)
      b[1]=                           1.25946;
      b[2]=                           0.07124;
      b[3]=                           4.12795;
      a[1]=                           -1.17505;
      a[2]=                            0.37550;
      a[3]=                           -0.01416;
      a[4]=                           -0.29533;
      a[5]=                           -0.24540;
      a[6]=                            0.16543;
      a[7]=                            0.98867;
      a[8]=                           -0.46846;
      a[9]=                            0.20750;
      a[10]=                            0.18957;
      a[11]=                           -0.01116;       
      dpru= (b[1] + a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//          dpru= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)
//      .        *(1d0+(a[10]*s +a[11]*s**2)/x)
      b[1]=                            4.01135;
      b[2]=                            0.17258;
      b[3]=                            5.20766;
      a[1]=                            8.67124;
      a[2]=                           -22.7888;
      a[3]=                            11.4720;
      a[4]=                            4.57608;
      a[5]=                           -9.64835;
      a[6]=                            4.61792;
      a[7]=                            7.25144;
      a[8]=                           -12.6313;
      a[9]=                            6.07314;
      a[10]=                            0.16931;
      a[11]=                           -0.09541;
      dprs= (b[1] + a[1]*s + a[2]*s*s +a[3]*s*s*s)*pow(x,(b[2] +a[4]*s + a[5]*s*s +a[6]*s*s*s))
	*pow((1.-x),(b[3] +a[7]*s + a[8]*s*s +a[9]*s*s*s))*(1.+(a[10]*s +a[11]*s*s)/x);                  
//          dprs= (b[1] +a[1]*s + a[2]*s**2 +a[3]*s**3)*x**
//      .        (b[2] +a[4]*s + a[5]*s**2 +a[6]*s**3)*(1d0-x)**
//      .        (b[3] +a[7]*s + a[8]*s**2 +a[9]*s**3)
//      .        *(1d0+(a[10]*s +a[11]*s**2)/x)
      b[1]=                            0.08250;
      b[2]=                           -1.61290;
      b[3]=                            2.01255;
      a[1]=                           -0.04512;
      a[2]=                           -0.00565;
      a[3]=                            0.00900;
      a[4]=                           -0.38012;
      a[5]=                           -0.06840;
      a[6]=                            0.08888;
      a[7]=                            0.63782;
      a[8]=                           -0.14146;
      a[9]=                            0.06083;
      a[10]=                           -0.02958;
      a[11]=                            0.01130;
      dprc= (b[1] + a[1]*sc + a[2]*sc*sc +a[3]*sc*sc*sc)*pow(x,(b[2] +a[4]*sc + a[5]*sc*sc +a[6]*sc*sc*sc))
	*pow((1.-x),(b[3] +a[7]*sc + a[8]*sc*sc +a[9]*sc*sc*sc))*(1.+(a[10]*sc +a[11]*sc*sc)/x);                  
//          dprc= (b[1] +a[1]*sc + a[2]*sc**2 +a[3]*sc**3)*x**
//      .        (b[2] +a[4]*sc + a[5]*sc**2 +a[6]*sc**3)*(1d0-x)**
//      .        (b[3] +a[7]*sc + a[8]*sc**2 +a[9]*sc**3)
//      .        *(1d0+(a[10]*sc +a[11]*sc**2)/x)
      b[1]=                            24.2916;
      b[2]=                            0.57939;
      b[3]=                            12.1207;
      a[1]=                          -88.3524;
      a[2]=                            93.1056;
      a[3]=                           -17.4089;
      a[4]=                           -0.80783;
      a[5]=                           -5.07200;
      a[6]=                           -2.45377;
      a[7]=                           -3.27370;
      a[8]=                            1.21188;
      a[9]=                           -5.50374;
      a[10]=                            0.14628;
      a[11]=                           -0.78634;
      dprb= (b[1] + a[1]*sb + a[2]*sb*sb +a[3]*sb*sb*sb)*pow(x,(b[2] +a[4]*sb + a[5]*sb*sb +a[6]*sb*sb*sb))
	*pow((1.-x),(b[3] +a[7]*sb + a[8]*sb*sb +a[9]*sb*sb*sb))*(1.+(a[10]*sb +a[11]*sb*sb)/x);                  
//          dprb= (b[1] +a[1]*sb + a[2]*sb**2 +a[3]*sb**3)*x**
//      .        (b[2] +a[4]*sb + a[5]*sb**2 +a[6]*sb**3)*(1d0-x)**
//      .        (b[3] +a[7]*sb + a[8]*sb**2 +a[9]*sb**3)
//      .        *(1d0+(a[10]*sb +a[11]*sb**2)/x)
    }
  
// --- Evaluate different contributions
  
  double dpd =      dpu;
  dks =      dku;
  double dprd= 0.5*dpru;
  if (qs < rmbb) 
    {
      dpb  = 0.;
      dkb  = 0.;
      dprb = 0.;
    }
  if (qs < rmcc) 
    {
      dpc  = 0.;
      dkc  = 0.;
      dprc = 0.;
    }
  if (ih == 1) 
    {
      dh[0]= dpg/2.;
      dh[1]= dpu/2.;
      dh[2]= dpu/2.;
      dh[3]= dpd/2.;
      dh[4]= dpd/2.;
      dh[5]= dps/2.;
      dh[6]= dps/2.;
      dh[7]= dpc/2.;
      dh[8]= dpc/2.;
      dh[9]= dpb/2.;
      dh[10]=dpb/2.;
    }
  else if (ih == 2) 
    {
      dh[0]= dkg/2.;
      dh[1]= dku/2.;
      dh[2]= dku/2.;
      dh[3]= dkd/2.;
      dh[4]= dkd/2.;
      dh[5]= dks/2.;
      dh[6]= dks/2.;
      dh[7]= dkc/2.;
      dh[8]= dkc/2.;
      dh[9]= dkb/2.;
      dh[10]=dkb/2.;
    }
  else if (ih == 3) 
    {
      dh[0]= dkg/2.;
      dh[1]= dkd/2.;
      dh[2]= dkd/2.;
      dh[3]= dku/2.;
      dh[4]= dku/2.;
      dh[5]= dks/2.;
      dh[6]= dks/2.;
      dh[7]= dkc/2.;
      dh[8]= dkc/2.;
      dh[9]= dkb/2.;
      dh[10]=dkb/2.;
    }      
  else if (ih == 4) 
    {
      dh[0]= dprg/2.;
      dh[1]= dpru/2.;
      dh[2]= dpru/2.;
      dh[3]= dprd/2.;
      dh[4]= dprd/2.;
      dh[5]= dprs/2.;
      dh[6]= dprs/2.;
      dh[7]= dprc/2.;
      dh[8]= dprc/2.;
      dh[9]= dprb/2.;
      dh[10]=dprb/2.;
    }
  else if (ih == 5) 
    {
      dh[0]= dpg/2.;
      dh[1]= dpu/2.;
      dh[2]= dpu/2.;
      dh[3]= dpd/2.;
      dh[4]= dpd/2.;
      dh[5]= dps/2.;
      dh[6]= dps/2.;
      dh[7]= dpc/2.;
      dh[8]= dpc/2.;
      dh[9]= dpb/2.;
      dh[10]=dpb/2.;
    }
  else if (ih == 6)
    {
      dh[0]= dprg/2.;
      dh[1]= dpru/4.;
      dh[2]= dpru/4.;
      dh[3]= dprd;
      dh[4]= dprd;
      dh[5]= dprs/2.;
      dh[6]= dprs/2.;
      dh[7]= dprc/2.;
      dh[8]= dprc/2.;
      dh[9]= dprb/2.;
      dh[10]=dprb/2.;
    }      
  else
    {  
      dh[0]= dpg+dkg+dprg;
      dh[1]= dpu+dku+dpru;
      dh[2]= dpu+dku+dpru;
      dh[3]= dpd+dkd+dprd;
      dh[4]= dpd+dkd+dprd;
      dh[5]= dps+dks+dprs;
      dh[6]= dps+dks+dprs;
      dh[7]= dpc+dkc+dprc;
      dh[8]= dpc+dkc+dprc;
      dh[9]= dpb+dkb+dprb;
      dh[10]=dpb+dkb+dprb;
    }  

  return dh[0]; // return gluon part
}
