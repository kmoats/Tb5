int eventfiles(void)
{
	//******************** DATA & EVENT FILES ********************
	
	
	Int_t tree;
	Int_t file;
	Int_t n_trees = 2;  // Set # of trees (= # of histograms to superimpose on each plot)
	Int_t n_files[] = {2,2,2,2,2,2,2,2,2,2};
	Int_t n_groups[] = {1,1,1,1,1,1,1,1,1,1};
	//	Int_t n_files[] = {2,4,82,8,100,100,/*30,*/100,100,20};
	//	Int_t n_groups[] = {1,2,41,4,50,50,/*15,*/50,50,10};
	Int_t n_back = 1;
	
	Int_t tree_ttbar = 9;	//Identify the tree corresponding to the ttbar + jets background (for charge misidentifcation purposes)
	Int_t tree_za = 1;	//Identify the tree corresponding to the z/gamma + jets background (for charge misidentifcation purposes)
	Int_t tree_wpwm = 3;	//Identify the tree corresponding to the w+w- + jets background (for charge misidentifcation purposes)
	Int_t tree_wz = 5;	//Identify the tree corresponding to the wz + jets background (for charge misidentifcation purposes)
	Int_t tree_wjets = 8;	//Identify the tree corresponding to the w + jets background (for jet faking lepton)
	Int_t tree_ttbarw = 7;
	
	
	Char_t *label[] =	{	"Z/#gamma + j", 
		"t#bar{t}W^{+}W^{-} + j", 
		"W^{+}W^{-} + j", 
		"W^{#pm}W^{#pm}W^{#mp} + j", 
		"W^{#pm}Z + j", 
		"W^{#pm}W^{#pm} + j",
		"t#bar{t}W^{#pm} + j", 
		"W^{#pm} + j", 
		"t#bar{t} + j", 
		"Signal"
	};
	
	Char_t *process[][2] =	{	{"pp_zajets_++","pp_zajets_--"},
		{"pp_tt~w+w-jets_++","pp_tt~w+w-jets_--"},
		{"pp_w+w-jets_++","pp_w+w-jets_--"},
		{"pp_w+w+w-jets_++","pp_w-w-w+jets_--"},
		{"pp_w+zjets_++","pp_w-zjets_--"},
		{"pp_w+w+jets_++","pp_w-w-jets_--"},
		{"pp_tt~w+jets_++","pp_tt~w-jets_--"},
		{"pp_w+jets_++","pp_w-jets_--"},
		{"pp_tt~jets_++","pp_tt~jets_--"},
		{"pp_Tb5t~j_++","pp_Tb5~tj_--"}
	};
	
	Char_t *model;
	
	Char_t *dat_file[n_trees][100];
	Char_t *lhe_file[n_trees][100];
	
	for(Int_t tree=1; tree<=n_trees; tree++)
	{
		if(tree==10)
		{
			model = "f700ni";
		}
		else
		{
			model = "sm";
		}
		
		for(Int_t group=1; group<=n_groups[tree-1]; group++)
		{
			{
				dat_file[tree-1][2*group-2] = Form("SubProcesses/%s/%s_%s_14TeV_100000_%d_results.dat",process[tree-1][0],process[tree-1][0],model,group);
				dat_file[tree-1][2*group-1] = Form("SubProcesses/%s/%s_%s_14TeV_100000_%d_results.dat",process[tree-1][1],process[tree-1][1],model,group);
				cout << dat_file[tree-1][2*group-2] << endl;
				
				lhe_file[tree-1][2*group-2] = Form("Events/%s/%s_%s_14TeV_100000_%d_events.lhe",process[tree-1][0],process[tree-1][0],model,group);
				lhe_file[tree-1][2*group-1] = Form("Events/%s/%s_%s_14TeV_100000_%d_events.lhe",process[tree-1][1],process[tree-1][1],model,group);
			}
		}
	}
	

	return 0;
}