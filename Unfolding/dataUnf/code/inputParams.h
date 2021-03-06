// header with input
#include "TROOT.h"
#include "TKey.h"
#include "TFile.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TObjArray.h"
#include "THStack.h"
#include "TLegend.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TMath.h"
#include "TCut.h"
#include "TPaletteAxis.h"
#include "TBranchElement.h"
#include "TPaveStats.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TProfile.h"
#include "THnSparse.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TColor.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <algorithm>

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"
#include "RooUnfoldInvert.h"

using namespace std;

///////////////////////////////////////////////
//           general parameters              //
///////////////////////////////////////////////
string unfPath = "/data_CMS/cms/diab/JpsiJet/Unfolding";
bool sameSample = false;
bool mc2015 = false;
bool flatPrior = true;
bool matrixInv = false;

int nIter = 3;
int nIter_pp = 3;
int nSIter = 25;
int nSIter_pp = 1;

double normPP = 3.31363e-09;//3.21299e-09;//3.4058383e-09;
double normPbPb = 1.4482e-08; //2.28119e-08 for 0-20% //4.17577e-08 for 20-90%

double taaUnc = 0.025;
double lumiPPUnc = 0.035;

bool noProbFit = false;
bool noSmearing = false;
///////////////////////////////////////////////
//              systematics                  //
///////////////////////////////////////////////
bool doCent = false;
bool doPeri = false;
int nIter_cent = 2;
int nSIter_cent = 28;

int nIter_peri = 2;
int nSIter_peri = 21;

int centShift = 0; // 0 is nominal, -1 for systDown and +1 for systUp

bool useSystTrM = false; //if true use nonprompt Tr on prompt data 

bool systErr = false;

int JESsyst = 0; // 0 is nominal, -1 for systDown and +1 for systUp
int JERsyst = 0; // 0 is nominal, -1 for systDown and +1 for systUp
double SF = 1.1; // 1.1 is nominal, 1.0 for systDown, 1.2 for systUp

bool nprPrior = false;

double SFvalPP[3][7];
double SFvalPbPb[3][14];

int nIter_syst = 10;
int nSIter_syst = 30;

int nIter_syst_pp = 10;
int nSIter_syst_pp = 1;

int nIter_syst_cent = 10;
int nSIter_syst_cent = 30;

int nIter_syst_peri = 10;
int nSIter_syst_peri = 13;

string systTag = "";

bool systDiff = true;
///////////////////////////////////////////////
//             jet parameters                //
///////////////////////////////////////////////
float jetR = 0.3;
//float jetR = 0.4;
float min_z = 0.064; 
float max_z = 1.0;

float min_jetpt = 0.;//10.;
float max_jetpt = 60.;

float max_jt_eta = 2.0;

int nBinZ_gen = 48;
int nBinZ_reco = 6;
  
int nBinJet_gen = 30;
int nBinJet_reco = 6;

float jetPt_gen_binWidth = (max_jetpt-min_jetpt)*1.0/nBinJet_gen;
float z_gen_binWidth = (max_z-min_z)*1.0/nBinZ_gen;
float jetPt_reco_binWidth = (max_jetpt-min_jetpt)*1.0/nBinJet_reco;
float z_reco_binWidth = (max_z-min_z)*1.0/nBinZ_reco;

float midLowerPt = 30;//(max_jetpt+min_jetpt-jetPt_reco_binWidth)*0.5;
float midUpperPt = 40;//(max_jetpt+min_jetpt+jetPt_reco_binWidth)*0.5;

int midLowerId = (midLowerPt-min_jetpt)/jetPt_gen_binWidth;//( (int) (nBinJet_reco/2) )*(nBinJet_gen/nBinJet_reco);

float unfStart = 0.22; // where we separate underflow from measured

///////////////////////////////////////////////
//            jpsi parameters                //
///////////////////////////////////////////////
float min_jp_pt = 6.5;
float max_jp_pt = 100.;//100. or 35.
float max_jp_eta = 2.4;//2.4 or 1.6
int min_cent = 0;
int max_cent = 180;

///////////////////////////////////////////////
//            style parameters               //
///////////////////////////////////////////////
TColor *pal = new TColor();
// good for primary marker colors
Int_t kmagenta = pal->GetColor(124,  0,124);
Int_t kviolet  = pal->GetColor( 72,  0,190);
Int_t kblue    = pal->GetColor(  9,  0,200);
Int_t kazure   = pal->GetColor(  0, 48, 97);
Int_t kcyan    = pal->GetColor(  0, 83, 98);
Int_t kteal    = pal->GetColor(  0, 92, 46);
Int_t kgreen   = pal->GetColor( 15, 85, 15);
Int_t kspring  = pal->GetColor( 75, 97, 53);
Int_t kyellow  = pal->GetColor(117,118,  0);
Int_t korange  = pal->GetColor(101, 42,  0);
Int_t kred     = pal->GetColor(190,  0,  3);
Int_t kpink    = pal->GetColor(180, 35,145);
// good for systematic band fill
Int_t kmagentaLight = pal->GetColor(215,165,215);
Int_t kvioletLight  = pal->GetColor(200,160,255);
Int_t kblueLight    = pal->GetColor(178,185,254);
Int_t kazureLight   = pal->GetColor(153,195,225);
Int_t kcyanLight    = pal->GetColor(140,209,224);
Int_t ktealLight    = pal->GetColor( 92,217,141);
Int_t kgreenLight   = pal->GetColor(135,222,135);
Int_t kspringLight  = pal->GetColor(151,207,116);
Int_t kyellowLight  = pal->GetColor(225,225,100);
Int_t korangeLight  = pal->GetColor(255,168,104);
Int_t kredLight     = pal->GetColor(253,169,179);
Int_t kpinkLight    = pal->GetColor(255,192,224);

Int_t col[] = {kBlack, kblue, kGreen+2, kViolet+1, kred, kGray+1, kYellow+1, kPink+1};
int markerStyle[] = {kFullSquare,kFullCircle,kOpenTriangleUp,kOpenTriangleDown,kOpenCross,kOpenCrossX,kOpenStar,kOpenDiamond};
int markerSize[] = {1,1,1,1,1,1,1};
int lineStyle[] = {1, 1, 7, 7, 8, 7,7};
int lineWidth[] = {1,1,1,1,1,1,1};

///////////////////////////////////////////////
//               functions                   //
///////////////////////////////////////////////
void printInput() {
  cout << "############### Unfolding input ###############"<<endl;
  cout << "### "<<min_z<<" < z < "<<max_z<<endl;
  cout << "### "<<min_jetpt<<" < jetpt < "<<max_jetpt<<endl;
  cout << "### Reco bins: "<<nBinZ_reco<<" z bins and "<<nBinJet_reco<<" jtpt bins"<<endl;
  cout << "### Gen bins: "<<nBinZ_gen<<" z bins and "<<nBinJet_gen<<" jtpt bins"<<endl;
  cout << "### jetPt_gen_binWidth = "<<jetPt_gen_binWidth<<endl;
  cout << "### z_gen_binWidth = "<<z_gen_binWidth<<endl;
  cout << "### jetPt_reco_binWidth = "<<jetPt_reco_binWidth<<endl;
  cout << "### z_reco_binWidth = "<<z_reco_binWidth<<endl;
  cout << "### midLowerPt = "<<midLowerPt<<endl;
  cout << "### midUpperPt = "<<midUpperPt<<endl;
  cout << "### midLowerId = "<<midLowerId<<endl;
  cout << "### unfStart = "<<unfStart<<endl;
  cout << "###############################################"<<endl;
}

bool setSystTag (bool doPbPb) {
  systTag = "";

  //if ((doCent&&doPeri) ||!doPbPb) {doCent = false; doPeri = false;}
  //normPbPb = 1.5292e-08 for0-90% //2.28119e-08 for 0-20% //4.63974e-08 for 20-90%
  if (doCent) {systTag = systTag+"_centBin"; min_cent=0; max_cent=40; normPbPb = 2.23119e-08;}// nSIter =18;}
  else if (doPeri) {systTag = systTag+"_periBin"; min_cent=40; max_cent=180; normPbPb = 4.39222e-08;};// nSIter =15;}

  if (JESsyst!=0 && (JERsyst!=0 || SF!=1.1 || centShift!=0) ) { cout <<"[ERROR] please check your systematic options"<<endl; return false;}
  if (JERsyst!=0 && (JESsyst!=0 || SF!=1.1 || centShift!=0) ) { cout <<"[ERROR] please check your systematic options"<<endl; return false;}
  if (SF!=1.1 && (JERsyst!=0 || JESsyst!=0 || centShift!=0) ) { cout <<"[ERROR] please check your systematic options"<<endl; return false;}
  if (centShift!=0 && (JERsyst!=0 || JESsyst!=0 || SF!=1.1) ) { cout <<"[ERROR] please check your systematic options"<<endl; return false;}
  if (matrixInv && (nSIter_pp > 1 || nIter>1)) {cout <<"[ERROR] please check your systematic options"<<endl; return false;}  

  if (JESsyst == 1) systTag = systTag + "_JESSystUp";
  else if (JESsyst == -1) systTag = systTag + "_JESSystDown";
  
  if (JERsyst == 1) systTag = systTag + "_JERSystUp";
  else if (JERsyst == -1) systTag = systTag + "_JERSystDown";
  
  if (SF == 1.2) systTag = systTag + "_SFSystUp";
  else if (SF == 1.0) systTag = systTag + "_SFSystDown";

  if (useSystTrM) systTag = systTag + "_TrMSyst";

  if (!flatPrior) systTag = systTag + "_priorSyst";
  if(nprPrior) systTag = systTag + "_nprPriorSyst";

  if (systTag == "") systTag = "_nominal";

  if (matrixInv) systTag = systTag + "_matrixInv";  
  if (noProbFit) systTag = systTag +"_noProbFit";
  if (noSmearing) systTag = systTag +"_noSmearing";

  if (centShift == 1) systTag = systTag + "_centShiftSystUp";
  else if (centShift == -1) systTag = systTag + "_centShiftSystDown";

  if (systErr) systTag = systTag+"_systError";
  cout<< "systTag = "<<systTag<<endl; 
  return true;
}

void setSFVal() {
  SFvalPP[0][0] = 1.1432 - 0.0222;
  SFvalPP[1][0] = 1.1432;
  SFvalPP[2][0] = 1.1432 + 0.0222;
  SFvalPP[0][1] = 1.1815 - 0.0484;
  SFvalPP[1][1] = 1.1815;
  SFvalPP[2][1] = 1.1815 + 0.0484;
  SFvalPP[0][2] = 1.0989 - 0.0456;
  SFvalPP[1][2] = 1.0989;
  SFvalPP[2][2] = 1.0989 + 0.0456;
  SFvalPP[0][3] = 1.1137 - 0.1397;
  SFvalPP[1][3] = 1.1137;
  SFvalPP[2][3] = 1.1137 + 0.1397;
  SFvalPP[0][4] = 1.1307 - 0.1470;
  SFvalPP[1][4] = 1.1307;
  SFvalPP[2][4] = 1.1307 + 0.1470;
  SFvalPP[0][5] = 1.1600 - 0.0976;
  SFvalPP[1][5] = 1.1600;
  SFvalPP[2][5] = 1.1600 + 0.0976;
  SFvalPP[0][6] = 1.2393 - 0.1909;
  SFvalPP[1][6] = 1.2393;
  SFvalPP[2][6] = 1.2393 + 0.1909;
  
  SFvalPbPb[0][0] = 1.1415;
  SFvalPbPb[1][0] = 1.1742;
  SFvalPbPb[2][0] = 1.2069;
  SFvalPbPb[0][1] = 1.1559;
  SFvalPbPb[1][1] = 1.1930;
  SFvalPbPb[2][1] = 1.2302;
  SFvalPbPb[0][2] = 1.0812;
  SFvalPbPb[1][2] = 1.1451;
  SFvalPbPb[2][2] = 1.2089;
  SFvalPbPb[0][3] = 1.1086;
  SFvalPbPb[1][3] = 1.1618;
  SFvalPbPb[2][3] = 1.2150;
  SFvalPbPb[0][4] = 1.0838;
  SFvalPbPb[1][4] = 1.1455;
  SFvalPbPb[2][4] = 1.2072;
  SFvalPbPb[0][5] = 1.0748;
  SFvalPbPb[1][5] = 1.1117;
  SFvalPbPb[2][5] = 1.1486;
  SFvalPbPb[0][6] = 1.0888;
  SFvalPbPb[1][6] = 1.1581;
  SFvalPbPb[2][6] = 1.2274;
}
