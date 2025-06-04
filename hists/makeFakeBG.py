import ROOT

data = ROOT.TFile.Open("Data.root")
fFake = ROOT.TFile.Open("Fake.root", "RECREATE")
fFake.cd()

signals = {
   "LLpp"       :       ["LFpp", "FFpp"],
   "LLmm"       :       ["LFmm", "FFmm"],
   "3LonZ"      :       ["LLFonZ", "LFFonZ", "FFFonZ"],
   "3LoffZp"    :       ["LLFoffZp", "LFFoffZp", "FFFoffZp"], 
   "3LoffZm"    :       ["LLFoffZm", "LFFoffZm", "FFFoffZm"],
}

regions = ["0Bjet", "1Bjet", "G1Bjet", "All"]

variables = {
      "lep1Pt"   :    [0,     60,     0,      300     ],
      "lep1Eta"  :    [1,     20,    -3,      3       ],
      "lep1Phi"  :    [2,     25,    -4,      4       ],
      "lep2Pt"   :    [3,     25,     0,      1000    ],
      "lep2Eta"  :    [4,     20,    -3,      3       ],
      "lep2Phi"  :    [5,     25,    -4,      4       ],
      "llM"      :    [6,     30,     0,      500     ],
      "llPt"     :    [7,     20,     0,      200     ],
      "llDr"     :    [8,     25,     0,      7       ],
      "llDphi"   :    [9,     15,     0,      4       ],
      "jet1Pt"   :    [10,    20,     0,      300     ],
      "jet1Eta"  :    [11,    20,    -3,      3       ],
      "jet1Phi"  :    [12,    25,    -4,      4       ],
      "njet"     :    [13,    10,     0,      10      ],
      "nbjet"    :    [14,     6,     0,      6       ],
      "Met"      :    [15,    30,     0,      210     ],
      "MetPhi"   :    [16,    20,    -4,      4       ],
      "nVtx"     :    [17,    70,     0,      70      ],
      "llMZw"    :    [18,    80,    70,      110     ],
      "lep1Pt200":    [19,    60,     0,      200     ],
      "eTC"      :    [20,     3,     0,      3       ],
}



for signal, fakeReg in signals.items():
   for reg in regions:
      for var, value in variables.items():
         name = signal+"_"+reg+"_"+var
         h = ROOT.TH1F(name, name, value[1], value[2], value[3])
         for n, freg in enumerate(fakeReg):
            name = freg+"_"+reg+"_"+var
            h.Add(data.Get(name), (-1)**n)
         h.Write()



