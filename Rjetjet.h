int Rjetjet(void)
{
	if(j >= 2)
	{
		for(Int_t m=1; m<=j; m++)
		{	
			for(Int_t k=m+1; k<=j; k++)
			{	
				dy = fabs(jet[m].y - jet[k].y);
				denom = sqrt(pow(jet[m].p1,2)+pow(jet[k].p1,2))*sqrt(pow(jet[m].p2,2)+pow(jet[k].p2,2));
				dphi = acos((jet[m].p1*jet[m].p2+jet[k].p1*jet[k].p2)/denom);
				
				Rjj[m][k].R = sqrt(dy*dy + dphi*dphi);
				
				if(Rjj[m][k].R < 0.4)
				{
					event.veto=1;		// Work on merging jets rather than vetoing the event
					continue;
				}
			}
		}
	}	

	return 0;
}