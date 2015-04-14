int misidlepton(void)
{
	if(tree == tree_wz)
	{
		if(fabs(particle[i].y) > 2.5)
		{
			findneutrino();
		}
		
		else
		{
			event.veto = 1;
		}
	}
	
	else 
	{
		if(tree == tree_ttbar || tree == tree_za || tree == tree_wpwm)
		{
			
			if(particle[i].pt >= lepton_pt_min && fabs(particle[i].y) <= lepton_y_max)
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
				
				eff_chargemisid(lepton_misid.id,lepton_misid.pt);
				
				event.weight = event.weight*eff_chargemisid;
			}
			else
			{
				findneutrino();
			}
		}
	}	
	
	return 0;
}