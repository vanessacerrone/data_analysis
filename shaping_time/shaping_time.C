// Author: Vanessa Cerrone

// Last modification: 09/03/2021

// Description: Plot del segnale in uscita dalla catena elettronica acquisito 
// con Arduino, al variare della resistenza dello shaper R_sh


using namespace std;

// Useful ROOT & C++ libraries

#include "TCanvas.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLegend.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TMath.h"
#include "TPaveText.h"
#include "TFitResult.h"
#include "TH2F.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>

using namespace std;

void shaping_time()
{
 

    TGraph *g = new TGraph("shaper_R1.txt");

    double x_min = 0; 
    double x_max = 0.3;
    double y_min = TMath::MinElement(g->GetN(),g->GetY())-0.2; 
    double y_max = TMath::MaxElement(g->GetN(),g->GetY())+0.2;

    TCanvas *c1 = new TCanvas("c1", "c1", 1080, 720);
    g->SetLineColor(kPink+7);
    g->SetMarkerColor(kPink+7);
    g->SetMarkerStyle(8);
    g->SetMarkerSize(0.8);
    g->SetLineWidth(3);
    g->SetTitle("Shaping Time ");
    g->GetXaxis()->SetTitle("time (ms)");
    g->GetXaxis()->SetTitleSize(0.04);
    g->GetXaxis()->SetTickLength(0.02);
    g->GetYaxis()->SetTitle("voltage (V)");
    g->GetYaxis()->SetTitleSize(0.04);
    g->GetYaxis()->SetTickLength(0.02);
    g->GetYaxis()->SetRangeUser(y_min, y_max);
    g->GetXaxis()->SetRangeUser(x_min,x_max);
    g->Draw("al");
    c1->Update();
    
    TGraph *g2 = new TGraph("shaper_R2.txt");
    g2->SetLineColor(kAzure+7);
    g2->SetLineWidth(3);
    g2->SetMarkerColor(kAzure+7);
    g2->SetMarkerStyle(33);
    g2->SetMarkerSize(1.2);
    g2->Draw("l same");
    
    TGraph *g3 = new TGraph("shaper_R3.txt");
    g3->SetLineColor(kGreen+1);
    g3->SetMarkerColor(kGreen+1);
    g3->SetMarkerStyle(45);
    g3->SetMarkerSize(1);
    g3->SetLineWidth(3);
    g3->Draw("l same");
    
    TGraph *g4 = new TGraph("shaper_R4.txt");
    g4->SetLineColor(kOrange+7);
    g4->SetMarkerColor(kAzure+7);
    g4->SetMarkerStyle(33);
    g4->SetMarkerSize(1.2);
    g4->SetLineWidth(3);
    g4->Draw("l same");
    
    
    auto legend = new TLegend(0.13,0.7,0.3,0.9);
    legend->SetTextSize(0.025);
    legend->AddEntry(g,"Acquired data w/ R_{sh}#approx 148k#Omega ","l");
    legend->AddEntry(g2,"Acquired data w/ R_{sh}#approx 100k#Omega ","l");
    legend->AddEntry(g3,"Acquired data w/ R_{sh}#approx 82k#Omega ","l");
    legend->AddEntry(g4,"Acquired data w/ R_{sh}#approx 180k#Omega","l");
    
      
    legend->Draw();
    
    
}
    


  





