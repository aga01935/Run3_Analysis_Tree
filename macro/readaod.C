#include <iostream>
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
using namespace std;

const char *input_file =  "AO2D_LHC24K4_gammatomulow.root";
const char *output_hist_file = "LHC24K4_gammagammamu_low.root";

const char *data_tree_name = "O2tree";

const char *mc_rectree_name = "O2mcrectree";
const char *mc_gentree_name = "O2mcgentree";
bool isMC = 1; // flag to run mc


int trk_size_max = 10;

// mass of particles in GeV
float muon_mass = 0.105658;
float pion_mass = 0.13957061;
float kaon_mass = 0.493677;
float electron_mass=  0.000511 ;  
    
TTree *datatree;
TTree *mcrectree;
TTree *mcgentree; 

void init_branch_variables(){

        
        
        

}// initialize branch variables

    
void readaod(float  particle_mass = muon_mass, int part_pid = 13, int trk_size = 2){ // number of track particle to look and particle_id
    
    
    // setting some limits here so that we do not have  set it  for each later
    float fit_max = 1000; // maximum fit amplitude; 
    float fit_min = -100; // minimum fit amplitude;
    float zna_max = 3000; // maxiumum zdc amplitude;
    float zna_min = -10 ; // minimum zdc amplitude;
    float znt_max = 100; // maxiumum zdc time ns;
    float znt_min = -100; // minimum zdc time ns;
    float vtx_max = 30 ; // maximum vertex position;
    float vtx_min = -30; //minimum vertex position
    
    int nbin_all = 1000; //number of bins for all plots keeping same for now
    
    
   
    //detector level QC info
    //fit
    TH1D* hFT0AAMP  = new TH1D("hFT0AAMP", "",nbin_all,fit_min,fit_max );
    TH1D* hFT0ACMP  = new TH1D("hFT0ACMP", "",nbin_all,fit_min,fit_max );
    TH1D* hFDDAAMP  = new TH1D("hFDDAAMP", "",nbin_all,fit_min,fit_max );
    TH1D* hFDDCAMP  = new TH1D("hFDDCAMP", "",nbin_all,fit_min,fit_max );
    
    //zdc **to do add proton if needed
    TH1D* hZNAAMP  = new TH1D("hZNAAMP","",nbin_all,zna_min,zna_max);
    TH1D* hZNCAMP  = new TH1D("hZNCAMP","",nbin_all,zna_min,zna_max);
    TH1D* hZNAtime = new TH1D("hZNAtime","",nbin_all,znt_min,znt_max);
    TH1D* hZNCtime = new TH1D("hZNCtime","",nbin_all,znt_min,znt_max);
    //vertex
    TH1D* hxVtx    = new TH1D("hxVtx","",nbin_all,vtx_min,vtx_max);
    TH1D* hyVtx    = new TH1D("hyVtx","",nbin_all,vtx_min,vtx_max);
    TH1D* hzVtx    = new TH1D("hzVtx","",nbin_all,vtx_min,vtx_max);
    
    
    
    
    
    
    
    
    
    
    
    //analyzed info
    float mass_max = 10; // maximum limit of mass 
    float mass_min = 0; // min limit of mass
    float pt_max  = 10; // maximum pt;
    float pt_min = 0; // minimum_pt;
    float rap_max = -10;
    float rap_min = -10;
    float phi_max = 7; // in case if 0 to 2pi ro 0 to pi
    float phi_min = -7; // in case if 0 to 2pi ro 0 to pi
    
    TH1D * InvariantMassRec = new TH1D("InvariantMassRec","",nbin_all,mass_min,mass_max);
    TH1D * InvariantMassRecTruth = new TH1D("InvariantMassRecTruth","",nbin_all,mass_min,mass_max);
    TH1D * InvariantMassGen = new TH1D("InvariantMassGen","",nbin_all,mass_min,mass_max);
    
    TH1D * PtRec       = new TH1D("PtRec","",nbin_all,pt_min,pt_max);
    TH1D * PtRecTruth  = new TH1D("PtRecTruth","",nbin_all,pt_min,pt_max);
    TH1D * PtGen       = new TH1D("PtGen","",nbin_all,pt_min,pt_max);
    
    TH1D * RapRec      = new TH1D("RapRec","",nbin_all,rap_min,rap_max);
    TH1D * RapRecTruth = new TH1D("RapRecTruth","",nbin_all,rap_min,rap_max);
    TH1D * RapGen      = new TH1D("RapGen","",nbin_all,rap_min,rap_max);
    
    TH1D * TrackPtRec  = new TH1D("TrackPtRec","",nbin_all,pt_min,pt_max);
    TH1D * TrackPtTruth = new TH1D("TrackPtTruth","",nbin_all,pt_min,pt_max);
    TH1D * TrackPtGen   = new TH1D("TrackPtGen","",nbin_all,pt_min,pt_max);
    
    TH1D * TrackEtaRec  = new TH1D("TrackEtaRec","",nbin_all,rap_min,rap_max);
    TH1D * TrackEtaTruth = new TH1D("TrackEtaTruth","",nbin_all,rap_min,rap_max);
    TH1D * TrackEtaGen   = new TH1D("TrackEtaGen","",nbin_all,rap_min,rap_max);
    
    TH1D * TrackPhiRec   = new TH1D("TrackPhiRec","",nbin_all,phi_min,phi_max);
    TH1D * TrackPhiTruth = new TH1D("TrackPhiTruth","",nbin_all,phi_min,phi_max);
    TH1D * TrackPhiGen   = new TH1D("TrackPhiGen","",nbin_all,phi_min,phi_max);
    
    
    
    //resolution rec- truth mc ( truth mc = mc particle with one to one correspondance with reconstructed particle)
    float res_min = -3;
    float res_max = 3; 
    int nbin_res = 100;
    TH1D* trackpt_res  =  new TH1D("trackpt_res","", nbin_res,res_min,res_max);
    TH1D* tracketa_res = new TH1D("tracketa_res","", nbin_res,res_min,res_max);
    TH1D* trackphi_res = new TH1D("trackphi_res","", nbin_res,res_min,res_max);
    
    
    
    
    // now reading files
    
    TString dirname;  // if there is directory
    TString data_tree_loc;
    TString rec_tree_loc;
    TString gen_tree_loc;
    
    TFile *inputFile = TFile::Open(input_file,"READ"); // reading input files
    if(!inputFile){
            cout << input_file <<" not found"<< endl;
        }    
    
    
    
    
 
    for (auto&& obj : *inputFile->GetListOfKeys()) { // loop to fill directory now it just expects on TDirectory 
           TKey* key = dynamic_cast<TKey*>(obj);     // if file has more than on directory then need to edit the code. 
           if (!key) continue; 

           if( string(key->GetClassName())== "TDirectoryFile") dirname = key->GetName();
        }
    if(dirname){
        data_tree_loc = TString::Format("%s/%s",dirname.Data(),data_tree_name);
        rec_tree_loc = TString::Format("%s/%s",dirname.Data(),mc_rectree_name);
        gen_tree_loc = TString::Format("%s/%s",dirname.Data(),mc_gentree_name);
    }
    else {
       
        data_tree_loc = TString::Format("%s",data_tree_name);
        rec_tree_loc = TString::Format("%s",mc_rectree_name);
        gen_tree_loc = TString::Format("%s",mc_gentree_name);
        
        
    }
    
    if(!isMC) {
            datatree = (TTree*)inputFile->Get(data_tree_loc.Data()); // data 
              }
    
    if(isMC){
        
        mcrectree = (TTree*)inputFile->Get(rec_tree_loc); // mcrec
       
        mcgentree = (TTree*)inputFile->Get(gen_tree_loc); //  // mcgen
        
        
    }
        //collision level info 
    int fGAPSIDE; // == 2 for exclusive 
    //ft0 amplituves
    float fFT0AAMP; 
    float fFT0CAMP;
    //fDD amplitudes
    float fFDDAAMP; 
    float fFDDCAMP;
    //fv0 amplitudes    
    float fFV0AAMP;
    //zdc neutron info    
    float fZAENERGY;
    float fZCENERGY;
    float fZATIME;
    float fZCTIME;
    //collision level tracks info    
    int fTOTSIGN;
    int fNPVTRACK;
    int fRUNNUM;
    int fUPCFLAG;    
    // vertex info 
    float fXVTX;
    float fYVTX;
    float fZVTX;    

    //mc vertex
    float fXVTXMC;
    float fYVTXMC;
    float fZVTXMC; 
    //mc gen vertex
    //mc vertex
    float fXVTXMC_gen;
    float fYVTXMC_gen;
    float fZVTXMC_gen;        


    //track info    
    int fPXS_size; // only one track info size is need to loop over tracks as all size are same 
    float fPXS[trk_size_max];
    float fPYS[trk_size_max];
    float fPZS[trk_size_max];
    float  fSIGNS[trk_size_max];

    //track info  mc rec
    float fPXSMC[trk_size_max];
    float fPYSMC[trk_size_max];
    float fPZSMC[trk_size_max];
    int  fPDGCODE[trk_size_max];
    int fPXSMC_gen_size;
    float fPXSMC_gen[trk_size_max];
    float fPYSMC_gen[trk_size_max];
    float fPZSMC_gen[trk_size_max];
    int  fPDGCODE_gen[trk_size_max];   
    //tpc pid
    float fSIGMAPI[trk_size_max];    
    float fSIGMAMU[trk_size_max];
    float fSIGMAEL[trk_size_max];
    float fSIGMAK[trk_size_max];    
    float fSIGMAPR[trk_size_max];    

    if(!isMC){
        datatree->SetBranchAddress("fGAPSIDE",&fGAPSIDE);
        datatree->SetBranchAddress("fPXS_size",&fPXS_size);
        datatree->SetBranchAddress("fNPVTRACK",&fNPVTRACK); 
        datatree->SetBranchAddress("fFT0AAMP",&fFT0AAMP);
        datatree->SetBranchAddress("fFT0CAMP",&fFT0CAMP);
        datatree->SetBranchAddress("fFDDAAMP",&fFDDAAMP);
        datatree->SetBranchAddress("fFDDCAMP",&fFDDCAMP);
        datatree->SetBranchAddress("fFV0AAMP",&fFV0AAMP); 
        datatree->SetBranchAddress("fZAENERGY",&fZAENERGY);
        datatree->SetBranchAddress("fZCENERGY",&fZCENERGY); 
        datatree->SetBranchAddress("fZATIME",&fZATIME);
        datatree->SetBranchAddress("fZCTIME",&fZCTIME); 
        datatree->SetBranchAddress("fUPCFLAG",&fUPCFLAG); 
        
        datatree->SetBranchAddress("fXVTX",&fXVTX);
        datatree->SetBranchAddress("fYVTX",&fYVTX);
        datatree->SetBranchAddress("fZVTX",&fZVTX);
        
        



        datatree->SetBranchAddress("fPXS",&fPXS);
        datatree->SetBranchAddress("fPYS",&fPYS);
        datatree->SetBranchAddress("fPZS",&fPZS);  
        
        datatree->SetBranchAddress("fSIGNS",&fSIGNS);
        datatree->SetBranchAddress("fTOTSIGN",&fTOTSIGN);    
        datatree->SetBranchAddress("fSIGMAMU",&fSIGMAMU);  
        datatree->SetBranchAddress("fSIGMAPI",&fSIGMAPI);
        datatree->SetBranchAddress("fSIGMAK",&fSIGMAK);     
    }
    
    if(isMC){
        mcrectree->SetBranchAddress("fGAPSIDE",&fGAPSIDE);
        mcrectree->SetBranchAddress("fPXS_size",&fPXS_size);
        mcrectree->SetBranchAddress("fNPVTRACK",&fNPVTRACK); 
        mcrectree->SetBranchAddress("fFT0AAMP",&fFT0AAMP);
        mcrectree->SetBranchAddress("fFT0CAMP",&fFT0CAMP);
        mcrectree->SetBranchAddress("fFDDAAMP",&fFDDAAMP);
        mcrectree->SetBranchAddress("fFDDCAMP",&fFDDCAMP);
        mcrectree->SetBranchAddress("fFV0AAMP",&fFV0AAMP); 
        mcrectree->SetBranchAddress("fZAENERGY",&fZAENERGY);
        mcrectree->SetBranchAddress("fZCENERGY",&fZCENERGY); 
        mcrectree->SetBranchAddress("fZATIME",&fZATIME);
        mcrectree->SetBranchAddress("fZCTIME",&fZCTIME);
        mcrectree->SetBranchAddress("fUPCFLAG",&fUPCFLAG); 
        
        mcrectree->SetBranchAddress("fXVTX",&fXVTX);
        mcrectree->SetBranchAddress("fYVTX",&fYVTX);
        mcrectree->SetBranchAddress("fZVTX",&fZVTX);
        
        mcrectree->SetBranchAddress("fXVTXMC",&fXVTXMC);
        mcrectree->SetBranchAddress("fYVTXMC",&fYVTXMC);
        mcrectree->SetBranchAddress("fZVTXMC",&fZVTXMC);
       
        mcrectree->SetBranchAddress("fPXS",&fPXS);
        mcrectree->SetBranchAddress("fPYS",&fPYS);
        mcrectree->SetBranchAddress("fPZS",&fPZS);
        mcrectree->SetBranchAddress("fPXSMC",&fPXSMC);
        mcrectree->SetBranchAddress("fPYSMC",&fPYSMC);
        mcrectree->SetBranchAddress("fPZSMC",&fPZSMC);
        mcrectree->SetBranchAddress("fPDGCODE",&fPDGCODE);
        mcrectree->SetBranchAddress("fSIGNS",fSIGNS);
        mcrectree->SetBranchAddress("fTOTSIGN",&fTOTSIGN);    
        mcrectree->SetBranchAddress("fSIGMAMU",&fSIGMAMU);  
        mcrectree->SetBranchAddress("fSIGMAPI",&fSIGMAPI);
        mcrectree->SetBranchAddress("fSIGMAK",&fSIGMAK);  
        
        mcgentree->SetBranchAddress("fXVTXMC",&fXVTXMC_gen);
        mcgentree->SetBranchAddress("fYVTXMC",&fYVTXMC_gen);
        mcgentree->SetBranchAddress("fZVTXMC",&fZVTXMC_gen); 
        mcgentree->SetBranchAddress("fPXSMC",&fPXSMC_gen);
        mcgentree->SetBranchAddress("fPYSMC",&fPYSMC_gen);
        mcgentree->SetBranchAddress("fPZSMC",&fPZSMC_gen);
        mcgentree->SetBranchAddress("fPDGCODE",&fPDGCODE_gen);
        mcgentree->SetBranchAddress("fPXSMC_size",&fPXSMC_gen_size);
    }// end of mc branch
    
    if(isMC){
    
    int nentry = mcrectree->GetEntries();
    for(int i =0; i<nentry ; i++){
       
        mcrectree->GetEntry(i);
        
        

        
        TLorentzVector parent_rec,parent_truth;
         
       // bool passed_pid = true;
        int trk_count =0; 
        if(fPXS_size == trk_size && fNPVTRACK == trk_size && fGAPSIDE==2 ) { // cuts
            
            hFT0AAMP->Fill(fFT0AAMP);
            hFT0ACMP->Fill(fFT0CAMP);
            hFDDAAMP->Fill(fFDDAAMP);
            hFDDCAMP->Fill(fFDDCAMP);

            hxVtx->Fill(fXVTX);
            hyVtx->Fill(fYVTX);
            hzVtx->Fill(fZVTX);

            hZNAAMP->Fill(fZAENERGY);
            hZNCAMP->Fill(fZCENERGY);
            hZNAtime->Fill(fZATIME);
            hZNCtime->Fill(fZCTIME);
           
           if (fZAENERGY>0) cout<< fZAENERGY << endl;
            vector <TLorentzVector> track_vect;
            vector <int> signs; 
            
            for (int j = 0; j < trk_size; j++) {
                TLorentzVector track_rec, track_truth;
                float part_sigma = -999.0;
                if(part_pid == 211 ) part_sigma = fSIGMAPI[j]; // pion case
                else if(part_pid == 13) part_sigma = fSIGMAMU[j]; //muon case
                else {
                    //case where no pid is defined add another elsif if you want to 
                    cout<< "particle pid provided is " << part_pid << "this case is not defined on this macro"<< endl;
                    break;
                }
                if (TMath::Abs(part_sigma) > 3) continue; // only saving pid < 3 line below
                trk_count++;
                track_rec.SetXYZM(fPXS[j], fPYS[j], fPZS[j], particle_mass);
                track_truth.SetXYZM(fPXSMC[j], fPYSMC[j], fPZSMC[j], particle_mass);

                // Fill track histograms
                TrackPtRec->Fill(track_rec.Pt());
                TrackEtaRec->Fill(track_rec.Eta());
                TrackPhiRec->Fill(track_rec.Phi());

                TrackPtTruth->Fill(track_truth.Pt());
                TrackEtaTruth->Fill(track_truth.Eta());
                TrackPhiTruth->Fill(track_truth.Phi());

                trackpt_res->Fill(track_rec.Pt()-track_truth.Pt());
                tracketa_res->Fill(track_rec.Eta()-track_truth.Eta());
                trackphi_res->Fill(track_rec.Phi()-track_truth.Phi());    



                track_vect.push_back(track_rec);
                
                float signrec;
                if (fSIGNS[j]>0) signrec = +1;
                if (fSIGNS[j]<0) signrec = -1;
                signs.push_back(signrec);
                parent_rec += track_rec;
                parent_truth += track_truth;
               }// for loop 
               
             // Fill parent particle histograms
            
            if(trk_count != trk_size)  continue; //should have n tracks after pid selections
            float totsign =0;
            for(int l = 0; l<signs.size(); l++){ totsign += signs[l]; 
                                              
                                               } // checking total sign
             
                
            if (totsign !=0) {
              cout<<   totsign << "###"<<  signs[0] << "  "<< signs[1]<<"  " << signs[2]<<"  " << signs[3]<<"  "<<endl; 
                continue;
            
            }
            InvariantMassRecTruth->Fill(parent_truth.M());
            PtRecTruth->Fill(parent_truth.Pt());
            RapRecTruth->Fill(parent_truth.Rapidity());
            InvariantMassRec->Fill(parent_rec.M());
            PtRec->Fill(parent_rec.Pt());
            RapRec->Fill(parent_rec.Rapidity());
            
            if(trk_count==4){
            TLorentzVector t1 = track_vect[0];
            TLorentzVector t2 = track_vect[1];
            TLorentzVector t3 = track_vect[2];
            TLorentzVector t4 = track_vect[3]  ;  
                
                
            }//trk_count = 4 case
                  
     } //  end cuts        
        
    }// end entry loop reconstructed 
        
        
    int nentrygen = mcgentree->GetEntries();
    for(int l =0; l<nentrygen ; l++){
        
        mcgentree->GetEntry(l);
       // if(fPXSMC_size== 2 ) { // cuts
        
        
        
        
         
        TLorentzVector parent;
        vector <TLorentzVector> track_vectgen;
        vector <int> signgen;
        vector<float> pxsmc,pysmc,pzsmc;
            for(int  j = 0; j<fPXSMC_gen_size;j ++){
            if( TMath::Abs(fPDGCODE_gen[j])==part_pid ) {    // selecting pion event 
                pxsmc.push_back(fPXSMC_gen[j]);
                pysmc.push_back(fPYSMC_gen[j]);
                pzsmc.push_back(fPZSMC_gen[j]);
                int sign;
                if(fPDGCODE_gen[j]>0) sign = +1;
                if(fPDGCODE_gen[j]<0) sign = -1;
                
                signgen.push_back(sign);
        
            }// pion only tracks    
                
              
            
            }// ntrack loop
               
            if(pxsmc.size()==trk_size ){ // making sure that we only have 2n track event at the end (trk_size = 2 ,4 , 6....)
                 
                for (int r = 0; r < trk_size; r++) {
                    TLorentzVector track;
                    track.SetXYZM(pxsmc[r], pysmc[r], pzsmc[r], particle_mass);
                    //tracks.push_back(track);

                    // Fill histograms for each track
                    TrackPtGen->Fill(track.Pt());
                    TrackEtaGen->Fill(track.Eta());
                    TrackPhiGen->Fill(track.Phi());
                    track_vectgen.push_back(track);
                    
                    parent += track;
                    
                 }//for loop end 
                 //cout<< parent.M()<< "   " << pxsmc.size()<<endl;
                 if(trk_size ==4){
                  TLorentzVector  t1 = track_vectgen[0];
                  TLorentzVector  t2 = track_vectgen[1];
                  TLorentzVector  t3 = track_vectgen[2];
                  TLorentzVector  t4 = track_vectgen[3] ;   
                
                
                     
                 }//track_count = 4
                 InvariantMassGen->Fill(parent.M());
                 PtGen->Fill(parent.Pt());
                 RapGen->Fill(parent.Rapidity()); 
                
            } // if size of track vector  = trk_size    
            
              
 
                
                
		   
            
            
         
        
    }// end entry loop generated    
        
        
        
        
        
        
        
    }// end is mc event
    
// saving them into file now   
TFile * out_file = TFile::Open(output_hist_file,"RECREATE");

 
out_file->cd();

//track level info we are intereseted in resolution that is  resolution = rec-mctruth    
    
//basic treees mas pt and rapidity of 
InvariantMassGen->Write();
InvariantMassRec->Write();
PtRec->Write();
RapRec->Write(); 
PtGen->Write();
RapGen->Write(); 

    
hFT0AAMP->Write();
hFT0ACMP->Write();
hFDDAAMP->Write();
hFDDCAMP->Write();
    
hZNAAMP->Write();
hZNCAMP->Write();
hZNAtime->Write();
hZNCtime->Write();

hxVtx->Write();
hyVtx->Write();
hzVtx->Write();    

RapRec->Write();
RapRecTruth->Write();
RapGen->Write();

TrackPtRec->Write();
TrackPtTruth->Write();
TrackPtGen->Write();

TrackEtaRec->Write();
TrackEtaTruth->Write();
TrackEtaGen->Write();

TrackPhiRec->Write();
TrackPhiTruth->Write();
TrackPhiGen->Write();

trackpt_res->Write();
tracketa_res->Write();
trackphi_res->Write();



    
    
    
    
    
    
    
out_file->Close();    
}//read aod
