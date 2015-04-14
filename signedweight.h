int signedweight(void)
{
	if(file % 2 == 1)
	{
		event.signed_weight = event.weight;
	}
	else if(file % 2 == 0)
	{
		event.signed_weight = -event.weight;
	}
	
	return 0;
}