import ROOT

def create_canvas_with_subpads():
    canvas = ROOT.TCanvas("canvas", "Systematics with Error", 800, 800)
    canvas.Divide(1, 2)  # 1 column, 2 rows

    # Get the pads
    pad1 = canvas.cd(1)
    pad1.SetPad(0, 0.35, 1, 1)  # Upper pad, larger (35% of canvas height for lower pad)

    pad2 = canvas.cd(2)
    pad2.SetPad(0, 0, 1, 0.35)  # Lower pad, smaller

    return canvas, pad1, pad2

def create_upper_pad_histogram():
    # Create and return a histogram for the upper pad
    hist = ROOT.TH1F("hist", "Histogram on Upper Pad", 100, -5, 5)
    hist.FillRandom("gaus", 1000)
    return hist

def create_lower_pad_graph():
    # Create and return a graph for the lower pad
    graph = ROOT.TGraph()
    for i in range(10):
        graph.SetPoint(i, i, i*i)
    graph.SetMarkerStyle(ROOT.kFullCircle)
    return graph

# Usage
canvas, pad1, pad2 = create_canvas_with_subpads()

# Get drawable objects
hist = create_upper_pad_histogram()
graph = create_lower_pad_graph()

# Draw objects on the pads
pad1.cd()
hist.Draw()
pad1.Update()

pad2.cd()
graph.Draw("AP")
pad2.Update()

# Ensure that the canvas is updated and all pads are drawn
canvas.Update()
canvas.Draw()  # Ensure the canvas is drawn in the ROOT GUI

# Optionally save the canvas in different formats to check
canvas.SaveAs("canvas_with_subpads.png")

print("Canvas saved as PNG and ROOT files. Check for visibility issues.")

