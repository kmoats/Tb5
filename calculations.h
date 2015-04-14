int calculations(void)
{
	// Calculate d_pt_ll, d_y_ll, m_t, etc...				
	//	event.d_pt_ll = sqrt(pow(lepton[1].p1-lepton[2].p1,2) + pow(lepton[1].p2-lepton[2].p2,2));
	//	event.d_y_ll = fabs(lepton[1].y - lepton[2].y);
	
	//	event.cos_phi_ll = (lepton[1].p1-lepton[2].p1)/event.d_pt_ll;
	
	sum_p1_leptons = lepton[1].p1+lepton[2].p1;
	sum_p2_leptons = lepton[1].p2+lepton[2].p2;
	sum_p3_leptons = lepton[1].p3+lepton[2].p3;				
	sum_p4_leptons = lepton[1].p4+lepton[2].p4;
	
	sum_p1_neutrinos = 0;
	sum_p2_neutrinos = 0;
	sum_p3_neutrinos = 0;				
	sum_p4_neutrinos = 0;
	
	for(Int_t m=1; m<=n; m++)
	{
		sum_p1_neutrinos = sum_p1_neutrinos + neutrino[m].p1;
		sum_p2_neutrinos = sum_p2_neutrinos + neutrino[m].p2;
		sum_p3_neutrinos = sum_p3_neutrinos + neutrino[m].p3;				
		sum_p4_neutrinos = sum_p4_neutrinos + neutrino[m].p4;			
	}
	
	sum_p1 = sum_p1_leptons+sum_p1_neutrinos;
	sum_p2 = sum_p2_leptons+sum_p2_neutrinos;
	sum_p3 = sum_p3_leptons+sum_p3_neutrinos;
	sum_p4 = sum_p4_leptons+sum_p4_neutrinos;
	
	pt_tot = sqrt(pow(sum_p1,2) + pow(sum_p2,2));
	
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
	

	return 0;
}