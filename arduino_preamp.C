// Author: Vanessa Cerrone

// Last modification: 07/03/2021

// Descrizione: implementa due funzioni void per eseguire due fit:
// - Fit esponenziale della fase di scarica del segnale in uscita del preamp, voltage [mV] vs time [ms]
// - Fit linearizzato ln(V) vs time [ms] con V resa opportunamente adimensionale

 
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


double myfit(double* x, double* par){
    Double_t a = par[0];
    Double_t b = par[1];
    Double_t tau = par[2];
    Double_t fitval = 0;

    fitval = ( a + b * exp(-x[0]/tau));

    return fitval;
}



double myfit2(double* x, double* par){
    Double_t a = par[0];
    Double_t b = par[1];
    Double_t fitval = 0;

    fitval = ( a + b *x[0] );

    return fitval;
}


// L'argomento dell funzione è una stringa che contiene il nome di un file di testo
// Nel file di testo i dati devono essere della forma x,y,errx,erry
// NB: Le y sono i valori di tensione già convertiti in mV

void arduino_exp(const string file_name){
    
    TCanvas *c1 = new TCanvas("c1", "c1", 1080, 1020);
    TGraphErrors *g1 = new TGraphErrors(file_name.c_str());
    g1->SetLineColor(kBlue);
    g1->SetMarkerColor(kBlue);
    g1->SetMarkerStyle(8);
    g1->SetMarkerSize(0.4);
    g1->SetTitle("Preamp - Arduino exponential fit");
    g1->GetXaxis()->SetTitle("time (ms)");
    g1->GetXaxis()->SetTitleSize(0.04);
    g1->GetXaxis()->SetTickLength(0.02);
    g1->GetYaxis()->SetTitle("voltage (mV)");
    g1->GetYaxis()->SetTitleSize(0.04);
    g1->GetYaxis()->SetTickLength(0.02);
    g1->GetYaxis()->SetRangeUser(0,400);
    g1->GetXaxis()->SetRangeUser(0,0.6);
    g1->Draw("al");

    // fit esponenziale
    TF1 *f1 = new TF1("f1",myfit,-0.01,0.6,3);
    f1->SetParNames("a","b","tau");
    f1->SetParameters(0,359,0.14);
    f1->SetLineColor(kRed);
    f1->SetLineWidth(3);
    TFitResultPtr fit_result = g1->Fit(f1,"RS"); //
    fit_result->Print("V");
    f1->Draw("same");
    
    // legenda
    auto legend = new TLegend(0.11,0.7,0.3,0.9);
    legend->AddEntry(g1,"Measures - Dataset 3 ","ep");
    legend->AddEntry("f1","Function y = a + b e^{-x/#tau}","l");
    legend->Draw();
    
    // box con parametri del fit
    TPaveText *pt = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt->AddText("Fit parameters");
    pt->AddText("a = ( 2.9 #pm 0.2 )mV");
    pt->AddText("b = ( 359.6 #pm 0.2 )mV");
    pt->AddText("#tau = ( 153.6 #pm 0.3 )#mus");
    pt->AddText("#chi^{(2)} / Ndof = 310 / 535");
    pt->AddText("#sigma_{y,post} = 1.4mV");
    pt->SetFillColor(0);
    pt->SetTextAlign(12);
    pt->SetBorderSize(1);
    pt->Draw("same");
    
    // rende il titolo colorato
    TText *t = pt->GetLineWith("parameters");
    t->SetTextColor(kRed);

    c1->Update();
    
    // grafico dei residui
    TGraphErrors *gr = new TGraphErrors();
    for (int i=0; i < g1->GetN(); i++) {
      double res = g1->GetY()[i] - f1->Eval(g1->GetX()[i]); // residuo
      gr->SetPoint(i,g1->GetX()[i],res);
      double eresy = g1->GetEY()[i];
      gr->SetPointError(i,0,eresy);
    
    }
    
    TCanvas *c2 = new TCanvas("c2", "c2", 1080, 300); // nuovo canvas
    gr->SetName("gr");
    gr->SetMarkerColor(kBlue);
    gr->SetLineColor(kBlue);
    gr->SetMarkerStyle(8);
    gr->SetMarkerSize(0.6);
    gr->SetTitle("");
    gr->GetXaxis()->SetTitle("time (ms)");
    gr->GetXaxis()->SetTitleSize(0.05);
    gr->GetXaxis()->SetTitleOffset(0.95);
    gr->GetXaxis()->SetTickLength(0.02);
    gr->GetYaxis()->SetTitle("y-y_{th} (mV)");
    gr->GetYaxis()->SetTitleSize(0.06);
    gr->GetYaxis()->SetTitleOffset(0.4);
    gr->GetYaxis()->SetTickLength(0.02);
    gr->SetFillColor(38);
    gr->SetFillStyle(3002);
    gr->Draw("3ap"); // rende il grafico leggibile dato il grande numero di dati
    gr->GetYaxis()->SetRangeUser(-8,8);
    gr->GetXaxis()->SetRangeUser(-0.1,0.6);
    
    // linea di zero per i residui
    TLine *line = new TLine(0, 0, 0.6, 0);
    line->SetLineStyle(2);
    line->SetLineColor(kBlack);
    line->Draw("same");
    c2->Update();
    
    // legenda
    auto legend2 = new TLegend(0.11,0.7,0.3,0.9);
    legend2->SetTextSize(0.05);
    legend2->AddEntry(gr,"Residui fit y = a + b e^{-x/#tau}","p");
    legend2->Draw();
    
    
    // errore a posteriori
    double err_post_squared = 0;

    for(unsigned int j = 0; j < g1->GetN();j++) {
        err_post_squared += pow( f1->Eval(g1->GetX()[j]) - g1->GetY()[j] , 2 ) / ( f1->GetNDF());
    }
    
double err_post= pow(err_post_squared,0.5);
                                
cout << "errore a posteriori  : \t" << err_post << endl;
                                
                                
   
    
}



// L'argomento dell funzione è una stringa che contiene il nome di un file di testo
// Nel file di testo i dati devono essere della forma x,y,errx,erry
// NB: Le y sono i valori di ln(V) adimensionalizzato
void arduino_log(const string file_name_log){
    TCanvas *c1 = new TCanvas("c1", "c1", 1080, 1020);
    TGraphErrors *g1 = new TGraphErrors(file_name_log.c_str());
    g1->SetLineColor(kBlue);
    g1->SetMarkerColor(kBlue);
    g1->SetMarkerStyle(8);
    g1->SetMarkerSize(0.4);
    g1->SetTitle("Preamp - Arduino linear fit");
    g1->GetXaxis()->SetTitle("time (ms)");
    g1->GetXaxis()->SetTitleSize(0.04);
    g1->GetXaxis()->SetTickLength(0.02);
    g1->GetYaxis()->SetTitle("ln(#bar{V})");
    g1->GetYaxis()->SetTitleSize(0.04);
    g1->GetYaxis()->SetTickLength(0.02);
    g1->GetYaxis()->SetRangeUser(-5,0);
    g1->GetXaxis()->SetRangeUser(0,0.6);
    g1->Draw("ap");
    
    
    TF1 *f1 = new TF1("f1",myfit2,-0.01,0.6,2); // fit linearizzato
    f1->SetParNames("a","b");
    f1->SetLineColor(kRed);
    f1->SetLineWidth(3);
    TFitResultPtr fit_result = g1->Fit(f1,"RS");
    fit_result->Print("V");
    f1->Draw("same");
    
    auto legend = new TLegend(0.11,0.7,0.3,0.9);
    legend->AddEntry(g1,"Measures - Dataset 3 ","ep");
    legend->AddEntry("f1","Function y = a + bx","l");
    legend->Draw();
    
    
    TPaveText *pt = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt->AddText("Fit parameters");
    pt->AddText("a = ( 0.0002 #pm 0.0007 )");
    pt->AddText("b = ( -6.507 #pm 0.006 )ms^{-1}");
    pt->AddText("#chi^{(2)} / Ndof = 460 / 545");
    pt->AddText("#sigma_{y,post} =0.07");
    pt->SetFillColor(0);
    pt->SetTextAlign(12);
    pt->SetBorderSize(1);
    pt->Draw("same");
    
    TText *t = pt->GetLineWith("parameters");
    t->SetTextColor(kRed);

    c1->Update();
    
    // grafico dei residui
    TGraphErrors *gr = new TGraphErrors();
    for (int i=0; i < g1->GetN(); i++) {
      double res = g1->GetY()[i] - f1->Eval(g1->GetX()[i]); // residuo
      gr->SetPoint(i,g1->GetX()[i],res);
      double eresy = g1->GetEY()[i];
      gr->SetPointError(i,0,eresy);
    
    }
    
    TCanvas *c2 = new TCanvas("c2", "c2", 1080, 300);
    gr->SetName("gr");
    gr->SetMarkerColor(kBlue);
    gr->SetLineColor(kBlue);
    gr->SetMarkerStyle(8);
    gr->SetMarkerSize(0.6);
    gr->SetTitle("");
    gr->GetXaxis()->SetTitle("time (ms)");
    gr->GetXaxis()->SetTitleSize(0.05);
    gr->GetXaxis()->SetTitleOffset(0.95);
    gr->GetXaxis()->SetTickLength(0.02);
    gr->GetYaxis()->SetTitle("y-y_{th}");
    gr->GetYaxis()->SetTitleSize(0.06);
    gr->GetYaxis()->SetTitleOffset(0.4);
    gr->GetYaxis()->SetTickLength(0.02);
    gr->SetFillColor(38);
    gr->SetFillStyle(3002);
    gr->Draw("3ap");
    gr->GetYaxis()->SetRangeUser(-0.5,0.5);
    gr->GetXaxis()->SetRangeUser(-0.1,0.6);
    
    //linea di zero per i residui
    TLine *line = new TLine(0, 0, 0.6, 0);
    line->SetLineStyle(2);
    line->SetLineColor(kBlack);
    line->Draw("same");
    c2->Update();
    
    auto legend2 = new TLegend(0.11,0.7,0.3,0.9);
    legend2->SetTextSize(0.05);
    legend2->AddEntry(gr,"Residui fit y = a + bx","p");
    legend2->Draw();
    
    // errore a posteriori
    double err_post_squared = 0;

    for(unsigned int j = 0; j < g1->GetN();j++) {
        err_post_squared += pow( f1->Eval(g1->GetX()[j]) - g1->GetY()[j] , 2 ) / ( f1->GetNDF());
    }
    
double err_post= pow(err_post_squared,0.5);
double b = f1->GetParameter(1);
  
                                
    cout << "errore a posteriori  : \t" << err_post << endl;
    
    cout << "tau  : \t" << -1/b << endl; // tempo caratteristico
    
}
