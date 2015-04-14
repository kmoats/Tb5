{
	gROOT->Reset();

	#include <stdio>
	#include <cmath>
	#include <eff_chargemisid>
	#include <findjet>
    #include <fakelepton>
	#include <Rjetjet>
	#include <Rjetlepton>
	#include <Rleptonlepton>
	#include <misidlepton>
	#include <findlepton>
	#include <findneutrino>
	#include <calculations>
	#include <impose_cuts>
	#include <signedweight>

	new TBrowser();
	Long64_t maxsize = 1E12;
	TTree::SetMaxTreeSize(maxsize); 

	Int_t n_part = 20;
	Int_t n_jets = 7;
	Int_t n_leptons = 2;
	Int_t n_neutrinos = 6;
	//Int_t n_tops = 2;

	struct event_t // the structure to hold the variables for the event data
	{
		Int_t n_particles;
//		Int_t idprup;
		Float_t weight;
		Float_t signed_weight;
//		Float_t scale;
//		Float_t aqed;
//		Float_t aqcd;
//		Float_t d_pt_ll;
//		Float_t d_y_ll;
//		Float_t cos_phi_ll;
		Float_t pt_miss;
		Float_t pt_ll;
		Float_t m_ll;
//		Float_t m_t;
		Float_t H_t;
		Int_t veto;
	};
	event_t event;
	
	struct R_t
	{
		Float_t R;
	};
	R_t Rjj[n_jets+1][n_jets+1];
	R_t Rjl[n_jets+1][n_leptons+1];
	R_t Rll[n_leptons+1][n_leptons+1];

	 	struct particle_t // the structure to hold the variables for each particle in the event
	{	
		Int_t id;
//		Int_t ist;
//		Int_t moth1;
//		Int_t moth2;
//		Int_t icol1;
//		Int_t icol2;
		Float_t p1;
		Float_t p2;
		Float_t p3;
		Float_t p4;
//		Float_t p5;
//		Float_t vtim;
//		Float_t spin;
		Float_t pt;
		Float_t E;
		Float_t y;
	};
	particle_t particle[n_part+1];
	particle_t jet[n_jets+1];
//	particle_t jet_hard;
	particle_t jet_forward;
	particle_t lepton[n_leptons+1];
	particle_t lepton_highpt;
	particle_t lepton_lowpt;
	particle_t lepton_misid;
	particle_t neutrino[n_neutrinos+1];
	//particle_t top[n_tops+1];
	
	
	Char_t line[151];
	
//******************** DATA & EVENT FILES ********************
	
	
	Int_t tree;
	Int_t file;
	Int_t group;
	Int_t n_trees = 14;  // Set # of trees (= # of histograms to superimpose on each plot)
//	Int_t n_files[] = {/*2,*/36,100,100,100,100,100,200,200,20,20,20,20,20,20};
//	Int_t n_groups[] = {/*1,*/18,50,50,50,50,50,100,100,10,10,10,10,10,10};
	Int_t n_files[] = {/*32,*/1,1,1,1,1,1,1,1,1,1,1,1,1,1};
//	Int_t n_files[] = {/*32,*/2,2,2,2,2,2,2,2,2,2,2,2,2,2};
	Int_t n_groups[] = {/*16,*/1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	Int_t n_back = 8;

	Int_t tree_ttbar = 8;	//Identify the tree corresponding to the ttbar + jets background (for charge misidentifcation purposes)
	Int_t tree_za = 999;	//Identify the tree corresponding to the z/gamma + jets background (for charge misidentifcation purposes)
	Int_t tree_wpwm = 2;	//Identify the tree corresponding to the w+w- + jets background (for charge misidentifcation purposes)
	Int_t tree_wz = 4;	//Identify the tree corresponding to the wz + jets background (for charge misidentifcation purposes)
	Int_t tree_wjets = 7;	//Identify the tree corresponding to the w + jets background (for jet faking lepton)
	
	
	Char_t *label[] =	{	//"Z/#gamma + j", 
							"t#bar{t}W^{+}W^{-} + jets", 
							"W^{+}W^{-} + jets", 
							"W^{#frac{+}{}}W^{#frac{+}{}}W^{#frac{}{+}} + jets", 
							"W^{#pm}Z + jets", 
							"W^{#pm}W^{#pm} + jets",
							"t#bar{t}W^{#pm} + jets", 
							"W^{#pm} + jets", 
							"t#bar{t} + jets", 
							"Signal, Iso",
							"Signal, Non-Iso",
							"Signal, Iso",
							"Signal, Non-Iso",
							"Signal, Iso",
							"Signal, Non-Iso"
//							"T_{a}^{d} Signal"
						};

	Char_t *process[][2] =	{	//{"pp_zajets_++","pp_zajets_--"},
									{"pp_tt~w+w-jets_++","pp_tt~w+w-jets_--"},
									{"pp_w+w-jets_++","pp_w+w-jets_--"},
									{"pp_w+w+w-jets_++","pp_w-w-w+jets_--"},
									{"pp_w+zjets_++","pp_w-zjets_--"},
									{"pp_w+w+jets_++","pp_w-w-jets_--"},
									{"pp_tt~w+jets_++","pp_tt~w-jets_--"},
									{"pp_w+jets_++","pp_w-jets_--"},
									{"pp_tt~jets_++","pp_tt~jets_--"},
									{"pp_Tb5t~j_++","pp_Tb5~tj_--"},
									{"pp_Tb5t~j_++","pp_Tb5~tj_--"},
									{"pp_Tb5t~j_++","pp_Tb5~tj_--"},
									{"pp_Tb5t~j_++","pp_Tb5~tj_--"},
									{"pp_Tb5t~j_++","pp_Tb5~tj_--"},
									{"pp_Tb5t~j_++","pp_Tb5~tj_--"}
									//{"pp_Tad~tj_++","pp_Tadt~j_--"}
								};

	Char_t *model;
	Char_t *path;

	string datfile_s[15000];
	string lhefile_s[15000];
	
	Char_t datfile[15000][151];		// Must convert file names from string to Char_t (function fgets accepts Char_t, not string)
	Char_t lhefile[15000][151];
	
	for(tree=1; tree<=n_trees; tree++)
	{
		if(tree == 9)
		{
			model = "M400i";
		}
		else if(tree == 10)
		{
			model = "M400ni";
		}
		else if(tree == 11)
		{
			model = "M600i";
		}
		else if(tree == 12)
		{
			model = "M600ni";
		}
		else if(tree == 13)
		{
			model = "M800i";
		}
		else if(tree == 14)
		{
			model = "M800ni";
		}
		else
		{
			model = "sm";
		}
		
		//if (tree==10)
		//{
		//	path = "";
		//}
		
		//else 
		//{
			path = "/Volumes/MadGraph/";
		//}
		
		for(group=1; group<=n_groups[tree-1]; group++)
		{
			datfile_s[(tree-1)*1000 + 2*group-2] = Form("%sSubProcesses/%s_%s/%s_%s_14TeV_100000_%i_results.dat",path,process[tree-1][0],model,process[tree-1][0],model,group);
			datfile_s[(tree-1)*1000 + 2*group-1] = Form("%sSubProcesses/%s_%s/%s_%s_14TeV_100000_%i_results.dat",path,process[tree-1][1],model,process[tree-1][1],model,group);

			lhefile_s[(tree-1)*1000 + 2*group-2] = Form("%sEvents/%s_%s/%s_%s_14TeV_100000_%i_events.lhe",path,process[tree-1][0],model,process[tree-1][0],model,group);
			lhefile_s[(tree-1)*1000 + 2*group-1] = Form("%sEvents/%s_%s/%s_%s_14TeV_100000_%i_events.lhe",path,process[tree-1][1],model,process[tree-1][1],model,group);
		/*
		// Show which input files are being used
			cout << datfile_s[(tree-1)*1000 + 2*group-2] << endl;
			cout << datfile_s[(tree-1)*1000 + 2*group-1] << endl;
			cout << lhefile_s[(tree-1)*1000 + 2*group-2] << endl;
			cout << lhefile_s[(tree-1)*1000 + 2*group-1] << endl;
		*/
		}
	}
	
	// Must convert file names from string to Char_t (function fgets accepts Char_t, not string)	
	for(tree=1; tree<=n_trees; tree++)
	{
		for(file=1; file<=n_files[tree-1]; file++)
		{
			for(Int_t i=0; i<=150; i++)
			{
				datfile[(tree-1)*1000 + file-1][i] = datfile_s[(tree-1)*1000 + file-1][i];	
				lhefile[(tree-1)*1000 + file-1][i] = lhefile_s[(tree-1)*1000 + file-1][i];
			}
		}
	}

//******************** VARIABLES FOR CROSS SECTIONS, ETC... ********************
	
	Float_t dat_x_sect[n_trees+1];
	Float_t lhe_x_sect[n_trees+1];
	
	Float_t x_sect[n_trees+1];
	Float_t x_sect_a[n_trees+1];
	Float_t x_sect_l[n_trees+1];
	Float_t x_sect_lj[n_trees+1];
	Float_t x_sect_ljh[n_trees+1];

	Float_t x_sect_l_600[n_trees+1];
	Float_t x_sect_lj_600[n_trees+1];
	Float_t x_sect_ljh_600[n_trees+1];
	
	Float_t x_sect_l_800[n_trees+1];
	Float_t x_sect_lj_800[n_trees+1];
	Float_t x_sect_ljh_800[n_trees+1];
	
	
	
	Float_t asym[n_trees+1];
	Float_t asym_a[n_trees+1];
	Float_t asym_l[n_trees+1];
	Float_t asym_lj[n_trees+1];
	Float_t asym_ljh[n_trees+1];

	Float_t asym_l_600[n_trees+1];
	Float_t asym_lj_600[n_trees+1];
	Float_t asym_ljh_600[n_trees+1];
	
	Float_t asym_l_800[n_trees+1];
	Float_t asym_lj_800[n_trees+1];
	Float_t asym_ljh_800[n_trees+1];
	
	Float_t eff_a[n_trees+1];
	Float_t eff_lept[n_trees+1];
	Float_t eff_jet[n_trees+1];
	Float_t eff_Ht[n_trees+1];

	Float_t	sigma_S1 = 0; 
	Float_t	sigma_B = 0; 
	Float_t	asym_S1 = 0; 
	Float_t	asym_B = 0; 
	Float_t	sigma_S2 = 0; 
	Float_t	asym_S2 = 0; 
	
	Float_t	sigma_S1_600 = 0; 
	Float_t	sigma_B_600 = 0; 
	Float_t	asym_S1_600 = 0; 
	Float_t	asym_B_600 = 0; 
	Float_t	sigma_S2_600 = 0; 
	Float_t	asym_S2_600 = 0; 
	
	Float_t	sigma_S1_800 = 0; 
	Float_t	sigma_B_800 = 0; 
	Float_t	asym_S1_800 = 0; 
	Float_t	asym_B_800 = 0; 
	Float_t	sigma_S2_800 = 0; 
	Float_t	asym_S2_800 = 0; 
	
	
	
	
	
	Float_t lum_5sigma1;
	Float_t lum_5sigma2;

	Float_t lum_5sigma1_600;
	Float_t lum_5sigma2_600;
	
	Float_t lum_5sigma1_800;
	Float_t lum_5sigma2_800;

	Float_t lum_5sigma12;
	Float_t lum_5sigma22;
	
	Float_t lum_5sigma12_600;
	Float_t lum_5sigma22_600;
	
	Float_t lum_5sigma12_800;
	Float_t lum_5sigma22_800;
	
	
	
	Float_t lum_10events1;
	Float_t lum_10events2;
	
	Float_t lum_disc1;
	Float_t lum_disc2;
	
	Float_t N_S1;
	Float_t N_S2;
	
	
	Float_t lum_5sigma1_600;
	Float_t lum_5sigma2_600;
	Float_t lum_10events1_600;
	Float_t lum_10events2_600;
	
	Float_t lum_disc1_600;
	Float_t lum_disc2_600;
	
	Float_t N_S1_600;
	Float_t N_S2_600;
	
	
	Float_t lum_5sigma1_800;
	Float_t lum_5sigma2_800;
	Float_t lum_10events1_800;
	Float_t lum_10events2_800;
	
	Float_t lum_disc1_800;
	Float_t lum_disc2_800;
	
	Float_t N_S1_800;
	Float_t N_S2_800;
	
	Float_t luminosity = 1;
	Float_t eff_chargemisid;// = 0.006;
	Float_t eff_jetmisid = 0.0001;

//******************** CUTS ON PLOTTED EVENTS ********************

//********** Acceptance Cuts **********
	Float_t lepton_y_max = 2.5;
	Float_t lepton_pt_min = 10;
	/*
	TCut acc_lepton_cut_1 = Form("lepton1.pt > %f", lepton_pt_min);
	TCut acc_lepton_cut_2 = Form("lepton2.pt > %f", lepton_pt_min);
	TCut acc_lepton_cut_3 = Form("fabs(lepton1.y) < %f", lepton_y_max);
	TCut acc_lepton_cut_4 = Form("fabs(lepton2.y) < %f", lepton_y_max);
	
	TCut acc_lepton_cuts = acc_lepton_cut_1 && acc_lepton_cut_2 && acc_lepton_cut_3 && acc_lepton_cut_4;
	*/
	Float_t jet_y_max = 5;
	Float_t jet_pt_min = 10;
	/*
	TCut acc_jet_pt_cut_1 = Form("jet1.pt > %f", jet_pt_min);
	TCut acc_jet_pt_cut_2 = Form("jet2.pt > %f", jet_pt_min);
	TCut acc_jet_pt_cut_3 = Form("jet3.pt > %f", jet_pt_min);
	TCut acc_jet_pt_cut_4 = Form("jet4.pt > %f", jet_pt_min);
	TCut acc_jet_pt_cut_5 = Form("jet5.pt > %f", jet_pt_min);
	TCut acc_jet_pt_cut_6 = Form("jet6.pt > %f", jet_pt_min);
	TCut acc_jet_pt_cut_7 = Form("jet7.pt > %f", jet_pt_min);
	
	TCut acc_jet_y_cut_1 = Form("fabs(jet1.y) < %f", jet_y_max);
	TCut acc_jet_y_cut_2 = Form("fabs(jet2.y) < %f", jet_y_max);
	TCut acc_jet_y_cut_3 = Form("fabs(jet3.y) < %f", jet_y_max);
	TCut acc_jet_y_cut_4 = Form("fabs(jet4.y) < %f", jet_y_max);
	TCut acc_jet_y_cut_5 = Form("fabs(jet5.y) < %f", jet_y_max);
	TCut acc_jet_y_cut_6 = Form("fabs(jet6.y) < %f", jet_y_max);
	TCut acc_jet_y_cut_7 = Form("fabs(jet7.y) < %f", jet_y_max);
	
	TCut acc_jet_pt_cuts = acc_jet_pt_cut_1 && acc_jet_pt_cut_2 && acc_jet_pt_cut_3 && acc_jet_pt_cut_4 && acc_jet_pt_cut_5 && acc_jet_pt_cut_6 && acc_jet_pt_cut_7;
	TCut acc_jet_y_cuts = acc_jet_y_cut_1 && acc_jet_y_cut_2 && acc_jet_y_cut_3 && acc_jet_y_cut_4 && acc_jet_y_cut_5 && acc_jet_y_cut_6 && acc_jet_y_cut_7;
	
	TCut acc_jet_cuts = acc_jet_pt_cuts && acc_jet_y_cuts;
	
	//Need to fix this to account for an arbitrary number of jets
	TCut acc_dRjj_cuts = "Rjj12.R > 0.4 && Rjj13.R > 0.4 && Rjj14.R > 0.4 && Rjj15.R > 0.4 && Rjj16.R > 0.4 && Rjj17.R > 0.4 && Rjj23.R > 0.4 && Rjj24.R > 0.4 && Rjj25.R > 0.4 && Rjj26.R > 0.4 && Rjj27.R > 0.4 && Rjj34.R > 0.4 && Rjj35.R > 0.4 && Rjj36.R > 0.4 && Rjj37.R > 0.4 && Rjj45.R > 0.4 && Rjj46.R > 0.4 && Rjj47.R > 0.4 && Rjj56.R > 0.4 && Rjj57.R > 0.4 && Rjj67.R > 0.4";
	TCut acc_dRjl_cuts = "Rjl11.R > 0.4 && Rjl12.R > 0.4 && Rjl21.R > 0.4 && Rjl22.R > 0.4 && Rjl31.R > 0.4 && Rjl32.R > 0.4 && Rjl41.R > 0.4 && Rjl42.R > 0.4 && Rjl51.R > 0.4 && Rjl52.R > 0.4 && Rjl61.R > 0.4 && Rjl62.R > 0.4 && Rjl71.R > 0.4 && Rjl72.R > 0.4";
	TCut acc_dRll_cuts = "Rll12.R > 0.4";
	TCut acc_dR_cuts = acc_dRjj_cuts && acc_dRjl_cuts && acc_dRll_cuts;
	*/
	TCut acceptance_cuts = Form("event.veto == %i", 0);

	//TCut acceptance_cuts = acc_lepton_cuts /*+ acc_jet_cuts */+ acc_dR_cuts;
	
//********** Leptonic Cuts **********	
	
	Float_t lepton_highpt_min = 50;
	Float_t lepton_lowpt_min = 0;
	Float_t pt_miss_min = 80;
	Float_t m_ll_min = 120;

	TCut lepton_cut_1 = Form("lepton_highpt.pt > %f", lepton_highpt_min);
	TCut lepton_cut_2 = Form("lepton_lowpt.pt > %f", lepton_lowpt_min);
	TCut lepton_cut_3 = Form("event.pt_miss > %f", pt_miss_min);
	TCut lepton_cut_4 = Form("event.m_ll > %f", m_ll_min);

	TCut lepton_cuts = lepton_cut_1 && lepton_cut_2 && lepton_cut_3 && lepton_cut_4;

	Float_t lepton_highpt_min_600 = 80;
	Float_t lepton_lowpt_min_600 = 0;
	Float_t pt_miss_min_600 = 80;
	Float_t m_ll_min_600 = 160;
	
	TCut lepton_cut_1_600 = Form("lepton_highpt.pt > %f", lepton_highpt_min_600);
	TCut lepton_cut_2_600 = Form("lepton_lowpt.pt > %f", lepton_lowpt_min_600);
	TCut lepton_cut_3_600 = Form("event.pt_miss > %f", pt_miss_min_600);
	TCut lepton_cut_4_600 = Form("event.m_ll > %f", m_ll_min_600);
	
	TCut lepton_cuts_600 = lepton_cut_1_600 && lepton_cut_2_600 && lepton_cut_3_600 && lepton_cut_4_600;
	
	Float_t lepton_highpt_min_800 = 150;
	Float_t lepton_lowpt_min_800 = 0;
	Float_t pt_miss_min_800 = 80;
	Float_t m_ll_min_800 = 240;
	
	TCut lepton_cut_1_800 = Form("lepton_highpt.pt > %f", lepton_highpt_min_800);
	TCut lepton_cut_2_800 = Form("lepton_lowpt.pt > %f", lepton_lowpt_min_800);
	TCut lepton_cut_3_800 = Form("event.pt_miss > %f", pt_miss_min_800);
	TCut lepton_cut_4_800 = Form("event.m_ll > %f", m_ll_min_800);
	
	TCut lepton_cuts_800 = lepton_cut_1_800 && lepton_cut_2_800 && lepton_cut_3_800 && lepton_cut_4_800;
	
//********** Jet Cuts **********

	Float_t jet_forward_y_min = 3;
	Float_t jet_forward_y_max = 5;
	Float_t jet_forward_pt_min = 30;
	Float_t jet_forward_E_min = 500;
	
	TCut jet_forward_cut_1 = Form("fabs(jet_forward.y) > %f", jet_forward_y_min);
	TCut jet_forward_cut_2 = Form("fabs(jet_forward.y) < %f", jet_forward_y_max);
	TCut jet_forward_cut_3 = Form("jet_forward.pt > %f", jet_forward_pt_min);
	TCut jet_forward_cut_4 = Form("jet_forward.E > %f", jet_forward_E_min);

	TCut jet_forward_cuts = jet_forward_cut_1 && jet_forward_cut_2 && jet_forward_cut_3 && jet_forward_cut_4;

	TCut jet_cuts = jet_forward_cuts;
	
//********** Ht Cut **********
	
	Float_t Ht_min = 450;

	TCut Ht_cut = Form("event.H_t > %f", Ht_min);

	Float_t Ht_min_600 = 550;
	
	TCut Ht_cut_600 = Form("event.H_t > %f", Ht_min_600);
	
	Float_t Ht_min_800 = 800;
	
	TCut Ht_cut_800 = Form("event.H_t > %f", Ht_min_800);
	
	
	TCut cuts = acceptance_cuts && lepton_cuts && jet_cuts && Ht_cut;
	TCut cuts_600 = acceptance_cuts && lepton_cuts_600 && jet_cuts && Ht_cut_600;
	TCut cuts_800 = acceptance_cuts && lepton_cuts_800 && jet_cuts && Ht_cut_800;
							

//******************** CREATE TREES ********************

	// create a new ROOT file
	TFile *f = new TFile("Tb5_file.root","RECREATE");

	// create a TTree
	TTree *Tree[n_trees+1]; 

	for(tree=1; tree<=n_trees; tree++)
	{
		Tree[tree] = new TTree(Form("Tree[%d]",tree),"Data from event file"); 

		// create branches for the event data
		TBranch *branch = Tree[tree]->Branch("event",&event.n_particles,"n_particles/I:weight/F:signed_weight/F:pt_miss/F:pt_ll/F:m_ll/F:H_t/F:veto/I");

		// create branches for the jet-jet separation
		for(Int_t j=1; j<=n_jets; j++)
		{	
			for(Int_t k=j+1; k<=n_jets; k++)
			{	
				TBranch *branch = Tree[tree]->Branch(Form("Rjj%d%d",j,k),&Rjj[j][k].R,"R/F");
			}
		}
	
		// create branches for the jet-lepton separation
		for(Int_t j=1; j<=n_jets; j++)
		{	
			for(Int_t k=1; k<=n_leptons; k++)
			{	
				TBranch *branch = Tree[tree]->Branch(Form("Rjl%d%d",j,k),&Rjl[j][k].R,"R/F");
			}
		}
	
		// create branches for the lepton-lepton separation
		for(Int_t j=1; j<=n_leptons; j++)
		{	
			for(Int_t k=j+1; k<=n_leptons; k++)
			{	
				TBranch *branch = Tree[tree]->Branch(Form("Rll%d%d",j,k),&Rll[j][k].R,"R/F");
			}
		}
			// create branches for each particle in the event
		for(Int_t k=1; k<=n_part; k++)
		{	
			TBranch *branch = Tree[tree]->Branch(Form("particle%d",k),&particle[k].id,"id/I:p1/F:p2/F:p3/F:p4/F:pt/F:E/F:y/F");
		}
	
		for(Int_t k=1; k<=n_jets; k++)
		{
			TBranch *branch = Tree[tree]->Branch(Form("jet%d",k),&jet[k].id,"id/I:p1/F:p2/F:p3/F:p4/F:pt/F:E/F:y/F");
		}
			
	//	TBranch *branch = Tree[tree]->Branch("jet_hard",&jet_hard.id,"id/I:p1/F:p2/F:p3/F:p4/F:pt/F:E/F:y/F");
		TBranch *branch = Tree[tree]->Branch("jet_forward",&jet_forward.id,"id/I:p1/F:p2/F:p3/F:p4/F:pt/F:E/F:y/F");

		for(Int_t k=1; k<=n_leptons; k++)
		{
			TBranch *branch = Tree[tree]->Branch(Form("lepton%d",k),&lepton[k].id,"id/I:p1/F:p2/F:p3/F:p4/F:pt/F:E/F:y/F");
		}
		
		TBranch *branch = Tree[tree]->Branch("lepton_highpt",&lepton_highpt.id,"id/I:p1/F:p2/F:p3/F:p4/F:pt/F:E/F:y/F");
		TBranch *branch = Tree[tree]->Branch("lepton_lowpt",&lepton_lowpt.id,"id/I:p1/F:p2/F:p3/F:p4/F:pt/F:E/F:y/F");
		TBranch *branch = Tree[tree]->Branch("lepton_misid",&lepton_misid.id,"id/I:p1/F:p2/F:p3/F:p4/F:pt/F:E/F:y/F");

		for(Int_t k=1; k<=n_neutrinos; k++)
		{
			TBranch *branch = Tree[tree]->Branch(Form("neutrino%d",k),&neutrino[k].id,"id/I:p1/F:p2/F:p3/F:p4/F:pt/F:E/F:y/F");
		}
/*
		for(Int_t k=1; k<=n_tops; k++)
		{
			TBranch *branch = Tree[tree]->Branch(Form("top%d",k),&top[k].id,"id/I:p1/F:p2/F:p3/F:p4/F:pt/F:E/F:y/F");
		}
*/
	}
	
//******************** KINEMATIC VARIABLES ********************

	Float_t dy;
	Float_t denom;
	Float_t dphi;
	
	Int_t j;
	Int_t l;
	Int_t n;
	Int_t t;
	
	Float_t	sum_p1_leptons;
	Float_t	sum_p2_leptons;
	Float_t	sum_p3_leptons;				
	Float_t	sum_p4_leptons;
	
	Float_t	sum_p1_neutrinos;
	Float_t	sum_p2_neutrinos;
	Float_t	sum_p3_neutrinos;				
	Float_t	sum_p4_neutrinos;
	
	Float_t	sum_p1;
	Float_t	sum_p2;
	Float_t	sum_p3;
	Float_t	sum_p4;
	
	Float_t	pt_tot;
	
	
//******************** FILL TREES WITH EVENT FILE DATA ********************

	for(tree=1; tree<=n_trees; tree++)
	{
		x_sect[tree] = 0;
		x_sect_l[tree] = 0;
		x_sect_lj[tree] = 0;
		x_sect_ljh[tree] = 0;
		
		asym[tree] = 0;
		asym_a[tree] = 0;
		asym_l[tree] = 0;
		asym_lj[tree] = 0;
		asym_ljh[tree] = 0;
		
		
		 x_sect_l_600[tree] = 0;
		 x_sect_lj_600[tree] = 0;
		 x_sect_ljh_600[tree] = 0;
		
		 x_sect_l_800[tree] = 0;
		 x_sect_lj_800[tree] = 0;
		 x_sect_ljh_800[tree] = 0;
		
		
		
		
		 asym_l_600[tree] = 0;
		 asym_lj_600[tree] = 0;
		 asym_ljh_600[tree] = 0;
		
		 asym_l_800[tree] = 0;
		 asym_lj_800[tree] = 0;
		 asym_ljh_800[tree] = 0;
		
		
		
		
		
		

		for(file=1; file<=n_files[tree-1]; file++)
		{
			//open the results.dat file and read in the cross section
			FILE *infile = fopen(datfile[(tree-1)*1000 + file-1],"r");
			fgets(&line,150,infile);
			sscanf(&line[0],"%f",&dat_x_sect[tree]);
			dat_x_sect[tree] = 1000*dat_x_sect[tree];
			fclose(infile);
		
			//open the events.lhe file 
			FILE *infile = fopen(lhefile[(tree-1)*1000 + file-1],"r");

			// fill the trees with the data in the event file
			while (!feof(infile))
			{
				fgets(&line,150,infile);
	
				if(line[0] == '#' && line[1] == ' ' && line[2] == ' ' && line[3] == 'I' && line[4] == 'n' && line[5] == 't' && line[6] == 'e' && line[7] == 'g' && line[8] == 'r' && line[9] == 'a' && line[10] == 't' && line[11] == 'e' && line[12] == 'd' && line[13] == ' ' && line[14] == 'w' && line[15] == 'e' && line[16] == 'i' && line[17] == 'g' && line[18] == 'h' && line[19] == 't' && line[20] == ' ' && line[21] == '(' && line[22] == 'p' && line[23] == 'b' && line[24] == ')' && line[25] == ' ' && line[26] == ' ' && line[27] == ':')
				{
					sscanf(&line[30],"%f",&lhe_x_sect[tree]);
					lhe_x_sect[tree] = 1000*lhe_x_sect[tree];
				}
			
				if(line[0] == '<' && line[1] == 'e' && line[2] == 'v' && line[3] == 'e' && line[4] == 'n' && line[5] == 't' && line[6] == '>' )
				{	
					fgets(&line,150,infile);
					sscanf(&line[0],"%d",&event.n_particles);
					sscanf(&line[8],"%f",&event.weight);

					j = 0;
					l = 0;
					n = 0;
			//		t = 0;
					
			//		jet_hard.pt = 0;	
					jet_forward.y = 0;	
					lepton_highpt.pt = 0;	
					lepton_lowpt.pt = 100000;	
					event.veto = 0;
			
				
					for(Int_t i=1; i<=event.n_particles; i++)
					{
						fgets(&line,150,infile);
						sscanf(&line[0],"%d",&particle[i].id);		
						sscanf(&line[35],"%f%f%f%f",&particle[i].p1,&particle[i].p2,&particle[i].p3,&particle[i].p4);
			
						// calculate pt, E and y for each particle in the event
						particle[i].pt = sqrt(pow(particle[i].p1,2) + pow(particle[i].p2,2));
						particle[i].E = particle[i].p4;
						//if(particle[i].p4-particle[i].p3 != 0)
						{
							particle[i].y = 0.5*log((particle[i].p4+particle[i].p3)/(particle[i].p4-particle[i].p3));
						}

						// find jets in the event
						if((abs(particle[i].id) == 1 || abs(particle[i].id) == 2 || abs(particle[i].id) == 3 || abs(particle[i].id) == 4 || abs(particle[i].id) == 5 || abs(particle[i].id) == 21 ) && i > 2 )
						{
							if(!(tree == tree_wjets && l==1))
							{
								findjet();
							}
														
							else // jet is misidentified as the second lepton (only in w+jets background)
							{
								fakelepton();
							}
						}
			
						// find leptons in the event
						else if(abs(particle[i].id) == 11 || abs(particle[i].id) == 13) 
						{
							if(((file % 2 == 1 && particle[i].id > 0)||(file % 2 == 0 && particle[i].id < 0))) // Lepton charge is misidentified (in tt~, w+w-, z/gamma and wz background)
							{
								misidlepton();	
							}
							
							else //if((file % 2 == 1 && particle[i].id < 0)||(file % 2 == 0 && particle[i].id > 0))
							{
								findlepton();							
							}
						}	
	/*				
						// find tops in the event
						if(abs(particle[i].id) == 6)
						{
							t++;
							top[t] = particle[i];
						}
	 */				

						// find neutrinos in the event
						else if(abs(particle[i].id) == 12 || abs(particle[i].id) == 14)
						{
							findneutrino();
						}
					}	
				
					if(l < 2 || j == 0)// || n == 0)
					{
						event.veto = 1;  // Require like-sign dileptons, jets and missing energy
						continue;
				//		break;
					}
					
					// calculate the jet-jet separation
					Rjetjet();
					// calculate the jet-lepton separation
					Rjetlepton();
					// calculate the lepton-lepton separation
					Rleptonlepton();
					
					// Calculate d_pt_ll, d_y_ll, m_t, etc...				
					calculations();
					
					//scale event weights to get the correct cross section (the value from results.dat)
					event.weight = event.weight*dat_x_sect[tree]/lhe_x_sect[tree]; 
					
					signedweight();
				
					//calculate cross section (in fb) by adding up the weights from each event
					x_sect[tree] = x_sect[tree] + (double)1000*event.weight/n_groups[tree-1];

					asym[tree] = asym[tree] + (double)1000*event.signed_weight/n_groups[tree-1];
					
					//acceptance cuts
					impose_cuts();
					
					Tree[tree]->Fill();
					
				}
			}
		
			fclose(infile);
		}
/*
		if(x_sect[tree] != 0)
		{
			eff_a[tree] = 100*x_sect_a[tree]/x_sect[tree];
		}
		else
		{
			eff_a[tree] = 0;
		}	
		if(x_sect_a[tree] != 0)
		{
			eff_lept[tree] = 100*x_sect_l[tree]/x_sect_a[tree];
		}
		else	
		{
			eff_lept[tree] = 0;
		}	
		if(x_sect_l[tree] != 0)
		{			
			eff_jet[tree] = 100*x_sect_lj[tree]/x_sect_l[tree];
		}
		else
		{
			eff_jet[tree] = 0;
		}	
		if(x_sect_lj[tree] != 0)
		{			
			eff_Ht[tree] = 100*x_sect_ljh[tree]/x_sect_lj[tree];
		}
		else	
		{
			eff_Ht[tree] = 0;
		}	
 */     cout << endl;
		cout << "400";
		cout << endl << Form("%s - Cross Section (fb): ", label[tree-1]) << endl;
	//	cout << "No Cuts:          " << 0.5*(x_sect[tree]+asym[tree]) << "   " << 0.5*(x_sect[tree]-asym[tree]) << "   " << x_sect[tree] << "   " << asym[tree] << endl;
	//	cout << "Selection Cuts:   " << 0.5*(x_sect_a[tree]+asym_a[tree]) << "   " << 0.5*(x_sect_a[tree]-asym_a[tree]) << "   " << x_sect_a[tree] << "   " << asym_a[tree] /*<< " (" << eff_a[tree] << " %)"*/ << endl;
		cout << "Leptonic Cuts:    " << 0.5*(x_sect_l[tree]+asym_l[tree]) << "   " << 0.5*(x_sect_l[tree]-asym_l[tree]) << "   " << x_sect_l[tree] << "   " << asym_l[tree] /*<< " (" << eff_lept[tree] << " %)"*/ << endl;
		cout << "Jet Cuts:         " << 0.5*(x_sect_lj[tree]+asym_lj[tree]) << "   " << 0.5*(x_sect_lj[tree]-asym_lj[tree]) << "   " << x_sect_lj[tree] << "   " << asym_lj[tree] /*<< " (" << eff_jet[tree] << " %)"*/ <<  endl;
		cout << "Ht Cut:           " << 0.5*(x_sect_ljh[tree]+asym_ljh[tree]) << "   " << 0.5*(x_sect_ljh[tree]-asym_ljh[tree]) << "   " << x_sect_ljh[tree] << "   " << asym_ljh[tree] /*<< " (" << eff_Ht[tree] << " %)"*/ << endl;

		cout << "600";
		cout << endl << Form("%s - Cross Section (fb): ", label[tree-1]) << endl;
	//	cout << "No Cuts:          " << 0.5*(x_sect[tree]+asym[tree]) << "   " << 0.5*(x_sect[tree]-asym[tree]) << "   " << x_sect[tree] << "   " << asym[tree] << endl;
	//	cout << "Selection Cuts:   " << 0.5*(x_sect_a[tree]+asym_a[tree]) << "   " << 0.5*(x_sect_a[tree]-asym_a[tree]) << "   " << x_sect_a[tree] << "   " << asym_a[tree] /*<< " (" << eff_a[tree] << " %)"*/ << endl;
		cout << "Leptonic Cuts:    " << 0.5*(x_sect_l_600[tree]+asym_l_600[tree]) << "   " << 0.5*(x_sect_l_600[tree]-asym_l_600[tree]) << "   " << x_sect_l_600[tree] << "   " << asym_l_600[tree] /*<< " (" << eff_lept[tree] << " %)"*/ << endl;
		cout << "Jet Cuts:         " << 0.5*(x_sect_lj_600[tree]+asym_lj_600[tree]) << "   " << 0.5*(x_sect_lj_600[tree]-asym_lj_600[tree]) << "   " << x_sect_lj_600[tree] << "   " << asym_lj_600[tree] /*<< " (" << eff_jet[tree] << " %)"*/ <<  endl;
		cout << "Ht Cut:           " << 0.5*(x_sect_ljh_600[tree]+asym_ljh_600[tree]) << "   " << 0.5*(x_sect_ljh_600[tree]-asym_ljh_600[tree]) << "   " << x_sect_ljh_600[tree] << "   " << asym_ljh_600[tree] /*<< " (" << eff_Ht[tree] << " %)"*/ << endl;
		
		cout << "800";
		cout << endl << Form("%s - Cross Section (fb): ", label[tree-1]) << endl;
	//	cout << "No Cuts:          " << 0.5*(x_sect[tree]+asym[tree]) << "   " << 0.5*(x_sect[tree]-asym[tree]) << "   " << x_sect[tree] << "   " << asym[tree] << endl;
	//	cout << "Selection Cuts:   " << 0.5*(x_sect_a[tree]+asym_a[tree]) << "   " << 0.5*(x_sect_a[tree]-asym_a[tree]) << "   " << x_sect_a[tree] << "   " << asym_a[tree] /*<< " (" << eff_a[tree] << " %)"*/ << endl;
		cout << "Leptonic Cuts:    " << 0.5*(x_sect_l_800[tree]+asym_l_800[tree]) << "   " << 0.5*(x_sect_l_800[tree]-asym_l_800[tree]) << "   " << x_sect_l_800[tree] << "   " << asym_l_800[tree] /*<< " (" << eff_lept[tree] << " %)"*/ << endl;
		cout << "Jet Cuts:         " << 0.5*(x_sect_lj_800[tree]+asym_lj_800[tree]) << "   " << 0.5*(x_sect_lj_800[tree]-asym_lj_800[tree]) << "   " << x_sect_lj_800[tree] << "   " << asym_lj_800[tree] /*<< " (" << eff_jet[tree] << " %)"*/ <<  endl;
		cout << "Ht Cut:           " << 0.5*(x_sect_ljh_800[tree]+asym_ljh_800[tree]) << "   " << 0.5*(x_sect_ljh_800[tree]-asym_ljh_800[tree]) << "   " << x_sect_ljh_800[tree] << "   " << asym_ljh_800[tree] /*<< " (" << eff_Ht[tree] << " %)"*/ << endl;
		
		
		
	}
	
	//for(tree=n_back+1; tree <= n_trees; tree++)
//	{
		sigma_S1 = sigma_S1 + x_sect_ljh[9];
		asym_S1 = asym_S1 + asym_ljh[9];
		
		sigma_S2 = sigma_S2 + x_sect_ljh[10];
		asym_S2 = asym_S2 + asym_ljh[10];
	
		sigma_S1_600 = sigma_S1_600 + x_sect_ljh_600[11];
		asym_S1_600 = asym_S1_600 + asym_ljh_600[11];
	
		sigma_S2_600 = sigma_S2_600 + x_sect_ljh_600[12];
		asym_S2_600 = asym_S2_600 + asym_ljh_600[12];
	
		sigma_S1_800 = sigma_S1_800 + x_sect_ljh_800[13];
		asym_S1_800 = asym_S1_800 + asym_ljh_800[13];
	
		sigma_S2_800 = sigma_S2_800 + x_sect_ljh_800[14];
		asym_S2_800 = asym_S2_800 + asym_ljh_800[14];
	
	//}
	cout << endl;
	cout << "Signal - 400i  (fb): " << 0.5*(sigma_S1 + asym_S1) << "   " << 0.5*(sigma_S1 - asym_S1) << "   " << sigma_S1 << "   " << asym_S1 << endl; 
	cout << "Signal - 400ni (fb): " << 0.5*(sigma_S2 + asym_S2) << "   " << 0.5*(sigma_S2 - asym_S2) << "   " << sigma_S2 << "   " << asym_S2 << endl; 
	cout << "Signal - 600i  (fb): " << 0.5*(sigma_S1_600 + asym_S1_600) << "   " << 0.5*(sigma_S1_600 - asym_S1_600) << "   " << sigma_S1_600 << "   " << asym_S1_600 << endl; 
	cout << "Signal - 600ni (fb): " << 0.5*(sigma_S2_600 + asym_S2_600) << "   " << 0.5*(sigma_S2_600 - asym_S2_600) << "   " << sigma_S2_600 << "   " << asym_S2_600 << endl; 
	cout << "Signal - 800i  (fb): " << 0.5*(sigma_S1_800 + asym_S1_800) << "   " << 0.5*(sigma_S1_800 - asym_S1_800) << "   " << sigma_S1_800 << "   " << asym_S1_800 << endl; 
	cout << "Signal - 800ni (fb): " << 0.5*(sigma_S2_800 + asym_S2_800) << "   " << 0.5*(sigma_S2_800 - asym_S2_800) << "   " << sigma_S2_800 << "   " << asym_S2_800 << endl; 
	
	for(tree=1; tree <= n_back; tree++)
	{
		sigma_B = sigma_B + x_sect_ljh[tree];
		asym_B = asym_B + asym_ljh[tree];
		
		sigma_B_600 = sigma_B_600 + x_sect_ljh_600[tree];
		asym_B_600 = asym_B_600 + asym_ljh_600[tree];
	
		sigma_B_800 = sigma_B_800 + x_sect_ljh_800[tree];
		asym_B_800 = asym_B_800 + asym_ljh_800[tree];
	}
	cout << "Background - 400 (fb): " << 0.5*(sigma_B + asym_B) << "   " << 0.5*(sigma_B - asym_B) << "   " << sigma_B << "   " << asym_B << endl; 
	cout << "Background - 600 (fb): " << 0.5*(sigma_B_600 + asym_B_600) << "   " << 0.5*(sigma_B_600 - asym_B_600) << "   " << sigma_B_600 << "   " << asym_B_600 << endl; 
	cout << "Background - 800 (fb): " << 0.5*(sigma_B_800 + asym_B_800) << "   " << 0.5*(sigma_B_800 - asym_B_800) << "   " << sigma_B_800 << "   " << asym_B_800 << endl; 

	lum_5sigma1 = 25*sigma_B/pow(sigma_S1,2);
	lum_5sigma2 = 25*asym_B/pow(asym_S1,2);

	lum_5sigma1_600 = 25*sigma_B_600/pow(sigma_S1_600,2);
	lum_5sigma2_600 = 25*asym_B_600/pow(asym_S1_600,2);
	
	lum_5sigma1_800 = 25*sigma_B_800/pow(sigma_S1_800,2);
	lum_5sigma2_800 = 25*asym_B_800/pow(asym_S1_800,2);

	lum_5sigma12 = 25*sigma_B/pow(sigma_S2,2);
	lum_5sigma22 = 25*asym_B/pow(asym_S2,2);
	
	lum_5sigma12_600 = 25*sigma_B_600/pow(sigma_S2_600,2);
	lum_5sigma22_600 = 25*asym_B_600/pow(asym_S2_600,2);
	
	lum_5sigma12_800 = 25*sigma_B_800/pow(sigma_S2_800,2);
	lum_5sigma22_800 = 25*asym_B_800/pow(asym_S2_800,2);
	
	cout << "Luminosity for 5 sigma - 400i  (fb^{-1}): " << lum_5sigma1		 << "   " << lum_5sigma2	  << endl; 
	cout << "Luminosity for 5 sigma - 600i  (fb^{-1}): " << lum_5sigma1_600  << "   " << lum_5sigma2_600  << endl; 
	cout << "Luminosity for 5 sigma - 800i  (fb^{-1}): " << lum_5sigma1_800  << "   " << lum_5sigma2_800  << endl; 
	cout << "Luminosity for 5 sigma - 400ni (fb^{-1}): " << lum_5sigma12	 << "   " << lum_5sigma22	  << endl; 
	cout << "Luminosity for 5 sigma - 600ni (fb^{-1}): " << lum_5sigma12_600 << "   " << lum_5sigma22_600 << endl; 
	cout << "Luminosity for 5 sigma - 800ni (fb^{-1}): " << lum_5sigma12_800 << "   " << lum_5sigma22_800 << endl;
	
	/*
	lum_10events1 = 10/sigma_S;
	lum_10events2 = 10/asym_S;
	cout << "Luminosity for 10 signal events (fb^{-1}): " << lum_10events1 << "   " << lum_10events2 << endl;

	if(lum_5sigma1 > lum_10events1)
	{
		lum_disc1 = lum_5sigma1;
	}
	else
	{
		lum_disc1 = lum_10events1;
	}
	if(lum_5sigma2 > lum_10events2)
	{
		lum_disc2 = lum_5sigma2;
	}
	else
	{
		lum_disc2 = lum_10events2;
	}
	
	N_S1 = sigma_S*lum_disc1;
	N_S2 = sigma_S*lum_disc2;
	
	cout << "Luminosity for 5 sigma and 10 signal events (fb^{-1}): " << lum_disc1 << "   " << lum_disc2 << endl;
	cout << "Signal Events: " << N_S1 << "   " << N_S2 << endl; 

		*/
		
//	check what the tree looks like
//	Tree[1]->Print();
//	f->Write();
//	delete f;

//************************ DRAW HISTOGRAMS ***************************
	gStyle->SetOptStat(0);
	gStyle->SetCanvasColor(kWhite);
	gStyle->SetCanvasBorderMode(0);
	gStyle->SetFrameFillColor(kWhite);
	gStyle->SetFrameBorderMode(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	gStyle->SetPadBottomMargin(0.12);
	gStyle->SetPadLeftMargin(0.15);
	gStyle->SetTitleOffset(1.2,"X");
	gStyle->SetTitleOffset(1.5,"Y");
	gStyle->SetTitleFillColor(kWhite);
	gStyle->SetTitleBorderSize(0);
	gStyle->SetTitleX(0.12);
	gStyle->SetTitleY(0.98);

	TH1F *hist[n_trees+1];

	legend = new TLegend(0.62,0.59,0.87,0.87);
	legend->SetFillColor(kWhite);

	legend2 = new TLegend(0.62,0.59,0.87,0.87);
	legend2->SetFillColor(kWhite);
	
	legend3 = new TLegend(0.62,0.59,0.87,0.87);
	legend3->SetFillColor(kWhite);
	
	Int_t n_canvas = 3;//4;//5;//6;//7;//8;//9;
	Char_t *canvas_title[] = {/*"jets",*//*"tagged jets", "leptons"*//*,"lepton y"*//*,"misid lepton"*//*,"tops"*//*,"d_pt_d_y"*//*,"m_ll",*/"Tb5_HT_400","Tb5_HT_600","Tb5_HT_800"/*,"jjj"*/};
	Int_t n_pad_x[] = {/*3,*//*1,2,*//*2,*//*2*//*,2*//*,2*//*,1*/,2,2,2/*,2*/};
	Int_t n_pad_y[] = {/*5,*//*3,2,*//*1,*//*1*//*,2*//*,2*//*,1*/,1,1,1/*,1*/};
	Int_t n_pads[n_canvas];
	Int_t pad;
	Int_t canvas_width[n_canvas];
	Int_t canvas_height[n_canvas];
	Int_t canvas;

	for(canvas = 0; canvas < n_canvas; canvas++)
	{
		n_pads[canvas]= n_pad_x[canvas]*n_pad_y[canvas];
		canvas_width[canvas]  = 400*n_pad_x[canvas];
		canvas_height[canvas] = 300*n_pad_y[canvas];
	}
	
	canvas_width[n_canvas-1]  = 400*n_pad_x[canvas-1];
	canvas_height[n_canvas-1] = 300*n_pad_y[canvas-1];

	Char_t *dist[][16] = {	//{"jet1.pt","jet1.E","jet1.y","jet2.pt","jet2.E","jet2.y","jet3.pt","jet3.E","jet3.y","jet4.pt","jet4.E","jet4.y","jet5.pt","jet5.E","jet5.y"},
						//	{/*"jet_hard.pt","jet_hard.E","jet_hard.y",*/"jet_forward.pt","jet_forward.E","jet_forward.y"},
						//	{"lepton_highpt.pt","lepton_lowpt.pt","event.pt_miss","event.m_ll"},
							//{"lepton_highpt.y","lepton_lowpt.y"},
							//{"lepton_misid.pt","lepton_misid.y"},
							//{"top1.pt","top1.y","top2.pt","top2.y"},
							//{"event.d_pt_ll","event.d_y_ll","event.pt_miss","event.cos_phi_ll"},
							//{"event.m_ll"},
							{"event.H_t","event.H_t"},
		{"event.H_t","event.H_t"},
		{"event.H_t","event.H_t"}
		/*,
							{"jjj_highpt.Mjjj","jj_highpt.Mjj"}	*/};
												
	Int_t n_bins[][16]  = {//	{100,120,80,100,120,80,100,120,80,100,120,80,100,120,80},
						//	{/*100,120,80,*/100,120,80},
						//	{100,100,100,100},
							//{80,80},
							//{100,80},
							//{100,80,100,80},
							//{100,100,100,100},
							//{100},
							{60,60},{60,60},{60,60}/*,
							{100,100}*/	};

	Float_t x_min[][16] = {//	{0,0,-10,0,0,-10,0,0,-10,0,0,-10,0,0,-10},
						//	{/*0,0,-10,*/0,0,-10},
						//	{0,0,0,0},
							//{-10,-10},
							//{0,-10},
							//{0,-10,0,-10},
							//{0,0,0,-1},
							//{0},
							{0,0},{0,0},{0,0}/*,
							{0,0}	*/};
											
	Float_t x_max[][16] = {	//{500,3000,10,500,3000,10,500,3000,10,500,3000,10,500,3000,10},
							//{/*500,3000,10,*/500,3000,10},
							//{500,500,500,1000},
							//{10,10},
							//{500,10},
							//{500,10,500,10},
							//{1000,10,500,1},
							//{1000},
							{3000,3000},{3000,3000},{3000,3000}/*,
							{1000,1000}	*/};
																								
	Char_t *title[][16] = {//	{"p_{T} of Jet 1","Energy of Jet 1","Rapidity of Jet 1","p_{T} of Jet 2","Energy of Jet 2","Rapidity of Jet 2","p_{T} of Jet 3","Energy of Jet 3","Rapidity of Jet 3","p_{T} of Jet 4","Energy of Jet 4","Rapidity of Jet 4","p_{T} of Jet 5","Energy of Jet 5","Rapidity of Jet 5"},
						//	{/*"p_{T} of Hardest Jet","Energy of Hardest Jet","Rapidity of Hardest Jet",*/"p_{T} of Most Forward Jet","Energy of Most Forward Jet","Rapidity of Most Forward Jet"},
						//	{"p_{T} of Hardest Lepton","p_{T} of Second Hardest Lepton","Missing Transverse Energy","M_{ll}"},
							//{"Rapidity of Hardest Lepton","Rapidity of Second Hardest Lepton"},
							//{"p_{T} of Misidentified Lepton","Rapidity of Misidentified Lepton"},
							//{"p_{T} of Top 1","Rapidity of Top 1","p_{T} of Top 2","Rapidity of Top 2"},
							//{"#Deltap_{T}(ll)","#Deltay(ll)","Missing p_{T}","cos#phi_{ll}"},
							//{"M_{ll}"},
							{"",""},{"",""},{"",""}/*,
							{"M_{jjj} of Highest p_{T} jjj","M_{jj} of Highest p_{T} jj within Highest p_{T} jjj"}	*/};			
								
	Char_t *x_title[][16] = {//	{"p_{T} (GeV)","E (GeV)","y","p_{T} (GeV)","E (GeV)","y","p_{T} (GeV)","E (GeV)","y","p_{T} (GeV)","E (GeV)","y","p_{T} (GeV)","E (GeV)","y"},
						//		{/*"p_{T} (GeV)","E (GeV)","y",*/"p_{T} (GeV)","E (GeV)","y"},
						//		{"p_{T} (GeV)","p_{T} (GeV)","#slash{E}_{T} (GeV)","M_{ll} (GeV)"},
								//{"y","y"},
								//{"p_{T} (GeV)", "y"},
								//{"p_{T} (GeV)", "y","p_{T} (GeV)", "y"},
								//{"#Deltap_{T}(ll) (GeV)","#Deltay(ll) (GeV)","p_{T}^{miss} (GeV)","cos#phi_{ll}"},
								//{"M_{ll} (GeV)"},
								{"H_{T} (GeV)","H_{T} (GeV)"},{"H_{T} (GeV)","H_{T} (GeV)"},{"H_{T} (GeV)","H_{T} (GeV)"}/*,
								{"M_{jjj} (GeV)","M_{jj} (GeV)"}*/	};												
	
	Char_t *y_title[][16] = {//	{"Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)","Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)","Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)"},
								//{/*"Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", */"#frac{d#sigma^{++}}{dp_{T}} + #frac{d#sigma^{--}}{dp_{T}}  (fb/5 GeV)", "#frac{d#sigma^{++}}{dE} + #frac{d#sigma^{--}}{dE}  (fb/25 GeV)", "#frac{d#sigma^{++}}{dy} + #frac{d#sigma^{--}}{dy}  (fb/bin)"},
							//	{/*"Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", */"#frac{d#sigma}{dp_{T}} (fb/5 GeV)", "#frac{d#sigma}{dE} (fb/25 GeV)", "#frac{d#sigma}{dy} (fb/bin)"},
//								{"#frac{d#sigma^{++}}{dp_{T}} + #frac{d#sigma^{--}}{dp_{T}}  (fb/5 GeV)", "#frac{d#sigma^{++}}{dp_{T}} + #frac{d#sigma^{--}}{dp_{T}}  (fb/5 GeV)", "#frac{d#sigma^{++}}{dp_{T}} + #frac{d#sigma^{--}}{dp_{T}}  (fb/5 GeV)", "#frac{d#sigma^{++}}{dM_{ll}} + #frac{d#sigma^{--}}{dM_{ll}}  (fb/10 GeV)"},
						//		{"#frac{d#sigma}{dp_{T}} (fb/5 GeV)", "#frac{d#sigma}{dp_{T}} (fb/5 GeV)", "#frac{d#sigma}{dp_{T}} (fb/5 GeV)", "#frac{d#sigma}{dM_{ll}} (fb/10 GeV)"},
								//{"Cross Section (fb/bin)", "Cross Section (fb/bin)"},
								//{"Cross Section (fb/bin)", "Cross Section (fb/bin)"},
								//{"Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)","Cross Section (fb/bin)"},
								//{"Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)","Cross Section (fb/bin)"},
								//{"Cross Section (fb/bin)"},
								{"#frac{d#sigma}{dH_{T}}(l^{+}l^{+}) + #frac{d#sigma}{dH_{T}}(l^{-}l^{-})  (fb/50 GeV)", "#frac{d#sigma}{dH_{T}}(l^{+}l^{+}) - #frac{d#sigma}{dH_{T}}(l^{-}l^{-})  (fb/50 GeV)"},
								{"#frac{d#sigma}{dH_{T}}(l^{+}l^{+}) + #frac{d#sigma}{dH_{T}}(l^{-}l^{-})  (fb/50 GeV)", "#frac{d#sigma}{dH_{T}}(l^{+}l^{+}) - #frac{d#sigma}{dH_{T}}(l^{-}l^{-})  (fb/50 GeV)"},
								{"#frac{d#sigma}{dH_{T}}(l^{+}l^{+}) + #frac{d#sigma}{dH_{T}}(l^{-}l^{-})  (fb/50 GeV)", "#frac{d#sigma}{dH_{T}}(l^{+}l^{+}) - #frac{d#sigma}{dH_{T}}(l^{-}l^{-})  (fb/50 GeV)"}/*,
								{"Cross Section (fb/bin)","Cross Section (fb/bin)"}	*/};
	
//***************************************************
//	for(canvas = n_canvas-1; canvas < n_canvas; canvas++)
	for(canvas = 0; canvas < n_canvas; canvas++)
	{
		TCanvas *C = new TCanvas(canvas_title[canvas],canvas_title[canvas],canvas_width[canvas],canvas_height[canvas]);
		C->Divide(n_pad_x[canvas],n_pad_y[canvas]);
/*
		if(canvas==0)
		{
		C->SetTitle("M_{T_{b}^{(5/3)}} = 400 GeV");
		}
		else if(canvas==1)
		{
			C->SetTitle("M_{T_{b}^{(5/3)}} = 600 GeV");
		}
		elseif(canvas==2)
		{
			C->SetTitle("M_{T_{b}^{(5/3)}} = 800 GeV");
		}
	*/	
		for(pad = 0; pad < n_pads[canvas]; pad++)
		{
			C->cd(pad+1);
		
			gPad->SetLogy();
		

		//	TH1F* back = new TH1F(Form("back%d%d",canvas,pad),Form("back%d%d",canvas,pad),n_bins[canvas][pad],x_min[canvas][pad],x_max[canvas][pad]);
		//	back->SetLineColor(kBlack);	
					
			for(tree = 1; tree<=n_trees; tree++)
			{
				hist[tree] = new TH1F(Form("hist[%d]",tree),Form("hist[%d]",tree),n_bins[canvas][pad],x_min[canvas][pad],x_max[canvas][pad]);
				
				// Set line and fillcolours of histograms
				if(tree == 1)
				{
					hist[tree]->SetLineColor(kBlack);
					//if (canvas==n_canvas-1)
					{
						hist[tree]->SetFillColor(kBlack);
						hist[tree]->SetFillStyle(3008);
					}
				}
				else if(tree == 2)
				{
					hist[tree]->SetLineColor(kBlack);
					//if (canvas==n_canvas-1)
					{
						hist[tree]->SetFillColor(kBlack);
						hist[tree]->SetFillStyle(3005);
					}
				}
				else if(tree == 3)
				{
					hist[tree]->SetLineColor(kBlack);
					//if (canvas==n_canvas-1)
					{
						hist[tree]->SetFillColor(kGray);
					}
				}
				else if(tree == 4)
				{
					hist[tree]->SetLineColor(kBlack);
					//if (canvas==n_canvas-1)
					{
						hist[tree]->SetFillColor(kGray+2);
					}
				}
				else if(tree == 5)
				{
					hist[tree]->SetLineColor(kBlack);
					//if (canvas==n_canvas-1)
					{
						hist[tree]->SetFillColor(kGray+3);
					}
				}
				else if(tree == 6)
				{
					hist[tree]->SetLineColor(kBlack);
					//if (canvas==n_canvas-1)
					{
						hist[tree]->SetFillColor(kBlack);
					}
				}
				else if(tree == 7)
				{
					hist[tree]->SetLineColor(kBlack);
					//if (canvas==n_canvas-1)
					{
						hist[tree]->SetFillColor(kBlack);
						hist[tree]->SetFillStyle(3013);

					}
				}
				else if(tree == 8)
				{
					hist[tree]->SetLineColor(kBlack);
					//if (canvas==n_canvas-1)
					{
						hist[tree]->SetFillColor(kBlack);
						hist[tree]->SetFillStyle(3004);

					}
				}
				else if(tree == 9)
				{
					hist[tree]->SetLineColor(kBlack);
					hist[tree]->SetLineStyle(2);
					hist[tree]->SetLineWidth(2);
					//if (canvas==n_canvas-1)
					{
					//	hist[tree]->SetFillColor(kBlack);
					}
				}
				else if(tree == 10)
				{
					hist[tree]->SetLineColor(kBlack);
					hist[tree]->SetLineStyle(1);
					hist[tree]->SetLineWidth(2);
					//if (canvas==n_canvas-1)
					{
					//	hist[tree]->SetFillColor(kBlack);
					}
				}
				else if(tree == 11)
				{
					hist[tree]->SetLineColor(kBlack);
					hist[tree]->SetLineStyle(2);
					hist[tree]->SetLineWidth(2);
					//if (canvas==n_canvas-1)
					{
						//	hist[tree]->SetFillColor(kBlack);
					}
				}
				else if(tree == 12)
				{
					hist[tree]->SetLineColor(kBlack);
					hist[tree]->SetLineStyle(1);
					hist[tree]->SetLineWidth(2);
					//if (canvas==n_canvas-1)
					{
						//	hist[tree]->SetFillColor(kBlack);
					}
				}
				else if(tree == 13)
				{
					hist[tree]->SetLineColor(kBlack);
					hist[tree]->SetLineStyle(2);
					hist[tree]->SetLineWidth(2);
					//if (canvas==n_canvas-1)
					{
						//	hist[tree]->SetFillColor(kBlack);
					}
				}
				else if(tree == 14)
				{
					hist[tree]->SetLineColor(kBlack);
					hist[tree]->SetLineStyle(1);
					hist[tree]->SetLineWidth(2);
					//if (canvas==n_canvas-1)
					{
						//	hist[tree]->SetFillColor(kBlack);
					}
				}
				else //default colour is black
				{
					hist[tree]->SetLineColor(kBlack);
					if (canvas==n_canvas-1)
					{
						hist[tree]->SetFillColor(kBlack);
					}
				}
				
				/*	if(tree < 9)
				{
					hist[tree]->SetLineColor(tree+1);
					hist[tree]->SetFillColor(tree+1);
				}
				else if(tree >= 9)
				{
					hist[tree]->SetLineColor(tree+2);
					hist[tree]->SetFillColor(tree+2);
				}
			*/
				
				//hist[tree]->Integral(0,1000);
				
				if(tree == 1)
				{
					if(canvas == 0)
					{
						if(pad == n_pads[canvas]-1)
						{
							Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.signed_weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts);
						}
					
						else					
						{
							Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts);
						}
					}
					else if(canvas == 1)
					{
						if(pad == n_pads[canvas]-1)
						{
							Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.signed_weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts_600);
						}
						
						else					
						{
							Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts_600);
						}
					}
					else if(canvas == 2)
					{
						if(pad == n_pads[canvas]-1)
						{
							Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.signed_weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts_800);
						}
						
						else					
						{
							Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts_800);
						}
					}
					else					
					{
						Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts);
					}
				}
				else if(tree > 1)
				{
					if(canvas == 0)
					{
						if(pad == n_pads[canvas]-1)
						{
							Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.signed_weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts,"same");
						}
						
						else					
						{
							Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts,"same");
						}
					}
					else if(canvas == 1)
					{
						if(pad == n_pads[canvas]-1)
						{
							Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.signed_weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts_600,"same");
						}
						
						else					
						{
							Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts_600,"same");
						}
					}
					else if(canvas == 2)
					{
						if(pad == n_pads[canvas]-1)
						{
							Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.signed_weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts_800,"same");
						}
						
						else					
						{
							Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts_800,"same");
						}
					}
					else					
					{
						Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.weight*%f",(double)(luminosity/n_groups[tree-1])) * cuts,"same");
					}
				}
				
	/*		
				if(tree > 1 && tree <= n_back)
				{
					back->Add(hist[tree],1);				
				}
	*/		
				
				/*legend->AddEntry(hist[tree],label[tree-1],"l");
				legend->Draw();*/
	
				hist[tree]->Clear();
				hist[tree]->GetXaxis()->SetTitle(x_title[canvas][pad]);
				hist[tree]->GetYaxis()->SetTitle(y_title[canvas][pad]);
				hist[tree]->SetTitle(title[canvas][pad]);

				hist[tree]->GetYaxis()->SetRangeUser(0.0001,1);

			}
		
			
			
		// Stack Histograms
			if (canvas==0)
			{	
				for(tree = 2; tree <= n_back; tree++)
				{
					hist[tree]->Add(hist[tree-1],1);
				}
				hist[9]->Add(hist[n_back],1);
				hist[10]->Add(hist[n_back],1);
 
		
		//Redraw Histograms	
			//	hist[n_trees]->Draw();
				hist[10]->Draw();
				hist[9]->Draw("same");
				for(tree = n_back; tree >= 1; tree--)
				{
					hist[tree]->Draw("same");
				}
			}			
			
			if (canvas==1)
			{	
				for(tree = 2; tree <= n_back; tree++)
				{
					hist[tree]->Add(hist[tree-1],1);
				}
				hist[11]->Add(hist[n_back],1);
				hist[12]->Add(hist[n_back],1);
				
				
				//Redraw Histograms	
				//hist[n_trees]->Draw();
				hist[12]->Draw();
				hist[11]->Draw("same");
				for(tree = n_back; tree >= 1; tree--)
				{
					hist[tree]->Draw("same");
				}
			}
			
			if (canvas==2)
			{	
				for(tree = 2; tree <= n_back; tree++)
				{
					hist[tree]->Add(hist[tree-1],1);
				}
				hist[13]->Add(hist[n_back],1);
				hist[14]->Add(hist[n_back],1);
				
				
				//Redraw Histograms	
				//hist[n_trees]->Draw();
				hist[14]->Draw();
				hist[13]->Draw("same");
				for(tree = n_back; tree >= 1; tree--)
				{
					hist[tree]->Draw("same");
				}
			}
			
			gPad->RedrawAxis();
		
	//		back->Draw("same");
	//		legend->AddEntry(Form("back%d%d",canvas,pad),"Total Bkgd.","l");
			
			
			if(canvas == 0)
			{
				legend->Clear();

				legend->AddEntry(hist[10],label[9],"l");
				legend->AddEntry(hist[9],label[8],"l");
				
				for(tree = n_back; tree >= 1; tree--)
				{
					legend->AddEntry(hist[tree],label[tree-1],"f");
				}
				
				legend->Draw();
				
			}
			
			else if(canvas == 1)
			{
				legend2->Clear();

				legend2->AddEntry(hist[12],label[11],"l");
				legend2->AddEntry(hist[11],label[10],"l");
				
				for(tree = n_back; tree >= 1; tree--)
				{
					legend2->AddEntry(hist[tree],label[tree-1],"f");
				}
				
				legend2->Draw();
			}
			
			else if(canvas == 2)
			{
				legend3->Clear();

				legend3->AddEntry(hist[14],label[13],"l");
				legend3->AddEntry(hist[13],label[12],"l");
				
				for(tree = n_back; tree >= 1; tree--)
				{
					legend3->AddEntry(hist[tree],label[tree-1],"f");
				}
				
				legend3->Draw();
			}
			
			//Draw Legend
		//	for(tree = n_back; tree >= 1; tree--)
			{
				//if (canvas==n_canvas-1)
				{
				//	if(tree <= n_back)
					{
			//			legend->AddEntry(hist[tree],label[tree-1],"f");
					}
				//	else if(tree > n_back)
					{
				//		legend->AddEntry(hist[tree],label[tree-1],"l");
						
					}

					
					
//					legend->AddEntry(hist[tree],label[tree-1],"l");
				}
			//	else
				{
			//		legend->AddEntry(hist[tree],label[tree-1],"l");
				}
				//legend->Draw();
			}
			
		//	legend->Draw();
		}
	}
}