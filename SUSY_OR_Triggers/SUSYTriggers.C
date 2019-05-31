#include "../CommonLibraries.C"

using namespace std;

void SUSYTriggers() {

    //--- load files
    string ntuple_path = "/eos/atlas/atlascerngroupdisk/phys-susy/2L2J-ANA-SUSY-2018-05/SusySkim2LJets/v1.6/SUSY2/SUSY2_Data/";
    string data_filename = ntuple_path + "data15-16_merged_processed.root";
    TChain* tch_data = new TChain("data"); tch_data->Add(data_filename.c_str());

    //-----------------------------
    // RunNumber histograms
    //-----------------------------

    TH1F *h_ee_pass_1L2L, *h_mm_pass_1L2L, *h_em_pass_1L2L, *h_ee_pass_1L2LOR, *h_mm_pass_1L2LOR, *h_em_pass_1L2LOR;
    int n_bins = 100;
    int low_run_number = 275000;
    int high_run_number = 315000;
    h_ee_pass_1L2L = new TH1F("h_ee_pass_1L2L", "", n_bins, low_run_number, high_run_number);
    h_mm_pass_1L2L = new TH1F("h_mm_pass_1L2L", "", n_bins, low_run_number, high_run_number);
    h_em_pass_1L2L = new TH1F("h_em_pass_1L2L", "", n_bins, low_run_number, high_run_number);
    h_ee_pass_1L2LOR = new TH1F("h_ee_pass_1L2LOR", "", n_bins, low_run_number, high_run_number);
    h_mm_pass_1L2LOR = new TH1F("h_mm_pass_1L2LOR", "", n_bins, low_run_number, high_run_number);
    h_em_pass_1L2LOR = new TH1F("h_em_pass_1L2LOR", "", n_bins, low_run_number, high_run_number);

    tch_data->Draw("RunNumber>>h_ee_pass_1L2L", "trigMatch_1L2LTrig && lepFlavor[0]==1 && lepFlavor[1]==1", "goff");
    tch_data->Draw("RunNumber>>h_mm_pass_1L2L", "trigMatch_1L2LTrig && lepFlavor[0]==2 && lepFlavor[1]==2", "goff");
    tch_data->Draw("RunNumber>>h_em_pass_1L2L", "trigMatch_1L2LTrig && ((lepFlavor[0]==2 && lepFlavor[1]==1) || (lepFlavor[0]==1 && lepFlavor[1]==2))", "goff");
    tch_data->Draw("RunNumber>>h_ee_pass_1L2LOR", "trigMatch_1L2LTrigOR && lepFlavor[0]==1 && lepFlavor[1]==1", "goff");
    tch_data->Draw("RunNumber>>h_mm_pass_1L2LOR", "trigMatch_1L2LTrigOR && lepFlavor[0]==2 && lepFlavor[1]==2", "goff");
    tch_data->Draw("RunNumber>>h_em_pass_1L2LOR", "trigMatch_1L2LTrigOR && ((lepFlavor[0]==2 && lepFlavor[1]==1) || (lepFlavor[0]==1 && lepFlavor[1]==2))", "goff");

    //--- ee
    TCanvas *can = new TCanvas("can","can",600,600);
    can->cd();

    h_ee_pass_1L2LOR->SetStats(0);
    h_ee_pass_1L2LOR->SetTitle("ee");
    h_ee_pass_1L2LOR->GetXaxis()->SetTitle("RunNumber");

    h_ee_pass_1L2LOR->Draw("hist");
    h_ee_pass_1L2L->SetLineColor(2);
    h_ee_pass_1L2L->Draw("samehist");

    TLegend* leg = new TLegend(0.2,0.7,0.48,0.88);
    leg->AddEntry(h_ee_pass_1L2L, "1L2L", "lp");
    leg->AddEntry(h_ee_pass_1L2LOR, "1L2LOR", "lp");

    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->Draw();

    can->Print("RunNumber_ee.pdf");

    //--- mm
    can = new TCanvas("can","can",600,600);
    can->cd();

    h_mm_pass_1L2LOR->SetStats(0);
    h_mm_pass_1L2LOR->SetTitle("mm");
    h_mm_pass_1L2LOR->GetXaxis()->SetTitle("RunNumber");

    h_mm_pass_1L2LOR->Draw("hist");
    h_mm_pass_1L2L->SetLineColor(2);
    h_mm_pass_1L2L->Draw("samehist");

    leg = new TLegend(0.2,0.7,0.48,0.88);
    leg->AddEntry(h_mm_pass_1L2L, "1L2L", "lp");
    leg->AddEntry(h_mm_pass_1L2LOR, "1L2LOR", "lp");

    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->Draw();

    can->Print("RunNumber_mm.pdf");

    //--- em
    can = new TCanvas("can","can",600,600);
    can->cd();

    h_em_pass_1L2LOR->SetStats(0);
    h_em_pass_1L2LOR->SetTitle("em");
    h_em_pass_1L2LOR->GetXaxis()->SetTitle("RunNumber");

    h_em_pass_1L2LOR->Draw("hist");
    h_em_pass_1L2L->SetLineColor(2);
    h_em_pass_1L2L->Draw("samehist");

    leg = new TLegend(0.2,0.7,0.48,0.88);
    leg->AddEntry(h_em_pass_1L2L, "1L2L", "lp");
    leg->AddEntry(h_em_pass_1L2LOR, "1L2LOR", "lp");

    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->Draw();

    can->Print("RunNumber_em.pdf");

    //-----------------------------
    // RunNumber vs. pT scatter
    //-----------------------------

    //--- ee
    can = new TCanvas("can","can",600,600);
    can->cd();

    tch_data->SetMarkerStyle(3);
    tch_data->SetMarkerColor(3);
    tch_data->Draw("RunNumber:lepPt[0]", "trigMatch_1L2LTrigOR && !trigMatch_1L2LTrig && lepFlavor[0]==1 && lepFlavor[1]==1");
    tch_data->SetMarkerColor(4);
    tch_data->GetHistogram()->SetTitle("ee");
    tch_data->GetHistogram()->GetXaxis()->SetTitle("");
    tch_data->Draw("RunNumber:lepPt[1]", "trigMatch_1L2LTrigOR && !trigMatch_1L2LTrig && lepFlavor[0]==1 && lepFlavor[1]==1", "same");

    can->Print("scatter_ee.png");

    //--- mm
    can = new TCanvas("can","can",600,600);
    can->cd();

    tch_data->SetMarkerStyle(3);
    tch_data->SetMarkerColor(3);
    tch_data->Draw("RunNumber:lepPt[0]", "trigMatch_1L2LTrigOR && !trigMatch_1L2LTrig && lepFlavor[0]==2 && lepFlavor[1]==2");
    tch_data->SetMarkerColor(4);
    tch_data->GetHistogram()->SetTitle("mm");
    tch_data->GetHistogram()->GetXaxis()->SetTitle("");
    tch_data->Draw("RunNumber:lepPt[1]", "trigMatch_1L2LTrigOR && !trigMatch_1L2LTrig && lepFlavor[0]==2 && lepFlavor[1]==2", "same");

    can->Print("scatter_mm.png");

    //--- em
    can = new TCanvas("can","can",600,600);
    can->cd();

    tch_data->SetMarkerStyle(3);
    tch_data->SetMarkerColor(3);
    tch_data->Draw("RunNumber:lepPt[0]", "trigMatch_1L2LTrigOR && !trigMatch_1L2LTrig && ((lepFlavor[0]==1 && lepFlavor[1]==2) || (lepFlavor[0]==2 && lepFlavor[1]==1))");
    tch_data->SetMarkerColor(4);
    tch_data->GetHistogram()->SetTitle("em");
    tch_data->GetHistogram()->GetXaxis()->SetTitle("");
    tch_data->Draw("RunNumber:lepPt[1]", "trigMatch_1L2LTrigOR && !trigMatch_1L2LTrig && ((lepFlavor[0]==1 && lepFlavor[1]==1) || (lepFlavor[0]==2 && lepFlavor[1]==1))", "same");

    can->Print("scatter_em.png");
}
