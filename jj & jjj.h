
/*
 struct jj_t
 {
 Float_t Mjj;
 Float_t ptjj;
 };
 jj_t jj[n_jets+1][n_jets+1];
 jj_t jj_highpt;
 
 struct jjj_t
 {
 Float_t Mjjj;
 Float_t ptminusMjjj;
 Float_t ptjjj;
 };
 jjj_t jjj[n_jets+1][n_jets+1][n_jets+1];
 jjj_t jjj_highpt;
 */


/*
 Float_t Mjjj_min = 0;
 Float_t Mjjj_max = 100000;
 
 TCut jjj_cut_1 = Form("jjj_highpt.Mjjj > %f", Mjjj_min);
 TCut jjj_cut_2 = Form("jjj_highpt.Mjjj < %f", Mjjj_max);
 
 TCut jjj_cuts = jjj_cut_1 && jjj_cut_2;
 
 Float_t Mjj_min = 0;
 Float_t Mjj_max = 100000;
 
 TCut jj_cut_1 = Form("jj_highpt.Mjj > %f", Mjj_min);
 TCut jj_cut_2 = Form("jj_highpt.Mjj < %f", Mjj_max);
 
 TCut jj_cuts = jj_cut_1 && jj_cut_2;
 */


/*
 // create branches for each 2-jet combination
 for(Int_t j=1; j<=n_jets; j++)
 {	
 for(Int_t k=j+1; k<=n_jets; k++)
 {	
 TBranch *branch = Tree[tree]->Branch(Form("jj%d%d",j,k),&jj[j][k].Mjj,"Mjj/F:ptjj:F");
 }
 }
 
 TBranch *branch = Tree[tree]->Branch("jj_highpt",&jj_highpt.Mjj,"Mjj/F:ptjj/F");
 
 // create branches for each 3-jet combination
 for(Int_t j=1; j<=n_jets; j++)
 {	
 for(Int_t k=j+1; k<=n_jets; k++)
 {	
 for(Int_t l=k+1; l<=n_jets; l++)
 {
 TBranch *branch = Tree[tree]->Branch(Form("jjj%d%d%d",j,k,l),&jjj[j][k][l].Mjjj,"Mjjj/F:ptminusMjjj/F:ptjjj/F");
 }
 }
 }
 
 TBranch *branch = Tree[tree]->Branch("jjj_highpt",&jjj_highpt.Mjjj,"Mjjj/F:ptminusMjjj/F:ptjjj/F");
 */


//		jjj_highpt.ptjjj=0;
//		jj_highpt.ptjj=0;


/*
 // calculate the invariant mass of each 2-jet combination
 for(Int_t m=1; m<=j; m++)
 {	
 for(Int_t k=m+1; k<=j; k++)
 {	
 Float_t	sum_p1 = jet[m].p1+jet[k].p1;
 Float_t	sum_p2 = jet[m].p2+jet[k].p2;
 Float_t	sum_p3 = jet[m].p3+jet[k].p3;
 Float_t	sum_p4 = jet[m].p4+jet[k].p4;
 
 jj[m][k].Mjj = sqrt(pow(sum_p4,2)-pow(sum_p3,2)-pow(sum_p2,2)-pow(sum_p1,2));
 jj[m][k].ptjj = sqrt(pow(jet[m].p1+jet[k].p1,2) + pow(jet[m].p2+jet[k].p2,2));
 }
 }
 
 // calculate the invariant mass of each 3-jet combination
 for(Int_t m=1; m<=j; m++)
 {	
 for(Int_t k=m+1; k<=j; k++)
 {
 for(Int_t o=k+1; o<=j; o++)
 {
 Float_t	sum_p1 = jet[m].p1+jet[k].p1+jet[o].p1;
 Float_t	sum_p2 = jet[m].p2+jet[k].p2+jet[o].p2;
 Float_t	sum_p3 = jet[m].p3+jet[k].p3+jet[o].p3;
 Float_t	sum_p4 = jet[m].p4+jet[k].p4+jet[o].p4;
 
 jjj[m][k][o].Mjjj = sqrt(pow(sum_p4,2)-pow(sum_p3,2)-pow(sum_p2,2)-pow(sum_p1,2));
 jjj[m][k][o].ptminusMjjj = jet[m].pt+jet[k].pt+jet[o].pt - jjj[m][k][o].Mjjj;
 jjj[m][k][o].ptjjj = sqrt(pow(jet[m].p1+jet[k].p1+jet[o].p1,2) + pow(jet[m].p2+jet[k].p2+jet[o].p2,2));
 
 if(jjj[m][k][o].ptjjj > jjj_highpt.ptjjj)
 {
 jjj_highpt = jjj[m][k][o];
 
 if(jj[m][k].ptjj > jj_highpt.ptjj)
 {
 jj_highpt = jj[m][k];
 }
 if(jj[m][o].ptjj > jj_highpt.ptjj)
 {
 jj_highpt = jj[m][o];
 }
 if(jj[k][o].ptjj > jj_highpt.ptjj)
 {
 jj_highpt = jj[k][o];
 }
 }
 }
 }
 }
 */
