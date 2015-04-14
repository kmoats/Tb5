int Rjetlepton(void)
{
	if(j>=1  && l>=1)
	{
		for(Int_t m=1; m<=j; m++)
		{	
			for(Int_t k=1; k<=l; k++)
			{	
				dy = fabs(jet[m].y - lepton[k].y);
				denom = sqrt(pow(jet[m].p1,2)+pow(lepton[k].p1,2))*sqrt(pow(jet[m].p2,2)+pow(lepton[k].p2,2));
				dphi = acos((jet[m].p1*jet[m].p2+lepton[k].p1*lepton[k].p2)/denom);
				
				Rjl[m][k].R = sqrt(dy*dy + dphi*dphi);	
				
				if(Rjl[m][k].R < 0.4)
				{
					//		event.weight=0;
					event.veto=1;
					continue;
				}
			}
		}
	}	

	return 0;
}