#include "../CommonLibraries.C"

using namespace std;

void TriggerEfficiency(string folder_name, string file_name, string feature) {

    //--- set triggers
    TCut offline_trigger("nLep_signal==2 && nJet30>=2 && met_Et>200");
    TCut online_trigger_2L("trigMatch_2LTrigOR");
    TCut online_trigger_1L2L("trigMatch_1L2LTrigOR");
    TCut test_trigger_1 = offline_trigger && online_trigger_2L;
    TCut test_trigger_2 = offline_trigger && online_trigger_1L2L;

    //--- load files
    string base_path = "/eos/atlas/atlascerngroupdisk/phys-susy/2L2J-ANA-SUSY-2018-05/SusySkim2LJets/v1.6/SUSY2/";
    string full_filename = base_path + "/" + folder_name + "/" + file_name + "_merged_processed.root";
    TChain* tch;
    if (file_name.find("data") != string::npos) tch = new TChain("data");
    else tch = new TChain((file_name + "_NoSys").c_str());
    tch->Add(full_filename.c_str());

    //--- draw trigger histograms
    TH1F *h_offline, *h_test_trigger_1, *h_test_trigger_2;
    h_offline = new TH1F("h_offline", "", 20, 0, 200);
    h_test_trigger_1 = new TH1F("h_test_trigger_1", "", 20, 0, 200);
    h_test_trigger_2 = new TH1F("h_test_trigger_2", "", 20, 0, 200);
    tch->Draw((feature + ">>h_offline").c_str(), offline_trigger, "goff");
    tch->Draw((feature + ">>h_test_trigger_1").c_str(), test_trigger_1, "goff");
    tch->Draw((feature + ">>h_test_trigger_2").c_str(), test_trigger_2, "goff");

    //--- divide to get efficiency histograms
    TH1F* h_efficiency_1 = (TH1F*) h_test_trigger_1->Clone("h_efficiency_1");
    h_efficiency_1->Divide(h_offline);
    h_efficiency_1->SetLineColor(1); h_efficiency_1->SetLineWidth(1);
    TH1F* h_efficiency_2 = (TH1F*) h_test_trigger_2->Clone("h_efficiency_2");
    h_efficiency_2->Divide(h_offline);
    h_efficiency_2->SetLineColor(2); h_efficiency_2->SetLineWidth(1);

    //--- make plots
    TCanvas *can = new TCanvas("can","can",600,600);
    can->cd();
    TPad* mainpad = new TPad("mainpad","mainpad",0.0,0.2,1.0,1.0);
    mainpad->Draw();
    mainpad->cd();

    h_efficiency_1->SetStats(0);
    h_efficiency_1->SetTitle("Trigger Efficiency");
    h_efficiency_1->SetMinimum(0.0);
    h_efficiency_1->SetMaximum(1.0);
    h_efficiency_1->GetXaxis()->SetTitle("m_{ll} [GeV]");
    h_efficiency_1->GetYaxis()->SetTitle("efficiency");

    h_efficiency_1->Draw("hist");
    h_efficiency_2->Draw("samehist");

    //--- draw legend and labels
    TLegend* leg = new TLegend(0.6,0.2,0.88,0.38);
    leg->AddEntry(h_efficiency_1,"2L trigger","lp");
    leg->AddEntry(h_efficiency_2, "1L2L trigger", "lp");

    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->Draw();

    //--- draw ratio
    can->cd();
    TPad* ratio_pad = new TPad("ratio_pad","ratio_pad",0.0,0.0,1.0,0.2);
    ratio_pad->Draw();
    ratio_pad->cd();
    ratio_pad->SetGridy();

    TH1F* h_ratio_2_1 = (TH1F*) h_efficiency_2->Clone("h_ratio_2_1");
    h_ratio_2_1->Divide(h_efficiency_1);

    h_ratio_2_1->GetXaxis()->SetTitle("");
    h_ratio_2_1->GetXaxis()->SetLabelSize(0.);
    h_ratio_2_1->GetYaxis()->SetNdivisions(5);
    h_ratio_2_1->GetYaxis()->SetTitle("");
    h_ratio_2_1->GetYaxis()->SetTitleSize(0.15);
    h_ratio_2_1->GetYaxis()->SetTitleOffset(0.3);
    h_ratio_2_1->GetYaxis()->SetLabelSize(0.15);
    h_ratio_2_1->SetMinimum(0.0);
    h_ratio_2_1->SetMaximum(2.0);
    h_ratio_2_1->GetYaxis()->SetRangeUser(0.0,2.0);

    h_ratio_2_1->SetStats(0); h_ratio_2_1->SetLineColor(2); h_ratio_2_1->SetLineWidth(2); h_ratio_2_1->SetMarkerStyle(20);
    h_ratio_2_1->Draw("hist");

    //--- save plot
    can->Print(("Plots/triggerEfficiency_" + folder_name + "_" + file_name + "_" + feature + ".png").c_str());
}
