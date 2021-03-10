// Author: Vanessa Cerrone
// Credit to Nicolò Lai for plot_func function
// Last modification: 06/03/2021

// Description: Catena completa Bode plot


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


double myfit(double* x, double* par){
    Double_t b = par[0];
    Double_t a = par[1];

    Double_t fitval = 0;

    fitval = (b * x[0] + a);

    return fitval;
}
 
double plot_func(double* x, double* par){
    double b = par[0];
    double a = par[1];

    double func = 0;

    func = (b * log10(x[0]) + a);

    return func;
}


// The argument is a string variable which contains the name of a txt file
// containing two columns:  f(Hz)   A(dB)
void catena_bode_plot(const string file_name){
    
    // Crea il grafico solo con i punti, f(Hz) e A(dB)
    TGraphErrors *g = new TGraphErrors(file_name.c_str());
    TCanvas *c1 = new TCanvas("c1", "c1", 1080, 720);
    c1->SetLogx();
    g->SetLineColor(kBlack);
    g->SetMarkerColor(kBlack);
    g->SetMarkerStyle(8);
    g->SetMarkerSize(0.6);
    g->SetTitle("Catena completa - Bode plot ");
    g->GetXaxis()->SetTitle("f(Hz)");
    g->GetXaxis()->SetTitleSize(0.04);
    g->GetXaxis()->SetTickLength(0.02);
    g->GetYaxis()->SetTitle("A(dB)");
    g->GetYaxis()->SetTitleSize(0.04);
    g->GetYaxis()->SetTickLength(0.02);
    g->GetYaxis()->SetRangeUser(TMath::MinElement(g->GetN(),g->GetY())-3,TMath::MaxElement(g->GetN(),g->GetY())+3);
    g->GetXaxis()->SetRangeUser(-100,1200000);
    c1->Update();
    g->Draw("ap");
    
}
    


// The argument is 2 string variables which contain the name of 2 txt file
//  1) f(Hz) vs A(dB)
//  2) with logarithmic frequencies  log(f) vs A(dB)


/*
 
    N.B. - IMPOSTA IL RANGE DEL FIT MANUALMENTE IN BASE AL CASO SPECIFICO
         - MODIFICA IL NUMERO DI DATI DA CONSIDERARE NEL CALCOLO DEGLI ERRORI A POSTERIORI
 
 */


void catena_bode_fit(const string file_name,const string file_name_log){
    
    TCanvas *c1 = new TCanvas("c1", "c1", 1080, 720);
    
    TGraphErrors *g = new TGraphErrors(file_name.c_str());
    c1->SetLogx();
    g->SetLineColor(kBlack);
    g->SetMarkerColor(kBlack);
    g->SetMarkerStyle(8);
    g->SetMarkerSize(0.8);
    g->SetTitle("Catena completa - Bode plot");
    g->GetXaxis()->SetTitle("f(Hz)");
    g->GetXaxis()->SetTitleSize(0.04);
    g->GetXaxis()->SetTickLength(0.02);
    g->GetYaxis()->SetTitle("A(dB)");
    g->GetYaxis()->SetTitleSize(0.04);
    g->GetYaxis()->SetTickLength(0.02);
    g->GetYaxis()->SetRangeUser(TMath::MinElement(g->GetN(),g->GetY())-3,TMath::MaxElement(g->GetN(),g->GetY())+3);
    g->GetXaxis()->SetRangeUser(-100,1200000);
    c1->Update();
    g->Draw("ap");
    
    
    TGraphErrors *g1= new TGraphErrors(file_name_log.c_str());

    TF1 *f1 = new TF1("f1",myfit,4.3,5.1,2); // fit retta da -40db
    f1->SetParNames("b","a");
    TFitResultPtr fit_result = g1->Fit(f1,"RS");
    //f1->Draw("SAME");
    fit_result->Print("V");
    
    // get parameters
    const double a = f1->GetParameter(1);
    const double b = f1->GetParameter(0);

    // new fuction with logarithmic frequencies
    TF1 *func = new TF1("plot_func", plot_func, 0, 10e+06, 2);

    // set the old parameters
    func->SetParameter(0, b);
    func->SetParameter(1, a);

    func->SetLineColor(kPink+10);
    func->SetLineWidth(4);
    func->SetLineStyle(9);
    func->Draw("same");
    

    TF1 *f2 = new TF1("f2",myfit,0.5,3.1,2); // fit retta orizzontale
    f2->SetParNames("d","c");
    TFitResultPtr fit_result2 = g1->Fit(f2,"RS"); //
    //f2->Draw("SAME");
    fit_result2->Print("V");
    
    // get parameters
    const double c = f2->GetParameter(1);
    const double d = f2->GetParameter(0);

    // new fuction with logarithmic frequencies
    TF1 *func2 = new TF1("plot_func2", plot_func, 0, 18000, 2);

    // set the old parameters
    func2->SetParameter(0, d);
    func2->SetParameter(1, c);

    func2->SetLineColor(kAzure+7);
    func2->SetLineWidth(4);
    func2->SetLineStyle(9);
    func2->Draw("same");
    
    
    /* EVENTUALI SIMULAZIONI
     
    TGraph *g3= new TGraph("simulation.txt");
    g3->SetLineColor(kGreen-3);
    g3->SetLineWidth(4);
    g3->SetLineStyle(2);
    g3->SetMarkerStyle(29);
    g3->SetMarkerColor(kAzure+10);
    g3->SetMarkerSize(0.5);
    g3->Draw("l SAME");
    
    c1->Update();
    
    TGraph *g4= new TGraph("simulation2.txt");
    g4->SetLineColor(kOrange+7);
    g4->SetLineWidth(4);
    g4->SetLineStyle(2);
    g4->SetMarkerStyle(29);
    g4->SetMarkerColor(kAzure+10);
    g4->SetMarkerSize(0.5);
    g4->Draw("l SAME");
    
    c1->Update();
    */
    
    
    // calcola la frequenza di taglio
    Double_t f_taglio;
    f_taglio= pow(10,(c-a)/(b-d));
    cout << "Frequenza di taglio da intersezione: \t" << f_taglio << " Hz" << endl;
    
    // disegna una linea verticale in corrispondenza della frequenza di taglio
    TLine *line_x_taglio = new TLine(f_taglio,TMath::MinElement(g->GetN(),g->GetY()),f_taglio,TMath::MaxElement(g->GetN(),g->GetY())+1);
    line_x_taglio->SetLineStyle(2);
    line_x_taglio->SetLineWidth(1);
    line_x_taglio->SetLineColor(kBlack);
    //line_x_taglio->Draw("same");
    
    // disegna una linea orizzontale in corrispondenza della massima amplificazione
    TLine *line_x_max = new TLine(0,18.717650007765,1000000,18.717650007765); // massimo preso da dati sperimentali
    line_x_max->SetLineStyle(2);
    line_x_max->SetLineWidth(1);
    line_x_max->SetLineColor(kBlack);
    //line_x_max->Draw("l same");
    c1->Update();
    
 
    
    auto legend = new TLegend(0.13,0.7,0.3,0.9);
    legend->SetTextSize(0.027);
    legend->AddEntry(g,"Measures","ep");
    legend->AddEntry(func,"Fit function y = a + bx ","l"); // retta da -40db
    legend->AddEntry(func2,"Fit function y = c + dx ","l"); // retta orizzontale
    //legend->AddEntry(line_x_taglio,"x = f_{t} , frequenza di taglio","l");
    //legend->AddEntry(g2,"LTspice simulated data","l");
    //legend->AddEntry(g3,"LTspice simulated data","l");
    legend->Draw();
    
    
    // mostra parametri dei fit in una finestra (da modificare i valori numerici)
    TPaveText *pt = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt->AddText("Fit parameters function y = a + bx");
    pt->AddText("a = ( 158 #pm 5 )dB");
    pt->AddText("b = ( -35 #pm 1 )dB/decade");
    pt->AddText("#chi^{(2)} / Ndof = 2.8 / 2");
    pt->AddText("#sigma_{y,post} = 0.1 dB");
    pt->SetFillColor(0);
    pt->SetTextAlign(12);
    pt->SetBorderSize(1);
    pt->Draw("same");

    // rende il titolo della finestra colorato
    TText *t = pt->GetLineWith("parameters");
    t->SetTextColor(kPink+10);
    
    
    TPaveText *pt2 = new TPaveText(0.2,0.1,0.5,0.5,"blNDC");
    pt2->AddText("Fit parameters function y = c + dx");
    pt2->AddText("c = ( 18.7 #pm 0.2 )dB");
    pt2->AddText("d = ( -0.07  #pm 0.09 )dB/decade");
    pt2->AddText("#chi^{(2)} / Ndof = 0.2 / 7");
    pt2->AddText("#sigma_{y,post} = 0.03 dB");
    pt2->SetFillColor(0);
    pt2->SetTextAlign(12);
    pt2->SetBorderSize(1);
    pt2->Draw("same");
    
    TText *t2 = pt2->GetLineWith("parameters");
    t2->SetTextColor(kAzure+7);
    
    c1->Update();
    
    
    
    // grafico dei residui
    auto mgerr = new TMultiGraph();
    
    TGraphErrors *gr = new TGraphErrors(file_name_log.c_str());  // retta da -40dB
    for (int i=23; i<28; i++) {
      double res = g1->GetY()[i] - f1->Eval(g1->GetX()[i]); // residuo
      gr->SetPoint(i,g1->GetX()[i],res);
      double eresy = g1->GetEY()[i];
      gr->SetPointError(i,0,eresy);
    }
    
    gr->SetName("gr");
    gr->SetMarkerColor(kPink+10);
    gr->SetLineColor(kBlack);
    gr->SetMarkerStyle(8);
    gr->SetMarkerSize(0.7);
    
    
    TGraphErrors *gr2 = new TGraphErrors(file_name_log.c_str());  // retta orizzontale
    for (int i=0; i<9; i++) {
      double res = g1->GetY()[i] - f2->Eval(g1->GetX()[i]); // residuo
      gr2->SetPoint(i,g1->GetX()[i],res);
      double eresy = g1->GetEY()[i];
      gr2->SetPointError(i,0,eresy);
    }
    
    gr2->SetName("gr2");
    gr2->SetMarkerColor(kAzure+7);
    gr2->SetLineColor(kBlack);
    gr2->SetMarkerStyle(8);
    gr2->SetMarkerSize(0.7);
    
    
    // nuovo canvas
    TCanvas *c2 = new TCanvas("c2", "c2", 1080, 300);
    
    mgerr->Add(gr);
    mgerr->Add(gr2);
    
    mgerr->SetTitle("");
    mgerr->GetXaxis()->SetTitle("log[f(Hz)/1Hz]");
    mgerr->GetXaxis()->SetTitleSize(0.05);
    mgerr->GetXaxis()->SetTitleOffset(0.95);
    mgerr->GetXaxis()->SetTickLength(0.02);
    mgerr->GetYaxis()->SetTitle("y-y_{th} (dB)");
    mgerr->GetYaxis()->SetTitleSize(0.06);
    mgerr->GetYaxis()->SetTitleOffset(0.4);
    mgerr->GetYaxis()->SetTickLength(0.02);
    mgerr->Draw("ap");
    mgerr->GetYaxis()->SetRangeUser(-0.8,0.8);
    mgerr->GetXaxis()->SetRangeUser(0,6);
    
    // linea di zero per i residui
    TLine *line = new TLine(0.7, 0, 5.2, 0);
    line->SetLineStyle(2);
    line->SetLineColor(kBlack);
    line->Draw("same");
    c2->Update();
    
    auto legend2 = new TLegend(0.11,0.7,0.3,0.9);
    legend2->SetTextSize(0.05);
    legend2->AddEntry(gr,"Residui fit y = a + bx","ep");
    legend2->AddEntry(gr2,"Residui fit y = c + dx","ep");
    legend2->Draw();
   
    
    // errore a posteriori
    double err_post_squared = 0;
   
    for(unsigned int j = 23; j < 28;j++) {
        err_post_squared += pow( a + ( b * g1->GetX()[j] ) - g1->GetY()[j] , 2 ) / ( f1->GetNDF());
    }
   
    
    double err_post_squared2 = 0;
     
     for(unsigned int j = 0; j < 9;j++) {
         err_post_squared2 += pow( c + ( d * g1->GetX()[j] ) - g1->GetY()[j] , 2 ) / ( f2->GetNDF());
      }
     
  
    double err_post= pow(err_post_squared,0.5);
    double err_post2= pow(err_post_squared2,0.5);
    cout << "errore a posteriori retta 20dB: \t" << err_post << endl;
    cout << "errore a posteriori retta orizzontale: \t" << err_post2 << endl;
  
}





