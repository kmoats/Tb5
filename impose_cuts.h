int impose_cuts(void)
{
	//acceptance cuts
	if(event.veto == 0)
	{								
		x_sect_a[tree] = x_sect_a[tree] + (double)1000*event.weight/n_groups[tree-1];
		asym_a[tree] = asym_a[tree] + (double)1000*event.signed_weight/n_groups[tree-1];
		
		
		//leptonic cuts
		if(lepton_highpt.pt > lepton_highpt_min && lepton_lowpt.pt > lepton_lowpt_min && event.pt_miss > pt_miss_min && event.m_ll > m_ll_min)
		{
			x_sect_l[tree] = x_sect_l[tree] + (double)1000*event.weight/n_groups[tree-1];
			asym_l[tree] = asym_l[tree] + (double)1000*event.signed_weight/n_groups[tree-1];
			
			//
			//jet cuts
			if(fabs(jet_forward.y) > jet_forward_y_min && fabs(jet_forward.y) < jet_forward_y_max && jet_forward.pt > jet_forward_pt_min && jet_forward.E > jet_forward_E_min)
			{					
				//			if(fabs(jet_hard.y) > jet_hard_y_min && fabs(jet_hard.y) < jet_hard_y_max && jet_hard.pt > jet_hard_pt_min && jet_hard.E > jet_hard_E_min)
				//			{
				//				if(jjj_highpt.Mjjj > Mjjj_min && jjj_highpt.Mjjj < Mjjj_max && jj_highpt.Mjj > Mjj_min && jj_highpt.Mjj < Mjj_max)
				//				{
				x_sect_lj[tree] = x_sect_lj[tree] + (double)1000*event.weight/n_groups[tree-1];
				asym_lj[tree] = asym_lj[tree] + (double)1000*event.signed_weight/n_groups[tree-1];

				//				
				// H_t cut
				if(event.H_t > Ht_min)
				{
					x_sect_ljh[tree] = x_sect_ljh[tree] + (double)1000*event.weight/n_groups[tree-1];
					asym_ljh[tree] = asym_ljh[tree] + (double)1000*event.signed_weight/n_groups[tree-1];
				}
				//	}
				//}
			}
		}
		
		
		
		
		//leptonic cuts
		if(lepton_highpt.pt > lepton_highpt_min_600 && lepton_lowpt.pt > lepton_lowpt_min_600 && event.pt_miss > pt_miss_min_600 && event.m_ll > m_ll_min_600)
		{
			x_sect_l_600[tree] = x_sect_l_600[tree] + (double)1000*event.weight/n_groups[tree-1];
			asym_l_600[tree] = asym_l_600[tree] + (double)1000*event.signed_weight/n_groups[tree-1];
			
			//
			//jet cuts
			if(fabs(jet_forward.y) > jet_forward_y_min && fabs(jet_forward.y) < jet_forward_y_max && jet_forward.pt > jet_forward_pt_min && jet_forward.E > jet_forward_E_min)
			{					
				//			if(fabs(jet_hard.y) > jet_hard_y_min && fabs(jet_hard.y) < jet_hard_y_max && jet_hard.pt > jet_hard_pt_min && jet_hard.E > jet_hard_E_min)
				//			{
				//				if(jjj_highpt.Mjjj > Mjjj_min && jjj_highpt.Mjjj < Mjjj_max && jj_highpt.Mjj > Mjj_min && jj_highpt.Mjj < Mjj_max)
				//				{
				x_sect_lj_600[tree] = x_sect_lj_600[tree] + (double)1000*event.weight/n_groups[tree-1];
				asym_lj_600[tree] = asym_lj_600[tree] + (double)1000*event.signed_weight/n_groups[tree-1];
				
				//				
				// H_t cut
				if(event.H_t > Ht_min_600)
				{
					x_sect_ljh_600[tree] = x_sect_ljh_600[tree] + (double)1000*event.weight/n_groups[tree-1];
					asym_ljh_600[tree] = asym_ljh_600[tree] + (double)1000*event.signed_weight/n_groups[tree-1];
				}
				//	}
				//}
			}
		
		}
		
		
		
		//leptonic cuts
		if(lepton_highpt.pt > lepton_highpt_min_800 && lepton_lowpt.pt > lepton_lowpt_min_800 && event.pt_miss > pt_miss_min_800 && event.m_ll > m_ll_min_800)
		{
			x_sect_l_800[tree] = x_sect_l_800[tree] + (double)1000*event.weight/n_groups[tree-1];
			asym_l_800[tree] = asym_l_800[tree] + (double)1000*event.signed_weight/n_groups[tree-1];
			
			//
			//jet cuts
			if(fabs(jet_forward.y) > jet_forward_y_min && fabs(jet_forward.y) < jet_forward_y_max && jet_forward.pt > jet_forward_pt_min && jet_forward.E > jet_forward_E_min)
			{					
				//			if(fabs(jet_hard.y) > jet_hard_y_min && fabs(jet_hard.y) < jet_hard_y_max && jet_hard.pt > jet_hard_pt_min && jet_hard.E > jet_hard_E_min)
				//			{
				//				if(jjj_highpt.Mjjj > Mjjj_min && jjj_highpt.Mjjj < Mjjj_max && jj_highpt.Mjj > Mjj_min && jj_highpt.Mjj < Mjj_max)
				//				{
				x_sect_lj_800[tree] = x_sect_lj_800[tree] + (double)1000*event.weight/n_groups[tree-1];
				asym_lj_800[tree] = asym_lj_800[tree] + (double)1000*event.signed_weight/n_groups[tree-1];
				
				//				
				// H_t cut
				if(event.H_t > Ht_min_800)
				{
					x_sect_ljh_800[tree] = x_sect_ljh_800[tree] + (double)1000*event.weight/n_groups[tree-1];
					asym_ljh_800[tree] = asym_ljh_800[tree] + (double)1000*event.signed_weight/n_groups[tree-1];
				}
				//	}
				//}
			}
		}
		
		
		
		
	}
	
	return 0;
}