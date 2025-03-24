#include <iostream>

#include "TFile.h"

const char* title = "ALICE SIM, PbPb #sqrt{s_{NN}} = 5.36 TeV";
const char* channel = "Run 3 #color[2]{#rho' #rightarrow 2#pi^{+}2#pi^{-}}";
const char* xtitle = "random";
const char* ytitle = "random y";
const char* draw_option = "e";
vector<const char*> cut_list;
int chid = 0;
TFile* file;
void help(){
cout << "run setchannel(channel_id) to set channel default is 4pi \n"
     << " or  \n setchannel(xtitle,ytitle , channel_title) \n"  
     << "channel id \n"
     << "11 = 4pi mass \n 2 = dimu \n";

}
void setchannel (int channel_id){ // some predefined channel for header and x y title
    if (channel_id ==11) {
        channel = "Run 3 #color[2]{#rho' #rightarrow 2#pi^{+}2#pi^{-}}";
        xtitle = "M_{2#pi^{+}2#pi^{-}} [GeV/#it{c}^{2}]";
        ytitle = "GeV/#it{c^{2}})";
        chid = 11;
    }
    
    if (channel_id ==0) {
        channel = "Run 3 #color[2]{#rho' #rightarrow 2#pi^{+}2#pi^{-}}";
        xtitle = "M_{2#pi^{+}2#pi^{-}} [GeV/#it{c}^{2}]";
        ytitle = "Efficiency Rec/Gen";
        chid = 0;
    }
    

}
void setchannel(const char* xt , const char* yt, const char* ch){ // custom channel
    channel = ch;
    xtitle = xt;
    ytitle = yt;
    chid =0;

}

void setdrawopt(const char* drawopt){
    draw_option = drawopt;
    cout<< "draw option set " << drawopt << endl;
}

void setcutlist(vector<const char*> ctlist){ //selection parameters to show in histogram
    for(int i=0;i<ctlist.size();i++){
        cut_list.push_back(ctlist[i]);
    }


}

//const char* cutlist = {}
TFile* openfile(const char* filename = "test.root" ){
file = TFile::Open(filename);
file->ls();
return file;
}
void BeautifyPad(){
  gPad->SetMargin(0.13,0.10,0.12,0.10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  // gPad->SetGridx();
  // gPad->SetGridy();
  gStyle->SetOptStat(0);
}
void BeautifyHisto(TH1* histogram){
  histogram->SetTitle("");
  histogram->GetXaxis()->SetTitleOffset(1.15);
  histogram->GetYaxis()->SetTitleOffset(0.85);
  histogram->GetXaxis()->SetTitleSize(0.045);
  histogram->GetYaxis()->SetTitleSize(0.045);
  histogram->GetXaxis()->SetLabelSize(0.045);
  histogram->GetYaxis()->SetLabelSize(0.045);
  histogram->GetXaxis()->SetTitleFont(42);
  histogram->GetYaxis()->SetTitleFont(42);
  histogram->GetXaxis()->SetLabelFont(42);
  histogram->GetYaxis()->SetLabelFont(42);
  histogram->SetLineWidth(5);
  histogram->SetLineColor(1);
  histogram->SetLineColor(kBlue);
  histogram->SetLineStyle(kSolid);
  histogram->SetLineWidth(3);
  histogram->SetMarkerStyle(kFullCircle);
  histogram->SetMarkerColor(kBlue);
  histogram->SetMarkerSize(1);
  histogram->GetXaxis()->SetTitleSize(0.05);
  histogram->GetYaxis()->SetTitleSize(0.05);
  histogram->GetXaxis()->SetLabelSize(0.04);
  histogram->GetYaxis()->SetLabelSize(0.04);

 // Set marker style and color
 histogram->SetMarkerStyle(20);
 histogram->SetMarkerSize(0.8);
 histogram->SetMarkerColor(kBlack);    

  
 histogram->Draw(draw_option);  
 histogram->GetXaxis()->SetTitle(xtitle);
 if(chid!=0) histogram->GetYaxis()->SetTitle( Form( "Counts / (%.3f %s",
                                                          histogram->GetXaxis()->GetBinWidth(1)
                                                      ,ytitle)
                                                    );
  else   histogram->GetYaxis()->SetTitle(ytitle);
  histogram->SetTitle("");  
    
 // histogram->Draw("");
}
TH1* gethisto(const char* histname){
        TH1* histogram= (TH1*) file->Get(histname);
if(!histogram) {
   cout<< " histogram is empty \n";
   return 0x0;
}
return histogram;
}//end get histo 

void plot(TH1* histogram, const char* pdfname = "temp.pdf" ,float xmin =0  , float xmax = 10){

    TCanvas * Canvas = new TCanvas("Canvas","",600,600); 
    
    int histmax = histogram->GetMaximumBin();
    Double_t x_max = histogram->GetBinCenter(histmax);
    Double_t y_max = histogram->GetBinContent(histmax);
    //setting range of the histogram
    
    histogram->GetXaxis()->SetRangeUser(xmin,xmax);
    histogram->GetYaxis()->SetRangeUser(0,y_max*1.5);   
    
   

    BeautifyPad();    
    BeautifyHisto(histogram);
    TLatex* latex = new TLatex();
    latex->SetTextSize(0.05);
    latex->SetTextFont(42);
    latex->SetTextAlign(11);
    latex->SetNDC();
    latex->DrawLatex(0.17,0.82,title);
    latex->SetTextSize(0.045);
    latex->DrawLatex(0.45,0.76,channel);
    //latex->DrawLatex(0.45,0.78,"#it{L} = 235 ub^{-1}");
    for(int k =0; k<cut_list.size();k++){
        latex->DrawLatex(0.45,0.64-k*0.8,cut_list[k]);    
    }
    



    //Canvas->Draw();
    Canvas->SaveAs(pdfname);    


    //fileList->Close();
}// end of jpsi fit


void plot_all(){
cout<< "now you can start plotting"<< endl;
}