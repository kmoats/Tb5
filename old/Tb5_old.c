{
	gROOT->Reset();

	#include <cmath>

	new TBrowser();
	Long64_t maxsize = 1E15;
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
//		Float_t R_jj[n_jets+1][n_jets+1];
//		Float_t R_jl[n_jets+1][n_leptons+1];
//		Float_t R_ll[n_leptons+1][n_leptons+1];
	};
	event_t event;
	
	struct R_t
	{
		Float_t R;
	};
	R_t Rjj[n_jets+1][n_jets+1];
	R_t Rjl[n_jets+1][n_leptons+1];
	R_t Rll[n_leptons+1][n_leptons+1];
/*
	struct jj_t
	{
		Float_t Mjj;
		Float_t ptjj;
	};
	jj_t jj[n_jets+1][n_jets+1];
	jj_t jj_highpt;
	
	struct jjj_t
	{
		Float_t Mjjj;
		Float_t ptminusMjjj;
		Float_t ptjjj;
	};
	jjj_t jjj[n_jets+1][n_jets+1][n_jets+1];
	jjj_t jjj_highpt;
*/
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
	
	Char_t *label[] = {"Signal","t#bar{t} + j", "Z/#gamma + j", "W^{+}W^{-} + j", "W^{#pm}Z + j", "t#bar{t}W^{#pm} + j","W^{#pm}W^{#pm} + j","W^{#pm}W^{#pm}W^{#mp} + j","t#bar{t}W^{+}W^{-} + j","W^{#pm} + j"};
	
	Int_t tree;
	Int_t file;
	Int_t n_trees = 10;  // Set # of trees (= # of histograms to superimpose on each plot)
	Int_t n_files[] = {2,2,2,2,2,2,2,2,2,2};
	Int_t n_groups[] = {1,1,1,1,1,1,1,1,1,1};
	Int_t n_back = 9;

	Int_t tree_ttbar = 2;	//Identify the tree corresponding to the ttbar + jets background (for charge misidentifcation purposes)
	Int_t tree_zgamma = 3;	//Identify the tree corresponding to the z/gamma + jets background (for charge misidentifcation purposes)
	Int_t tree_wpwm = 4;	//Identify the tree corresponding to the w+w- + jets background (for charge misidentifcation purposes)
	Int_t tree_wz = 5;	//Identify the tree corresponding to the wz + jets background (for charge misidentifcation purposes)
	Int_t tree_wjets = 10;	//Identify the tree corresponding to the w + jets background (for jet faking lepton)
	
	Char_t *dat_file[][3] = {	
								{"pp_Tb5t~j_14TeV_f700ni_10000_1_results.dat",
								 "pp_Tb5t~j_cc_14TeV_f700ni_10000_1_results.dat"},
								{"pp_tt~+jets_14TeV_sm_10000_1_results.dat",
								 "pp_tt~+jets_cc_14TeV_sm_10000_1_results.dat"},
								{"pp_ZA+jets_14TeV_sm_10000_1_results.dat",
								 "pp_ZA+jets_cc_14TeV_sm_10000_1_results.dat"},
								{"pp_w+w-+jets_14TeV_sm_10000_1_results.dat",
								 "pp_w+w-+jets_cc_14TeV_sm_10000_1_results.dat"},
								{"pp_w+z+jets_14TeV_sm_10000_1_results.dat",
								 "pp_w+z+jets_cc_14TeV_sm_10000_1_results.dat"},
								{"pp_tt~w+jets_14TeV_sm_10000_1_results.dat",
								 "pp_tt~w+jets_cc_14TeV_sm_10000_1_results.dat"},
								{"pp_wpwp+jets_14TeV_sm_10000_1_results.dat",
								 "pp_wpwp+jets_cc_14TeV_sm_10000_1_results.dat"},
								{"pp_wpwpwm+jets_14TeV_sm_10000_1_results.dat",
								 "pp_wpwpwm+jets_cc_14TeV_sm_10000_1_results.dat"},
								{"pp_tt~wpwm+jets_14TeV_sm_10000_1_results.dat",
							     "pp_tt~wpwm+jets_cc_14TeV_sm_10000_1_results.dat"},
								{"pp_w+jets_14TeV_sm_10000_1_results.dat",
							     "pp_w+jets_cc_14TeV_sm_10000_1_results.dat"}
							};
	
	Char_t *lhe_file[][3] = {	
								{"pp_Tb5t~j_14TeV_f700ni_10000_1_events.lhe",
								 "pp_Tb5t~j_cc_14TeV_f700ni_10000_1_events.lhe"},		
								{"pp_tt~+jets_14TeV_sm_10000_1_events.lhe",
								 "pp_tt~+jets_cc_14TeV_sm_10000_1_events.lhe"},
								{"pp_ZA+jets_14TeV_sm_10000_1_events.lhe",
								 "pp_ZA+jets_cc_14TeV_sm_10000_1_events.lhe"},
								{"pp_w+w-+jets_14TeV_sm_10000_1_events.lhe",
								 "pp_w+w-+jets_cc_14TeV_sm_10000_1_events.lhe"},
								{"pp_w+z+jets_14TeV_sm_10000_1_events.lhe",
								 "pp_w+z+jets_cc_14TeV_sm_10000_1_events.lhe"},
								{"pp_tt~w+jets_14TeV_sm_10000_1_events.lhe",
								 "pp_tt~w+jets_cc_14TeV_sm_10000_1_events.lhe"},
								{"pp_wpwp+jets_14TeV_sm_10000_1_events.lhe",
								 "pp_wpwp+jets_cc_14TeV_sm_10000_1_events.lhe"},
								{"pp_wpwpwm+jets_14TeV_sm_10000_1_events.lhe",
								 "pp_wpwpwm+jets_cc_14TeV_sm_10000_1_events.lhe"},
								{"pp_tt~wpwm+jets_14TeV_sm_10000_1_events.lhe",
							     "pp_tt~wpwm+jets_cc_14TeV_sm_10000_1_events.lhe"},
								{"pp_w+jets_14TeV_sm_10000_1_events.lhe",
								 "pp_w+jets_cc_14TeV_sm_10000_1_events.lhe"}
							};
	
	Float_t dat_x_sect[n_trees+1];
	Float_t lhe_x_sect[n_trees+1];
	
	Float_t x_sect[n_trees+1];
	Float_t x_sect_a[n_trees+1];
	Float_t x_sect_l[n_trees+1];
	Float_t x_sect_lj[n_trees+1];
	Float_t x_sect_ljh[n_trees+1];
	 
	Float_t eff_a[n_trees+1];
	Float_t eff_lept[n_trees+1];
	Float_t eff_jet[n_trees+1];
	Float_t eff_Ht[n_trees+1];

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
	Float_t pt_miss_min = 60;
//	Float_t d_pt_ll_min = 0;
//	Float_t d_y_ll_max = 100;
//	Float_t cos_phi_ll_min = 0;
	Float_t m_ll_min = 110;

	TCut lepton_cut_1 = Form("lepton_highpt.pt > %f", lepton_highpt_min);
	TCut lepton_cut_2 = Form("lepton_lowpt.pt > %f", lepton_lowpt_min);
	TCut lepton_cut_3 = Form("event.pt_miss > %f", pt_miss_min);
	TCut lepton_cut_4 = Form("event.m_ll > %f", m_ll_min);
//	TCut lepton_cut_5 = Form("event.d_pt_ll > %f", d_pt_ll_min);
//	TCut lepton_cut_6 = Form("event.d_y_ll < %f", d_y_ll_max);
//	TCut lepton_cut_7 = Form("fabs(event.cos_phi_ll) > %f", cos_phi_ll_min);

	TCut lepton_cuts = lepton_cut_1 && lepton_cut_2 && lepton_cut_3 && lepton_cut_4;

//********** Jet Cuts **********

	Float_t jet_forward_y_min = 3;
	Float_t jet_forward_y_max = 5;
	Float_t jet_forward_pt_min = 30;
	Float_t jet_forward_E_min = 700;
	
	TCut jet_forward_cut_1 = Form("fabs(jet_forward.y) > %f", jet_forward_y_min);
	TCut jet_forward_cut_2 = Form("fabs(jet_forward.y) < %f", jet_forward_y_max);
	TCut jet_forward_cut_3 = Form("jet_forward.pt > %f", jet_forward_pt_min);
	TCut jet_forward_cut_4 = Form("jet_forward.E > %f", jet_forward_E_min);

	TCut jet_forward_cuts = jet_forward_cut_1 && jet_forward_cut_2 && jet_forward_cut_3 && jet_forward_cut_4;
	/*
	Float_t jet_hard_y_min = 0;
	Float_t jet_hard_y_max = 5;
	Float_t jet_hard_pt_min = 0;//80;
	Float_t jet_hard_E_min = 0;
	
	TCut jet_hard_cut_1 = Form("fabs(jet_hard.y) > %f", jet_hard_y_min);
	TCut jet_hard_cut_2 = Form("fabs(jet_hard.y) < %f", jet_hard_y_max);
	TCut jet_hard_cut_3 = Form("jet_hard.pt > %f", jet_hard_pt_min);
	TCut jet_hard_cut_4 = Form("jet_hard.E > %f", jet_hard_E_min);
	
	
	TCut jet_hard_cuts = jet_hard_cut_1 && jet_hard_cut_2 && jet_hard_cut_3 && jet_hard_cut_4;
	*/
	/*
	Float_t Mjjj_min = 0;
	Float_t Mjjj_max = 100000;
	
	TCut jjj_cut_1 = Form("jjj_highpt.Mjjj > %f", Mjjj_min);
	TCut jjj_cut_2 = Form("jjj_highpt.Mjjj < %f", Mjjj_max);

	TCut jjj_cuts = jjj_cut_1 && jjj_cut_2;
	
	Float_t Mjj_min = 0;
	Float_t Mjj_max = 100000;
	
	TCut jj_cut_1 = Form("jj_highpt.Mjj > %f", Mjj_min);
	TCut jj_cut_2 = Form("jj_highpt.Mjj < %f", Mjj_max);
	
	TCut jj_cuts = jj_cut_1 && jj_cut_2;
	*/
	TCut jet_cuts = jet_forward_cuts;// && jet_hard_cuts;// && jjj_cuts && jj_cuts;
	
//********** Ht Cut **********
	
	Float_t Ht_min = 550;

	TCut Ht_cut = Form("event.H_t > %f", Ht_min);
	
	
	TCut cuts = acceptance_cuts && lepton_cuts && jet_cuts && Ht_cut;
							

//******************** CREATE TREES ********************

	// create a new ROOT file
	TFile *f = new TFile("Tb5_file.root","RECREATE");

	// create a TTree
	TTree *Tree[n_trees+1]; 

	for(tree=1; tree<=n_trees; tree++)
	{
		Tree[tree] = new TTree(Form("Tree[%d]",tree),"Data from event file"); 

		// create branches for the event data
		TBranch *branch = Tree[tree]->Branch("event",&event.n_particles,"n_particles/I:weight/F:pt_miss/F:pt_ll/F:m_ll/F:H_t/F:veto/I");

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
	/*
		// create branches for each 2-jet combination
		for(Int_t j=1; j<=n_jets; j++)
		{	
			for(Int_t k=j+1; k<=n_jets; k++)
			{	
				TBranch *branch = Tree[tree]->Branch(Form("jj%d%d",j,k),&jj[j][k].Mjj,"Mjj/F:ptjj:F");
			}
		}
		
		TBranch *branch = Tree[tree]->Branch("jj_highpt",&jj_highpt.Mjj,"Mjj/F:ptjj/F");

		// create branches for each 3-jet combination
		for(Int_t j=1; j<=n_jets; j++)
		{	
			for(Int_t k=j+1; k<=n_jets; k++)
			{	
				for(Int_t l=k+1; l<=n_jets; l++)
				{
					TBranch *branch = Tree[tree]->Branch(Form("jjj%d%d%d",j,k,l),&jjj[j][k][l].Mjjj,"Mjjj/F:ptminusMjjj/F:ptjjj/F");
				}
			}
		}
		
		TBranch *branch = Tree[tree]->Branch("jjj_highpt",&jjj_highpt.Mjjj,"Mjjj/F:ptminusMjjj/F:ptjjj/F");
*/
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

//****************************************************************

//Start filling the Trees with data from the event files
	for(tree=1; tree<=n_trees; tree++)
	{
		x_sect[tree] = 0;
		x_sect_l[tree] = 0;
		x_sect_lj[tree] = 0;
		x_sect_ljh[tree] = 0;

		for(file=1; file<=n_files[tree-1]; file++)
		{
			//open the results.dat file and read in the cross section
			FILE *infile = fopen(dat_file[tree-1][file-1],"r");
			fgets(&line,150,infile);
			sscanf(&line[0],"%f",&dat_x_sect[tree]);
			dat_x_sect[tree] = 1000*dat_x_sect[tree];
			fclose(infile);
		
			//open the events.lhe file 
			FILE *infile = fopen(lhe_file[tree-1][file-1],"r");

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

					Int_t j = 0;
					Int_t l = 0;
					Int_t n = 0;
			//		Int_t t = 0;
					
			//		jet_hard.pt = 0;	
					jet_forward.y = 0;	
					lepton_highpt.pt = 0;	
					lepton_lowpt.pt = 100000;	
					event.veto=0;
			//		jjj_highpt.ptjjj=0;
			//		jj_highpt.ptjj=0;
				
					for(Int_t i=1; i<=event.n_particles; i++)
					{
						fgets(&line,150,infile);
						sscanf(&line[0],"%d",&particle[i].id);		
						sscanf(&line[35],"%f%f%f%f",&particle[i].p1,&particle[i].p2,&particle[i].p3,&particle[i].p4);
			
						// calculate pt, E and y for each particle in the event
						particle[i].pt = sqrt(pow(particle[i].p1,2) + pow(particle[i].p2,2));
						particle[i].E = particle[i].p4;
						if(particle[i].p4-particle[i].p3 != 0)
						{
							particle[i].y = 0.5*log((particle[i].p4+particle[i].p3)/(particle[i].p4-particle[i].p3));
						}
			
						// find jets in the event
						if((abs(particle[i].id) == 1 || abs(particle[i].id) == 2 || abs(particle[i].id) == 3 || abs(particle[i].id) == 4 || abs(particle[i].id) == 5 || abs(particle[i].id) == 21 ) && i > 2 )
						{
							if(tree == tree_wjets && l == 1 )
							{
								if(particle[i].pt > lepton_pt_min && fabs(particle[i].y) < lepton_y_max)
								{
									l++;
									lepton[l] = particle[i];
									
									if(lepton[l].pt > lepton_highpt.pt) // find highest pt lepton
									{
										lepton_highpt = lepton[l];
									}
									
									if(lepton[l].pt < lepton_lowpt.pt) // find lowest pt lepton
									{
										lepton_lowpt = lepton[l];
									}
									
									event.weight = event.weight*eff_jetmisid;

								}
								else
								{
									n++;
									neutrino[n] = particle[i];
								}
							}
							
							else
							{
								if(particle[i].pt > jet_pt_min && fabs(particle[i].y) < jet_y_max)
								{
									j++;
									jet[j] = particle[i];		
						
								/*	if(jet[j].pt > jet_hard.pt) // find highest pt jet
									{
										jet_hard = jet[j];
									}		
								*/	if(fabs(jet[j].y) > fabs(jet_forward.y)) // find most forward jet
									{
										jet_forward = jet[j];
									}
								}
								else
								{
									n++;
									neutrino[n] = particle[i];
								}
							
							}
						}
						
						// find neutrinos in the event
						if(abs(particle[i].id) == 12 || abs(particle[i].id) == 14)
						{
							n++;
							neutrino[n] = particle[i];
						}	
			
						// find leptons in the event
						if(abs(particle[i].id) == 11 || abs(particle[i].id) == 13) 
						{
							if(((file % 2) == 1 && particle[i].id > 0)||((file % 2) == 0 && particle[i].id < 0)) // Need to fix this for charge conjugate process
							{
								if(tree == tree_wz)
								{
									if(fabs(particle[i].y) > 2.5)
									{
										n++;
										neutrino[n] = particle[i];
									}
								
									else
									{
										event.veto = 1;
									}
								}
								
								if(tree == tree_ttbar || tree == tree_zgamma || tree == tree_wpwm)
								{
									if(particle[i].pt > lepton_pt_min && fabs(particle[i].y) < lepton_y_max)
									{
										l++;
										lepton[l] = particle[i];
									
										if(lepton[l].pt > lepton_highpt.pt) // find highest pt lepton
										{
											lepton_highpt = lepton[l];
										}
										
										if(lepton[l].pt < lepton_lowpt.pt) // find lowest pt lepton
										{
											lepton_lowpt = lepton[l];
										}
																		
									lepton_misid = particle[i];
									
									if(abs(lepton_misid.id) == 11) // if electron
									{
										if(lepton_misid.pt >= 0 && lepton_misid.pt < 100)
										{
											eff_chargemisid = 0.0008;
										}
										if(lepton_misid.pt >= 100 && lepton_misid.pt < 300)
										{
											eff_chargemisid = 0.006;
										}
										if(lepton_misid.pt >= 300 && lepton_misid.pt < 500)
										{
											eff_chargemisid = 0.02;
										}
										if(lepton_misid.pt >= 500 && lepton_misid.pt < 1000)
										{
											eff_chargemisid = 0.06;
										}
										if(lepton_misid.pt >= 1000)
										{
											eff_chargemisid = 0.2;
										}
									}
									
									if(abs(lepton_misid.id) == 13) // if muon
									{
										if(lepton_misid.pt >= 0 && lepton_misid.pt < 100)
										{
											eff_chargemisid = 0.0004;
										}
										if(lepton_misid.pt >= 100 && lepton_misid.pt < 300)
										{
											eff_chargemisid = 0.002;
										}
										if(lepton_misid.pt >= 300 && lepton_misid.pt < 500)
										{
											eff_chargemisid = 0.01;
										}
										if(lepton_misid.pt >= 500 && lepton_misid.pt < 1000)
										{
											eff_chargemisid = 0.05;
										}
										if(lepton_misid.pt >= 1000)
										{
											eff_chargemisid = 0.2;
										}
									}
										event.weight = event.weight*eff_chargemisid;
									}
									else
									{
										n++;
										neutrino[n] = particle[i];
									}
								}	
							}
							
							if(((file % 2) == 1 && particle[i].id < 0)||((file % 2) == 0 && particle[i].id > 0))
							{
								if(particle[i].pt > lepton_pt_min && fabs(particle[i].y) < lepton_y_max)
								{
									l++;
									lepton[l] = particle[i];
									
									if(lepton[l].pt > lepton_highpt.pt) // find highest pt lepton
									{
										lepton_highpt = lepton[l];
									}
									
									if(lepton[l].pt < lepton_lowpt.pt) // find lowest pt lepton
									{
										lepton_lowpt = lepton[l];
									}
								}
								
								else
								{		
									n++;
									neutrino[n] = particle[i];
								}
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
					}	
				
					if(l < 2)
					{
						event.veto = 1;  // Require like-sign dileptons
					}
					
					if(j == 0)
					{
						event.veto = 1;  // Require jets
					}
					
					if(n == 0)
					{
						event.veto = 1;  // Require missing energy
					}
					
					// calculate the jet-jet separation
					if(j >= 2)
					{
						for(Int_t m=1; m<=j; m++)
						{	
							for(Int_t k=m+1; k<=j; k++)
							{	
								Float_t dy = fabs(jet[m].y - jet[k].y);
								Float_t denom = sqrt(pow(jet[m].p1,2)+pow(jet[k].p1,2))*sqrt(pow(jet[m].p2,2)+pow(jet[k].p2,2));
								Float_t dphi = acos((jet[m].p1*jet[m].p2+jet[k].p1*jet[k].p2)/denom);

								Rjj[m][k].R = sqrt(dy*dy + dphi*dphi);
												
								if(Rjj[m][k].R < 0.4)
								{
									event.veto=1;		// Work on merging jets rather than vetoing the event
								}
							}
						}
					}
					
					// calculate the jet-lepton separation
					if(j>=1  && l>=1)
					{
						for(Int_t m=1; m<=j; m++)
						{	
							for(Int_t k=1; k<=l; k++)
							{	
								Float_t dy = fabs(jet[m].y - lepton[k].y);
								Float_t denom = sqrt(pow(jet[m].p1,2)+pow(lepton[k].p1,2))*sqrt(pow(jet[m].p2,2)+pow(lepton[k].p2,2));
								Float_t dphi = acos((jet[m].p1*jet[m].p2+lepton[k].p1*lepton[k].p2)/denom);
				 
								Rjl[m][k].R = sqrt(dy*dy + dphi*dphi);	
					
								if(Rjl[m][k].R < 0.4)
								{
							//		event.weight=0;
									event.veto=1;
								}
							}
						}
					}
					
					// calculate the lepton-lepton separation
					if(l >= 2)
					{
						for(Int_t m=1; m<=l; m++)
						{	
							for(Int_t k=m+1; k<=l; k++)
							{	
								Float_t dy = fabs(lepton[m].y - lepton[k].y);
								Float_t denom = sqrt(pow(lepton[m].p1,2)+pow(lepton[k].p1,2))*sqrt(pow(lepton[m].p2,2)+pow(lepton[k].p2,2));
								Float_t dphi = acos((lepton[m].p1*lepton[m].p2+lepton[k].p1*lepton[k].p2)/denom);
						
								Rll[m][k].R = sqrt(dy*dy + dphi*dphi);
						
								if(Rll[m][k].R < 0.4)
								{
									event.veto=1;
								}
							}
						}
					}
				/*
					// calculate the invariant mass of each 2-jet combination
					for(Int_t m=1; m<=j; m++)
					{	
						for(Int_t k=m+1; k<=j; k++)
						{	
							Float_t	sum_p1 = jet[m].p1+jet[k].p1;
							Float_t	sum_p2 = jet[m].p2+jet[k].p2;
							Float_t	sum_p3 = jet[m].p3+jet[k].p3;
							Float_t	sum_p4 = jet[m].p4+jet[k].p4;

							jj[m][k].Mjj = sqrt(pow(sum_p4,2)-pow(sum_p3,2)-pow(sum_p2,2)-pow(sum_p1,2));
							jj[m][k].ptjj = sqrt(pow(jet[m].p1+jet[k].p1,2) + pow(jet[m].p2+jet[k].p2,2));
						}
					}
				
					// calculate the invariant mass of each 3-jet combination
					for(Int_t m=1; m<=j; m++)
					{	
						for(Int_t k=m+1; k<=j; k++)
						{
							for(Int_t o=k+1; o<=j; o++)
							{
								Float_t	sum_p1 = jet[m].p1+jet[k].p1+jet[o].p1;
								Float_t	sum_p2 = jet[m].p2+jet[k].p2+jet[o].p2;
								Float_t	sum_p3 = jet[m].p3+jet[k].p3+jet[o].p3;
								Float_t	sum_p4 = jet[m].p4+jet[k].p4+jet[o].p4;
						
								jjj[m][k][o].Mjjj = sqrt(pow(sum_p4,2)-pow(sum_p3,2)-pow(sum_p2,2)-pow(sum_p1,2));
								jjj[m][k][o].ptminusMjjj = jet[m].pt+jet[k].pt+jet[o].pt - jjj[m][k][o].Mjjj;
								jjj[m][k][o].ptjjj = sqrt(pow(jet[m].p1+jet[k].p1+jet[o].p1,2) + pow(jet[m].p2+jet[k].p2+jet[o].p2,2));

								if(jjj[m][k][o].ptjjj > jjj_highpt.ptjjj)
								{
									jjj_highpt = jjj[m][k][o];
								
									if(jj[m][k].ptjj > jj_highpt.ptjj)
									{
										jj_highpt = jj[m][k];
									}
									if(jj[m][o].ptjj > jj_highpt.ptjj)
									{
										jj_highpt = jj[m][o];
									}
									if(jj[k][o].ptjj > jj_highpt.ptjj)
									{
										jj_highpt = jj[k][o];
									}
								}
							}
						}
					}
				*/
					// Calculate d_pt_ll, d_y_ll, m_t, etc...				
				//	event.d_pt_ll = sqrt(pow(lepton[1].p1-lepton[2].p1,2) + pow(lepton[1].p2-lepton[2].p2,2));
				//	event.d_y_ll = fabs(lepton[1].y - lepton[2].y);
				
				//	event.cos_phi_ll = (lepton[1].p1-lepton[2].p1)/event.d_pt_ll;
				
					Float_t	sum_p1_leptons = lepton[1].p1+lepton[2].p1;
					Float_t	sum_p2_leptons = lepton[1].p2+lepton[2].p2;
					Float_t	sum_p3_leptons = lepton[1].p3+lepton[2].p3;				
					Float_t	sum_p4_leptons = lepton[1].p4+lepton[2].p4;
				
					Float_t	sum_p1_neutrinos = 0;
					Float_t	sum_p2_neutrinos = 0;
					Float_t	sum_p3_neutrinos = 0;				
					Float_t	sum_p4_neutrinos = 0;
					
					for(Int_t m=1; m<=n; m++)
					{
						sum_p1_neutrinos = sum_p1_neutrinos + neutrino[m].p1;
						sum_p2_neutrinos = sum_p2_neutrinos + neutrino[m].p2;
						sum_p3_neutrinos = sum_p3_neutrinos + neutrino[m].p3;				
						sum_p4_neutrinos = sum_p4_neutrinos + neutrino[m].p4;			
					}
					
					Float_t	sum_p1 = sum_p1_leptons+sum_p1_neutrinos;
					Float_t	sum_p2 = sum_p2_leptons+sum_p2_neutrinos;
					Float_t	sum_p3 = sum_p3_leptons+sum_p3_neutrinos;
					Float_t	sum_p4 = sum_p4_leptons+sum_p4_neutrinos;

					Float_t	pt_tot = sqrt(pow(sum_p1,2) + pow(sum_p2,2));

					event.pt_ll = sqrt(pow(sum_p1_leptons,2)+pow(sum_p2_leptons,2));
					
					event.pt_miss = sqrt(pow(sum_p1_neutrinos,2)+pow(sum_p2_neutrinos,2));
					
					event.m_ll = sqrt(pow(sum_p4_leptons,2)-pow(sum_p3_leptons,2)-pow(sum_p2_leptons,2)-pow(sum_p1_leptons,2));
					
				//	event.m_t  = sqrt(pow(sqrt(pow(event.m_ll,2) + pow(event.pt_ll,2)) + event.pt_miss,2) - pow(pt_tot,2));
				
					// Calculate Ht
					event.H_t = fabs(event.pt_miss);
					for(Int_t m=1; m<=j; m++)
					{
						event.H_t  = event.H_t + fabs(jet[m].pt);
					}
					for(Int_t m=1; m<=l; m++)
					{
						event.H_t  = event.H_t + fabs(lepton[m].pt);
					}
					
					//scale event weights to get the correct cross section (the value from results.dat)
					event.weight = event.weight*dat_x_sect[tree]/lhe_x_sect[tree]; 
				
					//calculate cross section (in fb) by adding up the weights from each event
					x_sect[tree] = x_sect[tree] + (double)1000*event.weight/n_groups[tree-1];

					//acceptance cuts
					if(event.veto == 0)
					{								
						x_sect_a[tree] = x_sect_a[tree] + (double)1000*event.weight/n_groups[tree-1];

						//leptonic cuts
						if(lepton_highpt.pt > lepton_highpt_min && lepton_lowpt.pt > lepton_lowpt_min && event.pt_miss > pt_miss_min && event.m_ll > m_ll_min)
						{
							x_sect_l[tree] = x_sect_l[tree] + (double)1000*event.weight/n_groups[tree-1];

							//jet cuts
							if(fabs(jet_forward.y) > jet_forward_y_min && fabs(jet_forward.y) < jet_forward_y_max && jet_forward.pt > jet_forward_pt_min && jet_forward.E > jet_forward_E_min)
							{					
					//			if(fabs(jet_hard.y) > jet_hard_y_min && fabs(jet_hard.y) < jet_hard_y_max && jet_hard.pt > jet_hard_pt_min && jet_hard.E > jet_hard_E_min)
								{
					//				if(jjj_highpt.Mjjj > Mjjj_min && jjj_highpt.Mjjj < Mjjj_max && jj_highpt.Mjj > Mjj_min && jj_highpt.Mjj < Mjj_max)
									{
										x_sect_lj[tree] = x_sect_lj[tree] + (double)1000*event.weight/n_groups[tree-1];
									
										// H_t cut
										if(event.H_t > Ht_min)
										{
											x_sect_ljh[tree] = x_sect_ljh[tree] + (double)1000*event.weight/n_groups[tree-1];
										}
									}
								}
							}
						}
					}

					Tree[tree]->Fill();
					
				}
			}
		
			fclose(infile);
		}

		if(x_sect[tree] == 0)
		{
			eff_a[tree] = 0;
		}
		if(x_sect[tree] != 0)
		{
			eff_a[tree] = 100*x_sect_a[tree]/x_sect[tree];
		}	
		if(x_sect_a[tree] == 0)
		{
			eff_lept[tree] = 0;
		}
		if(x_sect_a[tree] != 0)
		{
			eff_lept[tree] = 100*x_sect_l[tree]/x_sect_a[tree];
		}	
		if(x_sect_l[tree] == 0)
		{
			eff_jet[tree] = 0;
		}
		if(x_sect_l[tree] != 0)
		{
			eff_jet[tree] = 100*x_sect_lj[tree]/x_sect_l[tree];
		}	
		if(x_sect_lj[tree] == 0)
		{
			eff_Ht[tree] = 0;
		}
		if(x_sect_l[tree] != 0)
		{
			eff_Ht[tree] = 100*x_sect_ljh[tree]/x_sect_lj[tree];
		}	
	
		cout << Form("%s - Cross Section (fb): ", label[tree-1]) << endl;
		cout << "No Cuts:            "<< x_sect[tree] << endl;
		cout << "+ Selection Cuts:   "<< x_sect_a[tree] << " (" << eff_a[tree] << " %)"<< endl;
		cout << "+ Leptonic Cuts:    "<< x_sect_l[tree] << " (" << eff_lept[tree] << " %)"<< endl;
		cout << "+ Jet Cuts:         "<< x_sect_lj[tree] << " (" << eff_jet[tree] << " %)"<<  endl;
		cout << "+ Ht Cut:           "<< x_sect_ljh[tree] << " (" << eff_Ht[tree] << " %)"<<  endl;

	}

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
	gStyle->SetPadLeftMargin(0.12);
	gStyle->SetTitleOffset(1.2,"X");
	gStyle->SetTitleOffset(1.4,"Y");
	gStyle->SetTitleFillColor(kWhite);
	gStyle->SetTitleBorderSize(0);
	gStyle->SetTitleX(0.12);
	gStyle->SetTitleY(0.97);

	TH1F *hist[n_trees+1];

	legend = new TLegend(0.63,0.65,0.87,0.79);
	legend->SetFillColor(kWhite);

	Int_t n_canvas = 6;//7;//8;//9;
	Char_t *canvas_title[] = {"jets","tagged jets", "lepton pt","lepton y","misid lepton"/*,"tops"*//*,"d_pt_d_y"*/,"m_ll & H_t"/*,"jjj"*/};
	Int_t n_pad_x[] = {3,3,3,2,2/*,2*//*,2*/,2/*,2*/};
	Int_t n_pad_y[] = {5,1,1,1,1/*,2*//*,2*/,1/*,1*/};
	Int_t n_pads[n_canvas];
	Int_t pad;
	Int_t canvas_width[n_canvas];
	Int_t canvas_height[n_canvas];
	Int_t canvas;

	for(canvas = 0; canvas < n_canvas; canvas++)
	{
		n_pads[canvas]= n_pad_x[canvas]*n_pad_y[canvas];
		canvas_width[canvas]  = 300*n_pad_x[canvas];
		canvas_height[canvas] = 200*n_pad_y[canvas];
	}
	
	canvas_width[n_canvas-1]  = 300*n_pad_x[canvas-1];
	canvas_height[n_canvas-1] = 200*n_pad_y[canvas-1];

	Char_t *dist[][16] = {	{"jet1.pt","jet1.E","jet1.y","jet2.pt","jet2.E","jet2.y","jet3.pt","jet3.E","jet3.y","jet4.pt","jet4.E","jet4.y","jet5.pt","jet5.E","jet5.y"},
							{/*"jet_hard.pt","jet_hard.E","jet_hard.y",*/"jet_forward.pt","jet_forward.E","jet_forward.y"},
							{"lepton_highpt.pt","lepton_lowpt.pt","event.pt_miss"},
							{"lepton_highpt.y","lepton_lowpt.y"},
							{"lepton_misid.pt","lepton_misid.y"},
							//{"top1.pt","top1.y","top2.pt","top2.y"},
							//{"event.d_pt_ll","event.d_y_ll","event.pt_miss","event.cos_phi_ll"},
							{"event.m_ll","event.H_t"}/*,
							{"jjj_highpt.Mjjj","jj_highpt.Mjj"}	*/};
												
	Int_t n_bins[][16]  = {	{50,60,40,50,60,40,50,60,40,50,60,40,50,60,40},
							{/*50,60,40,*/50,60,40},
							{50,50,50},
							{40,40},
							{50,40},
							//{50,40,50,40},
							//{50,50,50,50},
							{100,60}/*,
							{100,100}*/	};

	Float_t x_min[][16] = {	{0,0,-10,0,0,-10,0,0,-10,0,0,-10,0,0,-10},
							{/*0,0,-10,*/0,0,-10},
							{0,0,0},
							{-10,-10},
							{0,-10},
							//{0,-10,0,-10},
							//{0,0,0,-1},
							{0,0}/*,
							{0,0}	*/};
											
	Float_t x_max[][16] = {	{500,3000,10,500,3000,10,500,3000,10,500,3000,10,500,3000,10},
							{/*500,3000,10,*/500,3000,10},
							{500,500,500},
							{10,10},
							{500,10},
							//{500,10,500,10},
							//{1000,10,500,1},
							{1000,3000}/*,
							{1000,1000}	*/};
																								
	Char_t *title[][16] = {	{"p_{T} of Jet 1","Energy of Jet 1","Rapidity of Jet 1","p_{T} of Jet 2","Energy of Jet 2","Rapidity of Jet 2","p_{T} of Jet 3","Energy of Jet 3","Rapidity of Jet 3","p_{T} of Jet 4","Energy of Jet 4","Rapidity of Jet 4","p_{T} of Jet 5","Energy of Jet 5","Rapidity of Jet 5"},
							{/*"p_{T} of Hardest Jet","Energy of Hardest Jet","Rapidity of Hardest Jet",*/"p_{T} of Most Forward Jet","Energy of Most Forward Jet","Rapidity of Most Forward Jet"},
							{"p_{T} of Hardest Lepton","p_{T} of Second Hardest Lepton","Missing p_{T}"},
							{"Rapidity of Hardest Lepton","Rapidity of Second Hardest Lepton"},
							{"p_{T} of Misidentified Lepton","Rapidity of Misidentified Lepton"},
							//{"p_{T} of Top 1","Rapidity of Top 1","p_{T} of Top 2","Rapidity of Top 2"},
							//{"#Deltap_{T}(ll)","#Deltay(ll)","Missing p_{T}","cos#phi_{ll}"},
							{"M_{ll}","H_{T}"}/*,
							{"M_{jjj} of Highest p_{T} jjj","M_{jj} of Highest p_{T} jj within Highest p_{T} jjj"}	*/};			
								
	Char_t *x_title[][16] = {	{"p_{T} (GeV)","E (GeV)","y","p_{T} (GeV)","E (GeV)","y","p_{T} (GeV)","E (GeV)","y","p_{T} (GeV)","E (GeV)","y","p_{T} (GeV)","E (GeV)","y"},
								{/*"p_{T} (GeV)","E (GeV)","y",*/"p_{T} (GeV)","E (GeV)","y"},
								{"p_{T} (GeV)","p_{T} (GeV)"},
								{"y","y"},
								{"p_{T} (GeV)", "y"},
								//{"p_{T} (GeV)", "y","p_{T} (GeV)", "y"},
								//{"#Deltap_{T}(ll) (GeV)","#Deltay(ll) (GeV)","p_{T}^{miss} (GeV)","cos#phi_{ll}"},
								{"M_{ll} (GeV)","H_{T} (GeV)"}/*,
								{"M_{jjj} (GeV)","M_{jj} (GeV)"}*/	};												
	
	Char_t *y_title[][16] = {	{"Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)","Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)","Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)"},
								{/*"Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)", */"Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)"},
								{"Cross Section (fb/bin)", "Cross Section (fb/bin)"},
								{"Cross Section (fb/bin)", "Cross Section (fb/bin)"},
								{"Cross Section (fb/bin)", "Cross Section (fb/bin)"},
								//{"Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)","Cross Section (fb/bin)"},
								//{"Cross Section (fb/bin)", "Cross Section (fb/bin)", "Cross Section (fb/bin)","Cross Section (fb/bin)"},
								{"Cross Section (fb/bin)", "Cross Section (fb/bin)"}/*,
								{"Cross Section (fb/bin)","Cross Section (fb/bin)"}	*/};
	
//***************************************************
//	for(canvas = n_canvas-1; canvas < n_canvas; canvas++)
	for(canvas = 0; canvas < n_canvas; canvas++)
	{
		TCanvas *C = new TCanvas(canvas_title[canvas],canvas_title[canvas],canvas_width[canvas],canvas_height[canvas]);
		C->Divide(n_pad_x[canvas],n_pad_y[canvas]);

		for(pad = 0; pad < n_pads[canvas]; pad++)
		{
			C->cd(pad+1);
		
			gPad->SetLogy();
		
			legend->Clear();

			TH1F* back = new TH1F(Form("back%d%d",canvas,pad),Form("back%d%d",canvas,pad),n_bins[canvas][pad],x_min[canvas][pad],x_max[canvas][pad]);
			back->SetLineColor(kBlack);	

			for(tree = 1; tree<=n_trees; tree++)
			{
				hist[tree] = new TH1F(Form("hist[%d]",tree),Form("hist[%d]",tree),n_bins[canvas][pad],x_min[canvas][pad],x_max[canvas][pad]);
				if(tree < 9)
				{
					hist[tree]->SetLineColor(tree+1);
				}
				if(tree >= 9)
				{
					hist[tree]->SetLineColor(tree+2);
				}

				if(tree == 1)
				{
					Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.weight*%f",(double)(luminosity/n_groups[tree-1])/*/x_sect_ljh[tree]*/) * cuts);
				}
				if(tree > 1)
				{
					Tree[tree]->Draw(Form("%s >> hist[%d]",dist[canvas][pad],tree),Form("1000*event.weight*%f",(double)(luminosity/n_groups[tree-1])/*/x_sect_ljh[tree]*/) * cuts, "same");
				}
			
				if(tree > 1 && tree <= n_back)
				{
					back->Add(hist[tree],1);				
				}
			
				legend->AddEntry(hist[tree],label[tree-1],"l");
				legend->Draw();
	
				hist[tree]->Clear();
				hist[tree]->GetXaxis()->SetTitle(x_title[canvas][pad]);
				hist[tree]->GetYaxis()->SetTitle(y_title[canvas][pad]);
				hist[tree]->SetTitle(title[canvas][pad]);
			}
		
			gPad->RedrawAxis();
		
			back->Draw("same");
			legend->AddEntry(Form("back%d%d",canvas,pad),"Total Bkgd.","l");
		
			legend->Draw();
		}
	}
}