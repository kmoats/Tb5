int findjet(void)
{
	if(particle[i].pt >= jet_pt_min && fabs(particle[i].y) <= jet_y_max)
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
		findneutrino();
	}

	return 0;
}