{
    TH1F *h1 = new TH1F("h1","h1", 100, -5,5);
    TH1F *h2 = new TH1F("h2","h1", 100, -5,5);

    Double_t g,r,w;
    for (int i=0; i<10000; i++) {
	g = (gRandom->Gaus(0,1));
	r = (gRandom->Uniform(1));
	w = (r>0.80) ? -1 : 1;
	h1->Fill(g);
	h2->Fill(g, w);
    }
    h1->Draw();
    h2->Draw("SAME");


}
