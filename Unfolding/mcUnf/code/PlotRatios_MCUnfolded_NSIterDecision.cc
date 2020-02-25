#include "inputParams.h"


void plot(bool doPrompt = false, bool doPbPb = true){
  if (!setCaseTag()) return;
  gSystem->mkdir(Form("%s/mcUnf/plots/",unfPath.c_str()));
  string filename = "";
  
  int iterMin = nSIter-3;
  int iterMax = nSIter+3;
  if (!doPbPb) {
    iterMin = nSIter_pp-5;
    iterMax = nSIter_pp;
  }

  
  TH1D *hZTrue = NULL;
  
  TCanvas * mycan1 = new TCanvas("mycan1","mycan1",900,500);
  mycan1->Divide(2,1);
  
  TLegend *legend = new TLegend(0.8,0.15,.95,0.9,"","brNDC");
  legend->SetHeader("");  
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.04);
  float xCoord2 = unfStart;
  TLine *line1(0x0);
  TLine *line2(0x0);

  TH1D* chi2Hist = new TH1D("chi2Hist","",150,0.5,150.5);//99,0.5,99.5);
  
  //for (int iIter = iterMin; iIter<=iterMax; iIter++)
  for (int iIter = 1; iIter<=99; iIter++)
    {
      if (!doPbPb && iIter>nSIter_pp) continue;
      filename = Form("%s/mcUnf/unfOutput/step%d/UnfoldedDistributions_%s_%s_%diter_%dz%dptBins%dz%dptMeasBins%s.root", unfPath.c_str(), iIter, doPbPb?"PbPb":"PP",doPrompt?"prompt":"nonprompt", nIter, nBinZ_gen, nBinJet_gen, nBinZ_reco, nBinJet_reco, caseTag.c_str());
      
      TFile *file = new TFile(filename.c_str());
      if (!file) {cout<<"[WARNING] file not found"<<filename<<endl; continue;}
      
      if (iIter==1) hZTrue = (TH1D*)file->Get(Form("hMTru_%d;1",midLowerId)); TH1D *hZTrueTemp = NULL;
      TH1D *hZUnf = (TH1D*) file->Get(Form("hMUnf_%d_Iter%d;1",midLowerId,nIter)); TH1D *hZUnf_temp = NULL;
      
      for (int i = 1; i < (nBinJet_gen/nBinJet_reco); i++) {
	if (iIter==1) {
	  hZTrueTemp = (TH1D*)file->Get(Form("hMTru_%d;1",midLowerId+i));
	  hZTrue->Add(hZTrueTemp);
	}
	
	hZUnf_temp = (TH1D*)file->Get(Form("hMUnf_%d_Iter%d;1",midLowerId+i,nIter));
	hZUnf->Add(hZUnf_temp);
      }

      if (iIter==1)
	hZTrue->Rebin(nBinZ_gen/nBinZ_reco);
      hZUnf->Rebin(nBinZ_gen/nBinZ_reco);
  
      mycan1->cd(1);
      //gPad->SetLogy();

      gPad->SetLeftMargin(0.12);
      gPad->SetRightMargin(0.2);

      if (iIter==1) {
	hZTrue->SetStats(0);
	hZTrue->SetTitle(Form("%s %s",doPrompt?"prompt":"nonprompt",doPbPb?"PbPb":"PP"));  
	hZTrue->GetXaxis()->SetTitle("z");
	hZTrue->GetYaxis()->SetTitleOffset(1.6);
	hZTrue->GetYaxis()->SetTitle(Form("dN/dz (per %f)",z_reco_binWidth));
	hZTrue->GetYaxis()->SetRangeUser(0,1.5*hZTrue->GetMaximum());
	hZTrue->SetLineColor(col[0]);
	hZTrue->SetMarkerColor(col[0]);
	hZTrue->SetMarkerStyle(0);
	hZTrue->SetMarkerSize(markerSize[0]);
	hZTrue->SetLineWidth(lineWidth[0]);
	legend->AddEntry(hZTrue, "truth","ep");
	hZTrue->Draw("EP");
      }
      if (iIter>=iterMin && iIter<=iterMax) {
      hZUnf->SetMarkerColor(col[iIter-iterMin+1]);
      hZUnf->SetMarkerStyle(markerStyle[iIter-iterMin+1]);
      hZUnf->SetMarkerSize(markerSize[iIter-iterMin+1]);
      hZUnf->SetLineColor(col[iIter-iterMin+1]);
      hZUnf->SetLineWidth(lineWidth[iIter-iterMin+1]);
      }
      
      Double_t chi2 = hZUnf->Chi2Test(hZTrue,"WW CHI2/NDF");
      cout <<"[INFO] For SI "<<iIter<<" CHI2/NDF = "<<chi2<<endl;
      chi2Hist->SetBinContent(chi2Hist->FindBin(iIter),chi2);
      if (iIter>=iterMin && iIter<=iterMax) {
      legend->AddEntry(hZUnf, Form("unf SI%d",iIter),"ep");
      hZUnf->Draw("E1same");
      }
      
      if (iIter==iterMax) {
	legend->Draw("same");
	line1 = new TLine(xCoord2,0,xCoord2,gPad->GetUymax());
	line1->SetLineColor(kRed);
	line1->SetLineStyle(2);
	line1->SetLineWidth(1);
	line1->Draw("same");
      }
      if (iIter>=iterMin && iIter<=iterMax) {
      mycan1->Update();
  
      mycan1->cd(2);
      TH1D *hZUnf_diffTruth;
      hZUnf_diffTruth = (TH1D*) hZUnf->Clone();
      hZUnf_diffTruth->Scale(-1);
      hZUnf_diffTruth->Add(hZTrue);
      hZUnf_diffTruth->Scale(-1);
      TH1D *hZUnf_ratioTruth;
      hZUnf_ratioTruth = (TH1D*) hZUnf_diffTruth->Clone();      
      hZUnf_ratioTruth->Divide(hZTrue);
      hZUnf_ratioTruth->SetStats(0);
      hZUnf_ratioTruth->GetYaxis()->SetTitle("(true-meas/meas)");
      hZUnf_ratioTruth->GetYaxis()->SetTitleOffset(1.6);
      hZUnf_ratioTruth->GetYaxis()->SetRangeUser(-0.5,0.5);
      hZUnf_ratioTruth->GetXaxis()->SetTitle("z");
      
      if (iIter==iterMin)
	hZUnf_ratioTruth->Draw("E1");
      else
	hZUnf_ratioTruth->Draw("E1same");
      
      mycan1->Update();
      }
      if(iIter==iterMax) {
	line1->Draw("same");
	line2 = new TLine(min_z,0,gPad->GetUxmax(),0);
	line2->SetLineColor(kblue);
	line2->SetLineStyle(1);
	line2->SetLineWidth(1);
	line2->Draw("same");
      }
      mycan1->Update();
    }  
  mycan1->SaveAs(Form("%s/mcUnf/plots/unf_mc_IterComparison_%s_%s_jetR%d_ratioTruth_%dz%dptBins%dz%dptMeasBins%s_%dIter_SIterFrom%dTo%d.pdf",unfPath.c_str(), doPbPb?"PbPb":"PP",doPrompt?"prompt":"nonprompt", (int) (jetR*10), nBinZ_gen, nBinJet_gen, nBinZ_reco, nBinJet_reco, caseTag.c_str(),nIter, iterMin,iterMax));
  TFile* fileChi2Save = new TFile(Form("%s/mcUnf/plots/unf_mc_chi2Hist_%s_%s_jetR%d_ratioTruth_%dz%dptBins%dz%dptMeasBins%s_%dIter_SIterFrom%dTo%d.root",unfPath.c_str(), doPbPb?"PbPb":"PP",doPrompt?"prompt":"nonprompt", (int) (jetR*10), nBinZ_gen, nBinJet_gen, nBinZ_reco, nBinJet_reco, caseTag.c_str(),nIter, iterMin,iterMax),"RECREATE");
  chi2Hist->Write();
  fileChi2Save->Close();
}

void PlotRatios_MCUnfolded_NSIterDecision(){
  //plot(bool doPrompt, bool doPbPb)
  plot(true,true);
  if (centShift==0)
    plot(true,false);

  plot(false,true);
  if (centShift==0)
  plot(false,false);
}