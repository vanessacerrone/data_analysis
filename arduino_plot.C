// Author: Vanessa Cerrone

// Last modification: 05/03/2021

// Description: - converts data acquired with Arduino Due ADC using the
//                calibration parameters.
//              - plots data ( voltage [mV] vs time [ms])
 
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


// The argument is a string variable which contains the name of a txt file
// Data lines should be x [a.u] ,y [ADC counts]

void arduino_plot(const string file_name)

{
    
TGraph *g = new TGraph(file_name.c_str());
    
    vector<double> time;
    vector<double> voltage;
    vector<double> errx;
    vector<double> erry;
    
    const double a= -0.546364;  // offset [V]
    const double b= 0.00076213; // slope [V/counts]
    const double err = 0.001395451090082; // sigma_y [V]
    
    for (unsigned int i = 0; i < g->GetN(); i++)
    {
        time.push_back(  g->GetX()[i] * 1.05 *1e-03); // using sampling time t = 1.05 us
    }
    
    for (unsigned int i = 0; i < g->GetN(); i++)
    {
        voltage.push_back( (a + g->GetY()[i]*b));
    }
    
    for (unsigned int i = 0; i < g->GetN(); i++)
    {
        errx.push_back(0); // setting sigma_x to zero
    }
   
    for (unsigned int i = 0; i < g->GetN(); i++)
    {
        erry.push_back(err*1000);
    }
    
    
    ofstream f("arduino_plot.txt"); // writes converted data in a txt file

    for(unsigned int i = 0; i < time.size(); i++)
    {
        f << time[i] << "\t" << voltage[i] << "\t" << errx[i] << "\t" << erry[i] <<'\n';
    }
    
    
    TGraph *g3 = new TGraph(g->GetN(), &time[0], &voltage[0]);
    TCanvas *c2 = new TCanvas("c2", "c2", 1080, 1020);
    g3->SetLineColor(kAzure+7);
    g3->SetLineWidth(3);
    g3->SetMarkerColor(kAzure+7;
    g3->SetMarkerStyle(8);
    g3->SetMarkerSize(0.7);
    g3->SetTitle("Shaper - Arduino waveform plot");
    g3->GetXaxis()->SetTitle("time (ms)");
    g3->GetXaxis()->SetTitleSize(0.04);
    g3->GetXaxis()->SetTickLength(0.02);
    g3->GetYaxis()->SetTitle("voltage (mV)");
    g3->GetYaxis()->SetTitleSize(0.04);
    g3->GetYaxis()->SetTickLength(0.02);
    g3->GetXaxis()->SetRangeUser(0,2);
    //g3->GetXaxis()->SetMaxDigits(2);
                       
    g3->Draw("al"); // plots converted data
    
    
    
}

