float eff_chargemisid(int,float)
{
	if(abs(lepton_misid.id) == 11) // if electron
	{
		if(lepton_misid.pt >= 0 && lepton_misid.pt < 100)
		{
			eff_chargemisid = 0.0008;
		}
		else if(lepton_misid.pt >= 100 && lepton_misid.pt < 300)
		{
			eff_chargemisid = 0.006;
		}
		else if(lepton_misid.pt >= 300 && lepton_misid.pt < 500)
		{
			eff_chargemisid = 0.02;
		}
		else if(lepton_misid.pt >= 500 && lepton_misid.pt < 1000)
		{
			eff_chargemisid = 0.06;
		}
		else //if(lepton_misid.pt >= 1000)
		{
			eff_chargemisid = 0.2;
		}
	}
	
	else if(abs(lepton_misid.id) == 13) // if muon
	{
		if(lepton_misid.pt >= 0 && lepton_misid.pt < 100)
		{
			eff_chargemisid = 0.0004;
		}
		else if(lepton_misid.pt >= 100 && lepton_misid.pt < 300)
		{
			eff_chargemisid = 0.002;
		}
		else if(lepton_misid.pt >= 300 && lepton_misid.pt < 500)
		{
			eff_chargemisid = 0.01;
		}
		else if(lepton_misid.pt >= 500 && lepton_misid.pt < 1000)
		{
			eff_chargemisid = 0.05;
		}
		else //if(lepton_misid.pt >= 1000)
		{
			eff_chargemisid = 0.2;
		}
	}
	
	return eff_chargemisid;
}