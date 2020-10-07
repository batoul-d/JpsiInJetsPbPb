#if !(defined(__CINT__) || defined(__CLING__)) || defined(__ACLIC__)
#include "inputParams.h"
#endif

void plot(bool doPrompt = true, bool doPbPb = false){
  if (!setSystTag(doPbPb)) return;
  
  string filename1 = "";
  string filename2 = "";
  string filename3 = "";
  string filename4 = "";
  string filenameMeas = "";
  string outputfile = "";

  int iterFinal = nSIter;
  if (!doPbPb) iterFinal = nSIter_pp;
  if (iterFinal<4) iterFinal=4;
  filename1 = Form("%s/dataUnf/unfOutput/step%i/UnfoldedDistributions_%s_%s_%diter_%dz%dptBins%dz%dptMeasBin_Diag%s.root",unfPath.c_str(),1,doPbPb?"PbPb":"PP",doPrompt?"prompt":"nonprompt", nIter, nBinZ_gen, nBinJet_gen, nBinZ_reco, nBinJet_reco, systTag.c_str());
  filename2 = Form("%s/dataUnf/unfOutput/step%i/UnfoldedDistributions_%s_%s_%diter_%dz%dptBins%dz%dptMeasBin_Diag%s.root",unfPath.c_str(),2,doPbPb?"PbPb":"PP",doPrompt?"prompt":"nonprompt", nIter, nBinZ_gen, nBinJet_gen, nBinZ_reco, nBinJet_reco, systTag.c_str());
  filename3 = Form("%s/dataUnf/unfOutput/step%i/UnfoldedDistributions_%s_%s_%diter_%dz%dptBins%dz%dptMeasBin_Diag%s.root",unfPath.c_str(),iterFinal-1,doPbPb?"PbPb":"PP",doPrompt?"prompt":"nonprompt", nIter, nBinZ_gen, nBinJet_gen, nBinZ_reco, nBinJet_reco, systTag.c_str());
  filename4 = Form("%s/dataUnf/unfOutput/step%i/UnfoldedDistributions_%s_%s_%diter_%dz%dptBins%dz%dptMeasBin_Diag%s.root",unfPath.c_str(),iterFinal,doPbPb?"PbPb":"PP",doPrompt?"prompt":"nonprompt", nIter, nBinZ_gen, nBinJet_gen, nBinZ_reco, nBinJet_reco, systTag.c_str());
  filenameMeas = Form("%s/dataUnf/unfOutput/step%i/UnfoldedDistributions_%s_%s_%diter_%dz%dptBins%dz%dptMeasBin%s.root",unfPath.c_str(),iterFinal,doPbPb?"PbPb":"PP",doPrompt?"prompt":"nonprompt", nIter, nBinZ_gen, nBinJet_gen, nBinZ_reco, nBinJet_reco, systTag.c_str());
  outputfile = Form("%s/dataUnf/plots/UnfoldedDistributions_%s_%s_%diter_%dz%dptBins%dz%dptMeasBin_SIter%i%s%s.pdf",unfPath.c_str(),doPbPb?"PbPb":"PP",doPrompt?"prompt":"nonprompt", nIter, nBinZ_gen, nBinJet_gen, nBinZ_reco, nBinJet_reco, iterFinal, systTag.c_str(), systErr?"":"_statError");

  TFile *file1 = new TFile(filename1.c_str());
  TFile *file2 = new TFile(filename2.c_str());
  TFile *file3 = new TFile(filename3.c_str());
  TFile *file4 = new TFile(filename4.c_str());

  TFile *fileMeas = new TFile(filenameMeas.c_str());

  TH2D *h2ZMeas = (TH2D*)fileMeas->Get("fh2MeasData;1");
  
  TH2D *h2UnfResp1 = (TH2D*)file1->Get("hReco_Iter1;1"); //this is the way it's saved in unfoldStepNewPrNominalDiag 
  TH2D *h2UnfResp2 = (TH2D*)file2->Get("hReco_Iter1;1"); //it's not wrong
  TH2D *h2UnfResp3 = (TH2D*)file3->Get("hReco_Iter1;1"); //don't panic everytime you see it
  TH2D *h2UnfResp4 = (TH2D*)file4->Get("hReco_Iter1;1");

  Int_t min_PriorFolded = h2UnfResp1->GetYaxis()->FindBin(midLowerPt+0.00001);
  Int_t max_PriorFolded = h2UnfResp1->GetYaxis()->FindBin(midUpperPt-0.00001);
  
  cout <<"midLowerPt = "<<midLowerPt<<"; midUpperPt = "<<midUpperPt<<endl;
  cout <<"min_PriorFolded = "<<min_PriorFolded<<"; max_PriorFolded = "<<max_PriorFolded<<endl;

  TH1D *hZMeas = h2ZMeas->ProjectionX("hZMeas",min_PriorFolded,min_PriorFolded);
  hZMeas->Draw();

  TH1D *hZUnf_SI1 = (TH1D*)h2UnfResp1->ProjectionX("hZUnf_SI1",min_PriorFolded,max_PriorFolded);
  TH1D *hZUnf_SI2 = (TH1D*)h2UnfResp2->ProjectionX("hZUnf_SI2",min_PriorFolded,max_PriorFolded);
  TH1D *hZUnf_SI3 = (TH1D*)h2UnfResp3->ProjectionX("hZUnf_SI3",min_PriorFolded,max_PriorFolded);
  TH1D *hZUnf_SI4 = (TH1D*)h2UnfResp4->ProjectionX("hZUnf_SI4",min_PriorFolded,max_PriorFolded);
  
  TH1D *hZUnf_SI1_ratioMeas;
  TH1D *hZUnf_SI2_ratioMeas;
  TH1D *hZUnf_SI3_ratioMeas;
  TH1D *hZUnf_SI4_ratioMeas;
  
  hZUnf_SI1_ratioMeas = (TH1D*)hZUnf_SI1->Clone("hZUnf_SI1_ratioMeas");
  hZUnf_SI1_ratioMeas->Divide(hZMeas);

  hZUnf_SI2_ratioMeas = (TH1D*)hZUnf_SI2->Clone("hZUnf_SI2_ratioMeas");
  hZUnf_SI2_ratioMeas->Divide(hZMeas);

  hZUnf_SI3_ratioMeas = (TH1D*)hZUnf_SI3->Clone("hZUnf_SI3_ratioMeas");
  hZUnf_SI3_ratioMeas->Divide(hZMeas);

  hZUnf_SI4_ratioMeas = (TH1D*)hZUnf_SI4->Clone("hZUnf_SI4_ratioMeas");
  hZUnf_SI4_ratioMeas->Divide(hZMeas);

  TCanvas * mycan1 = new TCanvas("mycan1","mycan1",900,500);
  mycan1->Divide(2,1);
  
  TLegend *legend = new TLegend(0.8,0.15,.95,0.9,"","brNDC");
  legend->SetHeader("");

  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.04);

  mycan1->cd(1);
  //gPad->SetLogy();

  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.2);
  hZMeas->SetStats(0);
  hZMeas->SetTitle(Form("%s %s",doPrompt?"prompt":"nonprompt",doPbPb?"PbPb":"pp"));

  hZMeas->SetMaximum(hZMeas->GetMaximum()*1.5);

  hZMeas->GetXaxis()->SetTitle("z");
  hZMeas->GetYaxis()->SetTitleOffset(1.9);
  hZMeas->GetYaxis()->SetTitle(Form("dN/dz (per %f)",z_reco_binWidth));

  hZMeas->SetLineColor(col[0]);
  hZMeas->SetMarkerColor(col[0]);
  hZMeas->SetMarkerStyle(markerStyle[0]);
  hZMeas->SetMarkerSize(markerSize[0]);
  hZMeas->SetLineWidth(lineWidth[0]);
  
  hZUnf_SI1->SetLineColor(col[2]);
  hZUnf_SI2->SetLineColor(col[3]);
  hZUnf_SI3->SetLineColor(col[4]);
  hZUnf_SI4->SetLineColor(col[5]);

  hZUnf_SI1->SetMarkerColor(col[2]);
  hZUnf_SI2->SetMarkerColor(col[3]);
  hZUnf_SI3->SetMarkerColor(col[4]);
  hZUnf_SI4->SetMarkerColor(col[5]);
  
  hZUnf_SI1->SetMarkerStyle(markerStyle[2]);
  hZUnf_SI2->SetMarkerStyle(markerStyle[3]);
  hZUnf_SI3->SetMarkerStyle(markerStyle[4]);
  hZUnf_SI4->SetMarkerStyle(markerStyle[5]);

  hZUnf_SI1->SetMarkerSize(markerSize[2]);
  hZUnf_SI2->SetMarkerSize(markerSize[3]);
  hZUnf_SI3->SetMarkerSize(markerSize[4]);
  hZUnf_SI4->SetMarkerSize(markerSize[5]);

  hZUnf_SI1->SetLineWidth(lineWidth[2]);
  hZUnf_SI2->SetLineWidth(lineWidth[3]);
  hZUnf_SI3->SetLineWidth(lineWidth[4]);
  hZUnf_SI4->SetLineWidth(lineWidth[5]);
  
  hZMeas->Draw("EP");
  hZUnf_SI1->Draw("EPsame");
  hZUnf_SI2->Draw("EPsame");
  hZUnf_SI3->Draw("EPsame");
  hZUnf_SI4->Draw("EPsame");
  
  legend->AddEntry(hZMeas, "meas","ep");
  legend->AddEntry(hZUnf_SI1, Form("unf SI#%d",1),"ep");
  legend->AddEntry(hZUnf_SI2, Form("unf SI#%d",2),"ep");
  legend->AddEntry(hZUnf_SI3, Form("unf SI#%d",iterFinal-1),"ep");
  legend->AddEntry(hZUnf_SI4, Form("unf SI#%d",iterFinal),"ep");
  
  legend->Draw("same");

  mycan1->Update();

  float xCoord = unfStart;
    
  TLine *line0 = new TLine(xCoord,0,xCoord,gPad->GetUymax());
  line0->SetLineColor(kred);
  line0->SetLineStyle(2);
  line0->SetLineWidth(2);
  //line0->Draw("same");
    
  mycan1->cd(2);

  hZUnf_SI1_ratioMeas->SetStats(0);
  hZUnf_SI1_ratioMeas->GetYaxis()->SetTitle("ratio to measured");
  hZUnf_SI1_ratioMeas->GetYaxis()->SetRangeUser(0.,2.);
  hZUnf_SI1_ratioMeas->GetXaxis()->SetTitle("z");

  hZUnf_SI1_ratioMeas->SetLineColor(col[2]);
  hZUnf_SI2_ratioMeas->SetLineColor(col[3]);
  hZUnf_SI3_ratioMeas->SetLineColor(col[4]);
  hZUnf_SI4_ratioMeas->SetLineColor(col[5]);

  hZUnf_SI1_ratioMeas->SetLineStyle(lineStyle[2]);
  hZUnf_SI2_ratioMeas->SetLineStyle(lineStyle[3]);
  hZUnf_SI3_ratioMeas->SetLineStyle(lineStyle[4]);
  hZUnf_SI4_ratioMeas->SetLineStyle(lineStyle[5]);

  hZUnf_SI1_ratioMeas->SetLineWidth(lineWidth[2]*2);
  hZUnf_SI2_ratioMeas->SetLineWidth(lineWidth[3]*2);
  hZUnf_SI3_ratioMeas->SetLineWidth(lineWidth[4]*2);
  hZUnf_SI4_ratioMeas->SetLineWidth(lineWidth[5]*2);

  hZUnf_SI1_ratioMeas->SetMarkerColor(col[2]);
  hZUnf_SI2_ratioMeas->SetMarkerColor(col[3]);
  hZUnf_SI3_ratioMeas->SetMarkerColor(col[4]);
  hZUnf_SI4_ratioMeas->SetMarkerColor(col[5]);
      
  hZUnf_SI1_ratioMeas->Draw("HIST");
  hZUnf_SI2_ratioMeas->Draw("HISTsame");
  hZUnf_SI3_ratioMeas->Draw("HISTsame");
  hZUnf_SI4_ratioMeas->Draw("HISTsame");
  
  mycan1->Update();

  TLine *line1 = new TLine(xCoord,0,xCoord,gPad->GetUymax());
  line1->SetLineColor(kred);
  line1->SetLineStyle(2);
  line1->SetLineWidth(2);
  line1->Draw("same");

  mycan1->Update();
  mycan1->SaveAs(outputfile.c_str());
  outputfile.replace(outputfile.find(".pdf"),4,".png");
  mycan1->SaveAs(outputfile.c_str());
  
}

void plotInv(bool doPrompt = true, bool doPbPb = false) {
 if (!setSystTag(doPbPb)) return;
  
  string filenameUnf = "";
  string outputfile = "";
  string filenameMeas = "";

  int iterFinal = nSIter;
  if (!doPbPb) iterFinal = nSIter_pp;

  filenameUnf = Form("%s/dataUnf/unfOutput/step%i/UnfoldedDistributions_%s_%s_%diter_%dz%dptBins%dz%dptMeasBin_Diag%s.root",unfPath.c_str(),1,doPbPb?"PbPb":"PP",doPrompt?"prompt":"nonprompt", nIter, nBinZ_gen, nBinJet_gen, nBinZ_reco, nBinJet_reco, systTag.c_str());
  filenameMeas = Form("%s/dataUnf/unfOutput/step%i/UnfoldedDistributions_%s_%s_%diter_%dz%dptBins%dz%dptMeasBin%s.root",unfPath.c_str(),iterFinal,doPbPb?"PbPb":"PP",doPrompt?"prompt":"nonprompt", nIter, nBinZ_gen, nBinJet_gen, nBinZ_reco, nBinJet_reco, systTag.c_str());
  outputfile = Form("%s/dataUnf/plots/UnfoldedDistributions_%s_%s_%diter_%dz%dptBins%dz%dptMeasBin_SIter%i%s_statError.pdf",unfPath.c_str(),doPbPb?"PbPb":"PP",doPrompt?"prompt":"nonprompt", nIter, nBinZ_gen, nBinJet_gen, nBinZ_reco, nBinJet_reco, iterFinal, systTag.c_str());

  TFile *fileUnf = new TFile(filenameUnf.c_str());
  TFile *fileMeas = new TFile(filenameMeas.c_str());

  TH2D *h2ZMeas = (TH2D*)fileMeas->Get("fh2MeasData;1");
  TH2D *h2UnfResp1 = (TH2D*)fileUnf->Get("hReco_Invert;1"); //this is the way it's save in unfoldStepNewPrNominalDiag 

  Int_t min_PriorFolded = h2UnfResp1->GetYaxis()->FindBin(midLowerPt+0.00001);
  Int_t max_PriorFolded = h2UnfResp1->GetYaxis()->FindBin(midUpperPt-0.00001);
  
  cout <<"min_PriorFolded = "<<min_PriorFolded<<"; max_PriorFolded = "<<max_PriorFolded<<endl;

  TH1D *hZMeas = h2ZMeas->ProjectionX("hZMeas",min_PriorFolded,min_PriorFolded);
  hZMeas->Draw();

  TH1D *hZUnf_Inv = (TH1D*)h2UnfResp1->ProjectionX("hZUnf_Inv",min_PriorFolded,max_PriorFolded);
  TH1D *hZUnf_Inv_ratioMeas;
  
  hZUnf_Inv_ratioMeas = (TH1D*)hZUnf_Inv->Clone("hZUnf_Inv_ratioMeas");
  hZUnf_Inv_ratioMeas->Divide(hZMeas);

  TCanvas * mycan1 = new TCanvas("mycan1","mycan1",900,500);
  mycan1->Divide(2,1);
  
  TLegend *legend = new TLegend(0.8,0.15,.95,0.9,"","brNDC");
  legend->SetHeader("");

  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.04);

  mycan1->cd(1);
  //gPad->SetLogy();

  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.2);
  hZMeas->SetStats(0);
  hZMeas->SetTitle(Form("%s %s",doPrompt?"prompt":"nonprompt",doPbPb?"PbPb":"pp"));

  hZMeas->SetMaximum(hZMeas->GetMaximum()*1.5);

  hZMeas->GetXaxis()->SetTitle("z");
  hZMeas->GetYaxis()->SetTitleOffset(1.9);
  hZMeas->GetYaxis()->SetTitle(Form("dN/dz (per %f)",z_reco_binWidth));

  hZMeas->SetLineColor(col[0]);
  hZMeas->SetMarkerColor(col[0]);
  hZMeas->SetMarkerStyle(markerStyle[0]);
  hZMeas->SetMarkerSize(markerSize[0]);
  hZMeas->SetLineWidth(lineWidth[0]);
  
  hZUnf_Inv->SetLineColor(col[2]);
  hZUnf_Inv->SetMarkerColor(col[2]);
  hZUnf_Inv->SetMarkerStyle(markerStyle[2]);
  hZUnf_Inv->SetMarkerSize(markerSize[2]);
  hZUnf_Inv->SetLineWidth(lineWidth[2]);
  
  hZMeas->Draw("EP");
  hZUnf_Inv->Draw("EPsame");
  
  legend->AddEntry(hZMeas, "meas","ep");
  legend->AddEntry(hZUnf_Inv, "unf","ep");
  
  legend->Draw("same");

  mycan1->Update();

  float xCoord = unfStart;
    
  TLine *line0 = new TLine(xCoord,0,xCoord,gPad->GetUymax());
  line0->SetLineColor(kred);
  line0->SetLineStyle(2);
  line0->SetLineWidth(2);
  //line0->Draw("same");
    
  mycan1->cd(2);

  hZUnf_Inv_ratioMeas->SetStats(0);
  hZUnf_Inv_ratioMeas->GetYaxis()->SetTitle("ratio to measured");
  hZUnf_Inv_ratioMeas->GetYaxis()->SetRangeUser(0.,1.7);
  hZUnf_Inv_ratioMeas->GetXaxis()->SetTitle("z");

  hZUnf_Inv_ratioMeas->SetLineColor(col[2]);
  hZUnf_Inv_ratioMeas->SetLineStyle(lineStyle[2]);
  hZUnf_Inv_ratioMeas->SetLineWidth(lineWidth[2]*2);
  hZUnf_Inv_ratioMeas->SetMarkerColor(col[2]);
  hZUnf_Inv_ratioMeas->Draw("HIST");
  
  mycan1->Update();

  TLine *line1 = new TLine(xCoord,0,xCoord,gPad->GetUymax());
  line1->SetLineColor(kred);
  line1->SetLineStyle(2);
  line1->SetLineWidth(2);
  line1->Draw("same");

  mycan1->Update();
  mycan1->SaveAs(outputfile.c_str());
  outputfile.replace(outputfile.find(".pdf"),4,".png");
  mycan1->SaveAs(outputfile.c_str());
 
}
void PlotRatios_DataUnfolded_afterDiag(){
  if (!matrixInv)
    plot(true,true);
  if (centShift==0 && !doCent && !doPeri) {
    if (!matrixInv)
      plot(true,false);
    else 
      plotInv(true,false);
  }
}
