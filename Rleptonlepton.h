int Rleptonlepton(void)
{
	if(l >= 2)
	{
		for(Int_t m=1; m<=l; m++)
		{	
			for(Int_t k=m+1; k<=l; k++)
			{	
				dy = fabs(lepton[m].y - lepton[k].y);
				denom = sqrt(pow(lepton[m].p1,2)+pow(lepton[k].p1,2))*sqrt(pow(lepton[m].p2,2)+pow(lepton[k].p2,2));
				dphi = acos((lepton[m].p1*lepton[m].p2+lepton[k].p1*lepton[k].p2)/denom);
				
				Rll[m][k].R = sqrt(dy*dy + dphi*dphi);
				
				if(Rll[m][k].R < 0.4)
				{
					event.veto=1;
					continue;
				}
			}
		}
	}
	
	return 0;
}