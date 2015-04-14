int findlepton(void)
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
	}
	
	else
	{		
		findneutrino();
	}	
	
	return 0;
}