// Author: Vanessa Cerrone

// Last modification: 06/03/2021

// Description: catena completa linear fit ( V_max [V] vs charge [pC] )


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
#include "TH2F.h"

#include <iostream>
#include <fstream>
#include <vector>


// defines function y = a + bx to fit data

double myfit(double* x, double* par){
    Double_t b = par[0];
    Double_t a = par[1];

    Double_t fitval = 0;

    fitval = (b * x[0] + a);

    return fitval;
}

// The argument is a string variable which contains the name of a txt file

void catena_lin(const string file_name) {
    
    // fit range defined by user
    const double XMIN1 = 50;
    const double XMAX1 = 150;
   
    // Data lines should be x,y,errx,erry
    TGraphErrors *g = new TGraphErrors(file_name.c_str());
    TCanvas *c1 = new TCanvas("c1", "c1", 1080, 1020);
    g->SetLineColor(kBlack);
    g->SetMarkerColor(kBlack);
    g->SetMarkerStyle(8);
    g->SetMarkerSize(0.8);
    g->SetTitle("Catena completa - linearity ");
    g->GetXaxis()->SetTitle("Q_{c} (pC)");
    g->GetXaxis()->SetTitleSize(0.04);
    g->GetXaxis()->SetTickLength(0.02);
    g->GetYaxis()->SetTitle("V_{pre}^{max} (V)");
    g->GetYaxis()->SetTitleSize(0.04);
    g->GetYaxis()->SetTickLength(0.02);
    g->Draw("ap");
    
    
    TF1 *f1 = new TF1("f1",myfit,XMIN1,XMAX1,2);
    f1->SetParNames("b","a");
    TFitResultPtr fit_result = g->Fit(f1,"RS");
    f1->SetLineWidth(3);
    f1->SetLineColor(kGreen-3);
    f1->Draw("SAME");
    fit_result->Print("V");
    gStyle->SetOptFit(0);
    c1->Update();

   
    auto legend = new TLegend(0.11,0.7,0.3,0.9);
    legend->AddEntry(g,"Measures","ep");
    legend->AddEntry("f1","Function y = a + bx","l");
    legend->Draw();
    
    // displays fit parameters in a window
    TPaveText *pt = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt->AddText("Fit parameters");
    pt->AddText("a = (  0.05 #pm 0.03 )V");
    pt->AddText("b = ( 0.0149 #pm 0.0003 )pF^{-1}");
    pt->AddText("#chi^{(2)} / Ndof = 1.2 / 12");
    pt->AddText("#sigma_{y,post} = 0.01V");
    pt->SetFillColor(0);
    pt->SetTextAlign(12);
    pt->SetBorderSize(1);
    pt->Draw("same");
    
    // makes it nicer to see :)
    TText *t = pt->GetLineWith("parameters");
    t->SetTextColor(kGreen-3);
    
    c1->Update();
    
    
    // calculates residuals
    TGraphErrors *gr = new TGraphErrors(file_name.c_str());
    for (int i=0; i<g->GetN(); i++) {
      double res = g->GetY()[i] - f1->Eval(g->GetX()[i]); // residual
      gr->SetPoint(i,g->GetX()[i],res);
      double eresy = g->GetEY()[i];
      gr->SetPointError(i,0,eresy);
    }
    
    
    // new canvas
    TCanvas *c2 = new TCanvas("c2", "c2", 1080, 320);
    gr->SetTitle("");
    gr->SetMarkerColor(kBlack);
    gr->SetMarkerStyle(8);
    gr->SetMarkerSize(0.7);
    gr->SetLineWidth(1);
    gr->GetXaxis()->SetTitle("Q_{c} (pC)");
    gr->GetXaxis()->SetTitleSize(0.05);
    gr->GetXaxis()->SetTickLength(0.02);
    gr->GetYaxis()->SetTitle("y-y_{th} (V)");
    gr->GetYaxis()->SetTitleSize(0.06);
    gr->GetYaxis()->SetTickLength(0.02);
    gr->Draw("ap");
    gr->GetYaxis()->SetRangeUser(-0.08,0.08);
        
    // linea di zero per i residui
    TLine *line = new TLine(XMIN1, 0, XMAX1, 0);
    line->SetLineStyle(2);
    line->SetLineColor(kBlack);
    line->Draw("same");
    c2->Update();
   
    
    auto legend2 = new TLegend(0.11,0.7,0.3,0.9);
    legend2->SetTextSize(0.045);
    legend2->AddEntry(gr,"Residuals","ep");
    legend2->Draw();
    
    
    double err_post_squared = 0;
    const double m = fit_result->Parameter(0);
    const double q = fit_result->Parameter(1);
    
    // errore a posteriori
    for(unsigned int j = 0; j < g->GetN(); j++) {
        err_post_squared += pow( q + ( m * g->GetX()[j] ) - g->GetY()[j] , 2 ) / (  g->GetN() - 2);
    }
    double err_post= pow(err_post_squared,0.5);

    cout << "errore a posteriori: \t" << err_post << endl;
   
}


  
    
